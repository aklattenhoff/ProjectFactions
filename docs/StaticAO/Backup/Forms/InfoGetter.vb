Imports System.IO
Imports System.Xml.Serialization

Public Class InfoGetter
    Private mSettings As Settings
    Private mTempFilePath As String = Path.Combine(Application.StartupPath, "AmbientOcclusionInfo.xml")

    Sub New()
        ' This call is required by the Windows Form Designer.
        InitializeComponent()

        ' Add any initialization after the InitializeComponent() call.
        mSettings = New Settings(True)

        LoadInfo()
        With mSettings
            UsernameText.Text = .Username
            BetaKeyText.Text = .BetaKey
            RaysPerVertexChooser.Value = .RaysPerVertex
            FloorSlicesChooser.Value = .FloorSlices
            NumberOfThreadsChooser.Value = .NumberOfThreads
            SaveOcclusionInModelCheck.Checked = .SaveOcclusionInModel
            UseOcclusionFromTVMCheck.Checked = .LoadOcclusionFromModel
            FsaaCheck.Checked = .Fsaa
            BentNormalsCheck.Checked = .BentNormals
            SamplesPerVertexChooser.Value = .SamplesPerVertex

            If .OcclusionComputer = AmbientOcclusionComputers.Raytracing Then
                RaytracingChoice.Checked = True
                HemicubeSettings.Enabled = False
            ElseIf .OcclusionComputer = AmbientOcclusionComputers.HemicubeRendering Then
                HemicubeChoice.Checked = True
                RaytracerSettings.Enabled = False
            End If

            Select Case .ChosenMesh
                Case MeshChoices.Skull
                    SkullChoice.Checked = True
                Case MeshChoices.Ship
                    ShipChoice.Checked = True
                Case MeshChoices.Text3D
                    Text3DChoice.Checked = True
                Case MeshChoices.TorusKnot
                    TorusKnotChoice.Checked = True
                Case MeshChoices.Rancor
                    RancorChoice.Checked = True
            End Select
        End With
    End Sub

    Private Sub btnProceed_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles btnProceed.Click
        If SaveCheck.Checked Then
            With mSettings
                .Username = UsernameText.Text
                .BetaKey = BetaKeyText.Text
                .RaysPerVertex = CInt(RaysPerVertexChooser.Value)
                .FloorSlices = CInt(FloorSlicesChooser.Value)
                .SaveOcclusionInModel = SaveOcclusionInModelCheck.Checked
                .LoadOcclusionFromModel = UseOcclusionFromTVMCheck.Checked
                .NumberOfThreads = CInt(NumberOfThreadsChooser.Value)
                .Fsaa = FsaaCheck.Checked
                .BentNormals = BentNormalsCheck.Checked
                .SamplesPerVertex = CInt(SamplesPerVertexChooser.Value)

                If RaytracingChoice.Checked Then
                    .OcclusionComputer = AmbientOcclusionComputers.Raytracing
                ElseIf HemicubeChoice.Checked Then
                    .OcclusionComputer = AmbientOcclusionComputers.HemicubeRendering
                End If

                If SkullChoice.Checked Then
                    .ChosenMesh = MeshChoices.Skull
                ElseIf ShipChoice.Checked Then
                    .ChosenMesh = MeshChoices.Ship
                ElseIf Text3DChoice.Checked Then
                    .ChosenMesh = MeshChoices.Text3D
                ElseIf TorusKnotChoice.Checked Then
                    .ChosenMesh = MeshChoices.TorusKnot
                ElseIf RancorChoice.Checked Then
                    .ChosenMesh = MeshChoices.Rancor
                End If
            End With

            SaveInfo()
        End If
    End Sub

    Private Sub LoadInfo()
        If File.Exists(mTempFilePath) Then
            Dim fs As New FileStream(mTempFilePath, FileMode.Open)

            Try
                Dim serializer As New XmlSerializer(GetType(Settings))
                mSettings = DirectCast(serializer.Deserialize(fs), Settings)
            Catch ex As Exception
                MessageBox.Show("Error during file loading (""" + ex.Message + """). Settings will not be loaded.")
            End Try

            fs.Close()
            fs = Nothing
        End If
    End Sub

    Private Sub SaveInfo()
        Dim fs As New FileStream(mTempFilePath, FileMode.Create)

        Try
            Dim serializer As New XmlSerializer(GetType(Settings))
            serializer.Serialize(fs, mSettings)
        Catch ex As Exception
            MessageBox.Show("Error during file saving (""" + ex.Message + """). Settings will not be saved.")
        End Try

        fs.Close()
        fs = Nothing
    End Sub

    ReadOnly Property GetSettings() As Settings
        Get
            Return mSettings
        End Get
    End Property

    ''' <summary>
    ''' The private, serializable class to contain the beta info
    ''' </summary>
    Public Structure Settings
        Public Username As String
        Public BetaKey As String
        Public FloorSlices As Integer
        Public RaysPerVertex As Integer
        Public ChosenMesh As MeshChoices
        Public SaveOcclusionInModel As Boolean
        Public LoadOcclusionFromModel As Boolean
        Public NumberOfThreads As Integer
        Public Fsaa As Boolean
        Public BentNormals As Boolean
        Public OcclusionComputer As AmbientOcclusionComputers
        Public SamplesPerVertex As Integer

        Sub New(ByVal InitializeDefaultValues As Boolean)
            If InitializeDefaultValues Then
                FloorSlices = 40
                RaysPerVertex = 4
                ChosenMesh = MeshChoices.Skull
                LoadOcclusionFromModel = True
                NumberOfThreads = 1
                OcclusionComputer = AmbientOcclusionComputers.Raytracing
                SamplesPerVertex = 3
            End If
        End Sub
    End Structure

    Private Sub UseOcclusionFromTVMCheck_CheckedChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles UseOcclusionFromTVMCheck.CheckedChanged
        SaveOcclusionInModelCheck.Enabled = Not UseOcclusionFromTVMCheck.Checked
    End Sub

    Private Sub BentNormalsWarning(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles SaveOcclusionInModelCheck.CheckedChanged, BentNormalsCheck.CheckedChanged
        If SaveOcclusionInModelCheck.Checked AndAlso BentNormalsCheck.Checked Then
            MessageBox.Show("Bent normals will over-write the mesh's vertex normals with the direction of least occlusion. Make sure to test it without saving first, and make a backup of the mesh with standard normals.", _
                            "Warning", MessageBoxButtons.OK, MessageBoxIcon.Warning)
        End If
    End Sub

    Private Sub RaytracingChoice_CheckedChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles RaytracingChoice.CheckedChanged
        RaytracerSettings.Enabled = RaytracingChoice.Checked
        RaysPerVertexChooser_ValueChanged(Nothing, Nothing)
    End Sub

    Private Sub HemicubeChoice_CheckedChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles HemicubeChoice.CheckedChanged
        HemicubeSettings.Enabled = HemicubeChoice.Checked
        SamplesPerVertexChooser_ValueChanged(Nothing, Nothing)
    End Sub

    Private Sub RaysPerVertexChooser_ValueChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles RaysPerVertexChooser.ValueChanged
        Static VertexCounts() As Integer = New Integer() {5, 13, 41, 145, 545, 2113}
        ActualSamplesPerVertexLabel.Text = CStr(VertexCounts(CInt(RaysPerVertexChooser.Value)))
    End Sub

    Private Sub SamplesPerVertexChooser_ValueChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles SamplesPerVertexChooser.ValueChanged
        ActualSamplesPerVertexLabel.Text = CStr(3 * (2 ^ SamplesPerVertexChooser.Value) ^ 2)
    End Sub
End Class