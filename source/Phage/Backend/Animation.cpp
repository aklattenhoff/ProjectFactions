#include "Animation.h"

#include "loadGLE.h"
#include "Debug/Debug.h"

#include "Skeleton.h"

Animation::Animation()
{
	_animationResolution = DEFAULT_RESOLUTION;
	_hasPropsFlag = false;
}

Animation::~Animation()
{
}

int Animation::CreateAnimation(const aiScene* scene, Skeleton* skeleton, int animIndex, int animResolution, float animSpeed)
{
	aiAnimation* animation = scene->mAnimations[animIndex];
	_animationResolution = animResolution;
	_animationSpeed = animSpeed;

	//Set the transformation inverse for the root node
	_rootTransformationInverse = scene->mRootNode->mTransformation;
	_rootTransformationInverse.Inverse();

	//Calculate number of animation frames
	_ticksPerSecond = (float)(animation->mTicksPerSecond == 0.0 ? 24.0 : animation->mTicksPerSecond);
	_durationInSeconds = (float)animation->mDuration / _ticksPerSecond;
	int totalFrames = (int)((float)_animationResolution * _durationInSeconds);

	//Resize animation vectors
	_animationMatrixVectorVector.resize(totalFrames);
	for (int i = 0;i < totalFrames;i ++)
		_animationMatrixVectorVector[i].resize(skeleton->GetOffSetMatrixVector()->size());

	DebugOutput("Total Frames: %i\n", totalFrames);

	//Calculate animation for each time frame
	aiMatrix4x4 identity;
	for (int i = 0;i < totalFrames;i ++)
	{
		float timeInSeconds = (float)i / (float)_animationResolution;
		float ticksPerSecond = _ticksPerSecond;
		float timeInTicks = timeInSeconds * ticksPerSecond;
		TraverseBoneTree(timeInTicks, scene->mRootNode, identity, animation, skeleton, &_animationMatrixVectorVector[i]);
	}

	float currentTime = 0.0f;
	float nextTime = 1000.0f;
	bool foundAllKeyframes = false;
	bool foundAKeyFrame = false;
	int numberOfKeyFrames = 0;
	/*for (float i = 0;i < _durationInSeconds;i += _durationInSeconds/24.0f)
	{
		_keyframeVector.push_back(KeyFrame());
		_keyframeVector.back().time = i;
		TraverseKeyFrame(i, scene->mRootNode, identity, animation, skeleton, &_keyframeVector.back());
	}*/

	//Make a copy of the bone map
	_boneMap = skeleton->GetBoneMap();

	//Find all the keyframes in an animation and create a KeyFrame object for each one
	while (!foundAllKeyframes)
	{
		numberOfKeyFrames ++;
		_keyframeVector.push_back(KeyFrame());
		_keyframeVector.back().time = currentTime;
		TraverseKeyFrame(currentTime, scene->mRootNode, identity, animation, skeleton, &_keyframeVector.back());

		for (unsigned int i = 0;i < animation->mNumChannels;i ++)
		{
			for (unsigned int j = 0;j < animation->mChannels[i]->mNumPositionKeys;j ++)
			{
				if (animation->mChannels[i]->mPositionKeys[j].mTime > currentTime)
				{
					//if (nextTime <= currentTime)
					//{
						//nextTime = animation->mChannels[i]->mPositionKeys[j].mTime;
					//}
					if (animation->mChannels[i]->mPositionKeys[j].mTime < nextTime)
					{
						nextTime = (float)animation->mChannels[i]->mPositionKeys[j].mTime;
						foundAKeyFrame = true;
					}
				}
			}
		}
		if (!foundAKeyFrame)
			foundAllKeyframes = true;
		else
			currentTime = nextTime;

		nextTime = 1000.0f;
		foundAKeyFrame = false;
	}
	/*for (unsigned int chanIndex = 0; chanIndex < animation->mNumChannels; chanIndex++)
	{
		for (unsigned int keyIndex = 0; keyIndex < animation->mChannels[chanIndex]->mNumPositionKeys; keyIndex++)
		{
			float time = animation->mChannels[chanIndex]->mPositionKeys[keyIndex].mTime;
		}
	}*/
	//if (_keyframeVector.size() > 0)
		//_keyframeVector.erase(_keyframeVector.end());
	DebugOutput("Number of keyframes: %i\n", numberOfKeyFrames);
	DebugOutput("Time:%f Last:%f\n", _durationInSeconds, _keyframeVector[numberOfKeyFrames-1].time);

	return 0;
}

