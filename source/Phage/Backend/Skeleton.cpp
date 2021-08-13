#include "Skeleton.h"

#include "loadGLE.h"
#include "Debug/Debug.h"

Skeleton::Skeleton()
{
}

Skeleton::~Skeleton()
{
}

int Skeleton::CreateSkeleton(const aiScene* scene)
{
	_boneVertexVector.resize(scene->mMeshes[0]->mNumFaces * 3);

	for (unsigned int i = 0;i < scene->mMeshes[0]->mNumBones; i++)
	{
		std::string boneName = scene->mMeshes[0]->mBones[i]->mName.data;

		int boneIndex = 0;

		if (_boneMap.find(boneName) == _boneMap.end())
		{
			boneIndex = _boneMap.size();
			_offSetMatrixVector.push_back(scene->mMeshes[0]->mBones[i]->mOffsetMatrix);
			_boneMap[boneName] = boneIndex;
			DebugOutput("Added bone %s at %i\n", boneName.c_str(), boneIndex);
		}
		else
			boneIndex = _boneMap[boneName];

		for (unsigned int j = 0;j < scene->mMeshes[0]->mBones[i]->mNumWeights;j ++)
		{
			//NOTE: This will need to be changed when multiple meshes are involved. For example, if on second mesh, Mesh[0].numVertices + vertexID.
			int vertexID = scene->mMeshes[0]->mBones[i]->mWeights[j].mVertexId;
			float weight = scene->mMeshes[0]->mBones[i]->mWeights[j].mWeight;

			for (int k = 0;k < 4;k ++)
			{
				if (_boneVertexVector[vertexID].Weight[k] == 0.0f)
				{
					_boneVertexVector[vertexID].ID[k] = boneIndex;
					_boneVertexVector[vertexID].Weight[k] = weight;
					break;
				}
			}
		}
	}

	return 0;
}

const std::vector<Skeleton::BoneVertex>* Skeleton::GetBoneVertexVector()
{
	return &_boneVertexVector;
}

const std::map<std::string, int>* Skeleton::GetBoneMap()
{
	return &_boneMap;
}

int Skeleton::GetBoneAt(std::string str)
{
	//for (std::map<std::string, int>::iterator it = _boneMap.begin(); it != _boneMap.end(); ++it)
		//printf("Bone: %s, %i\n", it->first.c_str(), it->second);

	return _boneMap[str];
}

const std::vector<aiMatrix4x4>* Skeleton::GetOffSetMatrixVector()
{
	return &_offSetMatrixVector;
}
