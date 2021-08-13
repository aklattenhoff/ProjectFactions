#include "PathFinder.h"

#include "Game/World.h"
#include "Phage/Phage_Terrain.h"

#include "Renderer/Renderer.h"
#include "Renderer/RS/GUIText.h"

#include "Phage/Phage_Camera.h"

#include "loadGLE.h"
#include "Debug/Debug.h"

static const int MAP_DIRECTION_NUM = 4;
static const int dirX[MAP_DIRECTION_NUM] = {1, 0, -1, 0};
static const int dirY[MAP_DIRECTION_NUM] = {0, 1, 0, -1};
static const int fromX[MAP_DIRECTION_NUM] = {-1, 0, 1, 0};
static const int fromY[MAP_DIRECTION_NUM] = {0, -1, 0, 1};

PathFinder::PathFinder()
{
	_obstacleMap = NULL;

	_closedMap = NULL;
	_priorityMap = NULL;
	_fromMap = NULL;

	_pathMap = NULL;
}

PathFinder::~PathFinder()
{
	delete[] _obstacleMap;

	for (int i = 0;i < _mapWidth;i ++)
		delete[] _closedMap[i];
	delete[] _closedMap;

	for (int i = 0;i < _mapWidth;i ++)
		delete[] _priorityMap[i];
	delete[] _priorityMap;

	for (int i = 0; i < _mapWidth; i++)
		delete[] _fromMap[i];
	delete[] _fromMap;

	for (int i = 0; i < _mapWidth; i++)
		delete[] _pathMap[i];
	delete[] _pathMap;
}

int PathFinder::Init()
{
	return 0;
}

int PathFinder::CreateObstacleMap(int cellWidth, int cellHeight, int cellRes)
{
	_cellWidth = cellWidth;
	_cellHeight = cellHeight;
	_cellRes = cellRes;

	_mapWidth = _cellWidth * _cellRes;
	_mapHeight = _cellHeight * _cellRes;

	_obstacleMap = new int[_mapWidth * _mapHeight];
	for (int i = 0;i < _mapWidth * _mapHeight;i ++)
		_obstacleMap[i] = MAP_OPEN;

	//A-star search
	_closedMap = new bool*[_mapWidth];
	for (int i = 0;i < _mapWidth;i ++)
		_closedMap[i] = new bool[_mapHeight];
	for (int j = 0;j < _mapHeight;j ++)
		for (int i = 0;i < _mapWidth;i ++)
			_closedMap[i][j] = false;

	_priorityMap = new int*[_mapWidth];
	for (int i = 0;i < _mapWidth;i ++)
		_priorityMap[i] = new int[_mapHeight];
	for (int j = 0;j < _mapHeight;j ++)
		for (int i = 0;i < _mapWidth;i ++)
			_priorityMap[i][j] = 0;

	_fromMap = new int*[_mapWidth];
	for (int i = 0; i < _mapWidth; i++)
		_fromMap[i] = new int[_mapHeight];
	for (int j = 0; j < _mapHeight; j++)
		for (int i = 0; i < _mapWidth; i++)
			_fromMap[i][j] = 0;

	//Path Map
	_pathMap = new int*[_mapWidth];
	for (int i = 0; i < _mapWidth; i++)
		_pathMap[i] = new int[_mapHeight];
	for (int j = 0; j < _mapHeight; j++)
		for (int i = 0; i < _mapWidth; i++)
			_pathMap[i][j] = 0;

	return 0;
}

int PathFinder::GetResolution()
{
	return _cellRes;
}

int PathFinder::GetMapValue(int x, int y)
{
	return _obstacleMap[x + y * _mapWidth];
}

int PathFinder::GetMapValueFromPosition(glm::vec2 pos)
{
	int x = (int)pos.x * _cellRes;
	int y = (int)pos.y * _cellRes;
	return _obstacleMap[x + y * _mapWidth];
}

