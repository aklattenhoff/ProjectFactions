#include "Bloom.h"

#include "loadGLE.h"
#include "Debug/Debug.h"

#include "Texturer/TextureLoader.h"
#include "Game/Entity/Entity.h"
#include "Game/Entity/EntityBase.h"
#include "Game/Entity/EntityList.h"

//#include "Game/Terrain/Terrain.h"

#include "Renderer/Renderer.h"

Bloom::Bloom()
{
}

Bloom::~Bloom()
{
}

int Bloom::Init()
{
	//Framebuffer
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	glGenTextures(1, &fbTexture);
	glBindTexture(GL_TEXTURE_2D, fbTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 512, 512, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fbTexture, 0);

	/*glGenTextures(1, &fbDepth);
	glBindTexture(GL_TEXTURE_2D, fbDepth);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 512, 512, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, fbDepth, 0);*/

	glGenRenderbuffers(1, &RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 512, 512);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, RBO);

	printf("FRAMEBUFFER STATUS: %i\n", (int)glCheckFramebufferStatus(GL_FRAMEBUFFER));
	

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//Pass1 FBO
	glGenFramebuffers(1, &sc1FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, sc1FBO);

	glGenTextures(1, &sc1Texture);
	glBindTexture(GL_TEXTURE_2D, sc1Texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 512, 512, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, sc1Texture, 0);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//Pass2 FBO
	glGenFramebuffers(1, &sc2FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, sc2FBO);

	glGenTextures(1, &sc2Texture);
	glBindTexture(GL_TEXTURE_2D, sc2Texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 512, 512, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, sc2Texture, 0);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	guiScreen = new GUIIcon();
	guiScreen->Init();
	guiScreen->Generate(glm::vec2(512.0f, 512.0f));

	guiIcon = new GUIIcon();
	guiIcon->Init();
	guiIcon->Generate(glm::vec2(96.0f, 96.0f));

	circleTexture = TextureLoader_LoadFromFile("textures/selection32x32.png");

	glActiveTexture(GL_TEXTURE0+18);
	glBindTexture(GL_TEXTURE_2D, fbTexture);
	glActiveTexture(GL_TEXTURE0+0);

	return 0;
}