bool Animation::HasProps()
{
	return _hasPropsFlag;
}

int Animation::AddProp(Animation::PropDesc prop)
{
	_propDescVector.push_back(prop);

	_hasPropsFlag = true;

	return 0;
}

int Animation::NumberOfProps()
{
	return (int)_propDescVector.size();
}

Animation::PropDesc Animation::GetPropAtIndex(int index)
{
	return _propDescVector.at(index);
}

int Animation::AddSoundEffect(std::string name, float time)
{
	SoundEffect newSfx;
	newSfx.name = name;
	newSfx.time = time;

	_sfxVector.push_back(newSfx);

	return 0;
}

int Animation::GetSoundEffectSize()
{
	return (int)_sfxVector.size();
}

std::string Animation::GetSoundEffectName(int index)
{
	return _sfxVector[index].name;
}

float Animation::GetSoundEffectTime(int index)
{
	return _sfxVector[index].time;
}

aiVector3D Animation::GetBoneTranslation(int boneIndex, float timeInSeconds)
{
	timeInSeconds = fmod(timeInSeconds*_animationSpeed, _durationInSeconds);
	//Find index of time interval before time
	int indexA = 0;
	bool withinKeyFrame = false;
	for (unsigned int i = 0;i < _keyframeVector.size() - 1;i ++)
	{
		if (timeInSeconds < _keyframeVector[i+1].time)
		{
			indexA = i;
			withinKeyFrame = true;
			break;
		}
	}

	int indexB = indexA + 1;

	if (!withinKeyFrame)
	{
		indexA = _keyframeVector.size() - 1;
		indexB = 0;
	}

	float timeDelta = (_keyframeVector[indexB].time - _keyframeVector[indexA].time);
	float factor = (timeInSeconds - _keyframeVector[indexA].time) / timeDelta;

	if (!withinKeyFrame)
	{
		timeDelta = _durationInSeconds - _keyframeVector[indexA].time;
		factor = (timeInSeconds - _keyframeVector[indexA].time) / timeDelta;
	}

	aiVector3D deltaTranslation = _keyframeVector[indexB].transformVector[boneIndex].translation - _keyframeVector[indexA].transformVector[boneIndex].translation;
	aiVector3D newTranslation = _keyframeVector[indexA].transformVector[boneIndex].translation + factor * deltaTranslation;

	return newTranslation;
}

aiQuaternion Animation::GetBoneRotation(int boneIndex, float timeInSeconds)
{
	timeInSeconds = fmod(timeInSeconds*_animationSpeed, _durationInSeconds);
	//Find index of time interval before time
	int indexA = 0;
	bool withinKeyFrame = false;
	for (unsigned int i = 0;i < _keyframeVector.size() - 1;i ++)
	{
		if (timeInSeconds < _keyframeVector[i+1].time)
		{
			indexA = i;
			withinKeyFrame = true;
			break;
		}
	}

	int indexB = indexA + 1;

	if (!withinKeyFrame)
	{
		indexA = _keyframeVector.size() - 1;
		indexB = 0;
	}

	float timeDelta = (_keyframeVector[indexB].time - _keyframeVector[indexA].time);
	float factor = (timeInSeconds - _keyframeVector[indexA].time) / timeDelta;

	if (!withinKeyFrame)
	{
		timeDelta = _durationInSeconds - _keyframeVector[indexA].time;
		factor = (timeInSeconds - _keyframeVector[indexA].time) / timeDelta;
	}
	
	aiQuaternion returnQuat;
	aiQuaternion::Interpolate(returnQuat, _keyframeVector[indexA].transformVector[boneIndex].rotation, _keyframeVector[indexB].transformVector[boneIndex].rotation, factor);

	return returnQuat;
}

