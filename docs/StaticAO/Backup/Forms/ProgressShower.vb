Imports System.Threading
Imports Microsoft.VisualBasic

Public Class ProgressShower
    Private Tasks As Dictionary(Of String, Integer)
    Private Current As String
    Public Panicked As Boolean

    Private FirstTime As DateTime
    Private TimeLeft As TimeSpan

    Public Sub New(ByVal ActionName As String, ByVal Tasks As IEnumerable(Of String))
        ' This call is required by the Windows Form Designer.
        InitializeComponent()

        GlobalActionName.Text = ActionName
        Me.Tasks = New Dictionary(Of String, Integer)
        For Each TaskName As String In Tasks
            Me.Tasks.Add(TaskName, 0)
        Next
        FirstTime = DateTime.Now
    End Sub

    Sub DecrementRemainingTime()
        While GlobalActionProgress.Value < 100 AndAlso TimeLeft.TotalSeconds > 0 AndAlso Not Panicked
            TimeLeft = TimeLeft.Subtract(TimeSpan.FromSeconds(1))
            EstimatedTimeLeft.Text = String.Format("Estimated time left : {0} minutes and {1} seconds.", TimeLeft.Minutes, TimeLeft.Seconds)
            Thread.Sleep(1000)
        End While

        Thread.CurrentThread.Abort()
    End Sub

    Public WriteOnly Property CurrentTask() As String
        Set(ByVal value As String)
            Current = value
            CurrentTaskName.Text = Current
            Application.DoEvents()
        End Set
    End Property

    Public WriteOnly Property TaskProgress() As Integer
        Set(ByVal value As Integer)
            Static ProgressCount As Integer = 0

            If Tasks(Current) <> value Then
                Tasks(Current) = value
                CurrentTaskProgress.Value = CInt(IIf(value > 100, 100, value))
                CurrentTaskPercentComplete.Text = value & "%"

                Dim TotalComplete As Single
                For Each CompletedValue As Integer In Tasks.Values
                    TotalComplete += CompletedValue
                Next
                TotalComplete /= Tasks.Count
                GlobalActionPercentComplete.Text = CInt(TotalComplete) & "%"
                GlobalActionProgress.Value = CInt(TotalComplete)

                ProgressCount += 1
                If ProgressCount = 10 Then
                    CheckTimeRemaning()
                    ProgressCount = 0
                End If
            End If

            Application.DoEvents()
        End Set
    End Property

    Private Sub CheckTimeRemaning()
        Static LastTotal As Integer = 0
        Static LastTime As DateTime = FirstTime
        Static TimeDecrementer As New Thread(AddressOf DecrementRemainingTime)

        If TimeDecrementer.ThreadState = ThreadState.Unstarted Then TimeDecrementer.Start()

        Dim TotalComplete As Integer = GlobalActionProgress.Value

        If TotalComplete <> LastTotal Then
            TimeLeft = New TimeSpan(CLng(DateTime.Now.Subtract(LastTime).Ticks / (TotalComplete - LastTotal) * (100 - TotalComplete)))
            EstimatedTimeLeft.Text = String.Format("Estimated time left : {0} minutes and {1} seconds.", TimeLeft.Minutes, TimeLeft.Seconds)

            LastTime = DateTime.Now
            LastTotal = TotalComplete
        End If
    End Sub

    Private Sub Panic_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Panic.Click
        Panicked = True
    End Sub
End Class