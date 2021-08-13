#include "Phage_TerrainMesh.h"

#include "loadGLE.h"
#include "Debug/Debug.h"

#include "Phage_TerrainMeshPatch.h"

#include "Phage_Shader.h"

Phage::PhageTerrainMesh::PhageTerrainMesh()
{
}

Phage::PhageTerrainMesh::~PhageTerrainMesh()
{
}

int Phage::PhageTerrainMesh::Init()
{
	_patchScale = 16;

	_losLocation = -1;
	_losTextureSamplerLocation = -1;

	return 0;
}

int Phage::PhageTerrainMesh::CreateTerrainMesh(int w, int h, int textureScale)
{
	_width = w;
	_height = h;
	_textureScale = textureScale;
	_dpi = 512.0f / _textureScale;

	//_patchVector.resize((_width/_patchScale) * (_height/_patchScale));

	//Generate the heightmap values for each grid intersection
	srand((unsigned int)time(NULL));
	_heightMap = new float[(_width+1)*(_height+1)];
	for (int j = 1;j < _height;j ++)
	{
		for (int i = 1;i < _width;i ++)
		{
			_heightMap[(j*(_width+1)+i)] = ((float)(rand()%10))/10.0f + 3.5f;//5.0f*sin(((float)(i))/5.0f)*sin(((float)(j))/5.0f) + 1.0f;//((float)(rand()%20))/10.0f;
		}
	}
	for (int k = 0;k < (_width*_height)/1024;k ++)
	{
		int areaX = rand()%_width;
		int areaY = rand()%_height;
		int areaW = rand()%4;
		int areaH = rand()%4;
		float areaElev = ((float)(rand()%40))/10.0f - 2.0f;
		for (int j = areaY-areaH;j <= areaY+areaH;j ++)
			for (int i = areaX-areaW;i <= areaX+areaW;i ++)
			{
				if (i < 1 || j < 1 || i >= _width || j >= _height)
					continue;
				float elevX = ((float)(areaW-abs(i-areaX)))/((float)areaW);
				elevX = glm::clamp(elevX, 0.0f, 1.0f);
				float elevY = ((float)(areaH-abs(j-areaY)))/((float)areaH);
				elevY = glm::clamp(elevY, 0.0f, 1.0f);
				_heightMap[(j*(_width+1)+i)] += ((float)areaElev);
			}
		//_heightMap[(rand()%_height+1)*(_width+1)+(rand()%_width+1)] = ((float)(rand()%20))/10.0f;
	}

	//Smooth out terrain
	int smoothDist = 3;
	for (int j = 1;j < _height-smoothDist;j ++)
	{
		for (int i = 1;i < _width-smoothDist;i ++)
		{
			float avgSmooth = 0.0f;
			float smoothNum = 0.0f;
			for (int smj = 0;smj <= smoothDist;smj ++)
				for (int smi = 0;smi <= smoothDist;smi ++)
				{
					avgSmooth += _heightMap[((j+smj)*(_width+1)+i+smi)];
					smoothNum += 1.0f;
				}
			avgSmooth /= smoothNum;
			_heightMap[(j*(_width+1)+i)] = avgSmooth;
		}
	}

	//Generate vertices
	_vertices = new float[_width*_height*6*3];
	for (int j = 0;j < _height;j ++)
	{
		for (int i = 0;i < _width;i ++)
		{
			_vertices[(j*_width+i)*18] = (float)i;
			_vertices[(j*_width+i)*18+1] = 0.0f;
			_vertices[(j*_width+i)*18+2] = (float)j;

			_vertices[(j*_width+i)*18+3] = (float)i+1.0f;
			_vertices[(j*_width+i)*18+4] = 0.0f;
			_vertices[(j*_width+i)*18+5] = (float)j+1.0f;

			_vertices[(j*_width+i)*18+6] = (float)i+1.0f;
			_vertices[(j*_width+i)*18+7] = 0.0f;
			_vertices[(j*_width+i)*18+8] = (float)j;


			_vertices[(j*_width+i)*18+9] = (float)i;
			_vertices[(j*_width+i)*18+10] = 0.0f;
			_vertices[(j*_width+i)*18+11] = (float)j;

			_vertices[(j*_width+i)*18+12] = (float)i;
			_vertices[(j*_width+i)*18+13] = 0.0f;
			_vertices[(j*_width+i)*18+14] = (float)j+1.0f;

			_vertices[(j*_width+i)*18+15] = (float)i+1.0f;
			_vertices[(j*_width+i)*18+16] = 0.0f;
			_vertices[(j*_width+i)*18+17] = (float)j+1.0f;
		}
	}

	for (int j = 1;j < _height;j ++)
	{
		for (int i = 1;i < _width;i ++)
		{
			_vertices[((j-1)*_width+(i-1))*18+4] = _heightMap[(j*(_width+1)+i)];
			_vertices[((j-1)*_width+(i-1))*18+16] = _heightMap[(j*(_width+1)+i)];

			_vertices[((j-1)*_width+(i))*18+13] = _heightMap[(j*(_width+1)+i)];

			_vertices[((j)*_width+(i))*18+1] = _heightMap[(j*(_width+1)+i)];
			_vertices[((j)*_width+(i))*18+10] = _heightMap[(j*(_width+1)+i)];

			_vertices[((j)*_width+(i-1))*18+7] = _heightMap[(j*(_width+1)+i)];
		}
	}

	//Generate texture coordinates
	float textureScaleF = (float)textureScale;
	int textureScaleI = (int)textureScale;
	_textureCoords = new float[_width*_height*6*2];
	for (int j = 0;j < _height;j ++)
	{
		for (int i = 0;i < _width;i ++)
		{
			_textureCoords[(j*_width+i)*12] = (float)(i%textureScaleI)/textureScaleF;
			_textureCoords[(j*_width+i)*12+1] = (float)(j%textureScaleI)/textureScaleF;

			_textureCoords[(j*_width+i)*12+2] = (float)(i%textureScaleI)/textureScaleF+1.0f/textureScaleF;
			_textureCoords[(j*_width+i)*12+3] = (float)(j%textureScaleI)/textureScaleF+1.0f/textureScaleF;

			_textureCoords[(j*_width+i)*12+4] = (float)(i%textureScaleI)/textureScaleF+1.0f/textureScaleF;
			_textureCoords[(j*_width+i)*12+5] = (float)(j%textureScaleI)/textureScaleF;


			_textureCoords[(j*_width+i)*12+6] = (float)(i%textureScaleI)/textureScaleF;
			_textureCoords[(j*_width+i)*12+7] = (float)(j%textureScaleI)/textureScaleF;

			_textureCoords[(j*_width+i)*12+8] = (float)(i%textureScaleI)/textureScaleF;
			_textureCoords[(j*_width+i)*12+9] = (float)(j%textureScaleI)/textureScaleF+1.0f/textureScaleF;

			_textureCoords[(j*_width+i)*12+10] = (float)(i%textureScaleI)/textureScaleF+1.0f/textureScaleF;
			_textureCoords[(j*_width+i)*12+11] = (float)(j%textureScaleI)/textureScaleF+1.0f/textureScaleF;
		}
	}

	//Generate the alphamap coordinates
	float alphaScaleF = 1.0f;
	int alphaScaleI = (int)alphaScaleF;
	_alphaCoords = new float[_width*_height*6*2];
	for (int j = 0;j < _height;j ++)
	{
		for (int i = 0;i < _width;i ++)
		{
			_alphaCoords[(j*_width+i)*12] = (float)(i%alphaScaleI)/alphaScaleF;
			_alphaCoords[(j*_width+i)*12+1] = (float)(j%alphaScaleI)/alphaScaleF;

			_alphaCoords[(j*_width+i)*12+2] = (float)(i%alphaScaleI)/alphaScaleF+1.0f/alphaScaleF;
			_alphaCoords[(j*_width+i)*12+3] = (float)(j%alphaScaleI)/alphaScaleF+1.0f/alphaScaleF;

			_alphaCoords[(j*_width+i)*12+4] = (float)(i%alphaScaleI)/alphaScaleF+1.0f/alphaScaleF;
			_alphaCoords[(j*_width+i)*12+5] = (float)(j%alphaScaleI)/alphaScaleF;


			_alphaCoords[(j*_width+i)*12+6] = (float)(i%alphaScaleI)/alphaScaleF;
			_alphaCoords[(j*_width+i)*12+7] = (float)(j%alphaScaleI)/alphaScaleF;

			_alphaCoords[(j*_width+i)*12+8] = (float)(i%alphaScaleI)/alphaScaleF;
			_alphaCoords[(j*_width+i)*12+9] = (float)(j%alphaScaleI)/alphaScaleF+1.0f/alphaScaleF;

			_alphaCoords[(j*_width+i)*12+10] = (float)(i%alphaScaleI)/alphaScaleF+1.0f/alphaScaleF;
			_alphaCoords[(j*_width+i)*12+11] = (float)(j%alphaScaleI)/alphaScaleF+1.0f/alphaScaleF;
		}
	}

	//Calculate flat normals
	_normals = new float[_width*_height*6*3];
	for (int j = 0;j < _height;j ++)
	{
		for (int i = 0;i < _width;i ++)
		{
			glm::vec3 U = glm::vec3(_vertices[(j*_width+i)*18+3], _vertices[(j*_width+i)*18+4], _vertices[(j*_width+i)*18+5]) -
							glm::vec3(_vertices[(j*_width+i)*18], _vertices[(j*_width+i)*18+1], _vertices[(j*_width+i)*18+2]);
			glm::vec3 V = glm::vec3(_vertices[(j*_width+i)*18+6], _vertices[(j*_width+i)*18+7], _vertices[(j*_width+i)*18+8]) -
							glm::vec3(_vertices[(j*_width+i)*18], _vertices[(j*_width+i)*18+1], _vertices[(j*_width+i)*18+2]);
			float total = (U.y*V.z-U.z*V.y)+(U.z*V.x-U.x*V.z)+(U.x*V.y-U.y*V.x);
			if (total == 0.0f) total = 0.01f;

			glm::vec3 normalVec = glm::normalize(glm::vec3((U.y*V.z-U.z*V.y)/total, (U.z*V.x-U.x*V.z)/total, (U.x*V.y-U.y*V.x)/total));
			_normals[(j*_width+i)*18] = normalVec.x;//(U.y*V.z-U.z*V.y)/total;
			_normals[(j*_width+i)*18+1] = normalVec.y;//(U.z*V.x-U.x*V.z)/total;
			_normals[(j*_width+i)*18+2] = normalVec.z;//(U.x*V.y-U.y*V.x)/total;

			_normals[(j*_width+i)*18+3] = normalVec.x;//(U.y*V.z-U.z*V.y)/total;
			_normals[(j*_width+i)*18+4] = normalVec.y;//(U.z*V.x-U.x*V.z)/total;
			_normals[(j*_width+i)*18+5] = normalVec.z;//(U.x*V.y-U.y*V.x)/total;

			_normals[(j*_width+i)*18+6] = normalVec.x;//(U.y*V.z-U.z*V.y)/total;
			_normals[(j*_width+i)*18+7] = normalVec.y;//(U.z*V.x-U.x*V.z)/total;
			_normals[(j*_width+i)*18+8] = normalVec.z;//(U.x*V.y-U.y*V.x)/total;


			U =				glm::vec3(_vertices[(j*_width+i)*18+12], _vertices[(j*_width+i)*18+13], _vertices[(j*_width+i)*18+14]) -
							glm::vec3(_vertices[(j*_width+i)*18+9], _vertices[(j*_width+i)*18+10], _vertices[(j*_width+i)*18+11]);
			V =				glm::vec3(_vertices[(j*_width+i)*18+15], _vertices[(j*_width+i)*18+16], _vertices[(j*_width+i)*18+17]) -
							glm::vec3(_vertices[(j*_width+i)*18+9], _vertices[(j*_width+i)*18+10], _vertices[(j*_width+i)*18+11]);
			total = (U.y*V.z-U.z*V.y)+(U.z*V.x-U.x*V.z)+(U.x*V.y-U.y*V.x);
			if (total == 0.0f) total = 0.01f;

			normalVec = glm::normalize(glm::vec3((U.y*V.z-U.z*V.y)/total, (U.z*V.x-U.x*V.z)/total, (U.x*V.y-U.y*V.x)/total));
			_normals[(j*_width+i)*18+9] = normalVec.x;//(U.y*V.z-U.z*V.y)/total;
			_normals[(j*_width+i)*18+10] = normalVec.y;//(U.z*V.x-U.x*V.z)/total;
			_normals[(j*_width+i)*18+11] = normalVec.z;//(U.x*V.y-U.y*V.x)/total;

			_normals[(j*_width+i)*18+12] = normalVec.x;//(U.y*V.z-U.z*V.y)/total;
			_normals[(j*_width+i)*18+13] = normalVec.y;//(U.z*V.x-U.x*V.z)/total;
			_normals[(j*_width+i)*18+14] = normalVec.z;//(U.x*V.y-U.y*V.x)/total;

			_normals[(j*_width+i)*18+15] = normalVec.x;//(U.y*V.z-U.z*V.y)/total;
			_normals[(j*_width+i)*18+16] = normalVec.y;//(U.z*V.x-U.x*V.z)/total;
			_normals[(j*_width+i)*18+17] = normalVec.z;//(U.x*V.y-U.y*V.x)/total;
		}
	}

	//Calculate smooth normals
	_smoothNormals = new float[_width*_height*6*3];
	for (int i = 0;i < _width;i ++)
	{
		for (int k = 0;k < 18;k += 3)
		{
			_smoothNormals[((0)*_width+i)*18+k+0] = 0.0f;
			_smoothNormals[((0)*_width+i)*18+k+1] = 1.0f;
			_smoothNormals[((0)*_width+i)*18+k+2] = 0.0f;
		}
		for (int k = 0;k < 18;k += 3)
		{
			_smoothNormals[((_height-1)*_width+i)*18+k+0] = 0.0f;
			_smoothNormals[((_height-1)*_width+i)*18+k+1] = 1.0f;
			_smoothNormals[((_height-1)*_width+i)*18+k+2] = 0.0f;
		}
	}
	for (int i = 0;i < _height;i ++)
	{
		for (int k = 0;k < 18;k += 3)
		{
			_smoothNormals[((i)*_width+0)*18+k+0] = 0.0f;
			_smoothNormals[((i)*_width+0)*18+k+1] = 1.0f;
			_smoothNormals[((i)*_width+0)*18+k+2] = 0.0f;
		}
		for (int k = 0;k < 18;k += 3)
		{
			_smoothNormals[((i)*_width+(_width-1))*18+k+0] = 0.0f;
			_smoothNormals[((i)*_width+(_width-1))*18+k+1] = 1.0f;
			_smoothNormals[((i)*_width+(_width-1))*18+k+2] = 0.0f;
		}
	}
	for (int j = 1;j < _height;j ++)
	{
		for (int i = 1;i < _width;i ++)
		{
			glm::vec3 v1 = glm::vec3(_normals[((j-1)*_width+(i-1))*18+3], _normals[((j-1)*_width+(i-1))*18+4], _normals[((j-1)*_width+(i-1))*18+5]);
			glm::vec3 v2 = glm::vec3(_normals[((j-1)*_width+(i-1))*18+15], _normals[((j-1)*_width+(i-1))*18+16], _normals[((j-1)*_width+(i-1))*18+17]);
			glm::vec3 v3 = glm::vec3(_normals[((j-1)*_width+(i))*18+12], _normals[((j-1)*_width+(i))*18+13], _normals[((j-1)*_width+(i))*18+14]);
			glm::vec3 v4 = glm::vec3(_normals[((j)*_width+(i))*18], _normals[((j)*_width+(i))*18+1], _normals[((j)*_width+(i))*18+2]);
			glm::vec3 v5 = glm::vec3(_normals[((j)*_width+(i))*18+9], _normals[((j)*_width+(i))*18+10], _normals[((j)*_width+(i))*18+11]);
			glm::vec3 v6 = glm::vec3(_normals[((j)*_width+(i-1))*18+6], _normals[((j)*_width+(i-1))*18+7], _normals[((j)*_width+(i-1))*18+8]);

			glm::vec3 v = glm::normalize(v1+v2+v3+v4+v5+v6);

			_smoothNormals[((j-1)*_width+(i-1))*18+3] = v.x;
			_smoothNormals[((j-1)*_width+(i-1))*18+4] = v.y;
			_smoothNormals[((j-1)*_width+(i-1))*18+5] = v.z;

			_smoothNormals[((j-1)*_width+(i-1))*18+15] = v.x;
			_smoothNormals[((j-1)*_width+(i-1))*18+16] = v.y;
			_smoothNormals[((j-1)*_width+(i-1))*18+17] = v.z;

			_smoothNormals[((j-1)*_width+(i))*18+12] = v.x;
			_smoothNormals[((j-1)*_width+(i))*18+13] = v.y;
			_smoothNormals[((j-1)*_width+(i))*18+14] = v.z;

			_smoothNormals[((j)*_width+(i))*18] = v.x;
			_smoothNormals[((j)*_width+(i))*18+1] = v.y;
			_smoothNormals[((j)*_width+(i))*18+2] = v.z;

			_smoothNormals[((j)*_width+(i))*18+9] = v.x;
			_smoothNormals[((j)*_width+(i))*18+10] = v.y;
			_smoothNormals[((j)*_width+(i))*18+11] = v.z;

			_smoothNormals[((j)*_width+(i-1))*18+6] = v.x;
			_smoothNormals[((j)*_width+(i-1))*18+7] = v.y;
			_smoothNormals[((j)*_width+(i-1))*18+8] = v.z;
		}
	}

	_tangents = new float[_width*_height*6*3];
	_bitangents = new float[_width*_height*6*3];
	for (int j = 0;j < _height;j ++)
	{
		for (int i = 0;i < _width;i ++)
		{
			for (int tri = 0;tri < 2;tri ++)
			{
				glm::vec3 v0 = glm::vec3(_vertices[((j)*_width+(i))*18+(9*tri)+0], _vertices[((j)*_width+(i))*18+(9*tri)+1], _vertices[((j)*_width+(i))*18+(9*tri)+2]);
				glm::vec3 v1 = glm::vec3(_vertices[((j)*_width+(i))*18+(9*tri)+3], _vertices[((j)*_width+(i))*18+(9*tri)+4], _vertices[((j)*_width+(i))*18+(9*tri)+5]);
				glm::vec3 v2 = glm::vec3(_vertices[((j)*_width+(i))*18+(9*tri)+6], _vertices[((j)*_width+(i))*18+(9*tri)+7], _vertices[((j)*_width+(i))*18+(9*tri)+8]);

				glm::vec2 uv0 = glm::vec2(_textureCoords[((j)*_width+(i))*12+(6*tri)+0], _textureCoords[((j)*_width+(i))*12+(6*tri)+1]);
				glm::vec2 uv1 = glm::vec2(_textureCoords[((j)*_width+(i))*12+(6*tri)+2], _textureCoords[((j)*_width+(i))*12+(6*tri)+3]);
				glm::vec2 uv2 = glm::vec2(_textureCoords[((j)*_width+(i))*12+(6*tri)+4], _textureCoords[((j)*_width+(i))*12+(6*tri)+5]);

				glm::vec3 deltaV1 = v1 - v0;
				glm::vec3 deltaV2 = v2 - v0;

				glm::vec2 deltaUV1 = uv1 - uv0;
				glm::vec2 deltaUV2 = uv2 - uv0;

				float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
				glm::vec3 Tangent = (deltaV1 * deltaUV2.y - deltaV2 * deltaUV1.y)*r;
				glm::vec3 Bitangent = (deltaV2 * deltaUV1.x - deltaV1 * deltaUV2.x)*r;

				for (unsigned int vert = 0; vert < 3; vert++)
				{
					_tangents[((j)*_width+(i))*18+(9*tri)+(3*vert)+0] = Tangent.x;
					_tangents[((j)*_width+(i))*18+(9*tri)+(3*vert)+1] = Tangent.y;
					_tangents[((j)*_width+(i))*18+(9*tri)+(3*vert)+2] = Tangent.z;

					_bitangents[((j)*_width+(i))*18+(9*tri)+(3*vert)+0] = Bitangent.x;
					_bitangents[((j)*_width+(i))*18+(9*tri)+(3*vert)+1] = Bitangent.y;
					_bitangents[((j)*_width+(i))*18+(9*tri)+(3*vert)+2] = Bitangent.z;
				}
			}
		}
	}

	//Generate patches
	for (int j = 0;j < _height;j += _patchScale)
	{
		for (int i = 0;i < _width;i += _patchScale)
		{
			std::vector<float> patchVertices;
			for (int l = 0;l < _patchScale; l++)
				for (int k = 0;k < _patchScale; k++)
					for (int square = 0;square < 18;square ++)
						patchVertices.push_back(_vertices[((j+l)*_width+(i+k))*18+square]);

			std::vector<float> patchTexCoords;
			for (int l = 0;l < _patchScale; l++)
				for (int k = 0;k < _patchScale; k++)
					for (int square = 0;square < 12;square ++)
						patchTexCoords.push_back(_textureCoords[((j+l)*_width+(i+k))*12+square]);

			std::vector<float> patchSmoothNormals;
			for (int l = 0;l < _patchScale; l++)
				for (int k = 0;k < _patchScale; k++)
					for (int square = 0;square < 18;square ++)
						patchSmoothNormals.push_back(_smoothNormals[((j+l)*_width+(i+k))*18+square]);

			std::vector<float> patchTangents;
			for (int l = 0;l < _patchScale; l++)
				for (int k = 0;k < _patchScale; k++)
					for (int square = 0;square < 18;square ++)
						patchTangents.push_back(_tangents[((j+l)*_width+(i+k))*18+square]);

			std::vector<float> patchBitangents;
			for (int l = 0;l < _patchScale; l++)
				for (int k = 0;k < _patchScale; k++)
					for (int square = 0;square < 18;square ++)
						patchBitangents.push_back(_bitangents[((j+l)*_width+(i+k))*18+square]);

			std::vector<float> patchAlphaCoords;
			for (int l = 0;l < _patchScale; l++)
				for (int k = 0;k < _patchScale; k++)
					for (int square = 0;square < 12;square ++)
						patchAlphaCoords.push_back(_alphaCoords[((j+l)*_width+(i+k))*12+square]);

			_patchVector.push_back(new Phage::PhageTerrainMeshPatch());
			_patchVector.back()->Init();
			_patchVector.back()->CreatePatch(_patchScale, _patchScale, patchVertices, patchTexCoords, patchSmoothNormals, patchTangents, patchAlphaCoords);
		}
	}

	return 0;
}

