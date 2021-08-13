#include "LOS.h"

#include "loadGLE.h"
#include "Debug/Debug.h"

static const int losTable[16] = 
{
	0x00000001,
	0x00000002,
	0x00000004,
	0x00000008,
	0x00000010,
	0x00000020,
	0x00000040,
	0x00000080,
	0x00000100,
	0x00000200,
	0x00000400,
	0x00000800,
	0x00001000,
	0x00002000,
	0x00004000,
	0x00008000
};

static const uint32_t losTable32[32] = 
{
	0x00000001, 0x00000002, 0x00000004, 0x00000008,
	0x00000010, 0x00000020, 0x00000040, 0x00000080,
	0x00000100, 0x00000200, 0x00000400, 0x00000800,
	0x00001000, 0x00002000, 0x00004000, 0x00008000,
	0x00010000, 0x00020000, 0x00040000, 0x00080000,
	0x00100000, 0x00200000, 0x00400000, 0x00800000,
	0x01000000, 0x02000000, 0x04000000, 0x08000000,
	0x10000000, 0x20000000, 0x40000000, 0x80000000
};

LOS::LOS()
{
	_losMap = NULL;
	fogOfWarMap = NULL;
	discoveryMap = NULL;
}

LOS::~LOS()
{
	delete[] _losMap;
	delete[] fogOfWarMap;
	delete[] discoveryMap;
}

int LOS::Init(int w, int h)
{
	DebugOutput("Init LOS\n");

	_width = w;
	_height = h;

	int chunks = (int)ceil(((float)w)/16.0);
	_losMap = new int*[chunks];
	for (int i = 0;i < chunks;i ++)
		_losMap[i] = new int[h];
	
	for (int j = 0;j < chunks;j ++)
		for (int i = 0;i < h;i ++)
			_losMap[j][i] = 0x00;

	_chunks = (int)(ceil((float)_width/32.0));
	/*_losData = new uint32_t*[_height];
	for (int i = 0;i < _height;i ++)
		_losData[i] = new uint32_t[_chunks];

	for (int i = 0;i < _height;i ++)
		for (int rowVal = 0;rowVal < _chunks;rowVal ++)
			_losData[i][rowVal] = 0xFFFFFFFF;*/

	_losData = new uint32_t[_chunks*_height];
	for (int i = 0;i < _chunks*_height;i ++)
		_losData[i] = 0x00000000; //Note this is backwards of what you would think - |AA|BB|GG|RR|

	for (int i = 0;i < _height;i +=2)
	{
		_losData[i*_chunks+0] = 0x00FF0000;
		_losData[i*_chunks+2] = 0x00FF00FF;
		_losData[i*_chunks+4] = 0x0000FF00;
		_losData[i*_chunks+6] = 0x00FF00FF;
	}

	glGenTextures(1, &_losTexture);
	glBindTexture(GL_TEXTURE_2D, _losTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _chunks, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, _losData);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	return 0;
}

int LOS::CreateLOSPoints(float radius, LOSPoints* points)
{
	/*int cells = (int)ceil(radius);
	int chunks = (int)ceil(radius/16.0f);
	//The number of rows is twice the ceil of the radius
	points->resize(cells*2+1);
	//The number of cols is the ceil of the radius/chunksize times 2
	for (int i = 0;i < points->size();i ++)
		points->at(i).resize(chunks*2);

	for (int i = 0;i < points->size();i ++)
		for (int k = 0;k < chunks*2;k ++)
			points->at(i).at(k) = 0x00;

	for (int j = -cells;j <= cells;j ++)
		for (int i = -cells;i <= cells;i ++)
		{
			int row = j+cells;
			int col = i+cells;
			int chunkCol = floor(((float)i)/16.0f)+chunks;
			int colInChunk = (i+chunks*16)%16;

			//calculate the distance to the center
			float distance = glm::distance(glm::vec2(0.0f, 0.0f), glm::vec2(i, j));
			if (distance <= radius)
			{
				points->at(row).at(chunkCol) |= losTable[15-colInChunk];
			}
		}*/

	int cells = (int)ceil(radius);
	int chunks = (int)ceil(radius/32.0f);
	//The number of rows is twice the ceil of the radius
	points->resize(cells*2+1);
	//The number of cols is the ceil of the radius/chunksize times 2
	for (int i = 0;i < (int)points->size();i ++)
		points->at(i).resize(chunks*2);

	for (int i = 0;i < (int)points->size();i ++)
		for (int k = 0;k < chunks*2;k ++)
			points->at(i).at(k) = 0x00;

	for (int j = -cells;j <= cells;j ++)
	{
		for (int i = -cells;i <= cells;i ++)
		{
			int row = j+cells;
			int col = i+cells;
			int chunkCol = (int)floor(((float)i)/32.0f)+chunks;
			int colInChunk = (i+chunks*32)%32;

			//calculate the distance to the center
			float distance = glm::distance(glm::vec2(0.0f, 0.0f), glm::vec2(i, j));
			if (distance < radius)
			{
				points->at(row).at(chunkCol) |= losTable32[31-colInChunk];
			}
		}
	}

	return 0;
}

