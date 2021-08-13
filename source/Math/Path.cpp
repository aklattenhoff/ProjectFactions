#include "Path.h"

#include "loadGLE.h"
#include "Debug/Debug.h"

Path::Path()
{
}

Path::~Path()
{
}

int Path::PushBack(Node n)
{
	_path.push_back(n);

	return 0;
}

int Path::SetAt(int index, Node n)
{
	if ((int)_path.size() > index)
		_path[index] = n;

	return 0;
}

int Path::PopBack()
{
	_path.pop_back();

	return 0;
}

int Path::EraseFront()
{
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
	_path.clear();

	return 0;
}

int Path::DivideAt(int index, float div)
{
	_path[index] /= div;

	return 0;
}
