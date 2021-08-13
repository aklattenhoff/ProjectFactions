#ifndef _PATH_H
#define _PATH_H

#include <vector>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

class PathFinder;

class Path
{
public:
	typedef glm::vec2 Node;
public:
	Path();
	~Path();

	int AttachPathFinder(PathFinder* pathfinder);
	int DetachPathFinder();

	int PushBack(Node n);
	int SetAt(int index, Node n);
	int PopBack();
	int EraseFront();
	Node Back();
	Node At(int index);
	unsigned int Size();
	Node Front();
	int Clear();

	int DivideAt(int index, float div);

private:
	PathFinder* _pathFinder;
	std::vector<Node> _path;
};

#endif
