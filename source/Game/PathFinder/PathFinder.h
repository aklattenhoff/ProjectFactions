#ifndef _PATH_FINDER_H
#define _PATH_FINDER_H

#include "Game/Math/BoundingBox2DSet.h"
#include "Game/Math/Path.h"

class World;

////////////////////////////////////////////////////////////////////////////////
//PathFinder
//Description:
//Notes: For this class, the term "cell" is used for a 1x1 square based on the
//the terrain. The term "grid" is used for a 1x1 square based on the pathfinder.
//So, the number of grids wide a cell is is equal to (grid * resolution)
////////////////////////////////////////////////////////////////////////////////
class PathFinder
{
public:
	struct ObstacleSubset
	{
		int top;	//The distance from the y-axis
		int left;	//The distance from the x-axis
		int height;	//The number of scan lines
		std::vector<int> scanLineMin;	//The vector of scan line min
		std::vector<int> scanLineMax;	//The vector of scan line max
	};
	typedef std::vector<ObstacleSubset> ObstacleSubsetGroup;
public:
	static const int MAP_OPEN = 0;
	static const int MAP_CLOSED = 1;
public:
	PathFinder();
	~PathFinder();

	int Init();
	int CreateObstacleMap(int cellWidth, int cellHeight, int cellRes);
	int GetResolution();
	int GetMapValue(int x, int y);
	int GetMapValueFromPosition(glm::vec2 pos);

	//Assign to obstacle map
	int GenerateObstacleSubsetGroup(BoundingBox2D bb, glm::vec3 translation, float rotation, int value, ObstacleSubsetGroup* output);

	//Search obstacle map
	int FindPath(glm::vec2 start, glm::vec2 end, Path* path);
	int FindPathToInstance(glm::vec2 start, glm::vec2 end, ObstacleSubsetGroup* subsetGroup, Path* path);
	int CheckClearLine(glm::vec2 pt1, glm::vec2 pt2, bool worldCoord = true);

	int AddToPathMap(int x, int y);
	int RemoveFromPathMap(int x, int y);
	int GetPathMapVal(int x, int y);

	//Debug
	int DebugDrawObstacleMap(World* world);
	int DebugDrawPathMap(World* world);
private:
	struct Node
	{
		int x, y, level, priority;
		Node(int xp, int yp, int l, int p)
		{
			x = xp;
			y = yp;
			level = l;
			priority = p;
		}
	};
private:
	int CalcScanLine(glm::vec2 pt1, glm::vec2 pt2, std::vector<int>* scanMin, std::vector<int>* scanMax, int height);
	//A-star search
	int CalcNodePriority(Node* node, int endX, int endY);
	int SmoothPath(Path* path);
	int SmoothAndReversePath(Path* path);
private:
	int _cellWidth;
	int _cellHeight;
	int _cellRes;
	int _mapWidth;
	int _mapHeight;

	int* _obstacleMap;

	//A-star search
	bool** _closedMap;
	int** _priorityMap;
	int** _fromMap;

	int** _pathMap;

	Path foundPath;
};

#endif
