#include "Phage_Terrain.h"

#include "Phage_TerrainMesh.h"
#include "Phage_TerrainTexture.h"
#include "Phage_TerrainDecal.h"

#include "loadGLE.h"
#include "Debug/Debug.h"

//#include "Graphics/Utility/Texture.h"
//#include "Graphics/Utility/TextureUtility.h"

Phage::PhageTerrain::PhageTerrain()
{
	_phageTerrainMesh = NULL;
}

Phage::PhageTerrain::~PhageTerrain()
{
	delete _phageTerrainMesh;
}

int Phage::PhageTerrain::Init()
{
	_mouseOnTerrain = glm::vec3(0.0f);

	return 0;
}

int Phage::PhageTerrain::CreateTerrain(int w, int h, int textureScale)
{
	DebugOutput("Creating Phage Terrain...\n");

	DebugOutput("Creating Phage Terrain Mesh\n");
	_phageTerrainMesh = new Phage::PhageTerrainMesh();
	_phageTerrainMesh->Init();
	_phageTerrainMesh->CreateTerrainMesh(w, h, textureScale);

	DebugOutput("Creating Phage Terrain Texture\n");
	_phageTerrainTexture = new Phage::PhageTerrainTexture();
	_phageTerrainTexture->Init();
	_phageTerrainTexture->CreateTerrainMap(w, h);
	
	DebugOutput("Adding Phage Terrain Textures\n");
	_phageTerrainTexture->AddTerrainTexture("textures/terrain/none.png");
	_phageTerrainTexture->AddTerrainTexture("textures/terrain/grass_01.png");
	_phageTerrainTexture->AddTerrainTexture("textures/terrain/grass_02.png");
	_phageTerrainTexture->AddTerrainTexture("textures/terrain/grass_03.png");
	_phageTerrainTexture->AddTerrainTexture("textures/terrain/grass_04.png");
	//_phageTerrainTexture->AddTerrainTexture("textures/terrain/snow_2.png");
	//_phageTerrainTexture->AddTerrainTexture("textures/terrain/grass_lush.png");
	DebugOutput("Painting Phage Terrain\n");
	//_phageTerrainTexture->SetTerrainMap(2, 2, 1, 2);
	//_phageTerrainTexture->SetAlphaMap(2, 2, 0, 58);
	//_phageTerrainTexture->SetTerrainMap(3, 2, 1, 2);
	//_phageTerrainTexture->SetAlphaMap(3, 2, 0, 56);
	//_phageTerrainTexture->SetTerrainMap(2, 2, 2, 2);
	//_phageTerrainTexture->SetAlphaMap(2, 2, 1, 0);
	srand((unsigned int)time(NULL));
	/*for (int i = 0;i < w*h/3;i ++)
	{
		int x = rand()%(w-1);
		int y = rand()%(h-1);
		int val = rand()%3+2;
		_phageTerrainTexture->PaintMap(x, y, val);
		_phageTerrainTexture->PaintMap(x+1, y, val);
		_phageTerrainTexture->PaintMap(x+1, y+1, val);
		_phageTerrainTexture->PaintMap(x, y+1, val);
	}*/
	for (int i = 0;i < w*h/8;i ++)
	{
		int x = rand()%(w-1);
		int y = rand()%(h-1);
		_phageTerrainTexture->PaintMap(x, y, 2);
		_phageTerrainTexture->PaintMap(x+1, y, 2);
		_phageTerrainTexture->PaintMap(x+1, y+1, 2);
		_phageTerrainTexture->PaintMap(x, y+1, 2);
	}
	for (int i = 0;i < w*h/8;i ++)
	{
		int x = rand()%(w-1);
		int y = rand()%(h-1);
		_phageTerrainTexture->PaintMap(x, y, 3);
		_phageTerrainTexture->PaintMap(x+1, y, 3);
		_phageTerrainTexture->PaintMap(x+1, y+1, 3);
		_phageTerrainTexture->PaintMap(x, y+1, 3);
	}
	for (int i = 0;i < w*h/16;i ++)
	{
		int x = rand()%(w-1);
		int y = rand()%(h-1);
		_phageTerrainTexture->PaintMap(x, y, 4);
		_phageTerrainTexture->PaintMap(x+1, y, 4);
		_phageTerrainTexture->PaintMap(x+1, y+1, 4);
		_phageTerrainTexture->PaintMap(x, y+1, 4);
	}

	/*for (int j = 1;j < 30;j ++)
		for (int i = 1;i < 30;i ++)
			_phageTerrainTexture->PaintMap(i, j, 2);*/

	DebugOutput("Adding Phage Terrain Alpha Textures\n");
	_phageTerrainTexture->AddAlphaTexture("textures/terrain/alphamaps/blend_0.png");
	_phageTerrainTexture->AddAlphaTexture("textures/terrain/alphamaps/blend_1.png");
	_phageTerrainTexture->AddAlphaTexture("textures/terrain/alphamaps/blend_2.png");
	_phageTerrainTexture->AddAlphaTexture("textures/terrain/alphamaps/blend_3.png");
	_phageTerrainTexture->AddAlphaTexture("textures/terrain/alphamaps/blend_4.png");
	_phageTerrainTexture->AddAlphaTexture("textures/terrain/alphamaps/blend_5.png");
	_phageTerrainTexture->AddAlphaTexture("textures/terrain/alphamaps/blend_6.png");
	_phageTerrainTexture->AddAlphaTexture("textures/terrain/alphamaps/blend_7.png");
	_phageTerrainTexture->AddAlphaTexture("textures/terrain/alphamaps/blend_8.png");
	_phageTerrainTexture->AddAlphaTexture("textures/terrain/alphamaps/blend_9.png");
	_phageTerrainTexture->AddAlphaTexture("textures/terrain/alphamaps/blend_10.png");
	_phageTerrainTexture->AddAlphaTexture("textures/terrain/alphamaps/blend_11.png");
	_phageTerrainTexture->AddAlphaTexture("textures/terrain/alphamaps/blend_12.png");
	_phageTerrainTexture->AddAlphaTexture("textures/terrain/alphamaps/blend_13.png");
	_phageTerrainTexture->AddAlphaTexture("textures/terrain/alphamaps/blend_14.png");
	_phageTerrainTexture->AddAlphaTexture("textures/terrain/alphamaps/blend_15.png");

	DebugOutput("Binding Phage Terrain Textures\n");
	_phageTerrainTexture->BindTextures();

	int error = 0;
	//_grassTexture = TextureUtility::CreateTextureFromFile("textures/terrain/grass.png", true, true, &error);
	//_grassBumpTexture = TextureUtility::CreateTextureFromFile("textures/terrain/grass_bump.png", true, true, &error);
	//_grassSBCTexture = TextureUtility::CreateTextureFromFile("textures/terrain/grass_sbc.png", true, true, &error);

	return 0;
}