float Phage::PhageTerrainMesh::GetHeight(int x, int y)
{
	return _heightMap[(y*(_width+1)+x)];
}

glm::ivec2 Phage::PhageTerrainMesh::GetSize()
{
	return glm::ivec2(_width, _height);
}

int Phage::PhageTerrainMesh::GetTextureScale()
{
	return _textureScale;
}

float Phage::PhageTerrainMesh::GetTextureDPI()
{
	return _dpi;
}

glm::vec3 Phage::PhageTerrainMesh::CalculateMouseOnTerrain(glm::vec3 mouseNear, glm::vec3 mouseZero)
{
	glm::vec3 camNear = mouseNear;
	glm::vec3 camFar = mouseZero;
	glm::vec3 directionVector = camFar - camNear;
	directionVector += TERRAIN_MESH_MIN_HEIGHT * directionVector / directionVector.y;
	camFar = camNear + directionVector;

	//Number of divisions in each direction
	int divisionsX = abs((int)camFar.x - (int)camNear.x);
	int divisionsY = abs((int)camFar.z - (int)camNear.z);
	int maxDivisions = divisionsX > divisionsY ? divisionsX : divisionsY;

	//Increment value in each direction
	int incrementX = directionVector.x < 0.0f ? -1 : 1;
	int incrementY = directionVector.z < 0.0f ? -1 : 1;

	//Determine which triangle to check first
	bool firstTriangle = false;
	if (abs(directionVector.x) > abs(directionVector.z))
	{
		if (directionVector.x < 0.0f)
			firstTriangle = true;
	}
	else
	{
		if (directionVector.z > 0.0f)
			firstTriangle = true;
	}

	Phage::ray ray;
	ray.CreateRayFromTwoPoints(camNear, camFar);

	glm::vec3 loopPoint = camNear;
	int startX = (int)camNear.x;
	int startY = (int)camNear.z;
	int loopX = 0;
	int loopY = 0;

	//terrainGridDebug->DrawLine(camera, glm::vec2(camNear.x, camNear.z), glm::vec2(camFar.x, camFar.z));

	glm::vec4 mouseIntersect = glm::vec4(-1.0f);
	glm::vec4 mousePoint = glm::vec4(-1.0f);

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
			loopY = (int)(camNear.z + ((abs(interX - camNear.x) / abs(directionVector.x)) * directionVector.z));

			if (loopX >= 0 && loopX < _width && loopY >= 0 && loopY < _height)
			{

				//terrainGridDebug->DrawGridSquare(camera, loopX, loopY);

				glm::vec3 v0 = glm::vec3((float)loopX, GetHeight(loopX, loopY), (float)loopY);
				glm::vec3 v1 = glm::vec3((float)loopX+1, GetHeight(loopX+1, loopY), (float)loopY);
				glm::vec3 v2 = glm::vec3((float)loopX+1, GetHeight(loopX+1, loopY+1), (float)loopY+1);
				glm::vec3 v3 = glm::vec3((float)loopX, GetHeight(loopX, loopY+1), (float)loopY+1);

				//TODO:Find a way to prioritize the triangle in the quad
				if (firstTriangle)
				{
					if (ray.CalculateIntersectTriangle(v0, v1, v2, &mouseIntersect) == 0)
					{
						if (mousePoint.w < 0.0f || mouseIntersect.w < mousePoint.w)
							mousePoint = mouseIntersect;
					}
					else if (ray.CalculateIntersectTriangle(v0, v2, v3, &mouseIntersect) == 0)
					{
						if (mousePoint.w < 0.0f || mouseIntersect.w < mousePoint.w)
							mousePoint = mouseIntersect;
					}
				}
				else
				{
					if (ray.CalculateIntersectTriangle(v0, v2, v3, &mouseIntersect) == 0)
					{
						if (mousePoint.w < 0.0f || mouseIntersect.w < mousePoint.w)
							mousePoint = mouseIntersect;
					}
					else if (ray.CalculateIntersectTriangle(v0, v1, v2, &mouseIntersect) == 0)
					{
						if (mousePoint.w < 0.0f || mouseIntersect.w < mousePoint.w)
							mousePoint = mouseIntersect;
					}
				}
			}
		}

		//Check in vertical direction
		if (loopCounter <= divisionsY)
		{
			loopY = startY + loopCounter * incrementY;
			float interY = (float)loopY;
			if (directionVector.z < 0.0f) interY ++;
			loopX = (int)(camNear.x + ((abs(interY - camNear.z) / abs(directionVector.z)) * directionVector.x));

			if (loopX >= 0 && loopX < _width && loopY >= 0 && loopY < _height)
			{

				//terrainGridDebug->DrawGridSquare(camera, loopX, loopY);

				glm::vec3 v0 = glm::vec3((float)loopX, GetHeight(loopX, loopY), (float)loopY);
				glm::vec3 v1 = glm::vec3((float)loopX+1, GetHeight(loopX+1, loopY), (float)loopY);
				glm::vec3 v2 = glm::vec3((float)loopX+1, GetHeight(loopX+1, loopY+1), (float)loopY+1);
				glm::vec3 v3 = glm::vec3((float)loopX, GetHeight(loopX, loopY+1), (float)loopY+1);

				//TODO:Find a way to prioritize the triangle in the quad
				if (firstTriangle)
				{
					if (ray.CalculateIntersectTriangle(v0, v1, v2, &mouseIntersect) == 0)
					{
						if (mousePoint.w < 0.0f || mouseIntersect.w < mousePoint.w)
							mousePoint = mouseIntersect;
					}
					else if (ray.CalculateIntersectTriangle(v0, v2, v3, &mouseIntersect) == 0)
					{
						if (mousePoint.w < 0.0f || mouseIntersect.w < mousePoint.w)
							mousePoint = mouseIntersect;
					}
				}
				else
				{
					if (ray.CalculateIntersectTriangle(v0, v2, v3, &mouseIntersect) == 0)
					{
						if (mousePoint.w < 0.0f || mouseIntersect.w < mousePoint.w)
							mousePoint = mouseIntersect;
					}
					else if (ray.CalculateIntersectTriangle(v0, v1, v2, &mouseIntersect) == 0)
					{
						if (mousePoint.w < 0.0f || mouseIntersect.w < mousePoint.w)
							mousePoint = mouseIntersect;
					}
				}
			}
		}

		loopCounter ++;
		if (loopCounter > maxDivisions)
		{
			loopFlag = true;
		}
	}

	return glm::vec3(mousePoint.x, mousePoint.y, mousePoint.z);
}

