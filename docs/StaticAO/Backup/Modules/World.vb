Imports MTV3D65
Imports System.io
Imports Microsoft.VisualBasic

Public Module World
    Private ReadOnly Globals As TVGlobals
    Private ReadOnly Scene As TVScene
    Private ReadOnly Screen2D As TVScreen2DImmediate
    Private ReadOnly TexFact As TVTextureFactory
    Private ReadOnly Lights As TVLightEngine
    Private ReadOnly MatFact As TVMaterialFactory
    Private ReadOnly Maths As TVMathLibrary
    Private ReadOnly Text2D As TVScreen2DText

    Private MainMesh As TVMesh, Floor As TVMesh
    Private AmbientOcclusionShader As TVShader
    Private Progress As ProgressShower

    Private MaterialSpecularPower As Single = 8
    Private DiffuseOcclusionFactor As Single = 0
    Private DirectionalLight As LightInfo
    Private PointLights(0 To 3) As LightInfo

    Public Enum MeshChoices
        Skull
        Ship
        Text3D
        Teapot
        TorusKnot
        Rancor
    End Enum

    Public Enum AmbientOcclusionComputers
        Raytracing
        HemicubeRendering
    End Enum

    Private Structure LightInfo
        Public Index As Integer
        Public Enabled As Boolean
        Public Info As TV_LIGHT
    End Structure

    Sub New()
        Globals = New TVGlobals
        Scene = New TVScene
        Screen2D = New TVScreen2DImmediate
        TexFact = New TVTextureFactory
        Lights = New TVLightEngine
        MatFact = New TVMaterialFactory
        Maths = New TVMathLibrary
        Text2D = New TVScreen2DText
    End Sub

    Sub Initialize()
        With MatFact
            Dim index As Integer

            index = .CreateMaterial("Default")
            .SetEmissive(index, 0, 0, 0, 1)
            ' Put a significant value because the occlusion will only filter ambient reflection
            .SetAmbient(index, 0.4, 0.4, 0.4, 1)
            .SetDiffuse(index, 0.6, 0.6, 0.6, 1)
            .SetSpecular(index, 0.5, 0.5, 0.5, 1)
            .SetPower(index, MaterialSpecularPower)
        End With

        With DirectionalLight.Info
            .direction = New TV_3DVECTOR(0.7, -0.3, 0.4)
            Maths.TVVec3Normalize(.direction, .direction)
            .diffuse = New TV_COLOR(0.5, 0.5, 0.5, 1)
            .range = 1000
            .bManaged = True
            .type = CONST_TV_LIGHTTYPE.TV_LIGHT_DIRECTIONAL
        End With
        DirectionalLight.Index = Lights.CreateLight(DirectionalLight.Info)
        DirectionalLight.Enabled = True
        Lights.EnableLight(DirectionalLight.Index, True)

        For i As Integer = 0 To 3
            With PointLights(i).Info
                If i = 3 Then
                    .diffuse = New TV_COLOR(0.5, 0, 0.5, 0)
                Else
                    .diffuse = New TV_COLOR(CSng(IIf(i = 0, 0.5, 0)), CSng(IIf(i = 1, 0.5, 0)), CSng(IIf(i = 2, 0.5, 0)), 1)
                End If
                .bManaged = True
                .range = 1000
                .type = CONST_TV_LIGHTTYPE.TV_LIGHT_POINT
            End With
            PointLights(i).Index = Lights.CreateLight(PointLights(i).Info)
            If i Mod 2 = 0 Then
                PointLights(i).Enabled = True
            End If
            Lights.EnableLight(PointLights(i).Index, PointLights(i).Enabled)
        Next

        With TexFact
            .LoadTexture("Textures\Diffuse.dds", "D")           ' The stone wall diffuse map
            .LoadTexture("Textures\Height.dds", "H")            ' The stone wall height map
            .LoadTexture("Textures\NormalGloss.dds", "NG")      ' The stone wall normal map with gloss map
            .LoadTexture("Textures\Flat.dds", "F")              ' A flat normal map
            .LoadTexture("Textures\White.dds", "W")             ' A white diffuse map
            .LoadTexture("Textures\SkullDiffuse.dds", "SD")     ' The skull diffuse map
            .LoadTexture("Textures\SkullNormal.dds", "SN")      ' The skull normal map
            .LoadTexture("Textures\RancorDiffuse.dds", "RD")    ' The rancor diffuse map
            .LoadTexture("Textures\RancorNormal.dds", "RN")     ' The rancor normal map
        End With

        AmbientOcclusionShader = Scene.CreateShader("Shader")
        With AmbientOcclusionShader
            .CreateFromEffectFile("Shaders\AmbientOcclusion.fx")
            .SetEffectParamFloat("diffuseOcclusionFactor", DiffuseOcclusionFactor)
        End With

        Floor = Scene.CreateMeshBuilder("Floor")
        With Floor
            .AddFloorGrid(0, -100, -100, 100, 100, Settings.FloorSlices, Settings.FloorSlices)
            .ComputeNormals()

            .SetTextureEx(CONST_TV_LAYER.TV_LAYER_BASETEXTURE, Globals.GetTex("W"))
            .SetTextureEx(CONST_TV_LAYER.TV_LAYER_NORMALMAP, Globals.GetTex("F"))
            .SetTextureEx(CONST_TV_LAYER.TV_LAYER_HEIGHTMAP, Globals.GetTex("F"))

            ' Managed mode allows 4 point lights, whereas OFFSET_BUMPMAPPING modes limit to 2
            .SetLightingMode(CONST_TV_LIGHTINGMODE.TV_LIGHTING_MANAGED)
            ' The DIFFUSE mesh format element will force in a vertex color channel
            .SetMeshFormat(CONST_TV_MESHFORMAT.TV_MESHFORMAT_DIFFUSE Or _
                CONST_TV_MESHFORMAT.TV_MESHFORMAT_TEX1 Or _
                CONST_TV_MESHFORMAT.TV_MESHFORMAT_BUMPMAPINFO)
            .SetMaterial(Globals.GetMat("Default"))
        End With

        MainMesh = Scene.CreateMeshBuilder("ActiveMesh")
        Dim MeshPath As String = ""
        With MainMesh
            Select Case Settings.ChosenMesh
                Case MeshChoices.Skull
                    MeshPath = "Models\Skull.TVM"
                    .LoadTVM(MeshPath, False, False)
                    .SetTextureEx(CONST_TV_LAYER.TV_LAYER_BASETEXTURE, Globals.GetTex("SD"))
                    .SetTextureEx(CONST_TV_LAYER.TV_LAYER_NORMALMAP, Globals.GetTex("SN"))
                    .SetTextureEx(CONST_TV_LAYER.TV_LAYER_HEIGHTMAP, Globals.GetTex("F"))
                    .RotateY(-90)
                Case MeshChoices.Ship
                    MeshPath = "Models\Ship.TVM"
                    .LoadTVM(MeshPath, False, False)
                    .SetTextureEx(CONST_TV_LAYER.TV_LAYER_BASETEXTURE, Globals.GetTex("D"))
                    .SetTextureEx(CONST_TV_LAYER.TV_LAYER_NORMALMAP, Globals.GetTex("NG"))
                    .SetTextureEx(CONST_TV_LAYER.TV_LAYER_HEIGHTMAP, Globals.GetTex("H"))
                Case MeshChoices.Text3D
                    MeshPath = "Models\3DText.TVM"
                    .LoadTVM(MeshPath, False, False)
                    .SetTextureEx(CONST_TV_LAYER.TV_LAYER_BASETEXTURE, Globals.GetTex("W"))
                    .SetTextureEx(CONST_TV_LAYER.TV_LAYER_HEIGHTMAP, Globals.GetTex("F"))
                    .SetTextureEx(CONST_TV_LAYER.TV_LAYER_NORMALMAP, Globals.GetTex("F"))
                Case MeshChoices.TorusKnot
                    MeshPath = "Models\TorusKnot.TVM"
                    .LoadTVM(MeshPath, False, False)
                    .SetTextureEx(CONST_TV_LAYER.TV_LAYER_BASETEXTURE, Globals.GetTex("W"))
                    .SetTextureEx(CONST_TV_LAYER.TV_LAYER_NORMALMAP, Globals.GetTex("F"))
                    .SetTextureEx(CONST_TV_LAYER.TV_LAYER_HEIGHTMAP, Globals.GetTex("F"))
                Case MeshChoices.Rancor
                    MeshPath = "Models\Rancor.TVM"
                    .LoadTVM(MeshPath, False, False)
                    .SetTextureEx(CONST_TV_LAYER.TV_LAYER_BASETEXTURE, Globals.GetTex("RD"))
                    .SetTextureEx(CONST_TV_LAYER.TV_LAYER_NORMALMAP, Globals.GetTex("RN"))
                    .SetTextureEx(CONST_TV_LAYER.TV_LAYER_HEIGHTMAP, Globals.GetTex("F"))
            End Select

            ' Center the mesh on its "feet"
            Dim Minimum As TV_3DVECTOR, Maximum As TV_3DVECTOR, Center As TV_3DVECTOR
            .GetBoundingBox(Minimum, Maximum)
            Center = (Minimum + Maximum) / 2
            .SetMeshCenter(Center.x, Minimum.y, Center.z)

            ' Make the mesh 100 units big maximum
            Dim Diff As TV_3DVECTOR = Maximum - Minimum
            Dim Factor As Single = 125.0F / Math.Max(Math.Max(Diff.x, Diff.y), Diff.z)
            .SetScale(Factor, Factor, Factor)

            ' A slight vertical offset to ensure that collision rays hit the floor
            .SetPosition(0, 0.1F, 0)

            ' This is needed! SetPosition and SetScale apparently do not affect the matrices
            .ForceMatrixUpdate()

            ' See above comments...
            .SetLightingMode(CONST_TV_LIGHTINGMODE.TV_LIGHTING_MANAGED)
            .SetMeshFormat(CONST_TV_MESHFORMAT.TV_MESHFORMAT_DIFFUSE Or _
                CONST_TV_MESHFORMAT.TV_MESHFORMAT_TEX1 Or _
                CONST_TV_MESHFORMAT.TV_MESHFORMAT_BUMPMAPINFO)
            .SetMaterial(Globals.GetMat("Default"))
        End With

        ' Build the list of occluders
        Dim Occluders As New List(Of TVMesh)
        With Occluders
            .Add(MainMesh)
            .Add(Floor)
        End With

        Dim Computer As AmbientOcclusionComputer

        If Not Settings.LoadOcclusionFromModel Then
            Progress = New ProgressShower("Ambient Occlusion", New String() {"Main mesh", "Floor mesh"})
            Progress.Show()

            If Settings.OcclusionComputer = AmbientOcclusionComputers.Raytracing Then
                Computer = New RaytracingComputer(MainMesh, Occluders, Progress, Settings.RaysPerVertex, Settings.NumberOfThreads)
            Else
                Computer = New HemicubeComputer(MainMesh, Occluders, Progress, Settings.SamplesPerVertex)
            End If
            Progress.CurrentTask = "Main mesh"
            Computer.Execute()

            If Settings.SaveOcclusionInModel Then
                MainMesh.SaveTVM(MeshPath)
            End If
        Else
            Progress = New ProgressShower("Ambient Occlusion", New String() {"Floor mesh"})
            Progress.Show()
        End If

        If Settings.OcclusionComputer = AmbientOcclusionComputers.Raytracing Then
            Computer = New RaytracingComputer(Floor, Occluders, Progress, Settings.RaysPerVertex, Settings.NumberOfThreads)
        Else
            Computer = New HemicubeComputer(Floor, Occluders, Progress, Settings.SamplesPerVertex)
        End If
        Progress.CurrentTask = "Floor mesh"
        Computer.Execute()

        Progress.Close()
        Progress = Nothing

        Floor.SetShader(AmbientOcclusionShader)
        MainMesh.SetShader(AmbientOcclusionShader)
    End Sub

    Sub Update()
        Static AccumulatedTime As Single
        AccumulatedTime += Engine.AccurateTimeElapsed * 0.0005F

        For i As Integer = 0 To 3
            With PointLights(i)
                .Info.position = New TV_3DVECTOR(CSng(Math.Cos(AccumulatedTime + (Math.PI / 2) * i)) * 100, 20, CSng(Math.Sin(AccumulatedTime + (Math.PI / 2) * i)) * 100)
                Lights.SetLight(.Index, .Info)
            End With
        Next
    End Sub

    Property SetDiffuseOcclusionFactor() As Single
        Set(ByVal value As Single)
            If value > 1 Then value = 1
            If value < 0 Then value = 0
            DiffuseOcclusionFactor = value
            AmbientOcclusionShader.SetEffectParamFloat("diffuseOcclusionFactor", value)
        End Set
        Get
            Return DiffuseOcclusionFactor
        End Get
    End Property

    WriteOnly Property SetLightEnabled(ByVal i As Integer) As Boolean
        Set(ByVal value As Boolean)
            PointLights(i).Enabled = value
            Lights.EnableLight(PointLights(i).Index, value)
        End Set
    End Property

    WriteOnly Property SpecularPower() As Single
        Set(ByVal value As Single)
            MaterialSpecularPower = value
            MatFact.SetPower(Globals.GetMat("Default"), value)
        End Set
    End Property

    Sub Render()
        Scene.RenderAll(True)
    End Sub

    Sub PostRender()
        Dim X As Single, Y As Single

        With Screen2D
            .Action_Begin2D()
            If DirectionalLight.Enabled Then
                Dim EndPoint As TV_3DVECTOR = DirectionalLight.Info.direction * 50 + New TV_3DVECTOR(0, 100, 0)
                .Draw_Line3D(0, 100, 0, EndPoint.x, EndPoint.y, EndPoint.z)
                If .Math_3DPointTo2D(EndPoint, X, Y, True) Then
                    .Draw_FilledCircle(X, Y, 2)
                End If
            End If

            For Each PointLight As LightInfo In PointLights
                If PointLight.Enabled AndAlso .Math_3DPointTo2D(PointLight.Info.position, X, Y, True) Then
                    .Draw_FilledCircle(X, Y, 5, 7, Globals.RGBA(PointLight.Info.diffuse.r, PointLight.Info.diffuse.g, PointLight.Info.diffuse.b, 1))
                End If
            Next

            .Action_End2D()
        End With

        Static TitleFont As Integer = Text2D.TextureFont_GetFontByName("Title")
        Static SmallFont As Integer = Text2D.TextureFont_GetFontByName("Small")

        With Text2D
            .Action_BeginText()

            .TextureFont_DrawText("Zaknafein's Static Ambient Occlusion Sample", 8, 25, Globals.RGBA(1, 1, 1, 0.75F), TitleFont)
            .TextureFont_DrawText(String.Format("Diffuse Occlusion Factor [d|D] : {0:0.##}", DiffuseOcclusionFactor), 8, 50, Globals.RGBA(1, 1, 1, 0.5F), SmallFont)
            .TextureFont_DrawText(String.Format("Point Light 1 Enabled/Disabled [1|F1] : {0}", PointLights(0).Enabled), 8, 65, Globals.RGBA(1, 1, 1, 0.5F), SmallFont)
            .TextureFont_DrawText(String.Format("Point Light 2 Enabled/Disabled [2|F2] : {0}", PointLights(1).Enabled), 8, 80, Globals.RGBA(1, 1, 1, 0.5F), SmallFont)
            .TextureFont_DrawText(String.Format("Point Light 3 Enabled/Disabled [3|F3] : {0}", PointLights(2).Enabled), 8, 95, Globals.RGBA(1, 1, 1, 0.5F), SmallFont)
            .TextureFont_DrawText(String.Format("Point Light 4 Enabled/Disabled [4|F4] : {0}", PointLights(3).Enabled), 8, 110, Globals.RGBA(1, 1, 1, 0.5F), SmallFont)
            .TextureFont_DrawText(String.Format("Material Specular Power [Numpad 1-9] : {0}", MaterialSpecularPower), 8, 125, Globals.RGBA(1, 1, 1, 0.5F), SmallFont)

            .Action_EndText()
        End With
    End Sub
End Module
