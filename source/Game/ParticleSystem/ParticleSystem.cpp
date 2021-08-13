#include "ParticleSystem.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "Phage/Phage.h"
//#include "Graphics/Camera.h"
#include "Texturer/TextureLoader.h"

#include "Renderer/Renderer.h"

ParticleSystem::ParticleSystem()
{
	//_quad = NULL;

	_testTimer = 0;
}

ParticleSystem::~ParticleSystem()
{
	//delete _quad;

	for (unsigned int i = 0;i < _particleVector.size();i ++)
		delete _particleVector[i];
}

int ParticleSystem::Init()
{
	//_quad = new Quad();
	//_quad->CreateBuffers(glm::vec3(-0.5f, 0.5f, 0.0f), glm::vec3(0.5f, 0.5f, 0.0f), glm::vec3(0.5f, -0.5f, 0.0f), glm::vec3(-0.5f, -0.5f, 0.0f));

	_cloudTexture = TextureLoader_LoadFromFile("textures/cloud.png");

	return 0;
}

int ParticleSystem::Draw(World* world, float camRotation)
{
	_testTimer ++;
	if (_testTimer > 60)
	{
		_testTimer = 0;
		_particleVector.insert(_particleVector.begin(), new Particle());
		_particleVector.front()->Init(_cloudTexture, glm::vec3(20.0f, 1.6f, 16.5f), glm::vec3(0.001f, 0.005f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f),
										(float)(rand()%360), 0.2f, 0.0015f, 20*60);
		//_particleVector.push_back(new Particle());
		//_particleVector.back()->Init(_cloudTexture, glm::vec3(20.0f, 2.0f, 16.5f), glm::vec3(0.0f, 0.001f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f),
										//(float)(rand()%360), 0.2f, 0.01f, 20*60);
	}

	for (unsigned int i = 0;i < _particleVector.size();i ++)
	{
		Particle* particle = _particleVector[i];

		particle->Step();
		if (particle->IsDead())
		{
			delete particle;
			_particleVector.erase(_particleVector.begin()+i);
			i --;
			continue;
		}

		glBindTexture(GL_TEXTURE_2D, particle->GetTexture());

		//<PhCam>//Camera* camera = RendererSingleton->GetCamera();
		Phage::PhageCamera* phageCamera = RendererSingleton->GetPhageCamera();

		/*//<PhCam>//camera->GetShader()->UseProgram(SHADER_DEFAULT);
		camera->GetMVPMatrix()->Push(MODEL);
		camera->GetMVPMatrix()->translate(MODEL, particle->GetPosition().x, particle->GetPosition().y, particle->GetPosition().z);
		//world->GetCamera()->GetMVPMatrix()->rotate(MODEL, particle->GetRotation(), 0.0f, 0.0f, 1.0f);
		camera->GetMVPMatrix()->rotate(MODEL, -camRotation, 0.0f, 1.0f, 0.0f);
		camera->GetMVPMatrix()->rotate(MODEL, -55.0f, 1.0f, 0.0f, 0.0f);
		camera->GetMVPMatrix()->rotate(MODEL, particle->GetRotation(), 0.0f, 0.0f, 1.0f);
		camera->GetMVPMatrix()->scale(MODEL, particle->GetScale(), particle->GetScale(), particle->GetScale());
		camera->GetShader()->SetColor(1.0f, 1.0f, 1.0f, particle->GetAlpha());
		camera->UpdateMatrix(SHADER_DEFAULT);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//glEnable(GL_ALPHA_TEST);
		//glAlphaFunc(GL_GREATER, 0.0f);
		_quad->Draw();
		//glDisable(GL_ALPHA_TEST);
		glDisable(GL_BLEND);
		camera->GetShader()->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
		camera->GetMVPMatrix()->Pop(MODEL);*/
	}

	return 0;
}
