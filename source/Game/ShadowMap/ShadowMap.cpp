#include "ShadowMap.h"

#include "loadGLE.h"
#include "Debug/Debug.h"

#include "Phage/Phage.h"
#include "Texturer/TextureLoader.h"
#include "Game/Entity/Entity.h"
#include "Game/Entity/EntityBase.h"
#include "Game/Entity/EntityList.h"

//#include "Game/Terrain/Terrain.h"

#include "Game/Input/Input.h"

#include "Renderer/Renderer.h"

ShadowMap::ShadowMap()
{
	_translateX = 3.5f;
	_translateY = 2.5f;
}

ShadowMap::~ShadowMap()
{
}

int ShadowMap::Init(World* world)
{
	//Framebuffer
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	//glGenTextures(1, &fbTexture);
	//glBindTexture(GL_TEXTURE_2D, fbTexture);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 512, 512, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fbTexture, 0);

	glGenTextures(1, &fbDepth);
	glBindTexture(GL_TEXTURE_2D, fbDepth);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, fbDepth, 0);

	//glDrawBuffer(GL_NONE);

	//glGenRenderbuffers(1, &RBO);
	//glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	//glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA, 1024, 1024);
	//glBindRenderbuffer(GL_RENDERBUFFER, 0);
	//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, RBO);

	printf("ASDASDASDASDFRAMEBUFFER STATUS: %i\n", (int)glCheckFramebufferStatus(GL_FRAMEBUFFER));
	

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

	glActiveTexture(GL_TEXTURE0+25);
	glBindTexture(GL_TEXTURE_2D, fbDepth);
	glActiveTexture(GL_TEXTURE0+0);

	glm::vec3 lightInvDir = glm::vec3(5.0f, 10.0f, 3.0f);

	glm::vec2 v1 = glm::normalize(glm::vec2(lightInvDir.x, lightInvDir.z));
	glm::vec2 v2 = glm::normalize(glm::vec2(0.0f, 1.0f));

	_angle = glm::angle(v1, v2);
	glm::vec2 newP = glm::rotate(glm::vec2(0.5f, 0.5f), _angle);
	printf("Angle: %f\nPoint (%f, %f)\n", _angle, newP.x, newP.y);

	glm::mat4 depthProjectionMatrix = glm::ortho<float>(-16, 16, 0, -20, -10, 40);
	glm::mat4 depthViewMatrix = glm::lookAt(lightInvDir, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0, 1, 0));
	//glm::rotate(depthViewMatrix, 55.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	//glm::translate(depthViewMatrix, glm::vec3(-11.0f, -32.0f, -44.0f));
	glm::mat4 depthModelMatrix = glm::mat4(1.0);
	glm::mat4 depthMVP = depthProjectionMatrix * depthViewMatrix * depthModelMatrix;

	//RendererSingleton->GetCamera()->GetShader()->SetShadowMatrix(&depthMVP[0][0]);

	glm::mat4 biasMatrix(
		0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.5f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f);

	glm::mat4 depthBiasMVP = biasMatrix * depthMVP;

	//RendererSingleton->GetCamera()->GetShader()->SetShadowBiasMatrix(&depthBiasMVP[0][0]);

	//world->GetCamera()->GetShader()->SetShadowScale(1.0f);

	return 0;
}

