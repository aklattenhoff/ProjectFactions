#include "Model.h"

#include "loadGLE.h"
#include "Debug/Debug.h"

//Class declarations
#include "Skeleton.h"
#include "Animation.h"
//#include "Graphics/Utility/Texture.h"

//#include "Graphics/Utility/TextureUtility.h"
//#include "Graphics/Camera.h"

Model::Model()
{
	_name = "invalid";

	_hasNormals = false;;
	_hasTangentsAndBitangents = false;
	_hasAnimation = false;

	_hasMainTexture = false;
	_hasBumpTexture = false;
	_hasSBCTexture = false;

	//_mainTexture = NULL;
	//_bumpTexture = NULL;
	//_sbcTexture = NULL;
}

Model::~Model()
{
	//if (_mainTexture)
		//TextureUtility::DeleteTexture(_mainTexture);
	//if (_bumpTexture)
		//TextureUtility::DeleteTexture(_bumpTexture);
	//if (_sbcTexture)
		//TextureUtility::DeleteTexture(_sbcTexture);
}

int Model::Init()
{
	_orientationFlag = ORIENTATION_Y;
	_scale = 1.0f;

	return 0;
}

int Model::SetName(std::string name)
{
	_name = name;

	return 0;
}

std::string Model::GetName()
{
	return _name;
}

int Model::SetOrientation(int orientationFlag)
{
	_orientationFlag = orientationFlag;

	return 0;
}

int Model::GetOrientation()
{
	return _orientationFlag;
}

int Model::SetScale(float scale)
{
	_scale = scale;

	return 0;
}

float Model::GetScale()
{
	return _scale;
}

