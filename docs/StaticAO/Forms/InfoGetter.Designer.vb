<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()> _
Partial Class InfoGetter
    Inherits System.Windows.Forms.Form

    'Form overrides dispose to clean up the component list.
    <System.Diagnostics.DebuggerNonUserCode()> _
    Protected Overrides Sub Dispose(ByVal disposing As Boolean)
        If disposing AndAlso components IsNot Nothing Then
            components.Dispose()
        End If
        MyBase.Dispose(disposing)
    End Sub

    'Required by the Windows Form Designer
    Private components As System.ComponentModel.IContainer

    'NOTE: The following procedure is required by the Windows Form Designer
    'It can be modified using the Windows Form Designer.  
    'Do not modify it using the code editor.
    <System.Diagnostics.DebuggerStepThrough()> _
    Private Sub InitializeComponent()
        Me.BetaKeyText = New System.Windows.Forms.TextBox
        Me.UsernameText = New System.Windows.Forms.TextBox
        Me.lblUsername = New System.Windows.Forms.Label
        Me.lblBetaKey = New System.Windows.Forms.Label
        Me.btnProceed = New System.Windows.Forms.Button
        Me.BetaInfoGroup = New System.Windows.Forms.GroupBox
        Me.SaveCheck = New System.Windows.Forms.CheckBox
        Me.btnExit = New System.Windows.Forms.Button
        Me.AmbientOcclusionSettings = New System.Windows.Forms.GroupBox
        Me.HemicubeChoice = New System.Windows.Forms.RadioButton
        Me.RaytracingChoice = New System.Windows.Forms.RadioButton
        Me.UseOcclusionFromTVMCheck = New System.Windows.Forms.CheckBox
        Me.SaveOcclusionInModelCheck = New System.Windows.Forms.CheckBox
        Me.TorusKnotChoice = New System.Windows.Forms.RadioButton
        Me.Text3DChoice = New System.Windows.Forms.RadioButton
        Me.RancorChoice = New System.Windows.Forms.RadioButton
        Me.ShipChoice = New System.Windows.Forms.RadioButton
        Me.SkullChoice = New System.Windows.Forms.RadioButton
        Me.Label4 = New System.Windows.Forms.Label
        Me.NumberOfThreadsChooser = New System.Windows.Forms.NumericUpDown
        Me.FloorSlicesChooser = New System.Windows.Forms.NumericUpDown
        Me.RaysPerVertexChooser = New System.Windows.Forms.NumericUpDown
        Me.Label1 = New System.Windows.Forms.Label
        Me.Label3 = New System.Windows.Forms.Label
        Me.RaysPerVertexLabel = New System.Windows.Forms.Label
        Me.BentNormalsCheck = New System.Windows.Forms.CheckBox
        Me.FsaaCheck = New System.Windows.Forms.CheckBox
        Me.SceneSettings = New System.Windows.Forms.GroupBox
        Me.RaytracerSettings = New System.Windows.Forms.GroupBox
        Me.HemicubeSettings = New System.Windows.Forms.GroupBox
        Me.SamplesPerVertexChooser = New System.Windows.Forms.NumericUpDown
        Me.Label6 = New System.Windows.Forms.Label
        Me.Label5 = New System.Windows.Forms.Label
        Me.Label7 = New System.Windows.Forms.Label
        Me.ActualSamplesPerVertexLabel = New System.Windows.Forms.Label
        Me.BetaInfoGroup.SuspendLayout()
        Me.AmbientOcclusionSettings.SuspendLayout()
        CType(Me.NumberOfThreadsChooser, System.ComponentModel.ISupportInitialize).BeginInit()
        CType(Me.FloorSlicesChooser, System.ComponentModel.ISupportInitialize).BeginInit()
        CType(Me.RaysPerVertexChooser, System.ComponentModel.ISupportInitialize).BeginInit()
        Me.SceneSettings.SuspendLayout()
        Me.RaytracerSettings.SuspendLayout()
        Me.HemicubeSettings.SuspendLayout()
        CType(Me.SamplesPerVertexChooser, System.ComponentModel.ISupportInitialize).BeginInit()
        Me.SuspendLayout()
        '
        'BetaKeyText
        '
        Me.BetaKeyText.Location = New System.Drawing.Point(11, 85)
        Me.BetaKeyText.Name = "BetaKeyText"
        Me.BetaKeyText.Size = New System.Drawing.Size(181, 21)
        Me.BetaKeyText.TabIndex = 2
        Me.BetaKeyText.UseSystemPasswordChar = True
        '
        'UsernameText
        '
        Me.UsernameText.Location = New System.Drawing.Point(11, 37)
        Me.UsernameText.Name = "UsernameText"
        Me.UsernameText.Size = New System.Drawing.Size(181, 21)
        Me.UsernameText.TabIndex = 1
        '
        'lblUsername
        '
        Me.lblUsername.AutoSize = True
        Me.lblUsername.Font = New System.Drawing.Font("Tahoma", 8.25!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.lblUsername.Location = New System.Drawing.Point(8, 21)
        Me.lblUsername.Name = "lblUsername"
        Me.lblUsername.Size = New System.Drawing.Size(55, 13)
        Me.lblUsername.TabIndex = 2
        Me.lblUsername.Text = "Username"
        '
        'lblBetaKey
        '
        Me.lblBetaKey.AutoSize = True
        Me.lblBetaKey.Font = New System.Drawing.Font("Tahoma", 8.25!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.lblBetaKey.Location = New System.Drawing.Point(8, 69)
        Me.lblBetaKey.Name = "lblBetaKey"
        Me.lblBetaKey.Size = New System.Drawing.Size(50, 13)
        Me.lblBetaKey.TabIndex = 3
        Me.lblBetaKey.Text = "Beta Key"
        '
        'btnProceed
        '
        Me.btnProceed.DialogResult = System.Windows.Forms.DialogResult.OK
        Me.btnProceed.Location = New System.Drawing.Point(258, 374)
        Me.btnProceed.Name = "btnProceed"
        Me.btnProceed.Size = New System.Drawing.Size(81, 23)
        Me.btnProceed.TabIndex = 8
        Me.btnProceed.Text = "Proceed"
        Me.btnProceed.UseVisualStyleBackColor = True
        '
        'BetaInfoGroup
        '
        Me.BetaInfoGroup.Controls.Add(Me.BetaKeyText)
        Me.BetaInfoGroup.Controls.Add(Me.UsernameText)
        Me.BetaInfoGroup.Controls.Add(Me.lblUsername)
        Me.BetaInfoGroup.Controls.Add(Me.lblBetaKey)
        Me.BetaInfoGroup.Location = New System.Drawing.Point(12, 12)
        Me.BetaInfoGroup.Name = "BetaInfoGroup"
        Me.BetaInfoGroup.Size = New System.Drawing.Size(205, 124)
        Me.BetaInfoGroup.TabIndex = 5
        Me.BetaInfoGroup.TabStop = False
        Me.BetaInfoGroup.Text = "Beta Information"
        '
        'SaveCheck
        '
        Me.SaveCheck.AutoSize = True
        Me.SaveCheck.Checked = True
        Me.SaveCheck.CheckState = System.Windows.Forms.CheckState.Checked
        Me.SaveCheck.Location = New System.Drawing.Point(12, 378)
        Me.SaveCheck.Name = "SaveCheck"
        Me.SaveCheck.Size = New System.Drawing.Size(162, 17)
        Me.SaveCheck.TabIndex = 6
        Me.SaveCheck.Text = "Save Info On This Computer"
        Me.SaveCheck.UseVisualStyleBackColor = True
        '
        'btnExit
        '
        Me.btnExit.DialogResult = System.Windows.Forms.DialogResult.Cancel
        Me.btnExit.Location = New System.Drawing.Point(345, 374)
        Me.btnExit.Name = "btnExit"
        Me.btnExit.Size = New System.Drawing.Size(80, 23)
        Me.btnExit.TabIndex = 7
        Me.btnExit.Text = "Exit"
        Me.btnExit.UseVisualStyleBackColor = True
        '
        'AmbientOcclusionSettings
        '
        Me.AmbientOcclusionSettings.Controls.Add(Me.HemicubeChoice)
        Me.AmbientOcclusionSettings.Controls.Add(Me.RaytracingChoice)
        Me.AmbientOcclusionSettings.Controls.Add(Me.UseOcclusionFromTVMCheck)
        Me.AmbientOcclusionSettings.Controls.Add(Me.SaveOcclusionInModelCheck)
        Me.AmbientOcclusionSettings.Location = New System.Drawing.Point(229, 12)
        Me.AmbientOcclusionSettings.Name = "AmbientOcclusionSettings"
        Me.AmbientOcclusionSettings.Size = New System.Drawing.Size(196, 124)
        Me.AmbientOcclusionSettings.TabIndex = 9
        Me.AmbientOcclusionSettings.TabStop = False
        Me.AmbientOcclusionSettings.Text = "Ambient Occlusion Settings"
        '
        'HemicubeChoice
        '
        Me.HemicubeChoice.AutoSize = True
        Me.HemicubeChoice.Location = New System.Drawing.Point(11, 43)
        Me.HemicubeChoice.Name = "HemicubeChoice"
        Me.HemicubeChoice.Size = New System.Drawing.Size(154, 17)
        Me.HemicubeChoice.TabIndex = 11
        Me.HemicubeChoice.Text = "Hemicube Rendering (GPU)"
        Me.HemicubeChoice.UseVisualStyleBackColor = True
        '
        'RaytracingChoice
        '
        Me.RaytracingChoice.AutoSize = True
        Me.RaytracingChoice.Location = New System.Drawing.Point(11, 20)
        Me.RaytracingChoice.Name = "RaytracingChoice"
        Me.RaytracingChoice.Size = New System.Drawing.Size(132, 17)
        Me.RaytracingChoice.TabIndex = 11
        Me.RaytracingChoice.Text = "Raytracing (Software)"
        Me.RaytracingChoice.UseVisualStyleBackColor = True
        '
        'UseOcclusionFromTVMCheck
        '
        Me.UseOcclusionFromTVMCheck.AutoSize = True
        Me.UseOcclusionFromTVMCheck.Checked = True
        Me.UseOcclusionFromTVMCheck.CheckState = System.Windows.Forms.CheckState.Checked
        Me.UseOcclusionFromTVMCheck.Location = New System.Drawing.Point(11, 72)
        Me.UseOcclusionFromTVMCheck.Name = "UseOcclusionFromTVMCheck"
        Me.UseOcclusionFromTVMCheck.Size = New System.Drawing.Size(142, 17)
        Me.UseOcclusionFromTVMCheck.TabIndex = 6
        Me.UseOcclusionFromTVMCheck.Text = "Use Occlusion From TVM"
        Me.UseOcclusionFromTVMCheck.UseVisualStyleBackColor = True
        '
        'SaveOcclusionInModelCheck
        '
        Me.SaveOcclusionInModelCheck.AutoSize = True
        Me.SaveOcclusionInModelCheck.Location = New System.Drawing.Point(11, 95)
        Me.SaveOcclusionInModelCheck.Name = "SaveOcclusionInModelCheck"
        Me.SaveOcclusionInModelCheck.Size = New System.Drawing.Size(134, 17)
        Me.SaveOcclusionInModelCheck.TabIndex = 6
        Me.SaveOcclusionInModelCheck.Text = "Save Occlusion In TVM"
        Me.SaveOcclusionInModelCheck.UseVisualStyleBackColor = True
        '
        'TorusKnotChoice
        '
        Me.TorusKnotChoice.AutoSize = True
        Me.TorusKnotChoice.Location = New System.Drawing.Point(11, 112)
        Me.TorusKnotChoice.Name = "TorusKnotChoice"
        Me.TorusKnotChoice.Size = New System.Drawing.Size(153, 17)
        Me.TorusKnotChoice.TabIndex = 11
        Me.TorusKnotChoice.Text = "Torus Knot (1573 vertices)"
        Me.TorusKnotChoice.UseVisualStyleBackColor = True
        '
        'Text3DChoice
        '
        Me.Text3DChoice.AutoSize = True
        Me.Text3DChoice.Location = New System.Drawing.Point(11, 89)
        Me.Text3DChoice.Name = "Text3DChoice"
        Me.Text3DChoice.Size = New System.Drawing.Size(139, 17)
        Me.Text3DChoice.TabIndex = 11
        Me.Text3DChoice.Text = "3D Text (3728 vertices)"
        Me.Text3DChoice.UseVisualStyleBackColor = True
        '
        'RancorChoice
        '
        Me.RancorChoice.AutoSize = True
        Me.RancorChoice.Location = New System.Drawing.Point(11, 66)
        Me.RancorChoice.Name = "RancorChoice"
        Me.RancorChoice.Size = New System.Drawing.Size(135, 17)
        Me.RancorChoice.TabIndex = 11
        Me.RancorChoice.Text = "Rancor (6300 vertices)"
        Me.RancorChoice.UseVisualStyleBackColor = True
        '
        'ShipChoice
        '
        Me.ShipChoice.AutoSize = True
        Me.ShipChoice.Location = New System.Drawing.Point(11, 43)
        Me.ShipChoice.Name = "ShipChoice"
        Me.ShipChoice.Size = New System.Drawing.Size(121, 17)
        Me.ShipChoice.TabIndex = 11
        Me.ShipChoice.Text = "Ship (4345 vertices)"
        Me.ShipChoice.UseVisualStyleBackColor = True
        '
        'SkullChoice
        '
        Me.SkullChoice.AutoSize = True
        Me.SkullChoice.Checked = True
        Me.SkullChoice.Location = New System.Drawing.Point(11, 20)
        Me.SkullChoice.Name = "SkullChoice"
        Me.SkullChoice.Size = New System.Drawing.Size(122, 17)
        Me.SkullChoice.TabIndex = 11
        Me.SkullChoice.TabStop = True
        Me.SkullChoice.Text = "Skull (1950 vertices)"
        Me.SkullChoice.UseVisualStyleBackColor = True
        '
        'Label4
        '
        Me.Label4.AutoSize = True
        Me.Label4.Location = New System.Drawing.Point(113, 147)
        Me.Label4.Name = "Label4"
        Me.Label4.Size = New System.Drawing.Size(12, 13)
        Me.Label4.TabIndex = 10
        Me.Label4.Text = "²"
        '
        'NumberOfThreadsChooser
        '
        Me.NumberOfThreadsChooser.Location = New System.Drawing.Point(142, 47)
        Me.NumberOfThreadsChooser.Maximum = New Decimal(New Integer() {4, 0, 0, 0})
        Me.NumberOfThreadsChooser.Minimum = New Decimal(New Integer() {1, 0, 0, 0})
        Me.NumberOfThreadsChooser.Name = "NumberOfThreadsChooser"
        Me.NumberOfThreadsChooser.Size = New System.Drawing.Size(36, 21)
        Me.NumberOfThreadsChooser.TabIndex = 9
        Me.NumberOfThreadsChooser.Value = New Decimal(New Integer() {1, 0, 0, 0})
        '
        'FloorSlicesChooser
        '
        Me.FloorSlicesChooser.Increment = New Decimal(New Integer() {5, 0, 0, 0})
        Me.FloorSlicesChooser.Location = New System.Drawing.Point(73, 147)
        Me.FloorSlicesChooser.Maximum = New Decimal(New Integer() {95, 0, 0, 0})
        Me.FloorSlicesChooser.Minimum = New Decimal(New Integer() {5, 0, 0, 0})
        Me.FloorSlicesChooser.Name = "FloorSlicesChooser"
        Me.FloorSlicesChooser.Size = New System.Drawing.Size(40, 21)
        Me.FloorSlicesChooser.TabIndex = 9
        Me.FloorSlicesChooser.Value = New Decimal(New Integer() {40, 0, 0, 0})
        '
        'RaysPerVertexChooser
        '
        Me.RaysPerVertexChooser.Location = New System.Drawing.Point(142, 20)
        Me.RaysPerVertexChooser.Maximum = New Decimal(New Integer() {5, 0, 0, 0})
        Me.RaysPerVertexChooser.Name = "RaysPerVertexChooser"
        Me.RaysPerVertexChooser.Size = New System.Drawing.Size(36, 21)
        Me.RaysPerVertexChooser.TabIndex = 9
        Me.RaysPerVertexChooser.Value = New Decimal(New Integer() {4, 0, 0, 0})
        '
        'Label1
        '
        Me.Label1.AutoSize = True
        Me.Label1.Location = New System.Drawing.Point(8, 49)
        Me.Label1.Name = "Label1"
        Me.Label1.Size = New System.Drawing.Size(120, 13)
        Me.Label1.TabIndex = 8
        Me.Label1.Text = "Number of CPU threads"
        '
        'Label3
        '
        Me.Label3.AutoSize = True
        Me.Label3.Location = New System.Drawing.Point(8, 149)
        Me.Label3.Name = "Label3"
        Me.Label3.Size = New System.Drawing.Size(59, 13)
        Me.Label3.TabIndex = 8
        Me.Label3.Text = "Floor slices"
        '
        'RaysPerVertexLabel
        '
        Me.RaysPerVertexLabel.AutoSize = True
        Me.RaysPerVertexLabel.Location = New System.Drawing.Point(6, 22)
        Me.RaysPerVertexLabel.Name = "RaysPerVertexLabel"
        Me.RaysPerVertexLabel.Size = New System.Drawing.Size(111, 13)
        Me.RaysPerVertexLabel.TabIndex = 8
        Me.RaysPerVertexLabel.Text = "Hemisphere iterations"
        '
        'BentNormalsCheck
        '
        Me.BentNormalsCheck.AutoSize = True
        Me.BentNormalsCheck.Location = New System.Drawing.Point(11, 76)
        Me.BentNormalsCheck.Name = "BentNormalsCheck"
        Me.BentNormalsCheck.Size = New System.Drawing.Size(136, 17)
        Me.BentNormalsCheck.TabIndex = 6
        Me.BentNormalsCheck.Text = "Calculate Bent Normals"
        Me.BentNormalsCheck.UseVisualStyleBackColor = True
        '
        'FsaaCheck
        '
        Me.FsaaCheck.AutoSize = True
        Me.FsaaCheck.Location = New System.Drawing.Point(11, 183)
        Me.FsaaCheck.Name = "FsaaCheck"
        Me.FsaaCheck.Size = New System.Drawing.Size(181, 17)
        Me.FsaaCheck.TabIndex = 6
        Me.FsaaCheck.Text = "Use Full-Scene Anti-Aliasing (4x)"
        Me.FsaaCheck.UseVisualStyleBackColor = True
        '
        'SceneSettings
        '
        Me.SceneSettings.Controls.Add(Me.Label4)
        Me.SceneSettings.Controls.Add(Me.TorusKnotChoice)
        Me.SceneSettings.Controls.Add(Me.SkullChoice)
        Me.SceneSettings.Controls.Add(Me.Text3DChoice)
        Me.SceneSettings.Controls.Add(Me.FloorSlicesChooser)
        Me.SceneSettings.Controls.Add(Me.ShipChoice)
        Me.SceneSettings.Controls.Add(Me.FsaaCheck)
        Me.SceneSettings.Controls.Add(Me.RancorChoice)
        Me.SceneSettings.Controls.Add(Me.Label3)
        Me.SceneSettings.Location = New System.Drawing.Point(12, 142)
        Me.SceneSettings.Name = "SceneSettings"
        Me.SceneSettings.Size = New System.Drawing.Size(205, 214)
        Me.SceneSettings.TabIndex = 10
        Me.SceneSettings.TabStop = False
        Me.SceneSettings.Text = "Scene Settings"
        '
        'RaytracerSettings
        '
        Me.RaytracerSettings.Controls.Add(Me.RaysPerVertexChooser)
        Me.RaytracerSettings.Controls.Add(Me.RaysPerVertexLabel)
        Me.RaytracerSettings.Controls.Add(Me.BentNormalsCheck)
        Me.RaytracerSettings.Controls.Add(Me.Label1)
        Me.RaytracerSettings.Controls.Add(Me.NumberOfThreadsChooser)
        Me.RaytracerSettings.Location = New System.Drawing.Point(229, 142)
        Me.RaytracerSettings.Name = "RaytracerSettings"
        Me.RaytracerSettings.Size = New System.Drawing.Size(196, 106)
        Me.RaytracerSettings.TabIndex = 11
        Me.RaytracerSettings.TabStop = False
        Me.RaytracerSettings.Text = "Raytracer Settings"
        '
        'HemicubeSettings
        '
        Me.HemicubeSettings.Controls.Add(Me.SamplesPerVertexChooser)
        Me.HemicubeSettings.Controls.Add(Me.Label6)
        Me.HemicubeSettings.Controls.Add(Me.Label5)
        Me.HemicubeSettings.Location = New System.Drawing.Point(229, 254)
        Me.HemicubeSettings.Name = "HemicubeSettings"
        Me.HemicubeSettings.Size = New System.Drawing.Size(196, 56)
        Me.HemicubeSettings.TabIndex = 12
        Me.HemicubeSettings.TabStop = False
        Me.HemicubeSettings.Text = "Hemicube Settings"
        '
        'SamplesPerVertexChooser
        '
        Me.SamplesPerVertexChooser.Location = New System.Drawing.Point(142, 24)
        Me.SamplesPerVertexChooser.Maximum = New Decimal(New Integer() {7, 0, 0, 0})
        Me.SamplesPerVertexChooser.Minimum = New Decimal(New Integer() {2, 0, 0, 0})
        Me.SamplesPerVertexChooser.Name = "SamplesPerVertexChooser"
        Me.SamplesPerVertexChooser.Size = New System.Drawing.Size(36, 21)
        Me.SamplesPerVertexChooser.TabIndex = 9
        Me.SamplesPerVertexChooser.Value = New Decimal(New Integer() {3, 0, 0, 0})
        '
        'Label6
        '
        Me.Label6.AutoSize = True
        Me.Label6.Location = New System.Drawing.Point(8, 26)
        Me.Label6.Name = "Label6"
        Me.Label6.Size = New System.Drawing.Size(137, 13)
        Me.Label6.TabIndex = 8
        Me.Label6.Text = "Samples/vertex    = 3×(2^"
        '
        'Label5
        '
        Me.Label5.AutoSize = True
        Me.Label5.Location = New System.Drawing.Point(177, 26)
        Me.Label5.Name = "Label5"
        Me.Label5.Size = New System.Drawing.Size(16, 13)
        Me.Label5.TabIndex = 10
        Me.Label5.Text = ")²"
        '
        'Label7
        '
        Me.Label7.AutoSize = True
        Me.Label7.ForeColor = System.Drawing.Color.DimGray
        Me.Label7.Location = New System.Drawing.Point(233, 325)
        Me.Label7.Name = "Label7"
        Me.Label7.Size = New System.Drawing.Size(139, 13)
        Me.Label7.TabIndex = 8
        Me.Label7.Text = "Actual samples per vertex :"
        '
        'ActualSamplesPerVertexLabel
        '
        Me.ActualSamplesPerVertexLabel.AutoSize = True
        Me.ActualSamplesPerVertexLabel.ForeColor = System.Drawing.Color.DimGray
        Me.ActualSamplesPerVertexLabel.Location = New System.Drawing.Point(368, 325)
        Me.ActualSamplesPerVertexLabel.Name = "ActualSamplesPerVertexLabel"
        Me.ActualSamplesPerVertexLabel.Size = New System.Drawing.Size(13, 13)
        Me.ActualSamplesPerVertexLabel.TabIndex = 8
        Me.ActualSamplesPerVertexLabel.Text = "0"
        '
        'InfoGetter
        '
        Me.AcceptButton = Me.btnProceed
        Me.AutoScaleDimensions = New System.Drawing.SizeF(6.0!, 13.0!)
        Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
        Me.CancelButton = Me.btnExit
        Me.ClientSize = New System.Drawing.Size(437, 409)
        Me.Controls.Add(Me.HemicubeSettings)
        Me.Controls.Add(Me.ActualSamplesPerVertexLabel)
        Me.Controls.Add(Me.Label7)
        Me.Controls.Add(Me.RaytracerSettings)
        Me.Controls.Add(Me.SceneSettings)
        Me.Controls.Add(Me.AmbientOcclusionSettings)
        Me.Controls.Add(Me.btnExit)
        Me.Controls.Add(Me.BetaInfoGroup)
        Me.Controls.Add(Me.btnProceed)
        Me.Controls.Add(Me.SaveCheck)
        Me.Font = New System.Drawing.Font("Tahoma", 8.25!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle
        Me.MaximizeBox = False
        Me.Name = "InfoGetter"
        Me.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen
        Me.Text = "Information Getter"
        Me.BetaInfoGroup.ResumeLayout(False)
        Me.BetaInfoGroup.PerformLayout()
        Me.AmbientOcclusionSettings.ResumeLayout(False)
        Me.AmbientOcclusionSettings.PerformLayout()
        CType(Me.NumberOfThreadsChooser, System.ComponentModel.ISupportInitialize).EndInit()
        CType(Me.FloorSlicesChooser, System.ComponentModel.ISupportInitialize).EndInit()
        CType(Me.RaysPerVertexChooser, System.ComponentModel.ISupportInitialize).EndInit()
        Me.SceneSettings.ResumeLayout(False)
        Me.SceneSettings.PerformLayout()
        Me.RaytracerSettings.ResumeLayout(False)
        Me.RaytracerSettings.PerformLayout()
        Me.HemicubeSettings.ResumeLayout(False)
        Me.HemicubeSettings.PerformLayout()
        CType(Me.SamplesPerVertexChooser, System.ComponentModel.ISupportInitialize).EndInit()
        Me.ResumeLayout(False)
        Me.PerformLayout()

    End Sub
    Friend WithEvents BetaKeyText As System.Windows.Forms.TextBox
    Friend WithEvents UsernameText As System.Windows.Forms.TextBox
    Friend WithEvents lblUsername As System.Windows.Forms.Label
    Friend WithEvents lblBetaKey As System.Windows.Forms.Label
    Friend WithEvents btnProceed As System.Windows.Forms.Button
    Friend WithEvents BetaInfoGroup As System.Windows.Forms.GroupBox
    Friend WithEvents SaveCheck As System.Windows.Forms.CheckBox
    Friend WithEvents btnExit As System.Windows.Forms.Button
    Friend WithEvents AmbientOcclusionSettings As System.Windows.Forms.GroupBox
    Friend WithEvents RaysPerVertexChooser As System.Windows.Forms.NumericUpDown
    Friend WithEvents RaysPerVertexLabel As System.Windows.Forms.Label
    Friend WithEvents Text3DChoice As System.Windows.Forms.RadioButton
    Friend WithEvents ShipChoice As System.Windows.Forms.RadioButton
    Friend WithEvents SkullChoice As System.Windows.Forms.RadioButton
    Friend WithEvents TorusKnotChoice As System.Windows.Forms.RadioButton
    Friend WithEvents Label4 As System.Windows.Forms.Label
    Friend WithEvents FloorSlicesChooser As System.Windows.Forms.NumericUpDown
    Friend WithEvents Label3 As System.Windows.Forms.Label
    Friend WithEvents SaveOcclusionInModelCheck As System.Windows.Forms.CheckBox
    Friend WithEvents UseOcclusionFromTVMCheck As System.Windows.Forms.CheckBox
    Friend WithEvents NumberOfThreadsChooser As System.Windows.Forms.NumericUpDown
    Friend WithEvents Label1 As System.Windows.Forms.Label
    Friend WithEvents FsaaCheck As System.Windows.Forms.CheckBox
    Friend WithEvents RancorChoice As System.Windows.Forms.RadioButton
    Friend WithEvents BentNormalsCheck As System.Windows.Forms.CheckBox
    Friend WithEvents HemicubeChoice As System.Windows.Forms.RadioButton
    Friend WithEvents RaytracingChoice As System.Windows.Forms.RadioButton
    Friend WithEvents SceneSettings As System.Windows.Forms.GroupBox
    Friend WithEvents RaytracerSettings As System.Windows.Forms.GroupBox
    Friend WithEvents HemicubeSettings As System.Windows.Forms.GroupBox
    Friend WithEvents Label6 As System.Windows.Forms.Label
    Friend WithEvents SamplesPerVertexChooser As System.Windows.Forms.NumericUpDown
    Friend WithEvents Label5 As System.Windows.Forms.Label
    Friend WithEvents Label7 As System.Windows.Forms.Label
    Friend WithEvents ActualSamplesPerVertexLabel As System.Windows.Forms.Label
End Class