std::vector<aiMatrix4x4>* Animation::GetAnimationMatrixVector(float timeInSeconds)
{
	//printf("Retrieving animaitonVector at %i\n", (int)((float)animationResolution * fmod(timeInSeconds, durationInSeconds)));
	int index = (int)((float)_animationResolution * fmod(timeInSeconds, _durationInSeconds));
	if (index < 0) index = 0;
	if (index >= (int)_animationMatrixVectorVector.size()) index = (int)_animationMatrixVectorVector.size()-1;
	return &_animationMatrixVectorVector[index];
}

std::vector<aiMatrix4x4> Animation::GetInterAnimMatrices(float timeInSeconds, Skeleton* skeleton)
{
	timeInSeconds = fmod(timeInSeconds*_animationSpeed, _durationInSeconds);
	//Find index of time interval before time
	int indexA = 0;
	bool withinKeyFrame = false;
	for (unsigned int i = 0;i < _keyframeVector.size() - 1;i ++)
	{
		if (timeInSeconds < _keyframeVector[i+1].time)
		{
			indexA = i;
			withinKeyFrame = true;
			break;
		}
	}

	int indexB = indexA + 1;

	if (!withinKeyFrame)
	{
		indexA = _keyframeVector.size() - 1;
		indexB = 0;
	}

	float timeDelta = (_keyframeVector[indexB].time - _keyframeVector[indexA].time);
	float factor = (timeInSeconds - _keyframeVector[indexA].time) / timeDelta;

	if (!withinKeyFrame)
	{
		timeDelta = _durationInSeconds - _keyframeVector[indexA].time;
		factor = (timeInSeconds - _keyframeVector[indexA].time) / timeDelta;
	}

	std::vector<aiMatrix4x4> returnMatrices;
	returnMatrices.resize(_keyframeVector[indexA].transformVector.size());
	//Interpolate
	for (unsigned int i = 0;i < _keyframeVector[indexA].transformVector.size();i ++)
	{
		aiVector3D deltaTranslation = _keyframeVector[indexB].transformVector[i].translation - _keyframeVector[indexA].transformVector[i].translation;
		aiVector3D newTranslation = _keyframeVector[indexA].transformVector[i].translation + factor * deltaTranslation;

		aiQuaternion newRotation;
		aiQuaternion::Interpolate(newRotation, _keyframeVector[indexA].transformVector[i].rotation, _keyframeVector[indexB].transformVector[i].rotation, factor);

		aiVector3D deltaScale = _keyframeVector[indexB].transformVector[i].scale - _keyframeVector[indexA].transformVector[i].scale;
		aiVector3D newScale = _keyframeVector[indexA].transformVector[i].scale + factor * deltaScale;

		aiMatrix4x4 translationMatrix;
		aiMatrix4x4::Translation(newTranslation, translationMatrix);

		aiMatrix4x4 rotationMatrix = aiMatrix4x4(newRotation.GetMatrix());

		aiMatrix4x4 scaleMatrix;
		aiMatrix4x4::Scaling(newScale, scaleMatrix);

		returnMatrices[i] = translationMatrix * rotationMatrix * scaleMatrix * skeleton->GetOffSetMatrixVector()->at(_keyframeVector[indexA].transformVector[i].index);
	}

	return returnMatrices;
}

float Animation::GetDurationInSeconds()
{
	return _durationInSeconds;
}