int Model::LoadScene(std::string fileName)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(fileName, NULL);

	if (!scene)
		return -1;

	//Set info
	_numMeshes = scene->mNumMeshes;
	_totalNumFaces = 0;
	_hasNormals = true;
	_hasTangentsAndBitangents = true;
	for (unsigned int i = 0;i < _numMeshes;i ++)
	{
		_totalNumFaces += scene->mMeshes[i]->mNumFaces;
		if (!scene->mMeshes[i]->HasNormals())
			_hasNormals = false;
		if (!scene->mMeshes[i]->HasTangentsAndBitangents())
			_hasTangentsAndBitangents = false;
	}

	//Animation
	//_hasAnimation = scene->HasAnimations();
	_skeleton = new Skeleton();
	_skeleton->CreateSkeleton(scene);
	//if (_hasAnimation)
	{
		//_skeleton = new Skeleton();
		//_skeleton->CreateSkeleton(scene);
		//for (unsigned int i = 0;i < scene->mNumAnimations;i ++)
		{
			//_animationMap[i] = new Animation();
			//_animationMap[i]->CreateAnimation(scene, _skeleton, i, Animation::DEFAULT_RESOLUTION);
		}
	}

	float* modelVertices = new float[_totalNumFaces*3*3];
	float* modelTexCoords = new float[_totalNumFaces*3*2];
	float* normals = new float[_totalNumFaces*3*3];
	float* tangentNorms = new float[_totalNumFaces*3*3];
	float* bitangentNorms = new float[_totalNumFaces*3*3];
	float tallestPoint = 0.0f;
	unsigned int vertexCounter = 0;
	for (unsigned int k = 0;k < _numMeshes;k ++)
	{
		for (unsigned int i = 0;i < scene->mMeshes[k]->mNumFaces;i ++)
		{
			for (unsigned int j = 0;j < 3;j ++)
			{
				modelVertices[vertexCounter+i*3*3+j*3+0] = scene->mMeshes[k]->mVertices[scene->mMeshes[k]->mFaces[i].mIndices[j]].x;
				modelVertices[vertexCounter+i*3*3+j*3+1] = scene->mMeshes[k]->mVertices[scene->mMeshes[k]->mFaces[i].mIndices[j]].y;
				if (modelVertices[vertexCounter+i*3*3+j*3+1] > tallestPoint)
					tallestPoint = modelVertices[i*3*3+j*3+1];
				modelVertices[vertexCounter+i*3*3+j*3+2] = scene->mMeshes[k]->mVertices[scene->mMeshes[k]->mFaces[i].mIndices[j]].z;

				modelTexCoords[vertexCounter+i*3*2+j*2+0] = scene->mMeshes[k]->mTextureCoords[k][scene->mMeshes[k]->mFaces[i].mIndices[j]].x;
				modelTexCoords[vertexCounter+i*3*2+j*2+1] = scene->mMeshes[k]->mTextureCoords[k][scene->mMeshes[k]->mFaces[i].mIndices[j]].y;

				if (_hasNormals)
				{
					normals[vertexCounter+i*3*3+j*3+0] = scene->mMeshes[k]->mNormals[scene->mMeshes[k]->mFaces[i].mIndices[j]].x;
					normals[vertexCounter+i*3*3+j*3+1] = scene->mMeshes[k]->mNormals[scene->mMeshes[k]->mFaces[i].mIndices[j]].y;
					normals[vertexCounter+i*3*3+j*3+2] = scene->mMeshes[k]->mNormals[scene->mMeshes[k]->mFaces[i].mIndices[j]].z;
				}

				if (_hasTangentsAndBitangents)
				{
					tangentNorms[vertexCounter+i*3*3+j*3+0] = scene->mMeshes[k]->mTangents[scene->mMeshes[k]->mFaces[i].mIndices[j]].x;
					tangentNorms[vertexCounter+i*3*3+j*3+1] = scene->mMeshes[k]->mTangents[scene->mMeshes[k]->mFaces[i].mIndices[j]].y;
					tangentNorms[vertexCounter+i*3*3+j*3+2] = scene->mMeshes[k]->mTangents[scene->mMeshes[k]->mFaces[i].mIndices[j]].z;

					bitangentNorms[vertexCounter+i*3*3+j*3+0] = scene->mMeshes[k]->mBitangents[scene->mMeshes[k]->mFaces[i].mIndices[j]].x;
					bitangentNorms[vertexCounter+i*3*3+j*3+1] = scene->mMeshes[k]->mBitangents[scene->mMeshes[k]->mFaces[i].mIndices[j]].y;
					bitangentNorms[vertexCounter+i*3*3+j*3+2] = scene->mMeshes[k]->mBitangents[scene->mMeshes[k]->mFaces[i].mIndices[j]].z;
				}
			}
			if (!_hasTangentsAndBitangents)
			{
				glm::vec3 v0 = glm::vec3(modelVertices[vertexCounter+i*3*3+0*3+0], modelVertices[vertexCounter+i*3*3+0*3+1], modelVertices[vertexCounter+i*3*3+0*3+2]);
				glm::vec3 v1 = glm::vec3(modelVertices[vertexCounter+i*3*3+1*3+0], modelVertices[vertexCounter+i*3*3+1*3+1], modelVertices[vertexCounter+i*3*3+1*3+2]);
				glm::vec3 v2 = glm::vec3(modelVertices[vertexCounter+i*3*3+2*3+0], modelVertices[vertexCounter+i*3*3+2*3+1], modelVertices[vertexCounter+i*3*3+2*3+2]);

				glm::vec2 uv0 = glm::vec2(modelTexCoords[vertexCounter+i*3*2+0*2+0], modelTexCoords[vertexCounter+i*3*2+0*2+1]);
				glm::vec2 uv1 = glm::vec2(modelTexCoords[vertexCounter+i*3*2+1*2+0], modelTexCoords[vertexCounter+i*3*2+1*2+1]);
				glm::vec2 uv2 = glm::vec2(modelTexCoords[vertexCounter+i*3*2+2*2+0], modelTexCoords[vertexCounter+i*3*2+2*2+1]);

				glm::vec3 estimatedFaceNormal = glm::vec3(0.0f);
				for (unsigned int j = 0; j < 3; j++)
				{
					estimatedFaceNormal += glm::vec3(normals[vertexCounter+i*3*3+j*3+0], normals[vertexCounter+i*3*3+j*3+1], normals[vertexCounter+i*3*3+j*3+2]);
				}
				estimatedFaceNormal /= 3.0f;

				glm::vec3 deltaV1 = v1 - v0;
				glm::vec3 deltaV2 = v2 - v0;

				glm::vec2 deltaUV1 = uv1 - uv0;
				glm::vec2 deltaUV2 = uv2 - uv0;

				//glm::vec3 Normal = glm::vec3(normals[vertexCounter+i*3*3+0*3+0], normals[vertexCounter+i*3*3+0*3+1], normals[vertexCounter+i*3*3+0*3+2]);

				float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
				glm::vec3 Tangent = (deltaV1 * deltaUV2.y - deltaV2 * deltaUV1.y)*r;
				glm::vec3 Bitangent = (deltaV2 * deltaUV1.x - deltaV1 * deltaUV2.x)*r;

				float mirrorValue = glm::dot(glm::cross(Tangent, Bitangent), estimatedFaceNormal);
				if (mirrorValue < 0.0f)
				{
					Tangent = -Tangent;
					Bitangent = -Bitangent;
				}

				//glm::vec3 smoothBitangent = glm::cross(Normal, Tangent);
				//glm::vec3 smoothTangent = glm::cross(smoothBitangent, Normal);

				for (unsigned int j = 0; j < 3; j++)
				{
					tangentNorms[vertexCounter+i*3*3+j*3+0] = Tangent.x;
					tangentNorms[vertexCounter+i*3*3+j*3+1] = Tangent.y;
					tangentNorms[vertexCounter+i*3*3+j*3+2] = Tangent.z;

					bitangentNorms[vertexCounter+i*3*3+j*3+0] = Bitangent.x;
					bitangentNorms[vertexCounter+i*3*3+j*3+1] = Bitangent.y;
					bitangentNorms[vertexCounter+i*3*3+j*3+2] = Bitangent.z;
				}
			}
		}
		vertexCounter += scene->mMeshes[k]->mNumFaces*3*3;
	}

	glGenVertexArrays(1, &_vao);
	glGenBuffers(1, &_vbo);
	glGenBuffers(1, &_tbo);
	glGenBuffers(1, &_nbo);
	glGenBuffers(1, &_bbo);
	glGenBuffers(1, &_tnbo);
	glGenBuffers(1, &_bnbo);

	glBindVertexArray(_vao);

	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, (_totalNumFaces*3*3)*4, modelVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, _tbo);
    glBufferData(GL_ARRAY_BUFFER, (_totalNumFaces*3*2)*4, modelTexCoords, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, _nbo);
    glBufferData(GL_ARRAY_BUFFER, _totalNumFaces*3*3*4, normals, GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, _bbo);
    glBufferData(GL_ARRAY_BUFFER, _totalNumFaces*3*32, &_skeleton->GetBoneVertexVector()->at(0), GL_STATIC_DRAW);
	glEnableVertexAttribArray(3);
	glVertexAttribIPointer(3, 4, GL_INT, sizeof(Skeleton::BoneVertex), (const GLvoid*)0);
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(Skeleton::BoneVertex), (const GLvoid*)16);

	glBindBuffer(GL_ARRAY_BUFFER, _tnbo);
    glBufferData(GL_ARRAY_BUFFER, _totalNumFaces*3*3*4, tangentNorms, GL_STATIC_DRAW);
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, _bnbo);
    glBufferData(GL_ARRAY_BUFFER, _totalNumFaces*3*3*4, bitangentNorms, GL_STATIC_DRAW);
	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindVertexArray(0);
	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	//Clean up arrays
	delete[] modelVertices;
	delete[] modelTexCoords;
	delete[] normals;
	delete[] tangentNorms;
	delete[] bitangentNorms;

	importer.FreeScene();

	return 0;
}

