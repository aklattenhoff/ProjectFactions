Imports MTV3D65

Public Class HemicubeComputer
    Inherits AmbientOcclusionComputer

    ' Some constants, mostly for debugging and testing... should not be modified unless you know what you're doing
    Const RENDER_TARGET_FORMAT As CONST_TV_RENDERSURFACEFORMAT = CONST_TV_RENDERSURFACEFORMAT.TV_TEXTUREFORMAT_X8R8G8B8
    Const DEBUG_MODE As Boolean = False
    Const DOWNSAMPLE_WITH_SHADER As Boolean = False

    ' TV3D Singletons
    Private Shared ReadOnly Scene As TVScene
    Private Shared ReadOnly CameraFactory As TVCameraFactory
    Private Shared ReadOnly Screen2D As TVScreen2DImmediate
    Private Shared ReadOnly TexFact As TVTextureFactory

    Private Enum HemicubeFace
        Right
        Left
        Top
        Front
        Back
    End Enum

    Private ReadOnly HemicubeSize As Integer, HemicubeLogSize As Integer        ' Size variables for the hemicube
    Private ReadOnly Hemicube As Dictionary(Of HemicubeFace, TVRenderSurface)   ' The hemicube as a set of faces
    Private ReadOnly HemicubeMipMaps() As TVRenderSurface       ' A mip-map chain for a single square hemicube face

    Private ReadOnly HemicubeSquareCamera As TVCamera           ' The square 1:1 aspect ratio camera
    Private ReadOnly HemicubeRectCamera As TVCamera             ' The rectangular 2:1 aspect ratio camera

    Private ReadOnly ThreePixelSampler As TVRenderSurface       ' A 2x2 RS that only has 3 pixels used
    Private ReadOnly SinglePixelSampler As TVRenderSurface      ' A 1x1 RS for the result of the 3 pixels average
    Private ReadOnly OcclusionBufferSize As Integer             ' The power-of-two size of the occlusion buffer
    Private ReadOnly OcclusionBuffer As TVRenderSurface         ' A buffer to avoid 1x1 static textures creation

    Private ReadOnly ThreePixelsAverageShader As TVShader       ' A 3-pixels-to-one shader
    Private ReadOnly Downsample4xShader As TVShader             ' A 4x downsampling shader (not really functional)
    Private ReadOnly DepthTestShader As TVShader                ' A simple shader that renders objects black

    Private ReadOnly SceneMeshes As IEnumerable(Of TVMesh)      ' Needed to apply the depth testing shader

    ' Initializes the TV3D singletons
    Shared Sub New()
        Scene = New TVScene
        CameraFactory = New TVCameraFactory
        Screen2D = New TVScreen2DImmediate
        TexFact = New TVTextureFactory
    End Sub

    ' Initializes all the instance variables (mostly rendersurfaces)
    Sub New(ByVal Mesh As TVMesh, ByVal SceneMeshes As IEnumerable(Of TVMesh), ByVal Progress As ProgressShower, Optional ByVal HemicubeSize As Integer = 8)
        MyBase.New(Mesh, Progress)

        HemicubeLogSize = HemicubeSize
        Me.HemicubeSize = CInt(2 ^ HemicubeSize)
        Me.SceneMeshes = SceneMeshes

        Hemicube = New Dictionary(Of HemicubeFace, TVRenderSurface)
        ' A single 1:1 surface for the top and four 2:1 surfaces for the sides
        With Scene
            Hemicube(HemicubeFace.Top) = .CreateRenderSurface(Me.HemicubeSize, Me.HemicubeSize, True, RENDER_TARGET_FORMAT)
            Hemicube(HemicubeFace.Right) = .CreateRenderSurface(Me.HemicubeSize, Me.HemicubeSize \ 2, True, RENDER_TARGET_FORMAT)
            Hemicube(HemicubeFace.Left) = .CreateRenderSurface(Me.HemicubeSize, Me.HemicubeSize \ 2, True, RENDER_TARGET_FORMAT)
            Hemicube(HemicubeFace.Front) = .CreateRenderSurface(Me.HemicubeSize, Me.HemicubeSize \ 2, True, RENDER_TARGET_FORMAT)
            Hemicube(HemicubeFace.Back) = .CreateRenderSurface(Me.HemicubeSize, Me.HemicubeSize \ 2, True, RENDER_TARGET_FORMAT)
        End With

        ' Init a mip-map chain; if the current surface's size is 2^n, the next is 2^(n-1) until n=0
        ' This chain is used for all hemicube faces (the top, and the two square assembled sides out of the four actual sides)
        ReDim HemicubeMipMaps(HemicubeLogSize - 1)
        For MipLevel As Integer = 0 To HemicubeLogSize - 1
            Dim MipMapSize As Integer = CInt(2 ^ (HemicubeLogSize - (MipLevel + 1)))
            HemicubeMipMaps(MipLevel) = Scene.CreateRenderSurface(MipMapSize, MipMapSize, False, RENDER_TARGET_FORMAT)
        Next

        ' The three-pixels sampler is a temporary RS that holds the three 1x1 surfaces resulting from the mip-mapping
        ThreePixelSampler = Scene.CreateRenderSurface(2, 2, False, RENDER_TARGET_FORMAT)
        With ThreePixelSampler
            .SetBackgroundColor(Globals.RGBA(1, 1, 1, 1))
            .StartRender()
            .EndRender()
        End With

        ' The single-pixel sampler used as the result for the three-pixels-sampler downsizing
        SinglePixelSampler = Scene.CreateRenderSurface(1, 1, False, RENDER_TARGET_FORMAT)

        ' The occlusion buffer holds the ambient occlusion value for each vertex of the model
        ' We want it square and power-of-two to ensure maximum access speed
        Dim SqrtVertexCount As Integer = CInt(Math.Ceiling(Math.Sqrt(Mesh.GetVertexCount())))
        OcclusionBufferSize = CInt(2 ^ (Math.Ceiling(Math.Log(SqrtVertexCount, 2))))
        OcclusionBuffer = Scene.CreateRenderSurface(OcclusionBufferSize, OcclusionBufferSize, False, CONST_TV_RENDERSURFACEFORMAT.TV_TEXTUREFORMAT_X8R8G8B8)
        With ThreePixelSampler
            .StartRender()
            .EndRender()
        End With

        ' The square camera, for the top RS, has a 90 degrees FOV and a square aspect ratio
        HemicubeSquareCamera = CameraFactory.CreateCamera
        HemicubeSquareCamera.SetCustomAspectRatio(1)
        HemicubeSquareCamera.SetViewFrustum(90, FAR_PLANE, NEAR_PLANE)

        ' The rectangular camera, for all the 4 sides, has a 45 degrees FOV and a 2:1 rectangular aspect ratio
        HemicubeRectCamera = CameraFactory.CreateCamera
        HemicubeRectCamera.SetCustomAspectRatio(2)
        HemicubeRectCamera.SetViewFrustum(45, FAR_PLANE, NEAR_PLANE)

        ' We want all hemicube faces to have a white background
        For Each Face As KeyValuePair(Of HemicubeFace, TVRenderSurface) In Hemicube
            Face.Value.SetBackgroundColor(Globals.RGBA(1, 1, 1, 1))

            If Face.Key = HemicubeFace.Top Then
                Face.Value.SetNewCamera(HemicubeSquareCamera)
            Else
                Face.Value.SetNewCamera(HemicubeRectCamera)
            End If
        Next

        ' The depth testing shader is used to simplify the rendering on the hemicube RSes
        DepthTestShader = Scene.CreateShader
        DepthTestShader.CreateFromEffectFile("Shaders\DepthTest.fx")

        ' This shader averages three pixels to one, skips the (2, 2)... very app-specific stuff. o_O
        ThreePixelsAverageShader = Scene.CreateShader
        ThreePixelsAverageShader.CreateFromEffectFile("Shaders\ThreePixelsAverage.fx")
        ThreePixelsAverageShader.SetEffectParamTexture("texTexture", ThreePixelSampler.GetTexture)

        ' This shader should downscale a 4x4 surface to a 1x1, whereas normal bilinear-filtered downscaling takes
        ' a 2x2 to a 1x1. It should then skip some overhead and speed up the mip-mapping process, but it's a bit
        ' faulty and doesn't provide much of a speedup anyway. :(
        Downsample4xShader = Scene.CreateShader
        Downsample4xShader.CreateFromEffectFile("Shaders\Downsample4x.fx")
    End Sub

    ' Gets a new vertex for processing
    Private Function GetNextVertex(ByRef id As Integer, ByRef x As Single, ByRef y As Single, ByRef z As Single, ByRef nx As Single, ByRef ny As Single, ByRef nz As Single, ByRef tu1 As Single, ByRef tv1 As Single) As Boolean
        Static CurrentVertexID As Integer = 0

        If Progress.Panicked OrElse CurrentVertexID = Mesh.GetVertexCount - 1 Then
            CurrentVertexID = 0
            Return False
        End If

        Mesh.GetVertex(CurrentVertexID, x, y, z, nx, ny, nz, tu1, tv1, 0, 0, 0)
        id = CurrentVertexID

        CurrentVertexID += 1

        Return True
    End Function

    ' Renders a single face of the hemicube
    Private Sub RenderHemicubeFace(ByVal Face As HemicubeFace, ByVal LookAt As TV_3DVECTOR, ByVal Camera As TVCamera)
        Camera.SetLookAt(LookAt.x, LookAt.y, LookAt.z)

        Hemicube(Face).StartRender()
        For Each Mesh As TVMesh In SceneMeshes
            Mesh.Render()
        Next
        Hemicube(Face).EndRender()

        ' ** Debugging **
        If DEBUG_MODE Then TexFact.SaveTexture(Hemicube(Face).GetTexture, Face.ToString & ".dds", CONST_TV_IMAGEFORMAT.TV_IMAGE_DDS)
    End Sub

    ' Downscales a hemicube face from its original (or assembled) size down to 1x1 and fits it in the 3-pixels sampler
    Private Sub DownscaleToSamplingMap(ByVal Face As HemicubeFace, ByVal PixelNumber As Integer, Optional ByVal FromLevel As Integer = 0)
        For MipLevel As Integer = FromLevel To HemicubeMipMaps.Length - 1
            Dim SourceTexture As Integer
            Dim SourceTexelSize As Single
            Dim MipMapSize As Integer
            Dim HalfDestTexelSize As Single

            If MipLevel = 0 Then
                SourceTexture = Hemicube(Face).GetTexture
            Else
                SourceTexture = HemicubeMipMaps(MipLevel - 1).GetTexture
            End If

            If DOWNSAMPLE_WITH_SHADER AndAlso MipLevel < HemicubeMipMaps.Length - 1 Then
                Downsample4xShader.SetEffectParamFloat("texelSize", SourceTexelSize)

                MipLevel += 1
                MipMapSize = CInt(2 ^ (HemicubeLogSize - (MipLevel + 1)))
                HalfDestTexelSize = CSng(0.5 / MipMapSize)
                SourceTexelSize = CSng(1 / 2 ^ (HemicubeLogSize - MipLevel))

                HemicubeMipMaps(MipLevel).StartRender(True)
                Screen2D.Draw_FullscreenQuadWithShader(Downsample4xShader, HalfDestTexelSize, HalfDestTexelSize, 1 + HalfDestTexelSize, 1 + HalfDestTexelSize, SourceTexture)
                HemicubeMipMaps(MipLevel).EndRender()
            Else
                MipMapSize = CInt(2 ^ (HemicubeLogSize - (MipLevel + 1)))

                HemicubeMipMaps(MipLevel).StartRender(True)
                Screen2D.Draw_Texture(SourceTexture, 0, 0, MipMapSize - 1, MipMapSize - 1)
                HemicubeMipMaps(MipLevel).EndRender()
            End If

            ' ** Debugging **
            If DEBUG_MODE Then TexFact.SaveTexture(HemicubeMipMaps(MipLevel).GetTexture, Face.ToString & "_Level" & MipLevel & ".dds", CONST_TV_IMAGEFORMAT.TV_IMAGE_DDS)
        Next

        ThreePixelSampler.StartRender(True)
        Screen2D.Draw_Sprite(HemicubeMipMaps(HemicubeMipMaps.Length - 1).GetTexture, PixelNumber Mod 2, PixelNumber \ 2)
        ThreePixelSampler.EndRender()
    End Sub

    ' Assembles two 2:1-aspect-ratio hemicube faces and constructs a half-sized 1:1 texture out of them
    ' For example, two 4x2 faces would give a single 2x2 texture
    Private Sub AssembleAndDownscaleOnce(ByVal Face1 As HemicubeFace, ByVal Face2 As HemicubeFace)
        Dim MipMapSize As Integer = CInt(2 ^ (HemicubeLogSize - 1))

        HemicubeMipMaps(0).StartRender(True)
        Screen2D.Action_Begin2D()
        Screen2D.Draw_Texture(Hemicube(Face1).GetTexture, 0, 0, MipMapSize - 1, (MipMapSize \ 2) - 1)
        Screen2D.Draw_Texture(Hemicube(Face2).GetTexture, 0, MipMapSize \ 2, MipMapSize - 1, MipMapSize - 1)
        Screen2D.Action_End2D()
        HemicubeMipMaps(0).EndRender()

        ' ** Debugging **
        If DEBUG_MODE Then TexFact.SaveTexture(HemicubeMipMaps(0).GetTexture, Face1.ToString & "_" & Face2.ToString & "_Level0.dds", CONST_TV_IMAGEFORMAT.TV_IMAGE_DDS)
    End Sub

    Public Overrides Sub Execute()
        Dim Id As Integer
        Dim X As Single, Y As Single, Z As Single
        Dim Nx As Single, Ny As Single, Nz As Single
        Dim Tu1 As Single, Tv1 As Single

        For Each Mesh As TVMesh In SceneMeshes
            Mesh.SetShader(DepthTestShader)
        Next

        While GetNextVertex(Id, X, Y, Z, Nx, Ny, Nz, Tu1, Tv1)
            If Id Mod 40 = 0 Then Progress.TaskProgress = CInt(Id / (Mesh.GetVertexCount - 1) * 100)

            Dim Position As New TV_3DVECTOR(X, Y, Z)
            ' Transform the position from local space to world space
            Position = World * Position
            Dim Normal As New TV_3DVECTOR(Nx, Ny, Nz)
            ' Transform the normal from local space to world space
            ' More details on this transformation : http://wiki.truevision3d.com/hlsl_phong_sample#calculating_the_normal_vector
            Normal = WorldInversedTransposed * Normal
            Maths.TVVec3Normalize(Normal, Normal)

            ' We need to find the two basis vectors of the "normal plane",
            ' which we need to find the 45 degrees vectors needed for the hemicube sides
            Dim Right As TV_3DVECTOR, Up As TV_3DVECTOR
            Dim TentativeUp As New TV_3DVECTOR(0, 1, 0)

            ' Handle the cases where the cross products won't be defined : (0, 1, 0) and (0, -1, 0)
            If Math.Abs(Normal.x) < 0.00001 AndAlso Math.Abs(Normal.z) < 0.00001 Then
                Right = New TV_3DVECTOR(1, 0, 0)
                If Math.Abs(1 - Normal.y) < 0.00001 Then
                    Up = New TV_3DVECTOR(0, 0, -1)
                Else 'If Math.Abs(-1 - Normal.y) < 0.00001 Then
                    Up = New TV_3DVECTOR(0, 0, 1)
                End If
            Else
                ' Cross to get two basis vectors in the normal plane
                Maths.TVVec3Cross(Right, TentativeUp, Normal)
                Maths.TVVec3Normalize(Right, Right)
                Maths.TVVec3Cross(Up, Normal, Right)
            End If

            ' Position does not change among cube faces
            HemicubeSquareCamera.SetPosition(Position.x, Position.y, Position.z)
            HemicubeRectCamera.SetPosition(Position.x, Position.y, Position.z)

            ' Render the hemicube top face
            ' This one has a 1:1 aspect ratio (square) and faces the normal
            RenderHemicubeFace(HemicubeFace.Top, Position + Normal, HemicubeSquareCamera)

            ' Render the 4 half-sides, with 2:1 ratios and different orientations
            ' The cryptic offset value uses some trigonometry; we need the point in exactly the
            ' right direction for these cameras to avoid tears in the resulting hemicube
            ' Proof is in my notebook... Ask if interested. :P
            Dim OffsetPosition As TV_3DVECTOR = Position + Normal * CSng(Math.Tan(Math.PI / 8))
            RenderHemicubeFace(HemicubeFace.Right, OffsetPosition + Right, HemicubeRectCamera)
            RenderHemicubeFace(HemicubeFace.Left, OffsetPosition - Right, HemicubeRectCamera)
            RenderHemicubeFace(HemicubeFace.Front, OffsetPosition + Up, HemicubeRectCamera)
            RenderHemicubeFace(HemicubeFace.Back, OffsetPosition - Up, HemicubeRectCamera)

            ' Mip them down as much as possible
            ' and arrange the three resulting pixels to a 2x2 sampling map
            DownscaleToSamplingMap(HemicubeFace.Top, 0)
            AssembleAndDownscaleOnce(HemicubeFace.Right, HemicubeFace.Left)
            DownscaleToSamplingMap(HemicubeFace.Right, 1, 1)
            AssembleAndDownscaleOnce(HemicubeFace.Front, HemicubeFace.Back)
            DownscaleToSamplingMap(HemicubeFace.Front, 2, 1)

            ' ** Debugging **
            If DEBUG_MODE Then TexFact.SaveTexture(ThreePixelSampler.GetTexture, "QuadPixelSamplingMap.dds", CONST_TV_IMAGEFORMAT.TV_IMAGE_DDS)

            ' Downsample the three pixels to a single one
            SinglePixelSampler.StartRender(True)
            Screen2D.Draw_FullscreenQuadWithShader(ThreePixelsAverageShader, 0, 0, 1, 1)
            SinglePixelSampler.EndRender()

            ' And draw it on the occlusion buffer
            Dim Point As New TV_2DVECTOR(Id Mod OcclusionBufferSize, Id \ OcclusionBufferSize)
            OcclusionBuffer.StartRender(True)
            Screen2D.Draw_Texture(SinglePixelSampler.GetTexture, Point.x, Point.y, Point.x, Point.y)
            OcclusionBuffer.EndRender()

            If DEBUG_MODE Then Debugger.Break()
        End While

        ' That may be problematic for meshes that already had shaders...
        For Each Mesh As TVMesh In SceneMeshes
            Mesh.SetShader(Nothing)
        Next

        ' ** Debugging **
        If DEBUG_MODE Then TexFact.SaveTexture(OcclusionBuffer.GetTexture, "OcclusionBuffer.dds", CONST_TV_IMAGEFORMAT.TV_IMAGE_DDS)

        ' Make a lockable static texture out of the occlusion buffer RS
        Dim OcclusionBufferStaticTexture As Integer = OcclusionBuffer.CreateStaticTextureFromRenderSurface(OcclusionBufferSize, OcclusionBufferSize)

        TexFact.LockTexture(OcclusionBufferStaticTexture, True)
        ' Loop a second time on all vertices
        While GetNextVertex(Id, X, Y, Z, Nx, Ny, Nz, Tu1, Tv1)
            ' Sample the occlusion factor
            Dim PixelColor As Integer = TexFact.GetPixel(OcclusionBufferStaticTexture, Id Mod OcclusionBufferSize, Id \ OcclusionBufferSize)
            Dim OcclusionFactor As Single = Globals.DecodeRGBA(PixelColor).r

            ' And set it!
            Dim Color As Integer = Globals.RGBA(OcclusionFactor, OcclusionFactor, OcclusionFactor, 1)
            Mesh.SetVertex(Id, X, Y, Z, Nx, Ny, Nz, Tu1, Tv1, 0, 0, Color)
        End While
        TexFact.UnlockTexture(OcclusionBufferStaticTexture, False)

        ' Clean up
        TexFact.DeleteTexture(OcclusionBufferStaticTexture)
    End Sub
End Class