float Animation::GetAnimationSpeed()
{
	return _animationSpeed;
}

aiVector3D Animation::GetRHandTrans(float timeInSeconds)
{
	float modTime = fmod(timeInSeconds*_animationSpeed, _durationInSeconds);
	int indexA = (int)((float)_animationResolution * modTime);
	int indexB = indexA + 1;

	if (indexA < 0) indexA = 0;
	if (indexA >= (int)_animationMatrixVectorVector.size()) indexA = (int)_animationMatrixVectorVector.size()-1;

	if (indexA >= (int)_animationMatrixVectorVector.size() - 1) indexB = 0;

	if (indexB < 0) indexB = 0;
	if (indexB >= (int)_animationMatrixVectorVector.size()) indexB = (int)_animationMatrixVectorVector.size()-1;

	float timeA = (float)indexA / (float)_animationResolution;
	float timeB = timeA + (1.0f / (float)_animationResolution);

	float coefA = timeB-modTime;
	float coefB = modTime-timeA;
	float divisor = timeB-timeA;

	aiVector3D returnVec(-1.0f);
	if ((int)_RHandTransVector.size() > indexA && (int)_RHandTransVector.size() > indexB)
	{
		returnVec.x = (_RHandTransVector[indexA].x * coefA + _RHandTransVector[indexB].x * coefB) / divisor;
		returnVec.y = (_RHandTransVector[indexA].y * coefA + _RHandTransVector[indexB].y * coefB) / divisor;
		returnVec.z = (_RHandTransVector[indexA].z * coefA + _RHandTransVector[indexB].z * coefB) / divisor;
	}

	return returnVec;
}

aiQuaternion Animation::GetRHandRot(float timeInSeconds)
{
	float modTime = fmod(timeInSeconds*_animationSpeed, _durationInSeconds);
	int indexA = (int)((float)_animationResolution * modTime);
	int indexB = indexA + 1;

	if (indexA < 0) indexA = 0;
	if (indexA >= (int)_animationMatrixVectorVector.size()) indexA = (int)_animationMatrixVectorVector.size()-1;

	if (indexA >= (int)_animationMatrixVectorVector.size() - 1) indexB = 0;

	if (indexB < 0) indexB = 0;
	if (indexB >= (int)_animationMatrixVectorVector.size()) indexB = (int)_animationMatrixVectorVector.size()-1;

	float timeA = (float)indexA / (float)_animationResolution;
	float timeB = timeA + (1.0f / (float)_animationResolution);

	float coefA = timeB-modTime;
	float coefB = modTime-timeA;
	float divisor = timeB-timeA;

	aiVector3D returnVec(-1.0f);
	if ((int)_RHandRotVector.size() > indexA && (int)_RHandRotVector.size() > indexB)
	{
		returnVec.x = (_RHandRotVector[indexA].x * coefA + _RHandRotVector[indexB].x * coefB) / divisor;
		returnVec.y = (_RHandRotVector[indexA].y * coefA + _RHandRotVector[indexB].y * coefB) / divisor;
		returnVec.z = (_RHandRotVector[indexA].z * coefA + _RHandRotVector[indexB].z * coefB) / divisor;
	}

	float timeDelta = (float)(timeB - timeA);
	float factor = (modTime - timeA) / timeDelta;
	
	aiQuaternion returnQuat;
	if ((int)_RHandRotVector.size() > indexA && (int)_RHandRotVector.size() > indexB)
		aiQuaternion::Interpolate(returnQuat, _RHandRotVector[indexA], _RHandRotVector[indexB], factor);

	if (_RHandRotVector.size() > 0)
		return returnQuat;
	//return returnVec;
	if (_RHandRotVector.size() > 0)
	{
		int index = (int)((float)_animationResolution * fmod(timeInSeconds, _durationInSeconds));
		if (index < 0) index = 0;
		if (index >= (int)_RHandRotVector.size()) index = (int)_RHandRotVector.size()-1;
		return _RHandRotVector[index];
	}
	else
		return aiQuaternion(-1.0f, -1.0f, -1.0f, -1.0f);
}