int PathFinder::GenerateObstacleSubsetGroup(BoundingBox2D bb, glm::vec3 translation, float rotation, int value, ObstacleSubsetGroup* output)
{
	//  0-------2
	//  |       |
	// p1---m---p2
	//  |       |
	//  1-------3
	//Find the four points of the bounding box
	glm::vec3 pointTemp;
	glm::vec2 point[4];
	pointTemp = glm::rotate(bb.midpoint, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f)) + translation + glm::rotate(bb.p1Vec, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));
	point[0] = glm::vec2(pointTemp.x, pointTemp.z);
	pointTemp = glm::rotate(bb.midpoint, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f)) + translation + glm::rotate(bb.p2Vec, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));
	point[1] = glm::vec2(pointTemp.x, pointTemp.z);
	pointTemp = glm::rotate(bb.midpoint, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f)) + translation + glm::rotate(bb.p3Vec, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));
	point[2] = glm::vec2(pointTemp.x, pointTemp.z);
	pointTemp = glm::rotate(bb.midpoint, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f)) + translation + glm::rotate(bb.p4Vec, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));
	point[3] = glm::vec2(pointTemp.x, pointTemp.z);

	//Expand for cell resolution
	for (int i = 0;i < 4;i ++)
		point[i] *= (float)_cellRes;

	//Find the AABB that encloses the rectangle
	glm::vec2 topLeft = glm::vec2((float)_mapWidth, (float)_mapHeight);
	glm::vec2 botRight = glm::vec2(0.0f, 0.0f);
	for (int i = 0;i < 4;i ++)
	{
		if (point[i].x < topLeft.x) topLeft.x = point[i].x;
		if (point[i].x > botRight.x) botRight.x = point[i].x;
		if (point[i].y < topLeft.y) topLeft.y = point[i].y;
		if (point[i].y > botRight.y) botRight.y = point[i].y;
	}
	int aabbHeight = (((int)botRight.y) - ((int)topLeft.y)) + 1;
	output->push_back(ObstacleSubset());
	output->back().scanLineMin.assign(aabbHeight, _mapWidth);
	output->back().scanLineMax.assign(aabbHeight, 0);

	for (int i = 0;i < 4;i ++)
	{
		point[i].x -= (int)topLeft.x;
		point[i].y -= (int)topLeft.y;
	}

	CalcScanLine(point[0], point[1], &output->back().scanLineMin, &output->back().scanLineMax, aabbHeight);
	CalcScanLine(point[1], point[3], &output->back().scanLineMin, &output->back().scanLineMax, aabbHeight);
	CalcScanLine(point[3], point[2], &output->back().scanLineMin, &output->back().scanLineMax, aabbHeight);
	CalcScanLine(point[2], point[0], &output->back().scanLineMin, &output->back().scanLineMax, aabbHeight);

	for (int j = 0;j < aabbHeight;j ++)
		for (int i = output->back().scanLineMin[j];i <= output->back().scanLineMax[j];i ++)
			_obstacleMap[(i+(int)topLeft.x)+(j+(int)topLeft.y)*_mapWidth] = value;

	output->back().height = aabbHeight;
	output->back().left = (int)topLeft.x;
	output->back().top = (int)topLeft.y;

	return 0;
}

int PathFinder::CalcScanLine(glm::vec2 pt1, glm::vec2 pt2, std::vector<int>* scanMin, std::vector<int>* scanMax, int height)
{
	glm::vec2 directionVector = pt2 - pt1;

	//Number of divisions in each direction
	int divisionsX = abs((int)pt2.x - (int)pt1.x);
	int divisionsY = abs((int)pt2.y - (int)pt1.y);
	int maxDivisions = divisionsX > divisionsY ? divisionsX : divisionsY;

	//Increment value in each direction
	int incrementX = directionVector.x < 0.0f ? -1 : 1;
	int incrementY = directionVector.y < 0.0f ? -1 : 1;

	int startX = (int)pt1.x;
	int startY = (int)pt1.y;
	int loopX = 0;
	int loopY = 0;

	int loopCounter = 1;
	bool loopFlag = false;
	while (!loopFlag)
	{
		//Check in horizontal direction
		if (loopCounter <= divisionsX)
		{
			loopX = startX + loopCounter * incrementX;
			float interX = (float)loopX;
			if (directionVector.x < 0.0f) interX ++;
			loopY = (int)(pt1.y + ((abs(interX - pt1.x) / abs(directionVector.x)) * directionVector.y));

			if (loopX < (*scanMin)[loopY]) (*scanMin)[loopY] = loopX;
			if (loopX > (*scanMax)[loopY]) (*scanMax)[loopY] = loopX;
		}

		//Check in vertical direction
		if (loopCounter <= divisionsY)
		{
			loopY = startY + loopCounter * incrementY;
			float interY = (float)loopY;
			if (directionVector.y < 0.0f) interY ++;
			loopX = (int)(pt1.x + ((abs(interY - pt1.y) / abs(directionVector.y)) * directionVector.x));

			if (loopX < (*scanMin)[loopY]) (*scanMin)[loopY] = loopX;
			if (loopX > (*scanMax)[loopY]) (*scanMax)[loopY] = loopX;
		}

		loopCounter ++;
		if (loopCounter > maxDivisions)
			loopFlag = true;
	}

	return 0;
}

