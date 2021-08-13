#ifndef _SKELETON_H
#define _SKELETON_H

#include <vector>
#include <map>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Skeleton
{
public:
	struct BoneVertex
	{
		int ID[4];
		float Weight[4];
		BoneVertex()
		{
			memset(ID, 0, sizeof(int)*4);
			memset(Weight, 0, sizeof(int)*4);
		}
	};
public:
	Skeleton();
	~Skeleton();

	int CreateSkeleton(const aiScene* scene);

	const std::vector<BoneVertex>* GetBoneVertexVector();
	const std::map<std::string, int>* GetBoneMap();
	int GetBoneAt(std::string str);
	const std::vector<aiMatrix4x4>* GetOffSetMatrixVector();
private:
	std::vector<BoneVertex> _boneVertexVector;
	std::map<std::string, int> _boneMap;
	std::vector<aiMatrix4x4> _offSetMatrixVector;
};
#endif