int Phage::PhageTerrainMesh::GetSubArea(glm::ivec2 p1, glm::ivec2 p2, std::vector<float>* vertices, std::vector<float>* normals, std::vector<float>* tangents)
{
	vertices->clear();
	normals->clear();
	tangents->clear();

	for (int j = p1.y;j < p2.y;j ++)
		for (int i = p1.x;i < p2.x;i ++)
			for (int k = 0;k < 18;k ++)
			{
				vertices->push_back(_vertices[(j*_width+i)*18+k]);
				normals->push_back(_normals[(j*_width+i)*18+k]);
				tangents->push_back(_tangents[(j*_width+i)*18+k]);
			}

	return 0;
}

int Phage::PhageTerrainMesh::Render(PhageShader* shader, int** losInfo, unsigned int losTexture)
{
	//if (_losLocation < 0)
		//_losLocation = shader->LoadUniformLocation(0, "losData");
	//if (_losTextureSamplerLocation < 0)
		//_losTextureSamplerLocation = shader->LoadUniformLocation(0, "losTextureSampler");

	glActiveTexture(GL_TEXTURE0+0);
	glBindTexture(GL_TEXTURE_2D, losTexture);

	//if (_fullLOSDataLocation < 0)
		//_fullLOSDataLocation = shader->LoadUniformLocation(0, "fullLOSData");
	//shader->SetUniformValue(0, _losLocation, (_width/16)*_height, &losInfo[0][0]);
	for (int i = 0;i < (int)_patchVector.size();i ++)
	{
		_patchVector[i]->Render();
	}

	return 0;
}