int PathFinder::CalcNodePriority(Node* node, int endX, int endY)
{
	int x = endX-node->x;
	int y = endY-node->y;
	return node->level + (abs(endX-node->x)+abs(endY-node->y))*100;
}

int PathFinder::SmoothPath(Path* path)
{
	Path smoothPath;

	//The path is sorted end to start. So, we start with the back of the vector
	int index = path->Size()-1;
	int smoothIndex = 0;
	int finalIndex = 0;
	bool smoothDone = false;
	//Only smooth if there are at least three points left
	while (index > 1)
	{
		smoothIndex = index - 1;
		finalIndex = 0;
		smoothDone = false;
		while (smoothIndex > 0 && !smoothDone)
		{
			smoothIndex --;
			if (CheckClearLine(path->At(index), path->At(smoothIndex)) == 1)
			{
				finalIndex = smoothIndex+1;
				smoothDone = true;
			}
		}

		smoothPath.PushBack(path->At(index));
		index = finalIndex;
	}
	if (index == 1)
	{
		smoothPath.PushBack(path->At(index));
		smoothPath.PushBack(path->At(index-1));
	}
	else
		smoothPath.PushBack(path->At(index));

	(*path) = smoothPath;

	return 0;
}

int PathFinder::SmoothAndReversePath(Path * path)
{
	Path smoothPath;

	//The path is sorted end to start. So, we start with the back of the vector
	int index = path->Size() - 1;
	int smoothIndex = 0;
	int finalIndex = 0;
	bool smoothDone = false;
	//Only smooth if there are at least three points left
	while (index > 1)
	{
		smoothIndex = index - 1;
		finalIndex = 0;
		smoothDone = false;
		while (smoothIndex > 0 && !smoothDone)
		{
			smoothIndex--;
			if (CheckClearLine(path->At(index), path->At(smoothIndex), false) == 1)
			{
				finalIndex = smoothIndex + 1;
				smoothDone = true;
			}
		}

		smoothPath.PushBack(path->At(index));
		index = finalIndex;
	}
	if (index == 1)
	{
		smoothPath.PushBack(path->At(index));
		smoothPath.PushBack(path->At(index - 1));
	}
	else
		smoothPath.PushBack(path->At(index));

	(*path) = smoothPath;

	return 0;
}

