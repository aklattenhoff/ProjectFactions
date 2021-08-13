Imports MTV3D65

Module Core
    Public Viewport As Viewport

    Public ReadOnly Engine As TVEngine

    Private ReadOnly Scene As TVScene
    Private ReadOnly Lights As TVLightEngine
    Private ReadOnly TexFact As TVTextureFactory
    Private ReadOnly Inputs As New TVInputEngine
    Private ReadOnly Text2D As TVScreen2DText
    Private ReadOnly Screen2D As TVScreen2DImmediate

    Private Camera As TVCamera

    Public StopLoop As Boolean = False

    Public Settings As InfoGetter.Settings

    Public FrustrumAngle As Single = 60
    Public FrustrumFarPlane As Single = 10000
    Public FrustrumNearPlane As Single = 1

    Sub New()
        Engine = New TVEngine
        Scene = New TVScene
        Lights = New TVLightEngine
        TexFact = New TVTextureFactory
        Text2D = New TVScreen2DText
        Screen2D = New TVScreen2DImmediate
    End Sub

    Sub Main()
        Application.SetCompatibleTextRenderingDefault(False)
        Application.EnableVisualStyles()
        ' Probably a bit unsafe, but avoids crazy workarounds
        Control.CheckForIllegalCrossThreadCalls = False

        If Not Initialize() Then Return

        MainLoop()
    End Sub

    Private Function Initialize() As Boolean
        Dim big As New InfoGetter
        If big.ShowDialog <> DialogResult.OK Then
            Return False
        End If

        Settings = big.GetSettings

        Viewport = New Viewport

        With Engine
            .SetDebugFile(Application.StartupPath.Substring(0, Application.StartupPath.LastIndexOf("\bin\")) + "\debug.txt")
            .SetBetaKey(Settings.Username, Settings.BetaKey)
            If (Settings.Fsaa) Then .SetAntialiasing(True, CONST_TV_MULTISAMPLE_TYPE.TV_MULTISAMPLE_4_SAMPLES)
            .SetVSync(False)
            .Init3DWindowed(Viewport.Handle)
            .SetAngleSystem(CONST_TV_ANGLE.TV_ANGLE_DEGREE)
            .DisplayFPS(True)
            .GetViewport().SetAutoResize(True)
            .SetSearchDirectory(Application.StartupPath.Substring(0, Application.StartupPath.LastIndexOf("\bin\")) + "\")

            Camera = .GetCamera
        End With

        With Scene
            .SetMaxAnisotropy(16)
            .SetTextureFilter(CONST_TV_TEXTUREFILTER.TV_FILTER_ANISOTROPIC)
            .SetAutoTransColor(CONST_TV_COLORKEY.TV_COLORKEY_USE_ALPHA_CHANNEL)
            .SetBackgroundColor(0.1, 0.1, 0.1)
        End With

        With Lights
            .SetSpecularLighting(True)
            .SetGlobalAmbient(0, 0, 0)
        End With

        TexFact.SetTextureMode(CONST_TV_TEXTUREMODE.TV_TEXTUREMODE_32BITS)

        Camera.SetViewFrustum(FrustrumAngle, FrustrumFarPlane, FrustrumNearPlane)

        Inputs.Initialize(True, True)

        With Text2D
            .TextureFont_Create("Title", "Trebuchet MS", 14, True, False, False, False)
            .TextureFont_Create("Small", "Tahoma", 9, False, False, False, False)
        End With

        With Screen2D
            .Settings_SetAlphaBlending(False, CONST_TV_BLENDINGMODE.TV_BLEND_NO)
            .Settings_SetTextureFilter(CONST_TV_TEXTUREFILTER.TV_FILTER_BILINEAR)
        End With

        World.Initialize()

        Viewport.Show()

        Return True
    End Function

    Private Sub MainLoop()
        Do
            Application.DoEvents()

            InputManager.Update()
            World.Update()

            Engine.Clear()

            World.Render()

            World.PostRender()

            Engine.RenderToScreen()
        Loop Until StopLoop
    End Sub
End Module
