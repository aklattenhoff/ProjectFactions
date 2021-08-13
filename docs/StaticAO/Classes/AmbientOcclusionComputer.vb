Imports MTV3D65

Public MustInherit Class AmbientOcclusionComputer
    ' Those values define the range of casted rays
    Protected Const NEAR_PLANE As Single = 0.01
    Protected Const FAR_PLANE As Single = 500

    Protected Shared ReadOnly Maths As TVMathLibrary
    Protected Shared ReadOnly Globals As TVGlobals

    Protected ReadOnly World As TV_3DMATRIX
    Protected ReadOnly WorldInversedTransposed As TV_3DMATRIX
    Protected ReadOnly Progress As ProgressShower
    Protected ReadOnly Mesh As TVMesh

    Shared Sub New()
        Maths = New TVMathLibrary
        Globals = New TVGlobals
    End Sub

    Protected Sub New(ByVal Mesh As TVMesh, ByVal Progress As ProgressShower)
        Me.Mesh = Mesh
        Me.Progress = Progress

        ' World and WorldIT matrices
        World = Mesh.GetMatrix
        Maths.TVMatrixInverse(WorldInversedTransposed, 0, World)
        Maths.TVMatrixTranspose(WorldInversedTransposed, WorldInversedTransposed)
        ' Clear the last column to avoid translations
        With WorldInversedTransposed
            .m14 = 0
            .m24 = 0
            .m34 = 0
            .m44 = 1
        End With
    End Sub

    MustOverride Sub Execute()
End Class