/*int PathFinder::FindPath(glm::vec2 start, glm::vec2 end, Path* path)
{
	//If start and end are the same grid, then 
	if (floor(start.x) == floor(end.x) && floor(start.y) == floor(end.y))
	{
		path->PushBack(start);
		path->PushBack(end);
		return -1;
	}

	for (int i = 0;i < _mapWidth;i ++)
		memset(_closedMap[i], 0, sizeof(_closedMap[0][0])*_mapHeight);
	for (int i = 0;i < _mapWidth;i ++)
		memset(_priorityMap[i], 0, sizeof(_priorityMap[0][0])*_mapHeight);

	int startX = (int)(start.x * (float)_cellRes);
	int startY = (int)(start.y * (float)_cellRes);
	int endX = (int)(end.x * (float)_cellRes);
	int endY = (int)(end.y * (float)_cellRes);

	std::vector<Node*> nodeVector;

	//Put start node in queue
	nodeVector.push_back(new Node(startX, startY, 0, 0));

	while (!nodeVector.empty())
	{
		//Find lowest priority
		int index = 0;
		for (unsigned int i = 0;i < nodeVector.size();i ++)
			if (nodeVector[i]->priority < nodeVector[index]->priority)
				index = i;
		Node* node = nodeVector.at(index);
		nodeVector.erase(nodeVector.begin()+index);
		//Mark on maps
		_priorityMap[node->x][node->y] = 0;
		_closedMap[node->x][node->y] = true;

		if (node->x == endX && node->y == endY)
		{
			//Found path!
			while (!nodeVector.empty())
			{
				delete nodeVector.back();
				nodeVector.pop_back();
			}

			foundPath.Clear();
			//int pathX = endX;
			//int pathY = endY;
			int pathX = node->x + fromX[_fromMap[node->x][node->y]];
			int pathY = node->y + fromY[_fromMap[node->x][node->y]];
			int previousDir = -1;
			foundPath.PushBack(glm::vec2(end.x * _cellRes, end.y * _cellRes));
			while (pathX != startX || pathY != startY)
			{
				if (pathX < 0 || pathX >= _mapWidth || pathY < 0 || pathY >= _mapHeight)
					break;
				if (previousDir != _fromMap[pathX][pathY])
					foundPath.PushBack(glm::vec2(pathX, pathY));
				previousDir = _fromMap[pathX][pathY];
				pathX += fromX[previousDir];
				pathY += fromY[previousDir];
			}
			//foundPath.push_back(glm::vec2(pathX, pathY));
			foundPath.PushBack(glm::vec2(start.x * _cellRes, start.y * _cellRes));

			SmoothPath(&foundPath);

			(*path) = foundPath;

			for (unsigned int i = 0;i < path->Size();i ++)
				//(*path).At(i) /= (float)resolution;
				path->DivideAt(i, (float)_cellRes);

			return 1;
		}

		//Create surrounding nodes
		for (int i = 0;i < MAP_DIRECTION_NUM;i ++)
		{
			int nextX = node->x+dirX[i];
			int nextY = node->y+dirY[i];
			//Check if it is a valid node
			bool valid = true;
			/*for (int k = 0;k < 4;k ++)
			{
				int validX = nextX+dirX[k];
				int validY = nextY+dirY[k];
				if (validX == node->x && validY == node->y)
					continue;
				if (validX < 0 || validX >= mapX || validY < 0 || validY >= mapY || closedMap[validX][validY] || map[validX+validY*mapX] == MAP_BLOCKED)
					valid = false;
			}*/
			/*int validX = nextX;
			int validY = nextY;
			if (validX < 0 || validX >= _mapWidth || validY < 0 || validY >= _mapHeight || _closedMap[validX][validY] || _obstacleMap[validX+validY*_mapWidth] == MAP_CLOSED)
				valid = false;
			if (!valid)
				continue;

			Node* newNode = new Node(nextX, nextY, node->level+(i%2==0?10:14)+(_fromMap[node->x][node->y]==i?5:0), 0);
			newNode->priority = CalcNodePriority(newNode, endX, endY);
			if (_priorityMap[nextX][nextY] == 0 || newNode->priority < _priorityMap[nextX][nextY])
			{
				_priorityMap[nextX][nextY] = newNode->priority;
				_fromMap[nextX][nextY] = i;
				nodeVector.push_back(newNode);
			}
			else
				delete newNode;
		}

		delete node;
	}

	return 0;
}*/