int Bloom::RenderTest(World* world)
{
	glDisable(GL_CULL_FACE);
	glActiveTexture(GL_TEXTURE0+27);
	glBindTexture(GL_TEXTURE_2D, sc2Texture);
	glActiveTexture(GL_TEXTURE0+0);

	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glViewport(0, 0, 512, 512);
	//world->GetCamera()->GetShader()->SetLighting(0);
	//world->GetCamera()->GetMVPMatrix()->SetProjectionMatrixType(ORTHO);
	//world->GetCamera()->GetShader()->UseProgram(SHADER_DEFAULT);
	//world->GetCamera()->GetMVPMatrix()->Push(ORTHO);
	//Make sure to set the matrix to upside down to compensate for openGL (0,0) being bottom left
	//world->GetCamera()->GetMVPMatrix()->SetOrthoMatrix(0.0f, 512.0f, 0.0f, 512.0f);

	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glEnable(GL_ALPHA_TEST);
	//glAlphaFunc(GL_GREATER, 0.99f);
	//glDisable(GL_DEPTH_TEST);

	/*world->GetCamera()->GetMVPMatrix()->Push(MODEL);
	world->GetCamera()->GetMVPMatrix()->translate(MODEL, 100.0f, 100.0f, 0.0f);
	world->GetCamera()->UpdateMatrix(SHADER_DEFAULT);
	guiIcon->DrawAtLocation(world->GetCamera(), 0.0f, 0.0f, circleTexture);
	world->GetCamera()->GetMVPMatrix()->Pop(MODEL);*/

	//world->GetCamera()->GetMVPMatrix()->SetProjectionMatrix(-25.0f, -world->GetCamera()->GetScreenResolution().x/world->GetCamera()->GetScreenResolution().y, 0.2f, 2000.0f);

	/*Camera* camera = RendererSingleton->GetCamera();

	for (int i = 0;i < world->GetEntityList()->Size();i ++)
	{
		Entity* entity = world->GetEntityList()->Get(i);

		camera->GetShader()->SetBump(1);
		camera->GetShader()->SetBloom(2);

		camera->GetShader()->UseProgram(SHADER_DEFAULT);
		camera->GetMVPMatrix()->Push(MODEL);
		camera->GetMVPMatrix()->translate(MODEL, entity->PosX(), entity->PosY(), entity->PosZ());
		camera->GetMVPMatrix()->rotate(MODEL, entity->RotY(), 0.0f, 1.0f, 0.0f);
		if (entity->GetModel()->GetOrientation() == Model::ORIENTATION_Z)
			camera->GetMVPMatrix()->rotate(MODEL, -90.0f, 1.0f, 0.0f, 0.0f);
		float scale = entity->GetModel()->GetScale();
		if (scale != 1.0f)
			camera->GetMVPMatrix()->scale(MODEL, scale, scale, scale);
		//camera->GetMVPMatrix()->rotate(MODEL, -90.0f, 1.0f, 0.0f, 0.0f);
		camera->UpdateMatrix(SHADER_DEFAULT);
		//glEnable(GL_BLEND);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//glEnable(GL_ALPHA_TEST);
		//glAlphaFunc(GL_GREATER, 0.0f);
		//if (entity->IsMouseOver())
			//world->GetCamera()->GetShader()->SetColor(1.5f, 1.5f, 1.5f, 1.0f);
		if (entity->GetModel())
		{
			entity->GetModel()->Draw(camera, entity->GetAnimationIndex(), entity->GetAnimationSeconds());
		}
		//if (entity->IsMouseOver())
			//world->GetCamera()->GetShader()->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
		//glDisable(GL_ALPHA_TEST);
		//glDisable(GL_BLEND);
		camera->GetMVPMatrix()->Pop(MODEL);

		camera->GetShader()->SetBloom(0);
		camera->GetShader()->SetBump(0);
	}

	//This doesn't work because the terrain overlay changes the framebuffer
	//world->GetTerrain()->Draw(world, world->GetCamera(), world->GetLOSMap());

	camera->GetShader()->SetLighting(0);
	camera->GetMVPMatrix()->SetProjectionMatrixType(ORTHO);
	camera->GetShader()->UseProgram(SHADER_DEFAULT);
	camera->GetMVPMatrix()->Push(ORTHO);
	//Make sure to set the matrix to upside down to compensate for openGL (0,0) being bottom left
	camera->GetMVPMatrix()->SetOrthoMatrix(0.0f, 512.0f, 0.0f, 512.0f);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glEnable(GL_ALPHA_TEST);
	//glAlphaFunc(GL_GREATER, 0.99f);
	glDisable(GL_DEPTH_TEST);

	glBindFramebuffer(GL_FRAMEBUFFER, sc1FBO);
	camera->GetShader()->SetConvolutionPass(1);
	camera->UpdateMatrix(SHADER_DEFAULT);
	guiScreen->DrawAtLocation(camera, 0.0f, 0.0f, fbTexture);

	glBindFramebuffer(GL_FRAMEBUFFER, sc2FBO);
	camera->GetShader()->SetConvolutionPass(2);
	camera->UpdateMatrix(SHADER_DEFAULT);
	guiScreen->DrawAtLocation(camera, 0.0f, 0.0f, sc1Texture);

	camera->GetShader()->SetConvolutionPass(0);

	//glDisable(GL_ALPHA_TEST);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	//world->GetCamera()->GetMVPMatrix()->Pop(ORTHO);
	camera->GetMVPMatrix()->SetOrthoMatrix(0.0f, camera->GetScreenResolution().x, 0.0f, camera->GetScreenResolution().y);
	glViewport(0, 0, (int)camera->GetScreenResolution().x, (int)camera->GetScreenResolution().y);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	camera->GetShader()->SetLighting(0);
	camera->GetMVPMatrix()->SetProjectionMatrixType(ORTHO);
	camera->GetShader()->UseProgram(SHADER_DEFAULT);
	camera->UpdateMatrix(SHADER_DEFAULT);
	//guiScreen->DrawAtLocation(world->GetCamera(), 0, 0, fbTexture);
	//guiScreen->DrawAtLocation(world->GetCamera(), 0, 512, sc1Texture);
	camera->GetShader()->SetBloom(1);
	//guiScreen->DrawAtLocation(world->GetCamera(), 512, 512, sc2Texture);
	camera->GetMVPMatrix()->SetProjectionMatrixType(PROJECTION);

	camera->GetMVPMatrix()->Pop(ORTHO);

	camera->GetShader()->SetBloom(0);*/

	glEnable(GL_CULL_FACE);

	return 0;
}

int Bloom::Clear()
{
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindFramebuffer(GL_FRAMEBUFFER, sc1FBO);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindFramebuffer(GL_FRAMEBUFFER, sc2FBO);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return 0;
}
