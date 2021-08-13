#ifndef _RS_PROP_H
#define _RS_PROP_H

#include <glm/glm.hpp>
#include <glm/ext.hpp>

class Model;
class Player;

namespace RS
{
class Prop
{
public:
	Prop();
	~Prop();

	int SetModel(Model* model, glm::vec3 pos, float rot = 0.0f);
	int SetBoneTranslation(float x, float y, float z);
	int SetBoneRotation(glm::vec3 axis, float angle);
	int SetOffset(glm::vec3 trans, glm::vec3 rot, glm::vec3 scale);
	int SetPlayer(Player* player);

	Model* GetModel();
	glm::vec3 GetPosition();
	float GetRotation();

	glm::vec3 GetBoneRotationAxis();
	float GetBoneRotationAngle();

	glm::vec3 GetBoneTranslation();

	glm::vec3 GetOffsetTranslation();
	glm::vec3 GetOffsetRotation();
	glm::vec3 GetOffsetScale();

	Player* GetPlayer();
private:
	Model* _model;
	glm::vec3 _position;
	float _rotation;

	glm::vec3 _boneTranslation;
	glm::vec3 _boneRotationAxis;
	float _boneRotationAngle;

	glm::vec3 _offsetTranslation;
	glm::vec3 _offsetRotation;
	glm::vec3 _offsetScale;

	Player* _player;
};
}
#endif
