<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()> _
Partial Class ProgressShower
    Inherits System.Windows.Forms.Form

    'Form overrides dispose to clean up the component list.
    <System.Diagnostics.DebuggerNonUserCode()> _
    Protected Overrides Sub Dispose(ByVal disposing As Boolean)
        Try
            If disposing AndAlso components IsNot Nothing Then
                components.Dispose()
            End If
        Finally
            MyBase.Dispose(disposing)
        End Try
    End Sub

    'Required by the Windows Form Designer
    Private components As System.ComponentModel.IContainer

    'NOTE: The following procedure is required by the Windows Form Designer
    'It can be modified using the Windows Form Designer.  
    'Do not modify it using the code editor.
    <System.Diagnostics.DebuggerStepThrough()> _
    Private Sub InitializeComponent()
        Me.GlobalActionProgress = New System.Windows.Forms.ProgressBar
        Me.Label1 = New System.Windows.Forms.Label
        Me.GlobalActionPercentComplete = New System.Windows.Forms.Label
        Me.CurrentTaskProgress = New System.Windows.Forms.ProgressBar
        Me.Label3 = New System.Windows.Forms.Label
        Me.CurrentTaskPercentComplete = New System.Windows.Forms.Label
        Me.GlobalActionName = New System.Windows.Forms.Label
        Me.CurrentTaskName = New System.Windows.Forms.Label
        Me.EstimatedTimeLeft = New System.Windows.Forms.Label
        Me.Panic = New System.Windows.Forms.Button
        Me.SuspendLayout()
        '
        'GlobalActionProgress
        '
        Me.GlobalActionProgress.Location = New System.Drawing.Point(12, 25)
        Me.GlobalActionProgress.Name = "GlobalActionProgress"
        Me.GlobalActionProgress.Size = New System.Drawing.Size(509, 39)
        Me.GlobalActionProgress.TabIndex = 0
        '
        'Label1
        '
        Me.Label1.AutoSize = True
        Me.Label1.Location = New System.Drawing.Point(12, 9)
        Me.Label1.Name = "Label1"
        Me.Label1.Size = New System.Drawing.Size(76, 13)
        Me.Label1.TabIndex = 1
        Me.Label1.Text = "Global Action :"
        '
        'GlobalActionPercentComplete
        '
        Me.GlobalActionPercentComplete.Location = New System.Drawing.Point(357, 67)
        Me.GlobalActionPercentComplete.Name = "GlobalActionPercentComplete"
        Me.GlobalActionPercentComplete.Size = New System.Drawing.Size(164, 13)
        Me.GlobalActionPercentComplete.TabIndex = 1
        Me.GlobalActionPercentComplete.Text = "0%"
        Me.GlobalActionPercentComplete.TextAlign = System.Drawing.ContentAlignment.TopRight
        '
        'CurrentTaskProgress
        '
        Me.CurrentTaskProgress.Location = New System.Drawing.Point(12, 92)
        Me.CurrentTaskProgress.Name = "CurrentTaskProgress"
        Me.CurrentTaskProgress.Size = New System.Drawing.Size(509, 23)
        Me.CurrentTaskProgress.TabIndex = 0
        '
        'Label3
        '
        Me.Label3.AutoSize = True
        Me.Label3.Location = New System.Drawing.Point(12, 76)
        Me.Label3.Name = "Label3"
        Me.Label3.Size = New System.Drawing.Size(74, 13)
        Me.Label3.TabIndex = 1
        Me.Label3.Text = "Current Task :"
        '
        'CurrentTaskPercentComplete
        '
        Me.CurrentTaskPercentComplete.Location = New System.Drawing.Point(357, 118)
        Me.CurrentTaskPercentComplete.Name = "CurrentTaskPercentComplete"
        Me.CurrentTaskPercentComplete.Size = New System.Drawing.Size(164, 13)
        Me.CurrentTaskPercentComplete.TabIndex = 1
        Me.CurrentTaskPercentComplete.Text = "0%"
        Me.CurrentTaskPercentComplete.TextAlign = System.Drawing.ContentAlignment.TopRight
        '
        'GlobalActionName
        '
        Me.GlobalActionName.AutoSize = True
        Me.GlobalActionName.Font = New System.Drawing.Font("Microsoft Sans Serif", 8.25!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.GlobalActionName.Location = New System.Drawing.Point(94, 9)
        Me.GlobalActionName.Name = "GlobalActionName"
        Me.GlobalActionName.Size = New System.Drawing.Size(45, 13)
        Me.GlobalActionName.TabIndex = 1
        Me.GlobalActionName.Text = "(None)"
        '
        'CurrentTaskName
        '
        Me.CurrentTaskName.AutoSize = True
        Me.CurrentTaskName.Font = New System.Drawing.Font("Microsoft Sans Serif", 8.25!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.CurrentTaskName.Location = New System.Drawing.Point(94, 76)
        Me.CurrentTaskName.Name = "CurrentTaskName"
        Me.CurrentTaskName.Size = New System.Drawing.Size(45, 13)
        Me.CurrentTaskName.TabIndex = 1
        Me.CurrentTaskName.Text = "(None)"
        '
        'EstimatedTimeLeft
        '
        Me.EstimatedTimeLeft.Location = New System.Drawing.Point(290, 9)
        Me.EstimatedTimeLeft.Name = "EstimatedTimeLeft"
        Me.EstimatedTimeLeft.Size = New System.Drawing.Size(231, 13)
        Me.EstimatedTimeLeft.TabIndex = 1
        Me.EstimatedTimeLeft.Text = "Estimated time left : (unknown)"
        Me.EstimatedTimeLeft.TextAlign = System.Drawing.ContentAlignment.TopRight
        '
        'Panic
        '
        Me.Panic.Location = New System.Drawing.Point(229, 144)
        Me.Panic.Name = "Panic"
        Me.Panic.Size = New System.Drawing.Size(75, 23)
        Me.Panic.TabIndex = 2
        Me.Panic.Text = "Panic!"
        Me.Panic.UseVisualStyleBackColor = True
        '
        'ProgressShower
        '
        Me.AutoScaleDimensions = New System.Drawing.SizeF(6.0!, 13.0!)
        Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
        Me.ClientSize = New System.Drawing.Size(533, 179)
        Me.ControlBox = False
        Me.Controls.Add(Me.Panic)
        Me.Controls.Add(Me.CurrentTaskPercentComplete)
        Me.Controls.Add(Me.EstimatedTimeLeft)
        Me.Controls.Add(Me.GlobalActionPercentComplete)
        Me.Controls.Add(Me.Label3)
        Me.Controls.Add(Me.CurrentTaskName)
        Me.Controls.Add(Me.GlobalActionName)
        Me.Controls.Add(Me.Label1)
        Me.Controls.Add(Me.CurrentTaskProgress)
        Me.Controls.Add(Me.GlobalActionProgress)
        Me.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow
        Me.Name = "ProgressShower"
        Me.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen
        Me.Text = "Computation in progress, please wait..."
        Me.ResumeLayout(False)
        Me.PerformLayout()

    End Sub
    Friend WithEvents GlobalActionProgress As System.Windows.Forms.ProgressBar
    Friend WithEvents Label1 As System.Windows.Forms.Label
    Friend WithEvents GlobalActionPercentComplete As System.Windows.Forms.Label
    Friend WithEvents CurrentTaskProgress As System.Windows.Forms.ProgressBar
    Friend WithEvents Label3 As System.Windows.Forms.Label
    Friend WithEvents CurrentTaskPercentComplete As System.Windows.Forms.Label
    Friend WithEvents GlobalActionName As System.Windows.Forms.Label
    Friend WithEvents CurrentTaskName As System.Windows.Forms.Label
    Friend WithEvents EstimatedTimeLeft As System.Windows.Forms.Label
    Friend WithEvents Panic As System.Windows.Forms.Button
End Class