aiMatrix4x4 Animation::GetRHandMat(float timeInSeconds)
{
	float modTime = fmod(timeInSeconds*_animationSpeed, _durationInSeconds);
	int indexA = (int)((float)_animationResolution * modTime);
	int indexB = indexA + 1;

	if (indexA < 0) indexA = 0;
	if (indexA >= (int)_RHandMatrixVector.size()) indexA = (int)_RHandMatrixVector.size()-1;

	if (indexA >= (int)_RHandMatrixVector.size() - 1) indexB = 0;

	if (indexB < 0) indexB = 0;
	if (indexB >= (int)_RHandMatrixVector.size()) indexB = (int)_RHandMatrixVector.size()-1;

	float timeA = (float)indexA / (float)_animationResolution;
	float timeB = timeA + (1.0f / (float)_animationResolution);

	float coefA = timeB-modTime;
	float coefB = modTime-timeA;
	float divisor = timeB-timeA;

	aiMatrix4x4 returnMatrices(-1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f);
	if ((int)_RHandMatrixVector.size() > indexA && (int)_RHandMatrixVector.size() > indexB)
	{
		returnMatrices.a1 = (_RHandMatrixVector[indexA].a1 * coefA + _RHandMatrixVector[indexB].a1 * coefB) / divisor;
		returnMatrices.a2 = (_RHandMatrixVector[indexA].a2 * coefA + _RHandMatrixVector[indexB].a2 * coefB) / divisor;
		returnMatrices.a3 = (_RHandMatrixVector[indexA].a3 * coefA + _RHandMatrixVector[indexB].a3 * coefB) / divisor;
		returnMatrices.a4 = (_RHandMatrixVector[indexA].a4 * coefA + _RHandMatrixVector[indexB].a4 * coefB) / divisor;

		returnMatrices.b1 = (_RHandMatrixVector[indexA].b1 * coefA + _RHandMatrixVector[indexB].b1 * coefB) / divisor;
		returnMatrices.b2 = (_RHandMatrixVector[indexA].b2 * coefA + _RHandMatrixVector[indexB].b2 * coefB) / divisor;
		returnMatrices.b3 = (_RHandMatrixVector[indexA].b3 * coefA + _RHandMatrixVector[indexB].b3 * coefB) / divisor;
		returnMatrices.b4 = (_RHandMatrixVector[indexA].b4 * coefA + _RHandMatrixVector[indexB].b4 * coefB) / divisor;

		returnMatrices.c1 = (_RHandMatrixVector[indexA].c1 * coefA + _RHandMatrixVector[indexB].c1 * coefB) / divisor;
		returnMatrices.c2 = (_RHandMatrixVector[indexA].c2 * coefA + _RHandMatrixVector[indexB].c2 * coefB) / divisor;
		returnMatrices.c3 = (_RHandMatrixVector[indexA].c3 * coefA + _RHandMatrixVector[indexB].c3 * coefB) / divisor;
		returnMatrices.c4 = (_RHandMatrixVector[indexA].c4 * coefA + _RHandMatrixVector[indexB].c4 * coefB) / divisor;

		returnMatrices.d1 = (_RHandMatrixVector[indexA].d1 * coefA + _RHandMatrixVector[indexB].d1 * coefB) / divisor;
		returnMatrices.d2 = (_RHandMatrixVector[indexA].d2 * coefA + _RHandMatrixVector[indexB].d2 * coefB) / divisor;
		returnMatrices.d3 = (_RHandMatrixVector[indexA].d3 * coefA + _RHandMatrixVector[indexB].d3 * coefB) / divisor;
		returnMatrices.d4 = (_RHandMatrixVector[indexA].d4 * coefA + _RHandMatrixVector[indexB].d4 * coefB) / divisor;
	}

	return returnMatrices;
}

