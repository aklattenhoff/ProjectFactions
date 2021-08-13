#ifndef _ANIMATION_H
#define _ANIMATION_H

#include <map>
#include <string>
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Skeleton;

class Animation
{
public:
	struct PropDesc
	{
		std::string name;
		std::string boneAttachment;
		aiVector3D offsetTranslate;
		aiVector3D offsetRotate;
		aiVector3D offsetScale;
		aiVector2D animTime;
	};
	struct BoneTransform
	{
		int index;
		aiVector3D translation;
		aiQuaternion rotation;
		aiVector3D scale;
	};
	struct KeyFrame
	{
		std::vector<BoneTransform> transformVector;
		float time;
	};
	struct SoundEffect
	{
		std::string name;
		float time;
	};
public:
	const static int DEFAULT_RESOLUTION = 10;
public:
	Animation();
	~Animation();

	/* CreateAnimation
	*  
	*  @param scene The pointer to the assimp scene
	*  @param skeleton The pointer to the Skeleton. Must have been created previously
	*  @param animIndex The index for the animation contained in the scene
	*  @param animResolution The number of frames per second that will be stored for the animation
	*  NOTE: The total number of frames that will be stored for the animation will be the resolution * durationInSeconds
	* 
	*  @return The error code if any
	*/
	int CreateAnimation(const aiScene* scene, Skeleton* skeleton, int animIndex, int animResolution, float animSpeed);

	bool HasProps();
	int AddProp(Animation::PropDesc prop);
	int NumberOfProps();
	Animation::PropDesc GetPropAtIndex(int index);

	int AddSoundEffect(std::string name, float time);
	int GetSoundEffectSize();
	std::string GetSoundEffectName(int index);
	float GetSoundEffectTime(int index);

	aiVector3D GetBoneTranslation(int boneIndex, float timeInSeconds);
	aiQuaternion GetBoneRotation(int boneIndex, float timeInSeconds);

	std::vector<aiMatrix4x4>* GetAnimationMatrixVector(float timeInSeconds);
	std::vector<aiMatrix4x4> GetInterAnimMatrices(float timeInSeconds, Skeleton* skeleton);
	float GetDurationInSeconds();
	float GetAnimationSpeed();
	aiVector3D GetRHandTrans(float timeInSeconds);
	aiQuaternion GetRHandRot(float timeInSeconds);
	aiMatrix4x4 GetRHandMat(float timeInSeconds);
private:
	int TraverseBoneTree(float time, aiNode* node, aiMatrix4x4 transform, aiAnimation* animation, Skeleton* skeleton, std::vector<aiMatrix4x4>* animMatrixVector);
	int TraverseKeyFrame(float time, aiNode* node, aiMatrix4x4 transform, aiAnimation* animation, Skeleton* skeleton, KeyFrame* keyframe);
	int TraverseKeyFrameWithIndex(int index, aiNode* node, aiMatrix4x4 transform, aiAnimation* animation, Skeleton* skeleton, KeyFrame* keyframe);

	aiVector3D CalcInterPosition(float time, aiNodeAnim* nodeAnim);
	aiQuaternion CalcInterRotation(float time, aiNodeAnim* nodeAnim);
	aiVector3D CalcInterScale(float time, aiNodeAnim* nodeAnim);
private:
	int _animationResolution;
	float _durationInSeconds;
	float _ticksPerSecond;
	float _animationSpeed;
	aiMatrix4x4 _rootTransformationInverse;

	const std::map<std::string, int>* _boneMap;

	std::vector<std::vector<aiMatrix4x4>> _animationMatrixVectorVector;
	std::vector<KeyFrame> _keyframeVector;

	std::vector<aiVector3D> _RHandTransVector;
	std::vector<aiQuaternion> _RHandRotVector;
	std::vector<aiMatrix4x4> _RHandMatrixVector;

	bool _hasPropsFlag;
	std::vector<Animation::PropDesc> _propDescVector;

	std::vector<SoundEffect> _sfxVector;
};
#endif
