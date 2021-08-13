Public Class Viewport

    Private Sub Viewport_FormClosed(ByVal sender As Object, ByVal e As System.Windows.Forms.FormClosedEventArgs) Handles Me.FormClosed
        StopLoop = True
    End Sub

End Class
