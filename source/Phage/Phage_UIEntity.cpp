#include "Phage_UIEntity.h"

Phage::PhageUIEntity::PhageUIEntity()
{
}

Phage::PhageUIEntity::~PhageUIEntity()
{
}

int Phage::PhageUIEntity::Init()
{
	_matrix.Identity();

	return 0;
}

int Phage::PhageUIEntity::CreateAndAddUITextureQuad(float x, float y, float w, float h, std::string fileName, bool upsideDown)
{
	_phageUITextureQuadVector.push_back(new Phage::PhageUITextureQuad());
	_phageUITextureQuadVector.back()->CreateTextureQuad(x, y, w, h, fileName, upsideDown);

	return 0;
}

int Phage::PhageUIEntity::CreateAndAddUITextureQuad(float x, float y, float w, float h, GLuint textureID, bool upsideDown)
{
	_phageUITextureQuadVector.push_back(new Phage::PhageUITextureQuad());
	_phageUITextureQuadVector.back()->CreateTextureQuad(x, y, w, h, textureID, upsideDown);

	return 0;
}

int Phage::PhageUIEntity::SetTextureID(GLuint textureID)
{
	if (_phageUITextureQuadVector.size() > 0)
		_phageUITextureQuadVector.back()->SetTextureID(textureID);

	return 0;
}

int Phage::PhageUIEntity::SetTranslate(float x, float y, float depth)
{
	_matrix.SetTranslate(x, y, depth);

	return 0;
}

int Phage::PhageUIEntity::SetScale(float x, float y)
{
	_matrix.SetScale(x, y, 1.0f);

	return 0;
}

int Phage::PhageUIEntity::SetRotate(float a)
{
	_matrix.SetRotate(a, 0.0f, 0.0f, 1.0f);

	return 0;
}

glm::mat4 Phage::PhageUIEntity::GetMatrix()
{
	return _matrix.GetMatrix(1) * _phageUITextureQuadVector.back()->GetMatrix().GetMatrix(0);
}

int Phage::PhageUIEntity::Draw()
{
	_phageUITextureQuadVector.back()->Draw();

	return 0;
}