int PathFinder::FindPath(glm::vec2 start, glm::vec2 end, Path* path)
{
	glm::ivec2 ip1 = start * (float)_cellRes;
	glm::ivec2 ip2 = end * (float)_cellRes;
	glm::vec2 fp1 = start * (float)_cellRes;
	glm::vec2 fp2 = end * (float)_cellRes;

	//cell offset (half the width of a cell)
	float co = (1.0f / (float)_cellRes) / 2.0f;

	//If start and end are the same grid, then just add the two points
	if (ip1.x == ip2.x && ip1.y == ip2.y)
	{
		path->PushBack(start);
		path->PushBack(end);
		//AddToPathMap(path->At(0).x, path->At(0).y);
		//AddToPathMap(path->At(1).x, path->At(1).y);
		return -1;
	}

	//If there is a clear path to the destination, simply add it and return
	if (CheckClearLine(start, end) == 0)
	{
		DebugOutput("Found clear line\n");
		path->PushBack(start);
		path->PushBack(end);
		AddToPathMap((int)path->At(0).x, (int)path->At(0).y);
		AddToPathMap((int)path->At(1).x, (int)path->At(1).y);
		path->AttachPathFinder(this);
		return 0;
	}

	//If we get here, we need to actually find path, so clear algorithm variables
	for (int i = 0; i < _mapWidth; i++)
		memset(_closedMap[i], 0, sizeof(_closedMap[0][0])*_mapHeight);
	for (int i = 0; i < _mapWidth; i++)
		memset(_priorityMap[i], 0, sizeof(_priorityMap[0][0])*_mapHeight);

	int startX = ip1.x;
	int startY = ip1.y;
	int endX = ip2.x;
	int endY = ip2.y;

	std::vector<Node*> nodeVector;

	//Put start node in queue
	nodeVector.push_back(new Node(startX, startY, 0, 0));

	while (!nodeVector.empty())
	{
		//Find lowest priority
		int index = 0;
		for (unsigned int i = 0; i < nodeVector.size(); i++)
			if (nodeVector[i]->priority < nodeVector[index]->priority)
				index = i;
		Node* node = nodeVector.at(index);
		nodeVector.erase(nodeVector.begin() + index);
		//Mark on maps
		_priorityMap[node->x][node->y] = 0;
		_closedMap[node->x][node->y] = true;

		if (node->x == endX && node->y == endY)
		{
			//Found path!
			while (!nodeVector.empty())
			{
				delete nodeVector.back();
				nodeVector.pop_back();
			}

			foundPath.Clear();
			//int pathX = endX;
			//int pathY = endY;
			int pathX = node->x + fromX[_fromMap[node->x][node->y]];
			int pathY = node->y + fromY[_fromMap[node->x][node->y]];
			int previousDir = -1;
			foundPath.PushBack(fp2);
			while (pathX != startX || pathY != startY)
			{
				if (pathX < 0 || pathX >= _mapWidth || pathY < 0 || pathY >= _mapHeight)
					break;
				if (previousDir != _fromMap[pathX][pathY])
					foundPath.PushBack(glm::vec2(pathX, pathY) + co);
				previousDir = _fromMap[pathX][pathY];
				pathX += fromX[previousDir];
				pathY += fromY[previousDir];
			}
			//foundPath.push_back(glm::vec2(pathX, pathY));
			foundPath.PushBack(fp1);

			SmoothAndReversePath(&foundPath);

			(*path) = foundPath;

			for (unsigned int i = 0; i < path->Size(); i++)
				//(*path).At(i) /= (float)resolution;
				path->DivideAt(i, (float)_cellRes);

			for (int pm = 0; pm < (int)path->Size(); pm++)
			{
				AddToPathMap((int)path->At(pm).x, (int)path->At(pm).y);
			}
			path->AttachPathFinder(this);

			return 1;
		}

		//Create surrounding nodes
		for (int i = 0; i < MAP_DIRECTION_NUM; i++)
		{
			int nextX = node->x + dirX[i];
			int nextY = node->y + dirY[i];
			//Check if it is a valid node
			bool valid = true;
			/*for (int k = 0;k < 4;k ++)
			{
			int validX = nextX+dirX[k];
			int validY = nextY+dirY[k];
			if (validX == node->x && validY == node->y)
			continue;
			if (validX < 0 || validX >= mapX || validY < 0 || validY >= mapY || closedMap[validX][validY] || map[validX+validY*mapX] == MAP_BLOCKED)
			valid = false;
			}*/
			int validX = nextX;
			int validY = nextY;
			if (validX < 0 || validX >= _mapWidth || validY < 0 || validY >= _mapHeight || _closedMap[validX][validY] || _obstacleMap[validX + validY * _mapWidth] == MAP_CLOSED)
				valid = false;
			//if (validX < 0 || validX >= _mapWidth || validY < 0 || validY >= _mapHeight || _pathMap[validX][validY] > 5)
				//valid = false;
			if (!valid)
				continue;

			int addedP = _pathMap[validX][validY] * 200;
			Node* newNode = new Node(nextX, nextY, node->level + (i % 2 == 0 ? 10 : 14) + (_fromMap[node->x][node->y] == i ? 5 : 0) + addedP, 0);
			newNode->priority = CalcNodePriority(newNode, endX, endY);
			if (_priorityMap[nextX][nextY] == 0 || newNode->priority < _priorityMap[nextX][nextY])
			{
				_priorityMap[nextX][nextY] = newNode->priority;
				_fromMap[nextX][nextY] = i;
				nodeVector.push_back(newNode);
			}
			else
				delete newNode;
		}

		delete node;
	}

	return 0;
}

