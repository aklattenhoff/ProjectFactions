#include "Prop.h"

#include "Game/Player/Player.h"

RS::Prop::Prop()
{
}

RS::Prop::~Prop()
{

}

int RS::Prop::SetModel(Model* model, glm::vec3 pos, float rot)
{
	_model = model;
	_position = pos;
	_rotation = rot;

	return 0;
}

int RS::Prop::SetBoneTranslation(float x, float y, float z)
{
	_boneTranslation.x = x;
	_boneTranslation.y = y;
	_boneTranslation.z = z;

	return 0;
}

int RS::Prop::SetBoneRotation(glm::vec3 axis, float angle)
{
	_boneRotationAxis = axis;
	_boneRotationAngle = angle;

	return 0;
}

int RS::Prop::SetOffset(glm::vec3 trans, glm::vec3 rot, glm::vec3 scale)
{
	_offsetTranslation = trans;
	_offsetRotation = rot;
	_offsetScale = scale;

	return 0;
}

int RS::Prop::SetPlayer(Player* player)
{
	_player = player;

	return 0;
}

Model* RS::Prop::GetModel()
{
	return _model;
}

glm::vec3 RS::Prop::GetPosition()
{
	return _position;
}

float RS::Prop::GetRotation()
{
	return _rotation;
}

glm::vec3 RS::Prop::GetBoneRotationAxis()
{
	return _boneRotationAxis;
}

float RS::Prop::GetBoneRotationAngle()
{
	return _boneRotationAngle;
}

glm::vec3 RS::Prop::GetBoneTranslation()
{
	return _boneTranslation;
}

glm::vec3 RS::Prop::GetOffsetTranslation()
{
	return _offsetTranslation;
}

glm::vec3 RS::Prop::GetOffsetRotation()
{
	return _offsetRotation;
}

glm::vec3 RS::Prop::GetOffsetScale()
{
	return _offsetScale;
}

Player* RS::Prop::GetPlayer()
{
	return _player;
}