int Animation::TraverseBoneTree(float time, aiNode* node, aiMatrix4x4 transform, aiAnimation* animation, Skeleton* skeleton, std::vector<aiMatrix4x4>* animMatrixVector)
{
	std::string nodeName = node->mName.data;
	aiNodeAnim* nodeAnim = NULL;

	//Find the nodeAnim given the name
	for (unsigned int i = 0;i < animation->mNumChannels;i ++)
	{
		if (nodeName.compare(animation->mChannels[i]->mNodeName.C_Str()) == 0)
			nodeAnim = animation->mChannels[i];
	}

	//Set default transformation matrix in case node does not have animation associated with it
	aiMatrix4x4 nodeTransform = node->mTransformation;

	//Interpolate for given time if there is animation
	if (nodeAnim)
	{
		aiVector3D scaleVector3D = CalcInterScale(time, nodeAnim);
		aiMatrix4x4 scaleMatrix;
		aiMatrix4x4::Scaling(scaleVector3D, scaleMatrix);

		aiQuaternion rotationVector3D = CalcInterRotation(time, nodeAnim);
		aiMatrix4x4 rotationMatrix = aiMatrix4x4(rotationVector3D.GetMatrix());

		aiVector3D translationVector3D = CalcInterPosition(time, nodeAnim);
		aiMatrix4x4 translationMatrix;
		aiMatrix4x4::Translation(translationVector3D, translationMatrix);

		nodeTransform = translationMatrix * rotationMatrix * scaleMatrix;
	}

	aiMatrix4x4 globalTransform = transform * nodeTransform;

	//Assigns matrix to the output animMatrixVector
	const std::map<std::string, int>* boneMap = skeleton->GetBoneMap();
	if (boneMap->find(nodeName) != boneMap->end())
	{
		//This if should not happen if the animMatrixVector was resized correctly
		if (boneMap->at(nodeName) >= (int)animMatrixVector->size())
			animMatrixVector->resize(boneMap->size());
		
		animMatrixVector->at(boneMap->at(nodeName)) = _rootTransformationInverse * globalTransform * skeleton->GetOffSetMatrixVector()->at(boneMap->at(nodeName));
	}

	{
		aiVector3D testTrans = _rootTransformationInverse * globalTransform * aiVector3D(0.0f, 0.0f, 0.0f);
		aiVector3D testRot = _rootTransformationInverse * globalTransform * aiVector3D(0.0f, 1.0f, 0.0f);
		testRot -= testTrans;
		aiMatrix4x4 testMat = /*aiMatrix4x4(aiQuaternion(aiVector3D(1.0f, 0.0, 0.0), -1.570796f).GetMatrix()) * */_rootTransformationInverse * globalTransform;
		aiVector3D scaleV, posV;
		aiQuaternion rotV;
		testMat.Decompose(scaleV, rotV, posV);

		if (nodeName.compare("Bip001_R_Hand") == 0)
		{
			DebugOutput("Found Right Hand: %f (%f, %f, %f)\n", time, testTrans.x, testTrans.y, testTrans.z);
			_RHandTransVector.push_back(testTrans);
			_RHandRotVector.push_back(rotV);
			_RHandMatrixVector.push_back(_rootTransformationInverse * globalTransform);
		}
	}

	for (unsigned int i = 0;i < node->mNumChildren;i ++)
		TraverseBoneTree(time, node->mChildren[i], globalTransform, animation, skeleton, animMatrixVector);

	return 0;
}

