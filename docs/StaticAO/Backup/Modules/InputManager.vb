Imports MTV3D65
Imports Microsoft.VisualBasic

Module InputManager
    Private ReadOnly Inputs As TVInputEngine
    Private ReadOnly Camera As TVCamera
    Private ReadOnly Maths As TVMathLibrary

    Private Rotation As TV_2DVECTOR = New TV_2DVECTOR(0, -55)
    Private Position As TV_3DVECTOR
    Private Zoom As Single = 150

    Private Origin As TV_3DVECTOR = New TV_3DVECTOR(0, 45, 0)

    Sub New()
        Inputs = New TVInputEngine
        Camera = Engine.GetCamera
        Maths = New TVMathLibrary
    End Sub

    Sub Update()
        Dim dx As Integer, dy As Integer, wheel As Integer
        Dim b1 As Boolean, b2 As Boolean

        Inputs.GetMouseState(dx, dy, b1, b2, False, False, wheel)

        Zoom += wheel / 6.0F

        If b1 Then
            Rotation.x -= dy / 5.0F
            Rotation.y -= dx / 5.0F

            Rotation.x = Math.Min(Math.Max(Rotation.x, -85.0F), 85.0F)
        End If

        If b2 Then
            Camera.SetPosition(Origin.x, Origin.y, Origin.z)
            Camera.MoveRelative(0, dy / 7.0F, dx / 7.0F)
            Origin = Camera.GetPosition
        End If

        Position = Maths.MoveAroundPoint(Origin, Zoom, Rotation.y, Rotation.x)

        Camera.SetPosition(Position.x, Position.y, Position.z)
        Camera.SetLookAt(Origin.x, Origin.y, Origin.z)

        With Inputs
            Dim ShiftPressed As Boolean = .IsKeyPressed(CONST_TV_KEY.TV_KEY_LEFTSHIFT) OrElse .IsKeyPressed(CONST_TV_KEY.TV_KEY_RIGHTSHIFT)

            If .IsKeyPressed(CONST_TV_KEY.TV_KEY_ESCAPE) Then
                StopLoop = True
            End If

            If .IsKeyPressed(CONST_TV_KEY.TV_KEY_D) Then
                SetDiffuseOcclusionFactor += 0.1F / 10.0F * CSng(IIf(ShiftPressed, -1, 1))
            End If

            If .IsKeyPressed(CONST_TV_KEY.TV_KEY_1) Then
                SetLightEnabled(0) = True
            ElseIf .IsKeyPressed(CONST_TV_KEY.TV_KEY_F1) Then
                SetLightEnabled(0) = False
            End If

            If .IsKeyPressed(CONST_TV_KEY.TV_KEY_2) Then
                SetLightEnabled(1) = True
            ElseIf .IsKeyPressed(CONST_TV_KEY.TV_KEY_F2) Then
                SetLightEnabled(1) = False
            End If

            If .IsKeyPressed(CONST_TV_KEY.TV_KEY_3) Then
                SetLightEnabled(2) = True
            ElseIf .IsKeyPressed(CONST_TV_KEY.TV_KEY_F3) Then
                SetLightEnabled(2) = False
            End If

            If .IsKeyPressed(CONST_TV_KEY.TV_KEY_4) Then
                SetLightEnabled(3) = True
            ElseIf .IsKeyPressed(CONST_TV_KEY.TV_KEY_F4) Then
                SetLightEnabled(3) = False
            End If

            If .IsKeyPressed(CONST_TV_KEY.TV_KEY_NUMPAD1) Then
                SpecularPower = 1
            End If
            If .IsKeyPressed(CONST_TV_KEY.TV_KEY_NUMPAD2) Then
                SpecularPower = 2
            End If
            If .IsKeyPressed(CONST_TV_KEY.TV_KEY_NUMPAD3) Then
                SpecularPower = 4
            End If
            If .IsKeyPressed(CONST_TV_KEY.TV_KEY_NUMPAD4) Then
                SpecularPower = 8
            End If
            If .IsKeyPressed(CONST_TV_KEY.TV_KEY_NUMPAD5) Then
                SpecularPower = 16
            End If
            If .IsKeyPressed(CONST_TV_KEY.TV_KEY_NUMPAD6) Then
                SpecularPower = 32
            End If
            If .IsKeyPressed(CONST_TV_KEY.TV_KEY_NUMPAD7) Then
                SpecularPower = 64
            End If
            If .IsKeyPressed(CONST_TV_KEY.TV_KEY_NUMPAD8) Then
                SpecularPower = 128
            End If
            If .IsKeyPressed(CONST_TV_KEY.TV_KEY_NUMPAD9) Then
                SpecularPower = 256
            End If
        End With

    End Sub
End Module