int PathFinder::FindPathToInstance(glm::vec2 start, glm::vec2 end, ObstacleSubsetGroup* subsetGroup, Path* path)
{
	for (int i = 0;i < _mapWidth;i ++)
		memset(_closedMap[i], 0, sizeof(_closedMap[0][0])*_mapHeight);
	for (int i = 0;i < _mapWidth;i ++)
		memset(_priorityMap[i], 0, sizeof(_priorityMap[0][0])*_mapHeight);

	//Remove map cells from map
	if (subsetGroup)
	{
		for (unsigned int k = 0;k < subsetGroup->size();k ++)
		{
			for (int j = 0;j < subsetGroup->at(k).height;j ++)
			{
				for (int i = subsetGroup->at(k).scanLineMin[j];i <= subsetGroup->at(k).scanLineMax[j];i ++)
				{
					_obstacleMap[(i+subsetGroup->at(k).left)+(j+subsetGroup->at(k).top)*_mapWidth] = 0;
				}
			}
		}
	}

	int startX = (int)(start.x * _cellRes);
	int startY = (int)(start.y * _cellRes);
	int endX = (int)(end.x * _cellRes);
	int endY = (int)(end.y * _cellRes);

	std::vector<Node*> nodeVector;

	//Put start node in queue
	nodeVector.push_back(new Node(startX, startY, 0, 0));

	while (!nodeVector.empty())
	{
		//Find lowest priority
		int index = 0;
		for (unsigned int i = 0;i < nodeVector.size();i ++)
			if (nodeVector[i]->priority < nodeVector[index]->priority)
				index = i;
		Node* node = nodeVector.at(index);
		nodeVector.erase(nodeVector.begin()+index);
		//Mark on maps
		_priorityMap[node->x][node->y] = 0;
		_closedMap[node->x][node->y] = true;

		//Check if the node is in the terrain map cell
		bool onInstanceFlag = false;
		for (unsigned int tCell = 0;tCell < subsetGroup->size();tCell ++)
		{
			int tCellX = node->x-subsetGroup->at(tCell).left;
			int tCellY = node->y-subsetGroup->at(tCell).top;
			if (tCellY >= 0 && tCellY < subsetGroup->at(tCell).height)
				if (tCellX >= subsetGroup->at(tCell).scanLineMin[tCellY] && tCellX <= subsetGroup->at(tCell).scanLineMax[tCellY])
					onInstanceFlag = true;
		}

		//if (onInstanceFlag)
		if (node->x == endX && node->y == endY)
		{
			//Found path!
			while (!nodeVector.empty())
			{
				delete nodeVector.back();
				nodeVector.pop_back();
			}

			foundPath.Clear();
			//int pathX = node->x + fromX[fromMap[node->x][node->y]];
			//int pathY = node->y + fromY[fromMap[node->x][node->y]];
			int pathX = endX;
			int pathY = endY;
			int previousDir = -1;
			while (pathX != startX || pathY != startY)
			{
				if (pathX < 0 || pathX >= _mapWidth || pathY < 0 || pathY >= _mapHeight)
					break;
				if (previousDir != _fromMap[pathX][pathY])
					foundPath.PushBack(glm::vec2(pathX, pathY));
				previousDir = _fromMap[pathX][pathY];
				pathX += fromX[previousDir];
				pathY += fromY[previousDir];
			}
			foundPath.PushBack(glm::vec2(pathX, pathY));

			SmoothAndReversePath(&foundPath);

			(*path) = foundPath;

			for (unsigned int i = 0;i < path->Size();i ++)
				//(*path).At(i) /= (float)resolution;
				path->DivideAt(i, (float)_cellRes);

			//Add back into map
			for (unsigned int k = 0;k < subsetGroup->size();k ++)
			{
				for (int j = 0;j < subsetGroup->at(k).height;j ++)
				{
					for (int i = subsetGroup->at(k).scanLineMin[j];i <= subsetGroup->at(k).scanLineMax[j];i ++)
					{
						_obstacleMap[(i+subsetGroup->at(k).left)+(j+subsetGroup->at(k).top)*_mapWidth] = 1;
					}
				}
			}

			return 1;
		}

		//Create surrounding nodes
		for (int i = 0;i < MAP_DIRECTION_NUM;i ++)
		{
			int nextX = node->x+dirX[i];
			int nextY = node->y+dirY[i];
			//Check if it is a valid node
			bool valid = true;
			for (int k = 0;k < 4;k ++)
			{
				int validX = nextX+dirX[k];
				int validY = nextY+dirY[k];
				if (validX == node->x && validY == node->y)
					continue;

				//Check if the node is in the terrain map cell
				bool onInstanceFlag = false;
				for (unsigned int tCell = 0;tCell < subsetGroup->size();tCell ++)
				{
					int tCellX = validX-subsetGroup->at(tCell).left;
					int tCellY = validY-subsetGroup->at(tCell).top;
					if (tCellY >= 0 && tCellY < subsetGroup->at(tCell).height)
						if (tCellX >= subsetGroup->at(tCell).scanLineMin[tCellY] && tCellX <= subsetGroup->at(tCell).scanLineMax[tCellY])
							onInstanceFlag = true;
				}

				if (validX < 0 || validX >= _mapWidth || validY < 0 || validY >= _mapHeight || _closedMap[validX][validY] || (_obstacleMap[validX+validY*_mapWidth] == MAP_CLOSED && !onInstanceFlag))
					valid = false;
			}
			if (!valid)
				continue;

			Node* newNode = new Node(nextX, nextY, node->level+(i%2==0?10:14)+(_fromMap[node->x][node->y]==i?5:0), 0);
			newNode->priority = CalcNodePriority(newNode, endX, endY);
			if (_priorityMap[nextX][nextY] == 0 || newNode->priority < _priorityMap[nextX][nextY])
			{
				_priorityMap[nextX][nextY] = newNode->priority;
				_fromMap[nextX][nextY] = i;
				nodeVector.push_back(newNode);
			}
			else
				delete newNode;
		}

		delete node;
	}

	//Add back into map
	if (subsetGroup)
	{
		for (unsigned int k = 0;k < subsetGroup->size();k ++)
		{
			for (int j = 0;j < subsetGroup->at(k).height;j ++)
			{
				for (int i = subsetGroup->at(k).scanLineMin[j];i <= subsetGroup->at(k).scanLineMax[j];i ++)
				{
					_obstacleMap[(i+subsetGroup->at(k).left)+(j+subsetGroup->at(k).top)*_mapWidth] = 1;
				}
			}
		}
	}

	return 0;
}