int LOS::SetLineOfSight(glm::vec2 center, LOS::LOSPoints* points)
{
	/*int cenX = (int)floor(center.x);
	int cenY = (int)floor(center.y);

	int colInChunk = cenX%16;
	int invColInChunk = 16-colInChunk;

	int radius = (points->size()-1)/2;

	glm::ivec2 pt1 = glm::vec2(cenX-radius, cenY-radius);
	glm::ivec2 pt2 = glm::vec2(cenX+radius, cenY+radius);
	pt1.x = glm::clamp(pt1.x, 0, _width);
	pt1.y = glm::clamp(pt1.y, 0, _height);
	pt2.x = glm::clamp(pt2.x, 0, _width);
	pt2.y = glm::clamp(pt2.y, 0, _height);

	int startRow = pt1.y - (cenY-radius);
	int endRow = pt2.y - (cenY+radius);

	int colLeft = glm::clamp((int)floor(((float)(cenX-radius))/16.0f), 0, _width);
	int colRight = glm::clamp((cenX+radius)/16, 0, _width);
	int startCol = colLeft - (int)floor(((float)(cenX-radius))/16.0f);
	int endCol = (colRight - (cenX+radius)/16);

	int chunkLeft = glm::clamp((int)floor(((float)(cenX-radius))/16.0f), 0, _width);
	int chunkRight = glm::clamp((int)floor(((float)(cenX+radius))/16.0f), 0, _width);
	int middleChunk = cenX/16;

	for (int j = startRow;j < points->size()+endRow;j ++)
	{*/
		//_losMap[0][j] |= points->at(j).at(0);
		//_losMap[0][j] |= points->at(j).at(1);
		/*for (int i = chunkLeft;i <= chunkRight;i ++)
		{
			int losMapY = (cenY-radius)+j;

			int offset = points->at(j).size()/2;
			int currentChunk = i-middleChunk;
			int p1 = currentChunk+offset;
			int p2 = p1-1;

			if (p2 >= 0)
				_losMap[i][losMapY] |= points->at(j).at(p2) << invColInChunk;
			if (p1 <= points->at(j).size())
				_losMap[i][losMapY] |= points->at(j).at(p1) >> colInChunk;
		}*/
		/*for (int i = startCol;i < points->at(j).size()+endCol + 1;i ++)
		{
			int losMapX = (cenX/16)+i-points->at(j).size()/2;
			if (losMapX < 0) continue;
			int losMapY = (cenY-radius)+j;

			if (i > 0)
			{
				_losMap[losMapX][losMapY] |= points->at(j).at(i-1) << invColInChunk;
			}
			if (i < points->at(j).size())
			{
				_losMap[losMapX][losMapY] |= points->at(j).at(i) >> colInChunk;
			}
		}
	}*/

	int cenX = (int)floor(center.x);
	int cenY = (int)floor(center.y);

	int colInChunk = cenX%32;
	int invColInChunk = 31-colInChunk;

	int radius = (points->size()-1)/2;

	glm::ivec2 pt1 = glm::vec2(cenX-radius, cenY-radius);
	glm::ivec2 pt2 = glm::vec2(cenX+radius, cenY+radius);
	pt1.x = glm::clamp(pt1.x, 0, _width);
	pt1.y = glm::clamp(pt1.y, 0, _height);
	pt2.x = glm::clamp(pt2.x, 0, _width);
	pt2.y = glm::clamp(pt2.y, 0, _height);

	int startRow = pt1.y - (cenY-radius);
	int endRow = pt2.y - (cenY+radius);

	int colLeft = glm::clamp((int)floor(((float)(cenX-radius))/32.0f), 0, _width);
	int colRight = glm::clamp((cenX+radius)/32, 0, _width);
	int startCol = colLeft - (int)floor(((float)(cenX-radius))/32.0f);
	int endCol = (colRight - (cenX+radius)/32);

	int chunkLeft = glm::clamp((int)floor(((float)(cenX-radius))/32.0f), 0, _width);
	int chunkRight = glm::clamp((int)floor(((float)(cenX+radius))/32.0f), 0, _width);
	int middleChunk = cenX/32;

	for (int j = startRow;j < (int)points->size()+endRow;j ++)
	{
		//_losData[j*_chunks+0] |= points->at(j).at(0);
		//_losData[j*_chunks+0] |= points->at(j).at(1);
		/*for (int i = chunkLeft;i <= chunkRight;i ++)
		{
			int losMapY = (cenY-radius)+j;

			int offset = points->at(j).size()/2;
			int currentChunk = i-middleChunk;
			int p1 = currentChunk+offset;
			int p2 = p1-1;

			if (p2 >= 0)
				_losData[losMapY*_chunks+i] |= points->at(j).at(p2) << invColInChunk;
			if (p1 <= points->at(j).size())
				_losData[losMapY*_chunks+i] |= points->at(j).at(p1) >> colInChunk;
		}*/
		for (int i = startCol;i < (int)points->at(j).size()+endCol + 1;i ++)
		{
			int losMapX = (cenX/32)+i-points->at(j).size()/2;
			if (losMapX < 0) continue;
			int losMapY = (cenY-radius)+j;

			if (i > 0)
			{
				_losData[losMapY*_chunks+losMapX] |= points->at(j).at(i-1) << invColInChunk;
			}
			if (i < (int)points->at(j).size())
			{
				_losData[losMapY*_chunks+losMapX] |= points->at(j).at(i) >> colInChunk;
			}
		}
	}

	return 0;
}

int LOS::ClearLineOfSight()
{
	/*int chunks = (int)ceil(((float)_width)/16.0);
	for (int j = 0;j < chunks;j ++)
		for (int i = 0;i < _height;i ++)
			_losMap[j][i] = 0x00;*/

	memset(_losData, 0x00000000, _chunks*_height*sizeof(uint32_t));

	return 0;
}

int** LOS::GetLOSPointer()
{
	return _losMap;
}

unsigned int LOS::GetLOSTexture()
{
	glBindTexture(GL_TEXTURE_2D, _losTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _chunks, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, _losData);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	return _losTexture;
}
