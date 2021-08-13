Imports MTV3D65
Imports system.threading

Class RaytracingComputer
    Inherits AmbientOcclusionComputer

    Private ReadOnly SphereIterations As Integer
    Private ReadOnly SceneMeshes As IEnumerable(Of TVMesh)
    Private ReadOnly Processors As Integer

    Private VertexGettingMutex As New Mutex()
    Private VertexSettingMutex As New Mutex()

    Private CurrentVertexID As Integer = 0
    Private VertexWorkers As List(Of Thread)

    Public Sub New(ByVal Mesh As TVMesh, ByVal SceneMeshes As IEnumerable(Of TVMesh), ByVal Progress As ProgressShower, Optional ByVal SphereIterations As Integer = 2, Optional ByVal ProcessorsCount As Integer = 1)
        MyBase.New(Mesh, Progress)

        Me.SphereIterations = SphereIterations
        Me.SceneMeshes = SceneMeshes
        Me.Processors = ProcessorsCount
    End Sub

    Public Overrides Sub Execute()
        ' Set collision for them to recieve rays
        For Each M As TVMesh In SceneMeshes
            M.SetCollisionEnable(True)
        Next

        ' Start the processors
        VertexWorkers = New List(Of Thread)
        For i As Integer = 0 To Processors - 1
            VertexWorkers.Add(New Thread(AddressOf ProcessVertex))
            VertexWorkers(i).Name = "Vertex Worker " & i
            VertexWorkers(i).Start()
        Next

        ' Wait for all them to finish, and don't exit before then
        Do
            Dim TrueForAll As Boolean = True
            For Each VertexProcessor As Thread In VertexWorkers
                TrueForAll = TrueForAll AndAlso VertexProcessor.ThreadState = ThreadState.Stopped
            Next
            If TrueForAll Then Exit Do

            Thread.Sleep(100)

            VertexSettingMutex.WaitOne()
            VertexGettingMutex.WaitOne()
            Progress.TaskProgress = CInt(CurrentVertexID / (Mesh.GetVertexCount - 1) * 100)
            VertexGettingMutex.ReleaseMutex()
            VertexSettingMutex.ReleaseMutex()
        Loop
    End Sub

    ' Gets a new vertex for processing
    Private Function GetNextVertex(ByRef id As Integer, ByRef x As Single, ByRef y As Single, ByRef z As Single, ByRef nx As Single, ByRef ny As Single, ByRef nz As Single, ByRef tu1 As Single, ByRef tv1 As Single) As Boolean
        If Progress.Panicked OrElse CurrentVertexID = Mesh.GetVertexCount - 1 Then
            Return False
        End If

        VertexGettingMutex.WaitOne()

        Mesh.GetVertex(CurrentVertexID, x, y, z, nx, ny, nz, tu1, tv1, 0, 0, 0)
        id = CurrentVertexID

        CurrentVertexID += 1

        VertexGettingMutex.ReleaseMutex()

        Return True
    End Function

    ' Get and process a fresh vertex
    Public Sub ProcessVertex()
        Dim Id As Integer
        Dim X As Single, Y As Single, Z As Single
        Dim Nx As Single, Ny As Single, Nz As Single
        Dim Tu1 As Single, Tv1 As Single

        ' Build up a half-geosphere of sample points
        Dim SamplePoints As IList(Of TV_3DVECTOR) = OctahedronSphereDistribution(SphereIterations)

        ' Occlusion per ray hit
        Dim OcclusionFactorPerRay As Double = 1 / SamplePoints.Count

        While GetNextVertex(Id, X, Y, Z, Nx, Ny, Nz, Tu1, Tv1)
            Dim Position As New TV_3DVECTOR(X, Y, Z)
            ' Transform the position from local space to world space
            Position = World * Position
            Dim Normal As New TV_3DVECTOR(Nx, Ny, Nz)
            ' Transform the normal from local space to world space
            ' More details on this transformation : http://wiki.truevision3d.com/hlsl_phong_sample#calculating_the_normal_vector
            Normal = WorldInversedTransposed * Normal
            Maths.TVVec3Normalize(Normal, Normal)

            ' We need to find the two basis vectors of the "normal plane",
            ' which we will then use, along with the normal, to find an arbitrary number of
            ' interpolated rays in a 180 degrees area.
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

            ' Build a rotation matrix that represents the vertex's orientation
            ' Rows 2 and 3 are inverted because the sample points are oriented on Z instead of Y
            Dim RotationMatrix As TV_3DMATRIX
            With RotationMatrix
                .m11 = Right.x : .m12 = Right.y : .m13 = Right.z
                .m21 = Up.x : .m22 = Up.y : .m23 = Up.z
                .m31 = Normal.x : .m32 = Normal.y : .m33 = Normal.z
                .m44 = 1
            End With

            Dim DirectionOfLeastOcclusion As TV_3DVECTOR
            Dim OcclusionFactor As Double = 0

            ' Fetch the sample points from the half-geosphere
            For Each SamplePoint As TV_3DVECTOR In SamplePoints
                ' Transform it with the vertex's orientation
                Dim WorldSpaceRay As TV_3DVECTOR = RotationMatrix * SamplePoint

                If RayCollision(Position + WorldSpaceRay * NEAR_PLANE, Position + WorldSpaceRay * FAR_PLANE) Then
                    ' If something was in the way, record it
                    OcclusionFactor += OcclusionFactorPerRay
                ElseIf Settings.BentNormals Then
                    ' Calculate bent normal if wanted
                    DirectionOfLeastOcclusion += WorldSpaceRay
                End If
            Next

            ' Calculate the bent normals if wanted
            If Settings.BentNormals Then
                Maths.TVVec3Normalize(DirectionOfLeastOcclusion, DirectionOfLeastOcclusion)

                Dim BentNormal As TV_4DVECTOR = New TV_4DVECTOR(DirectionOfLeastOcclusion.x, DirectionOfLeastOcclusion.y, DirectionOfLeastOcclusion.z, 0) * World
                Maths.TVVec4Normalize(BentNormal, BentNormal)
                Nx = BentNormal.x
                Ny = BentNormal.y
                Nz = BentNormal.z
            End If

            Dim Color As Integer = Globals.RGBA(1 - CSng(OcclusionFactor), 1 - CSng(OcclusionFactor), 1 - CSng(OcclusionFactor), 1)
            VertexSettingMutex.WaitOne()
            Mesh.SetVertex(Id, X, Y, Z, Nx, Ny, Nz, Tu1, Tv1, 0, 0, Color)
            VertexSettingMutex.ReleaseMutex()
        End While
    End Sub

    ' Builds a geosphere with near-perfect triangle distribution out of an octahedron
    ' by subdivising its edges by the supplied number of iterations
    ' Returns the list of vertices contained in the upper half of the geosphere
    ' Note - The geosphere is oriented on Z, or as if Z was the vertical axis
    ' See : http://local.wasp.uwa.edu.au/~pbourke/modelling_rendering/sphere_cylinder/
    Public Function OctahedronSphereDistribution(ByVal Iterations As Integer) As IList(Of TV_3DVECTOR)
        Dim Vertices As New List(Of TV_3DVECTOR)
        Dim Indices As New List(Of TV_3DVECTOR())

        Dim A As Single = CSng(1 / Math.Sqrt(2))
        Dim P() As TV_3DVECTOR = { _
            New TV_3DVECTOR(0, 0, 1), New TV_3DVECTOR(0, 0, -1), New TV_3DVECTOR(-A, -A, 0), _
            New TV_3DVECTOR(A, -A, 0), New TV_3DVECTOR(A, A, 0), New TV_3DVECTOR(-A, A, 0) _
        }

        Indices.Add(New TV_3DVECTOR() {P(0), P(3), P(4)})
        Indices.Add(New TV_3DVECTOR() {P(0), P(4), P(5)})
        Indices.Add(New TV_3DVECTOR() {P(0), P(5), P(2)})
        Indices.Add(New TV_3DVECTOR() {P(0), P(2), P(3)})
        Indices.Add(New TV_3DVECTOR() {P(1), P(4), P(3)})
        Indices.Add(New TV_3DVECTOR() {P(1), P(5), P(4)})
        Indices.Add(New TV_3DVECTOR() {P(1), P(2), P(5)})
        Indices.Add(New TV_3DVECTOR() {P(1), P(3), P(2)})

        Dim Pa As TV_3DVECTOR, Pb As TV_3DVECTOR, Pc As TV_3DVECTOR
        Dim Nt As Integer = 8
        Dim Ntold As Integer

        For it As Integer = 0 To Iterations - 1
            Ntold = Nt
            For i As Integer = 0 To Ntold - 1
                Pa = (Indices(i)(0) + Indices(i)(1)) / 2
                Pb = (Indices(i)(1) + Indices(i)(2)) / 2
                Pc = (Indices(i)(2) + Indices(i)(0)) / 2

                Maths.TVVec3Normalize(Pa, Pa)
                Maths.TVVec3Normalize(Pb, Pb)
                Maths.TVVec3Normalize(Pc, Pc)

                Indices.Add(New TV_3DVECTOR() {Indices(i)(0), Pa, Pc})
                Indices.Add(New TV_3DVECTOR() {Pa, Indices(i)(1), Pb})
                Indices.Add(New TV_3DVECTOR() {Pb, Indices(i)(2), Pc})
                Nt += 3

                Indices(i) = New TV_3DVECTOR() {Pa, Pb, Pc}
            Next
        Next

        For Each Face As TV_3DVECTOR() In Indices
            For Each Vertex As TV_3DVECTOR In Face
                If Vertex.z >= 0 AndAlso Not Vertices.Contains(Vertex) Then Vertices.Add(Vertex)
            Next
        Next

        Return Vertices
    End Function

    Public Function RayCollision(ByVal StartPos As TV_3DVECTOR, ByVal EndPos As TV_3DVECTOR) As Boolean
        ' This could be changed to make self-occlusion only
        ' Or occlude against the landscape or actors, etc.

        ' I chose not to use Scene.Collision because it is not threadsafe
        For Each Receiver As TVMesh In SceneMeshes
            ' In fact there are rare cases where Mesh.Collision isn't threadsafe either
            Dim ThreadsafeMyAss As Boolean = True

            While ThreadsafeMyAss
                ' ...so try/catch it to make sure
                Try
                    If Receiver.Collision(StartPos, EndPos, CONST_TV_TESTTYPE.TV_TESTTYPE_ACCURATETESTING) Then
                        Return True
                    End If

                    ThreadsafeMyAss = False
                Catch Ex As Exception
                End Try
            End While
        Next
    End Function
End Class