/*int ShadowMap::CalcFrustumPlaneIntersect(Camera* camera)
{
	//Calculate the corners of the screen in real world coordinates
	float epsilon = 0.0f;//1024.0f/pow(camera->getCameraPosition().y, 2.0f); //Variable for expanding the frustum-plane (trapezoid) intersection to be outside of the viewport
	//if (epsilon > 1.5f) epsilon = 1.5f;
	//else if (epsilon < 1.1f) epsilon = 1.1f;
	epsilon = 1.0f;
	glm::vec4 Corners[4];
	Corners[0] = camera->Calculate3DZeroPoint(-epsilon, epsilon); _corners[0].x = Corners[0].x; _corners[0].y = Corners[0].z;
	Corners[1] = camera->Calculate3DZeroPoint(epsilon, epsilon); _corners[1].x = Corners[1].x; _corners[1].y = Corners[1].z;
	Corners[2] = camera->Calculate3DZeroPoint(epsilon, -epsilon); _corners[2].x = Corners[2].x; _corners[2].y = Corners[2].z;
	Corners[3] = camera->Calculate3DZeroPoint(-epsilon, -epsilon); _corners[3].x = Corners[3].x; _corners[3].y = Corners[3].z;

	//printf("corners: (%f, %f), (%f, %f), (%f, %f), (%f, %f)\n",_corners[0].x, _corners[0].y, _corners[1].x, _corners[1].y
		//,_corners[2].x, _corners[2].y, _corners[3].x, _corners[3].y);

	_corners[0] = glm::rotate(_corners[0], _angle);
	_corners[1] = glm::rotate(_corners[1], _angle);
	_corners[2] = glm::rotate(_corners[2], _angle);
	_corners[3] = glm::rotate(_corners[3], _angle);

	//printf("Rotcorners: (%f, %f), (%f, %f), (%f, %f), (%f, %f)\n",_corners[0].x, _corners[0].y, _corners[1].x, _corners[1].y
		//,_corners[2].x, _corners[2].y, _corners[3].x, _corners[3].y);

	//Calculate the top-most and bottom-most points
	int x1 = 0;
	int x2 = 0;
	int y1 = 0;
	int y2 = 0;
	for (int i = 1;i < 4;i ++)
	{
		if (_corners[i].x < _corners[x1].x) x1 = i;
		if (_corners[i].x > _corners[x2].x) x2 = i;
		if (_corners[i].y < _corners[y1].y) y1 = i;
		if (_corners[i].y > _corners[y2].y) y2 = i;
	}

	//Clamp points within terrain bounds
	float leftBounds = _corners[x1].x;
	float rightBounds = _corners[x2].x;
	float topBounds = _corners[y1].y;
	float botBounds = _corners[y2].y;

	//printf("Bounds %f, %f, %f, %f\n", leftBounds, rightBounds, topBounds, botBounds);

	glm::vec3 lightInvDir = glm::vec3(5.0f, 10.0f, 3.0f);

	glm::mat4 depthProjectionMatrix = glm::ortho<float>(leftBounds, rightBounds, -topBounds*1.0f, -botBounds*1.0f, -10, 40);
	glm::mat4 depthViewMatrix = glm::lookAt(lightInvDir, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0, 1, 0));
	//glm::rotate(depthViewMatrix, 55.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	depthViewMatrix = glm::translate(depthViewMatrix, glm::vec3(_translateX, -0.0f, _translateY));
	glm::mat4 depthModelMatrix = glm::mat4(1.0);
	glm::mat4 depthMVP = depthProjectionMatrix * depthViewMatrix * depthModelMatrix;

	camera->GetShader()->SetShadowMatrix(&depthMVP[0][0]);

	glm::mat4 biasMatrix(
		0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.5f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f);

	glm::mat4 depthBiasMVP = biasMatrix * depthMVP;

	camera->GetShader()->SetShadowBiasMatrix(&depthBiasMVP[0][0]);

	float shadowScale = rightBounds-leftBounds;
	if (botBounds-topBounds > shadowScale) shadowScale = botBounds-topBounds;
	shadowScale /= 100.0f;
	shadowScale += 1.0f;
	shadowScale = glm::clamp(shadowScale, 1.0f, 2.5f);
	camera->GetShader()->SetShadowScale(shadowScale);
	//printf("SHADOW SCALE:%f\n", shadowScale);

	return 0;
}*/

