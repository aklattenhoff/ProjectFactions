#ifndef _MODEL_H
#define _MODEL_H

#include <string>
#include <vector>
#include <map>

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Texturer/Texturer.h"

class Skeleton;
class Animation;
//class Texture;
class Camera;

class Model
{
public:
	static const int ORIENTATION_Y = 0;
	static const int ORIENTATION_Z = 1;

	const static int ANIMATION_IDLE_1 = 0;
	const static int ANIMATION_IDLE_2 = 1;
	const static int ANIMATION_IDLE_3 = 2;
	const static int ANIMATION_BORED_1 = 3;
	const static int ANIMATION_BORED_2 = 4;
	const static int ANIMATION_BORED_3 = 5;
	const static int ANIMATION_WALKING = 6;
	const static int ANIMATION_GATHERING_FOOD = 100;
	const static int ANIMATION_GATHERING_WOOD = 101;
	const static int ANIMATION_GATHERING_STONE = 102;
	const static int ANIMATION_GATHERING_ALLOY = 103;
	const static int ANIMATION_GATHERING_HUNTABLE = 104;
	const static int ANIMATION_GATHERING_HERDABLE = 105;
	const static int ANIMATION_GATHERING_FARM = 106;
	const static int ANIMATION_GATHERING_TREE = 107;
	const static int ANIMATION_ATTACK_1 = 9;
	const static int ANIMATION_BUILD = 20;
	const static int ANIMATION_DEATH = 200;
	const static int ANIMATION_DEAD = 201;
public:
	Model();
	~Model();

	int Init();
	int SetName(std::string name);
	std::string GetName();
	int SetOrientation(int orientationFlag);
	int GetOrientation();
	int SetScale(float scale);
	float GetScale();
	int LoadScene(std::string fileName);
	int LoadMainTexture(std::string fileName);
	int LoadBumpTexture(std::string fileName);
	int LoadSBCTexture(std::string fileName);
	int AddAnimation(std::string fileName, int animFlag, float animSpeed);
	bool HasAnimation();
	Animation* GetAnimation(int animFlag);
	int Draw(Camera* camera, int animationIndex, float timeInSeconds);
	int Draw();

	Skeleton* GetSkeleton();
	std::vector<aiMatrix4x4> GetBoneMatrices(int animIndex, float animSecs);
private:
	std::string _name;

	int _orientationFlag;
	float _scale;

	unsigned int _numMeshes;
	unsigned int _totalNumFaces;
	bool _hasNormals;
	bool _hasTangentsAndBitangents;
	bool _hasAnimation;

	Skeleton* _skeleton;
	std::map<int, Animation*> _animationMap;

	GLuint _vao;
	GLuint _vbo;
	GLuint _tbo;
	GLuint _nbo;
	GLuint _tnbo;
	GLuint _bnbo;
	GLuint _bbo;

	bool _hasMainTexture;
	bool _hasBumpTexture;
	bool _hasSBCTexture;

	//Texture* _mainTexture;
	//Texture* _bumpTexture;
	//Texture* _sbcTexture;

	Texturer::Texture _mainTexturerTexture;
	Texturer::Texture _bumpTexturerTexture;
	Texturer::Texture _sbcTexturerTexture;
};

#endif