int Animation::TraverseKeyFrame(float time, aiNode* node, aiMatrix4x4 transform, aiAnimation* animation, Skeleton* skeleton, Animation::KeyFrame* keyframe)
{
	std::string nodeName = node->mName.data;
	aiNodeAnim* nodeAnim = NULL;

	//Find the nodeAnim given the name
	for (unsigned int i = 0; i < animation->mNumChannels; i++)
	{
		if (nodeName.compare(animation->mChannels[i]->mNodeName.C_Str()) == 0)
			nodeAnim = animation->mChannels[i];
	}

	//Set default transformation matrix in case node does not have animation associated with it
	aiMatrix4x4 nodeTransform = node->mTransformation;

	//Interpolate for given time if there is animation
	if (nodeAnim)
	{
		aiVector3D scaleVector3D = CalcInterScale(time, nodeAnim);
		aiMatrix4x4 scaleMatrix;
		aiMatrix4x4::Scaling(scaleVector3D, scaleMatrix);

		aiQuaternion rotationVector3D = CalcInterRotation(time, nodeAnim);
		aiMatrix4x4 rotationMatrix = aiMatrix4x4(rotationVector3D.GetMatrix());

		aiVector3D translationVector3D = CalcInterPosition(time, nodeAnim);
		aiMatrix4x4 translationMatrix;
		aiMatrix4x4::Translation(translationVector3D, translationMatrix);

		nodeTransform = translationMatrix * rotationMatrix * scaleMatrix;
	}

	aiMatrix4x4 globalTransform = transform * nodeTransform;

	//Assigns matrix to the output animMatrixVector
	const std::map<std::string, int>* boneMap = skeleton->GetBoneMap();
	if (boneMap->find(nodeName) != boneMap->end())
	{
		//This if should not happen if the animMatrixVector was resized correctly
		if (boneMap->at(nodeName) >= (int)keyframe->transformVector.size())
			keyframe->transformVector.resize(boneMap->size());

		BoneTransform boneTransform;
		aiMatrix4x4 transformMat = _rootTransformationInverse * globalTransform;
		transformMat.Decompose(boneTransform.scale, boneTransform.rotation, boneTransform.translation);
		boneTransform.index = boneMap->at(nodeName);
		keyframe->transformVector.at(boneMap->at(nodeName)) = boneTransform;
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++)
		TraverseKeyFrame(time, node->mChildren[i], globalTransform, animation, skeleton, keyframe);

	return 0;
}

int Animation::TraverseKeyFrameWithIndex(int index, aiNode* node, aiMatrix4x4 transform, aiAnimation* animation, Skeleton* skeleton, Animation::KeyFrame* keyframe)
{
	std::string nodeName = node->mName.data;
	aiNodeAnim* nodeAnim = NULL;

	//Find the nodeAnim given the name
	for (unsigned int i = 0; i < animation->mNumChannels; i++)
	{
		if (nodeName.compare(animation->mChannels[i]->mNodeName.C_Str()) == 0)
			nodeAnim = animation->mChannels[i];
	}

	//Set default transformation matrix in case node does not have animation associated with it
	aiMatrix4x4 nodeTransform = node->mTransformation;

	//Interpolate for given time if there is animation
	if (nodeAnim)
	{
		aiVector3D scaleVector3D = nodeAnim->mScalingKeys[index].mValue;
		aiMatrix4x4 scaleMatrix;
		aiMatrix4x4::Scaling(scaleVector3D, scaleMatrix);

		aiQuaternion rotationVector3D = nodeAnim->mRotationKeys[index].mValue;
		aiMatrix4x4 rotationMatrix = aiMatrix4x4(rotationVector3D.GetMatrix());

		aiVector3D translationVector3D = nodeAnim->mPositionKeys[index].mValue;
		aiMatrix4x4 translationMatrix;
		aiMatrix4x4::Translation(translationVector3D, translationMatrix);

		nodeTransform = translationMatrix * rotationMatrix * scaleMatrix;
	}

	aiMatrix4x4 globalTransform = transform * nodeTransform;

	//Assigns matrix to the output animMatrixVector
	const std::map<std::string, int>* boneMap = skeleton->GetBoneMap();
	if (boneMap->find(nodeName) != boneMap->end())
	{
		//This if should not happen if the animMatrixVector was resized correctly
		if (boneMap->at(nodeName) >= (int)keyframe->transformVector.size())
			keyframe->transformVector.resize(boneMap->size());

		BoneTransform boneTransform;
		aiMatrix4x4 transformMat = _rootTransformationInverse * globalTransform;
		transformMat.Decompose(boneTransform.scale, boneTransform.rotation, boneTransform.translation);
		boneTransform.index = boneMap->at(nodeName);
		keyframe->transformVector.at(boneMap->at(nodeName)) = boneTransform;
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++)
		TraverseKeyFrameWithIndex(index, node->mChildren[i], globalTransform, animation, skeleton, keyframe);

	return 0;
}