int ShadowMap::RenderTest(World* world)
{
	if (world->GetInput()->GetKeyboardInput(INPUT_KEYBOARD_DOWN, SDLK_KP_4))
		_translateX -= 0.1f;
	if (world->GetInput()->GetKeyboardInput(INPUT_KEYBOARD_DOWN, SDLK_KP_6))
		_translateX += 0.1f;
	if (world->GetInput()->GetKeyboardInput(INPUT_KEYBOARD_DOWN, SDLK_KP_2))
		_translateY -= 0.1f;
	if (world->GetInput()->GetKeyboardInput(INPUT_KEYBOARD_DOWN, SDLK_KP_8))
		_translateY += 0.1f;

	if (world->GetInput()->GetKeyboardInput(INPUT_KEYBOARD_PRESSED, SDLK_KP_5))
		DebugOutput("Translation: %f, %f\n", _translateX, _translateY);

	//CalcFrustumPlaneIntersect(RendererSingleton->GetCamera());

	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glActiveTexture(GL_TEXTURE0+25);
	glBindTexture(GL_TEXTURE_2D, fbDepth);
	glActiveTexture(GL_TEXTURE0+0);
	glViewport(0, 0, 1024, 1024);
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

	//RendererSingleton->GetCamera()->GetShader()->UseProgram(SHADER_SHADOW);

	glCullFace(GL_FRONT_FACE);

	for (int i = 0;i < world->GetEntityList()->Size();i ++)
	{
		Entity* entity = world->GetEntityList()->Get(i);

		if (entity->GetEntityBase()->EntityTypeInt() == ENTITY_TYPE_UNIT)
			continue;

		glm::mat4 modelMat = glm::mat4(1.0);
		modelMat = glm::translate(modelMat, glm::vec3(entity->PosX(), entity->PosY(), entity->PosZ()));
		modelMat = glm::rotate(modelMat, glm::radians(entity->RotY()), glm::vec3(0.0f, 1.0f, 0.0f));
		if (entity->GetModel()->GetOrientation() == Model::ORIENTATION_Z)
			modelMat = glm::rotate(modelMat, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		float scale = entity->GetModel()->GetScale();
		if (scale != 1.0f)
			modelMat = glm::scale(modelMat, glm::vec3(scale, scale, scale));

		//RendererSingleton->GetCamera()->GetShader()->SetFullModelMatrix(&modelMat[0][0]);

		//world->GetCamera()->GetShader()->UseProgram(SHADER_DEFAULT);
		//world->GetCamera()->GetMVPMatrix()->Push(MODEL);
		//world->GetCamera()->GetMVPMatrix()->translate(MODEL, entity->PosX(), entity->PosY(), entity->PosZ());
		//world->GetCamera()->GetMVPMatrix()->rotate(MODEL, entity->RotY(), 0.0f, 1.0f, 0.0f);
		//camera->GetMVPMatrix()->rotate(MODEL, -90.0f, 1.0f, 0.0f, 0.0f);
		//world->GetCamera()->UpdateMatrix(SHADER_DEFAULT);
		//world->GetCamera()->GetShader()->UpdateModelMatrix(glm::value_ptr(world->GetCamera()->GetMVPMatrix()->GetModelMatrix()));
		//glEnable(GL_BLEND);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//glEnable(GL_ALPHA_TEST);
		//glAlphaFunc(GL_GREATER, 0.99f);
		//if (entity->IsMouseOver())
			//world->GetCamera()->GetShader()->SetColor(1.5f, 1.5f, 1.5f, 1.0f);
		//entity->GetModel()->Draw(RendererSingleton->GetCamera(), 0, entity->GetAnimationSeconds());
		//if (entity->IsMouseOver())
			//world->GetCamera()->GetShader()->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
		//glDisable(GL_ALPHA_TEST);
		//glDisable(GL_BLEND);
		//world->GetCamera()->GetMVPMatrix()->Pop(MODEL);
	}

	//glViewport(0, 0, (int)RendererSingleton->GetCamera()->GetScreenResolution().x, (int)RendererSingleton->GetCamera()->GetScreenResolution().y);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glCullFace(GL_BACK);
	//world->GetCamera()->GetShader()->SetLighting(0);
	//world->GetCamera()->GetMVPMatrix()->SetProjectionMatrixType(ORTHO);
	//world->GetCamera()->GetShader()->UseProgram(SHADER_DEFAULT);
	//world->GetCamera()->UpdateMatrix(SHADER_DEFAULT);
	//guiScreen->DrawAtLocation(world->GetCamera(), 0, 0, fbDepth);
	//world->GetCamera()->GetMVPMatrix()->SetProjectionMatrixType(PROJECTION);

	return 0;
}

int ShadowMap::Clear()
{
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_DEPTH_BUFFER_BIT);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return 0;
}