/*int PathFinder::CheckClearLine(glm::vec2 pt1, glm::vec2 pt2)
{
	glm::vec2 directionVector = pt2 - pt1;

	//Number of divisions in each direction
	int divisionsX = abs((int)pt2.x - (int)pt1.x);
	int divisionsY = abs((int)pt2.y - (int)pt1.y);
	int maxDivisions = divisionsX > divisionsY ? divisionsX : divisionsY;

	//Increment value in each direction
	int incrementX = directionVector.x < 0.0f ? -1 : 1;
	int incrementY = directionVector.y < 0.0f ? -1 : 1;

	int startX = (int)pt1.x;
	int startY = (int)pt1.y;
	int loopX = 0;
	int loopY = 0;

	int loopCounter = 1;
	bool loopFlag = false;
	while (!loopFlag)
	{
		//Check in horizontal direction
		if (loopCounter <= divisionsX)
		{
			loopX = startX + loopCounter * incrementX;
			float interX = (float)loopX;
			if (directionVector.x < 0.0f) interX ++;
			loopY = (int)(pt1.y + ((abs(interX - pt1.x) / abs(directionVector.x)) * directionVector.y));

			if (_obstacleMap[loopX+loopY*_mapWidth] == MAP_CLOSED)
				return 1;

			/*for (int k = 0;k < 4;k ++)
			{
				int validX = loopX+dirX[k];
				int validY = loopY+dirY[k];
				if (map[validX+validY*mapX] == MAP_BLOCKED)
					return 1;
			}*/
			/*}

		//Check in vertical direction
		if (loopCounter <= divisionsY)
		{
			loopY = startY + loopCounter * incrementY;
			float interY = (float)loopY;
			if (directionVector.y < 0.0f) interY ++;
			loopX = (int)(pt1.x + ((abs(interY - pt1.y) / abs(directionVector.y)) * directionVector.x));

			if (_obstacleMap[loopX+loopY*_mapWidth] == MAP_CLOSED)
				return 1;

			/*for (int k = 0;k < 4;k ++)
			{
				int validX = loopX+dirX[k];
				int validY = loopY+dirY[k];
				if (map[validX+validY*mapX] == MAP_BLOCKED)
					return 1;
			}*/
		/*}

		loopCounter ++;
		if (loopCounter > maxDivisions)
		{
			loopFlag = true;
		}
	}

	return 0;
}*/