int Model::LoadMainTexture(std::string fileName)
{
	//int error = 0;
	//_mainTexture = TextureUtility::CreateTextureFromFile(fileName.c_str(), true, true, &error);
	//if (error != TextureUtility::NONE || !_mainTexture)
		//return -1;

	_mainTexturerTexture.file = fileName;
	_mainTexturerTexture.yflip = true;
	_mainTexturerTexture.mipmap = true;
	TexturerSingleton->LoadTexture(&_mainTexturerTexture);

	_hasMainTexture = true;

	return 0;
}

int Model::LoadBumpTexture(std::string fileName)
{
	//int error = 0;
	//_bumpTexture = TextureUtility::CreateTextureFromFile(fileName.c_str(), true, true, &error);
	//if (error != TextureUtility::NONE || !_bumpTexture)
		//return -1;

	_bumpTexturerTexture.file = fileName;
	_bumpTexturerTexture.yflip = true;
	_bumpTexturerTexture.mipmap = true;
	TexturerSingleton->LoadTexture(&_bumpTexturerTexture);

	_hasBumpTexture = true;

	return 0;
}

int Model::LoadSBCTexture(std::string fileName)
{
	//int error = 0;
	//_sbcTexture = TextureUtility::CreateTextureFromFile(fileName.c_str(), true, true, &error);
	//if (error != TextureUtility::NONE || !_sbcTexture)
		//return -1;

	_sbcTexturerTexture.file = fileName;
	_sbcTexturerTexture.yflip = true;
	_sbcTexturerTexture.mipmap = true;
	TexturerSingleton->LoadTexture(&_sbcTexturerTexture);

	_hasSBCTexture = true;

	return 0;
}