float Phage::PhageTerrain::CalculateHeight(float x, float y)
{
	int xInt = (int)x;
	int yInt = (int)y;
	float xRem = x - (float)xInt;
	float yRem = y - (float)yInt;

	glm::vec3 p1, p2, p3;
	//Find which triangle the point lies in
	if (xRem > yRem)	//First triangle
	{
		p1 = glm::vec3((float)xInt, _phageTerrainMesh->GetHeight(xInt, yInt), (float)yInt);
		p2 = glm::vec3((float)xInt+1, _phageTerrainMesh->GetHeight(xInt+1, yInt), (float)yInt);
		p3 = glm::vec3((float)xInt+1, _phageTerrainMesh->GetHeight(xInt+1, yInt+1), (float)yInt+1);
	}
	else	//Second triangle
	{
		p1 = glm::vec3((float)xInt, _phageTerrainMesh->GetHeight(xInt, yInt), (float)yInt);
		p2 = glm::vec3((float)xInt+1, _phageTerrainMesh->GetHeight(xInt+1, yInt+1), (float)yInt+1);
		p3 = glm::vec3((float)xInt, _phageTerrainMesh->GetHeight(xInt, yInt+1), (float)yInt+1);
	}

	float det = (p2.z - p3.z) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.z - p3.z);
	float l1 = ((p2.z - p3.z) * (x - p3.x) + (p3.x - p2.x) * (y - p3.z)) / det;
	float l2 = ((p3.z - p1.z) * (x - p3.x) + (p1.x - p3.x) * (y - p3.z)) / det;
	float l3 = 1.0f - l1 - l2;

	return l1 * p1.y + l2 * p2.y + l3 * p3.y;
}

int Phage::PhageTerrain::CreateAndAddTerrainDecal(std::string fileName, float x, float y, float rotation, float w, float h)
{
	_phageTerrainDecalVector.push_back(new Phage::PhageTerrainDecal());
	_phageTerrainDecalVector.back()->Init();
	_phageTerrainDecalVector.back()->CreateTerrainDecal(fileName, x, y, rotation, _phageTerrainMesh->GetTextureDPI(), w, h);

	glm::ivec4 gridPoints = _phageTerrainDecalVector.back()->GetGridPoints();
	std::vector<float> vertexData;
	std::vector<float> normalData;
	std::vector<float> tangentData;
	_phageTerrainMesh->GetSubArea(glm::ivec2(gridPoints.x, gridPoints.y), glm::ivec2(gridPoints.z, gridPoints.w), &vertexData, &normalData, &tangentData);
	_phageTerrainDecalVector.back()->SetVertexData(&vertexData, &normalData, &tangentData);

	return 0;
}

glm::vec3 Phage::PhageTerrain::CalculateMouseOnTerrain(glm::vec3 mouseNear, glm::vec3 mouseZero)
{
	_mouseOnTerrain = _phageTerrainMesh->CalculateMouseOnTerrain(mouseNear, mouseZero);

	return _mouseOnTerrain;
}

glm::vec3 Phage::PhageTerrain::GetMouseOnTerrain()
{
	return _mouseOnTerrain;
}

int Phage::PhageTerrain::PaintPosition(int x, int y, int value)
{
	_phageTerrainTexture->PaintMap(x, y, value);

	return 0;
}

glm::ivec2 Phage::PhageTerrain::GetTerrainSize()
{
	return _phageTerrainMesh->GetSize();
}

int Phage::PhageTerrain::Render(PhageShader* shader, int** losInfo, unsigned int losTexture)
{
	/*glActiveTexture(GL_TEXTURE0+3);
	glBindTexture(GL_TEXTURE_2D, _grassBumpTexture->GetTextureInt());
	glActiveTexture(GL_TEXTURE0+4);
	glBindTexture(GL_TEXTURE_2D, _grassSBCTexture->GetTextureInt());
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _grassTexture->GetTextureInt());*/
	_phageTerrainTexture->BindTextures();
	_phageTerrainMesh->Render(shader, losInfo, losTexture);

	return 0;
}

int Phage::PhageTerrain::RenderTerrainDecals()
{
	for (int i = 0;i < (int)_phageTerrainDecalVector.size();i ++)
	{
		_phageTerrainDecalVector[i]->Draw();
	}

	return 0;
}