int PathFinder::CheckClearLine(glm::vec2 pt1, glm::vec2 pt2, bool worldCoord)
{
	glm::ivec2 ip1 = worldCoord ? pt1 * (float)_cellRes : pt1;
	glm::ivec2 ip2 = worldCoord ? pt2 * (float)_cellRes : pt2;
	glm::vec2 fp1 = worldCoord ? pt1 * (float)_cellRes : pt1;
	glm::vec2 fp2 = worldCoord ? pt2 * (float)_cellRes : pt2;

	glm::vec2 directionVector = fp2 - fp1;

	//Number of divisions in each direction
	int divisionsX = abs(ip2.x - ip1.x);
	int divisionsY = abs(ip2.y - ip1.y);
	int maxDivisions = divisionsX > divisionsY ? divisionsX : divisionsY;

	//Increment value in each direction
	int incrementX = directionVector.x < 0.0f ? -1 : 1;
	int incrementY = directionVector.y < 0.0f ? -1 : 1;

	int startX = ip1.x;
	int startY = ip1.y;
	int loopX = 0;
	int loopY = 0;

	int loopCounter = 1;
	bool loopFlag = false;
	while (!loopFlag)
	{
		//Check in horizontal direction
		if (loopCounter <= divisionsX)
		{
			loopX = startX + loopCounter * incrementX;
			float interX = (float)loopX;
			if (directionVector.x < 0.0f) interX++;
			loopY = (int)(fp1.y + ((abs(interX - fp1.x) / abs(directionVector.x)) * directionVector.y));

			if (_obstacleMap[loopX + loopY * _mapWidth] == MAP_CLOSED)
				return 1;
			//if (_pathMap[loopX][loopY] > 5)
				//return 1;

			/*for (int k = 0;k < 4;k ++)
			{
			int validX = loopX+dirX[k];
			int validY = loopY+dirY[k];
			if (map[validX+validY*mapX] == MAP_BLOCKED)
			return 1;
			}*/
		}

		//Check in vertical direction
		if (loopCounter <= divisionsY)
		{
			loopY = startY + loopCounter * incrementY;
			float interY = (float)loopY;
			if (directionVector.y < 0.0f) interY++;
			loopX = (int)(fp1.x + ((abs(interY - fp1.y) / abs(directionVector.y)) * directionVector.x));

			if (_obstacleMap[loopX + loopY * _mapWidth] == MAP_CLOSED)
				return 1;
			//if (_pathMap[loopX][loopY] > 5)
				//return 1;

			/*for (int k = 0;k < 4;k ++)
			{
			int validX = loopX+dirX[k];
			int validY = loopY+dirY[k];
			if (map[validX+validY*mapX] == MAP_BLOCKED)
			return 1;
			}*/
		}

		loopCounter++;
		if (loopCounter > maxDivisions)
		{
			loopFlag = true;
		}
	}

	return 0;
}

int PathFinder::AddToPathMap(int x, int y)
{
	int cx = x * _cellRes;
	int cy = y * _cellRes;

	_pathMap[cx][cy] ++;

	return 0;
}

int PathFinder::RemoveFromPathMap(int x, int y)
{
	int cx = x * _cellRes;
	int cy = y * _cellRes;

	_pathMap[cx][cy] --;

	return 0;
}

int PathFinder::GetPathMapVal(int x, int y)
{
	int cx = x * _cellRes;
	int cy = y * _cellRes;

	return _pathMap[cx][cy];
}

int PathFinder::DebugDrawObstacleMap(World * world)
{
	glm::vec2 mapSize = glm::vec2(_mapWidth, _mapHeight);
	float mapRes = (float)_cellRes;
	for (int j = 0; j < mapSize.y; j++)
	{
		for (int i = 0; i < mapSize.x; i++)
		{
			if (GetMapValue(i, j) != 0)
			{

				//_phageScene->AddTerrainTileDebug(glm::vec2(i, j) / mapRes);
				float tX = (float)i / mapRes;
				float tY = (float)j / mapRes;
				float tD = 1.0f / mapRes;
				glm::vec3 terrP1 = glm::vec3(tX, world->GetPhageTerrain()->CalculateHeight(tX, tY), tY);
				glm::vec3 terrP2 = glm::vec3(tX + tD, world->GetPhageTerrain()->CalculateHeight(tX + tD, tY), tY);
				glm::vec3 terrP3 = glm::vec3(tX + tD, world->GetPhageTerrain()->CalculateHeight(tX + tD, tY + tD), tY + tD);
				glm::vec3 terrP4 = glm::vec3(tX, world->GetPhageTerrain()->CalculateHeight(tX, tY + tD), tY + tD);
				RendererSingleton->AddLineDrawInfo(terrP1, terrP2);
				RendererSingleton->AddLineDrawInfo(terrP2, terrP3);
				RendererSingleton->AddLineDrawInfo(terrP3, terrP4);
				RendererSingleton->AddLineDrawInfo(terrP4, terrP1);
			}
		}
	}

	return 0;
}

int PathFinder::DebugDrawPathMap(World * world)
{
	for (int x = 0;x < _cellWidth;x ++)
		for (int y = 0; y < _cellHeight; y++)
		{
			if (_pathMap[x][y] != 0)
			{
				glm::vec3 terrPoint = glm::vec3((float)x / (float)_cellRes, 0.0f, (float)y / (float)_cellRes);
				terrPoint.y = world->GetPhageTerrain()->CalculateHeight(terrPoint.x, terrPoint.z);
				glm::vec4 scrPoint = RendererSingleton->GetPhageCamera()->Calculate2DPoint(terrPoint.x, terrPoint.y, terrPoint.z);
				RS::GUIText text;
				text.SetText((int)scrPoint.x, (int)scrPoint.y, std::to_string(_pathMap[x][y]));
				RendererSingleton->AddGUIText(text);
			}
		}

	return 0;
}