int Model::AddAnimation(std::string fileName, int animFlag, float animSpeed)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(fileName, NULL);

	if (!scene)
		return -1;

	//Animation
	_hasAnimation = _hasAnimation | scene->HasAnimations();
	if (scene->HasAnimations())
	{
		_animationMap[animFlag] = new Animation();
		_animationMap[animFlag]->CreateAnimation(scene, _skeleton, 0, Animation::DEFAULT_RESOLUTION, animSpeed);
	}

	importer.FreeScene();

	return 0;
}

bool Model::HasAnimation()
{
	return _hasAnimation;
}

Animation* Model::GetAnimation(int animFlag)
{
	Animation* animation = NULL;
	try
	{
		animation = _animationMap.at(animFlag);
	}
	catch (const std::out_of_range&)
	{
		return NULL;
	}
	return animation;
}

/*int Model::Draw(Camera* camera, int animationIndex, float timeInSeconds)
{
	if (GetAnimation(animationIndex))
	{
		camera->GetShader()->SetAnimation(1);
		//std::vector<aiMatrix4x4>* transformationMatrixVector = _animationMap[animationIndex]->GetAnimationMatrixVector(timeInSeconds);
		//camera->GetShader()->SetBonesMatrices(transformationMatrixVector->size(), &transformationMatrixVector->at(0).a1);
		std::vector<aiMatrix4x4> transformationMatrixVector = _animationMap[animationIndex]->GetInterAnimMatrices(timeInSeconds, _skeleton);
		camera->GetShader()->SetBonesMatrices(transformationMatrixVector.size(), &transformationMatrixVector.at(0).a1);
	}

	//Draw Vertices
	glBindVertexArray(_vao);

	glActiveTexture(GL_TEXTURE0+3);
	//glBindTexture(GL_TEXTURE_2D, _bumpTexture->GetTextureInt());
	glActiveTexture(GL_TEXTURE0+4);
	//glBindTexture(GL_TEXTURE_2D, _sbcTexture->GetTextureInt());
	glActiveTexture(GL_TEXTURE0);

	//glBindTexture(GL_TEXTURE_2D, _mainTexture->GetTextureInt());
	glDrawArrays(GL_TRIANGLES, 0, _totalNumFaces*3);

	//camera->GetShader()->SetBump(0);

	glBindVertexArray(0);

	if (GetAnimation(animationIndex))
		camera->GetShader()->SetAnimation(0);

	return 0;
}*/

int Model::Draw()
{
	//Draw Vertices
	glBindVertexArray(_vao);

	glActiveTexture(GL_TEXTURE0+3);
	//glBindTexture(GL_TEXTURE_2D, _bumpTexture->GetTextureInt());
	glBindTexture(GL_TEXTURE_2D, _bumpTexturerTexture.id);
	glActiveTexture(GL_TEXTURE0+4);
	//glBindTexture(GL_TEXTURE_2D, _sbcTexture->GetTextureInt());
	glBindTexture(GL_TEXTURE_2D, _sbcTexturerTexture.id);
	glActiveTexture(GL_TEXTURE0);

	//glBindTexture(GL_TEXTURE_2D, _mainTexture->GetTextureInt());
	glBindTexture(GL_TEXTURE_2D, _mainTexturerTexture.id);
	glDrawArrays(GL_TRIANGLES, 0, _totalNumFaces*3);

	glBindVertexArray(0);

	return 0;
}

Skeleton* Model::GetSkeleton()
{
	return _skeleton;
}

std::vector<aiMatrix4x4> Model::GetBoneMatrices(int animIndex, float animSecs)
{
	return _animationMap[animIndex]->GetInterAnimMatrices(animSecs, _skeleton);
	//return (*_animationMap[animIndex]->GetAnimationMatrixVector(animSecs));
}