aiVector3D Animation::CalcInterPosition(float time, aiNodeAnim* nodeAnim)
{
	//Find index of time interval before time
	int indexA = -1;
	for (unsigned int i = 0;i < nodeAnim->mNumPositionKeys - 1;i ++)
	{
		if (time <= (float)nodeAnim->mPositionKeys[i+1].mTime)
		{
			indexA = i;
			break;
		}
	}

	if (indexA == -1)
	{
		return nodeAnim->mPositionKeys[0].mValue;
	}

	int indexB = indexA + 1;

	float timeDelta = (float)(nodeAnim->mPositionKeys[indexB].mTime - nodeAnim->mPositionKeys[indexA].mTime);
	float factor = (time - (float)nodeAnim->mPositionKeys[indexA].mTime) / timeDelta;
	aiVector3D delta = nodeAnim->mPositionKeys[indexB].mValue - nodeAnim->mPositionKeys[indexA].mValue;
	
	return nodeAnim->mPositionKeys[indexA].mValue + factor * delta;
}

aiQuaternion Animation::CalcInterRotation(float time, aiNodeAnim* nodeAnim)
{
	//Find index of time interval before time
	int indexA = -1;
	for (unsigned int i = 0;i < nodeAnim->mNumRotationKeys - 1;i ++)
	{
		if (time <= (float)nodeAnim->mRotationKeys[i+1].mTime)
		{
			indexA = i;
			break;
		}
	}

	if (indexA == -1)
	{
		return nodeAnim->mRotationKeys[0].mValue;
	}

	int indexB = indexA + 1;

	float timeDelta = (float)(nodeAnim->mRotationKeys[indexB].mTime - nodeAnim->mRotationKeys[indexA].mTime);
	float factor = (time - (float)nodeAnim->mRotationKeys[indexA].mTime) / timeDelta;
	
	aiQuaternion returnQuat;
	aiQuaternion::Interpolate(returnQuat, nodeAnim->mRotationKeys[indexA].mValue, nodeAnim->mRotationKeys[indexB].mValue, factor);

	return returnQuat;
}

aiVector3D Animation::CalcInterScale(float time, aiNodeAnim* nodeAnim)
{
	//Find index of time interval before time
	int indexA = -1;
	for (unsigned int i = 0;i < nodeAnim->mNumScalingKeys - 1;i ++)
	{
		if (time <= (float)nodeAnim->mScalingKeys[i+1].mTime)
		{
			indexA = i;
			break;
		}
	}

	if (indexA == -1)
	{
		return nodeAnim->mScalingKeys[0].mValue;
	}

	int indexB = indexA + 1;

	float timeDelta = (float)(nodeAnim->mScalingKeys[indexB].mTime - nodeAnim->mScalingKeys[indexA].mTime);
	float factor = (time - (float)nodeAnim->mScalingKeys[indexA].mTime) / timeDelta;
	aiVector3D delta = nodeAnim->mScalingKeys[indexB].mValue - nodeAnim->mScalingKeys[indexA].mValue;
	
	return nodeAnim->mScalingKeys[indexA].mValue + factor * delta;
}
