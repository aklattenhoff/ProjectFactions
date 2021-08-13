#include "Path.h"

#include "Game/PathFinder/PathFinder.h"

#include "loadGLE.h"
#include "Debug/Debug.h"

Path::Path()
{
	_pathFinder = NULL;
}

Path::~Path()
{
	if (_pathFinder)
	{
		for (int i = 0; i < (int)_path.size(); i++)
			_pathFinder->RemoveFromPathMap((int)_path[i].x, (int)_path[i].y);
	}
}

int Path::AttachPathFinder(PathFinder * pathfinder)
{
	_pathFinder = pathfinder;
	return 0;
}

int Path::DetachPathFinder()
{
	_pathFinder = NULL;

	return 0;
}

int Path::PushBack(Node n)
{
	_path.push_back(n);

	return 0;
}

int Path::SetAt(int index, Node n)
{
	if ((int)_path.size() > index)
	{
		if (_pathFinder)
		{
			_pathFinder->RemoveFromPathMap((int)_path[index].x, (int)_path[index].y);
			_pathFinder->AddToPathMap((int)n.x, (int)n.y);
		}
		_path[index] = n;
	}

	return 0;
}

int Path::PopBack()
{
	if (_pathFinder)
		_pathFinder->RemoveFromPathMap((int)_path.back().x, (int)_path.back().y);

	_path.pop_back();

	return 0;
}

int Path::EraseFront()
{
	if (_pathFinder)
		_pathFinder->RemoveFromPathMap((int)_path.front().x, (int)_path.front().y);

	_path.erase(_path.begin());

	return 0;
}

Path::Node Path::Back()
{
	return _path.back();
}

Path::Node Path::At(int index)
{
	return _path.at(index);
}

unsigned int Path::Size()
{
	return _path.size();
}

Path::Node Path::Front()
{
	return _path.front();
}

int Path::Clear()
{
	if (_pathFinder)
	{
		for (int i = 0; i < (int)_path.size(); i++)
			_pathFinder->RemoveFromPathMap((int)_path[i].x, (int)_path[i].y);
	}

	_path.clear();

	return 0;
}

int Path::DivideAt(int index, float div)
{
	_path[index] /= div;

	return 0;
}
