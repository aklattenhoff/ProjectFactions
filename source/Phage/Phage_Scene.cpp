#include "Phage_Scene.h"

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

#include "Phage_Window.h"
#include "Phage_Shader.h"
#include "Phage_Camera.h"
#include "Phage_Entity.h"
#include "Phage_Terrain.h"
#include "Phage_TerrainDecal.h"
#include "Phage_UIEntity.h"
#include "Phage_UIText.h"
#include "Phage_UILineRect.h"
#include "Phage_FrameBuffer.h"
#include "Phage_BoundingBox.h"
#include "Phage_TerrainTileDebug.h"
#include "Phage_Line.h"
#include "Phage_Prop.h"
#include "Phage_PropDesc.h"
#include "Phage_Quad.h"

#include "loadGLE.h"
#include "Debug/Debug.h"

#include "../Renderer/Renderer.h"

Phage::Scene::Scene()
{
	_initGraphicsFlag = false;
	_loadExtensionsFlag = false;

	_resolution.x = 800;
	_resolution.y = 600;

	_camera = NULL;

	_matrixLocation = -1;
	_modelMatrixLocation = -1;
	_cameraPositionLocaton = -1;
	_fbSizeLocation = -1;
	_fbShadowSizeLocation = -1;
	_animationFlagLocation = -1; 
	_boneMatrixLocation = -1;
	_passValueLocation = -1;
	_occlusionValueLocation = -1;
	_shadowFlagLocation = -1;
	_shadowMatrixLocation = -1;
	_shadowBiasLocation = -1;
	_postProcessValueLocation = -1;
	_shadowDepthLocation = -1;

	_postProcLoc = -1;
	_entityNumberLoc = -1;

	_phageUITextTest = NULL;
	_phageUILineRectTest = NULL;
	_phageUIEntityTexture = NULL;
	_phageBoundingBox = NULL;
	_phageTerrainTileDebug = NULL;
	_phageLine = NULL;

	_mainFrameBuffer = NULL;
	_mainFBUIEntityTexture = NULL;
	_silhouetteFrameBuffer = NULL;
	_shadowFrameBuffer = NULL;
	_ssaoFrameBuffer = NULL;
	_postProcessFrameBufferFront = NULL;
	_postProcessFrameBufferBack = NULL;
	_seeThroughFrameBuffer = NULL;
	_seeThroughQuad = NULL;

	_losInfo = NULL;
}

Phage::Scene::~Scene()
{
	delete _camera;
	delete _phageUITextTest;
	delete _phageUILineRectTest;
	delete _phageUIEntityTexture;
	delete _phageBoundingBox;
	delete _phageTerrainTileDebug;
	delete _phageLine;

	delete _mainFrameBuffer;
	delete _mainFBUIEntityTexture;
	delete _silhouetteFrameBuffer;
	delete _shadowFrameBuffer;
	delete _ssaoFrameBuffer;
	delete _postProcessFrameBufferFront;
	delete _postProcessFrameBufferBack;
	delete _seeThroughFrameBuffer;
	delete _seeThroughQuad;
}

int Phage::Scene::Init()
{
	//Call InitGraphics and LoadExtensions, they will bail out if they've already been called
	InitGraphics();
	LoadExtensions();


	_modelRotation = 0.0f;

	_phageUITextSizeVector.resize(72, NULL);

	_phageTextureQuad = new Phage::PhageUITextureQuad();
	_phageTextureQuad->CreateTextureQuad(0.0f, 0.0f, 1.0f, 1.0f, "textures/blank.png");

	_seeThroughQuad = new Phage::PhageQuad();
	_seeThroughQuad->CreateTextureQuad(glm::vec3(-0.5f, 0.5f, 0.0f), glm::vec3(0.5f, 0.5f, 0.0f), glm::vec3(0.5f, -0.5f, 0.0f), glm::vec3(-0.5f, -0.5f, 0.0f), "see_through.png");

	return 0;
}

int Phage::Scene::InitGraphics()
{
	//Bail out if we have already initialized the graphics
	if (_initGraphicsFlag)
		return 2;

	SDL_Init(SDL_INIT_EVERYTHING);
	/*SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	int MSAA = 0;
	if (MSAA == 2 || MSAA == 4 || MSAA == 8)
	{
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, MSAA);
		glEnable(GL_MULTISAMPLE_ARB);
		if (0)
			glHint(GL_MULTISAMPLE_FILTER_HINT_NV, GL_NICEST);
		else
			glHint(GL_MULTISAMPLE_FILTER_HINT_NV, GL_FASTEST);
	}*/

	//Output the OpenGL version to debug console
	int majVer = -1;
	int minVer = -1;
	int profile = -1;
	SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &majVer);
	SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &minVer);
	SDL_GL_GetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, &profile);
	DebugOutput("GL Version: %i.%i prof: %i\n", majVer, minVer, profile);

	//Set V-Sync
	if (SDL_GL_SetSwapInterval(1) == 0)
		DebugOutput("V-Sync set\n");
	else
		DebugOutput("V-Sync not supported\n");

	//Enable back-face culling
	glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

	//Enable depth testing
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_TRUE);
	//glClearDepth(1);

	//Turn off stencil buffer testing initially
	glDisable(GL_STENCIL_TEST);
	glClearStencil(0);

	//Set line drawing info
	glLineWidth(1.0f);

	//Set the initGraphicsFlag
	_initGraphicsFlag = true;

	return 0;
}

int Phage::Scene::LoadExtensions()
{
	if (_loadExtensionsFlag)
		return 2;

	DebugOutput("Loading GL Extensions\n");
	LoadGLExtensions();

	_loadExtensionsFlag = true;

	return 0;
}

int Phage::Scene::AttachToWindow(SDL_Window* window)
{
	_window = window;

	DebugOutput("Creating GL context\n");
	_glContext = SDL_GL_CreateContext(window);

	return 0;
}

int Phage::Scene::AttachToWindow(Phage::Window* window)
{
	_window = window->GetSDLWindow();

	DebugOutput("Creating GL context\n");
	_glContext = SDL_GL_CreateContext(window->GetSDLWindow());

	return 0;
}

int Phage::Scene::SetResolution(int w, int h)
{
	_resolution.x = (float)w;
	_resolution.y = (float)h;

	_silhouetteResolution.x = (float)(w/4);
	_silhouetteResolution.y = (float)(h/4);

	_shadowResolution.x = (float)(w*2);
	_shadowResolution.y = (float)(h*2);

	_ssaoResolution.x = (float)(w / 1);
	_ssaoResolution.y = (float)(h / 1);

	_seeThroughResolution.x = (float)(w / 4);
	_seeThroughResolution.y = (float)(h / 4);

	_camera->SetScreenResolution(_resolution.x, _resolution.y);
	glViewport(0, 0, (int)_resolution.x, (int)_resolution.y);

	if (_mainFrameBuffer)
		_mainFrameBuffer->DeleteFrameBuffer();
	else
	{
		_mainFrameBuffer = new Phage::PhageFrameBuffer();
		_mainFrameBuffer->Init();
	}
	_mainFrameBuffer->CreateFrameBuffer((int)_resolution.x, (int)_resolution.y, Phage::PhageFrameBuffer::COLOR_DEPTH_G_BUFFER, true, true);

	if (_silhouetteFrameBuffer)
		_silhouetteFrameBuffer->DeleteFrameBuffer();
	else
	{
		_silhouetteFrameBuffer = new Phage::PhageFrameBuffer();
		_silhouetteFrameBuffer->Init();
	}
	_silhouetteFrameBuffer->CreateFrameBuffer((int)_silhouetteResolution.x, (int)_silhouetteResolution.y, Phage::PhageFrameBuffer::DEPTH_BUFFER, false, false);

	if (_shadowFrameBuffer)
		_shadowFrameBuffer->DeleteFrameBuffer();
	else
	{
		_shadowFrameBuffer = new Phage::PhageFrameBuffer();
		_shadowFrameBuffer->Init();
	}
	_shadowFrameBuffer->CreateFrameBuffer((int)_shadowResolution.x, (int)_shadowResolution.y, Phage::PhageFrameBuffer::DEPTH_BUFFER, false, false);

	glActiveTexture(GL_TEXTURE0 + 30);
	glBindTexture(GL_TEXTURE_2D, _shadowFrameBuffer->GetDepthTextureID());
	glActiveTexture(GL_TEXTURE0);

	if (_ssaoFrameBuffer)
		_ssaoFrameBuffer->DeleteFrameBuffer();
	else
	{
		_ssaoFrameBuffer = new Phage::PhageFrameBuffer();
		_ssaoFrameBuffer->Init();
	}
	_ssaoFrameBuffer->CreateFrameBuffer((int)_ssaoResolution.x, (int)_ssaoResolution.y, Phage::PhageFrameBuffer::COLOR_SINGLE_CHANNEL, true, false);

	if (_postProcessFrameBufferFront)
		_postProcessFrameBufferFront->DeleteFrameBuffer();
	else
	{
		_postProcessFrameBufferFront = new Phage::PhageFrameBuffer();
		_postProcessFrameBufferFront->Init();
	}
	_postProcessFrameBufferFront->CreateFrameBuffer((int)_resolution.x, (int)_resolution.y, Phage::PhageFrameBuffer::COLOR_BUFFER, true, false);

	if (_postProcessFrameBufferBack)
		_postProcessFrameBufferBack->DeleteFrameBuffer();
	else
	{
		_postProcessFrameBufferBack = new Phage::PhageFrameBuffer();
		_postProcessFrameBufferBack->Init();
	}
	_postProcessFrameBufferBack->CreateFrameBuffer((int)_resolution.x, (int)_resolution.y, Phage::PhageFrameBuffer::COLOR_BUFFER, true, false);

	if (_seeThroughFrameBuffer)
		_seeThroughFrameBuffer->DeleteFrameBuffer();
	else
	{
		_seeThroughFrameBuffer = new Phage::PhageFrameBuffer();
		_seeThroughFrameBuffer->Init();
	}
	_seeThroughFrameBuffer->CreateFrameBuffer((int)_seeThroughResolution.x, (int)_seeThroughResolution.y, Phage::PhageFrameBuffer::COLOR_DEPTH_BUFFER, true, false);

	//Load see through image here for now
	IMG_Init(IMG_INIT_PNG);
	glGenTextures(1, &_seeThroughImage);
	SDL_Surface *surface = NULL;
	surface = IMG_Load("see_through.png");
	if (!surface) return -1;

	GLenum textureFormat;
	if (surface->format->BytesPerPixel == 4)
		textureFormat = (surface->format->Rmask == 0x000000ff) ? GL_RGBA : GL_BGRA;
	else if (surface->format->BytesPerPixel == 3)
		textureFormat = (surface->format->Rmask == 0x000000ff) ? GL_RGB : GL_BGR;

	glBindTexture(GL_TEXTURE_2D, _seeThroughImage);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//Bind tile map and add texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, textureFormat, GL_UNSIGNED_BYTE, surface->pixels);

	glBindTexture(GL_TEXTURE_2D, 0);
	if (surface)
		SDL_FreeSurface(surface);

	return 0;
}

Phage::vec2 Phage::Scene::GetResolution()
{
	return _resolution;
}

int Phage::Scene::SetShadowMapBounds(glm::vec3 nearPlane[4], glm::vec3 farPlane[4])
{
	if (_shadowMatrixLocation < 0)
		_shadowMatrixLocation = _shader->LoadUniformLocation(0, "shadowMatrix");

	if (_shadowBiasLocation < 0)
		_shadowBiasLocation = _shader->LoadUniformLocation(0, "shadowBias");

	if (_shadowDepthLocation < 0)
		_shadowDepthLocation = _shader->LoadUniformLocation(0, "shadowDepth");

	glm::vec3 lightInvDir = glm::vec3(3.0f, 8.0f, 1.0f);
	glm::vec3 lightDir = glm::normalize(glm::vec3(-lightInvDir.x, -lightInvDir.y, -lightInvDir.z));

	glm::mat4 depthViewMatrix = glm::lookAt(lightInvDir, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0, 1, 0));
	glm::mat4 inverseDepth = glm::inverse(depthViewMatrix);
	glm::vec4 center4 = inverseDepth * glm::vec4(0.0f, 0.0f, -1.0f, 1.0f);
	glm::vec3 center = glm::vec3(center4 / center4[3]);
	glm::vec4 upVec4 = inverseDepth * glm::vec4(0.0f, 1.0f, -1.0f, 1.0f);
	glm::vec3 upVec = glm::normalize((glm::vec3(upVec4 / upVec4[3]))- center);
	glm::vec4 rightVec4 = inverseDepth * glm::vec4(1.0f, 0.0f, -1.0f, 1.0f);
	glm::vec3 rightVec = glm::normalize((glm::vec3(rightVec4 / rightVec4[3]))- center);

	float leftBounds, rightBounds, topBounds, botBounds, nearBounds, farBounds;
	leftBounds = rightBounds = glm::dot(nearPlane[0], rightVec);
	topBounds = botBounds = glm::dot(nearPlane[0], upVec);
	nearBounds = farBounds = glm::dot(nearPlane[0], lightDir);
	for (int i = 1; i < 4; i++)
	{
		float rightTemp = glm::dot(nearPlane[i] - center, rightVec);
		float upTemp = glm::dot(nearPlane[i] - center, upVec);
		float nearTemp = glm::dot(nearPlane[i] - center, lightDir);

		if (rightTemp < leftBounds) leftBounds = rightTemp;
		if (rightTemp > rightBounds) rightBounds = rightTemp;
		if (upTemp < topBounds) topBounds = upTemp;
		if (upTemp > botBounds) botBounds = upTemp;
		if (nearTemp < nearBounds) nearBounds = nearTemp;
		if (nearTemp > farBounds) farBounds = nearTemp;
	}
	for (int i = 0; i < 4; i++)
	{
		float rightTemp = glm::dot(farPlane[i] - center, rightVec);
		float upTemp = glm::dot(farPlane[i] - center, upVec);
		float nearTemp = glm::dot(farPlane[i] - center, lightDir);

		if (rightTemp < leftBounds) leftBounds = rightTemp;
		if (rightTemp > rightBounds) rightBounds = rightTemp;
		if (upTemp < topBounds) topBounds = upTemp;
		if (upTemp > botBounds) botBounds = upTemp;
		if (nearTemp < nearBounds) nearBounds = nearTemp;
		if (nearTemp > farBounds) farBounds = nearTemp;
	}

	float depth = glm::abs(nearBounds - farBounds);

	float deltaHor = 0;// glm::abs(leftBounds - rightBounds) / 4;
	float deltaVer = 0;// glm::abs(topBounds - botBounds) / 4;
	float deltaDep = 0;// depth / 4;

	//DebugOutput("%f, %f, %f, %f, %f, %f, %f\n", leftBounds, rightBounds, topBounds, botBounds, center[0], center[1], center[2]);
	glm::mat4 depthProjectionMatrix = glm::ortho<float>(leftBounds - deltaHor, rightBounds + deltaHor, topBounds - deltaHor, botBounds + deltaHor, nearBounds - deltaDep, farBounds + deltaDep);
	glm::mat4 depthModelMatrix = glm::mat4(1.0);
	glm::mat4 depthMVP = depthProjectionMatrix * depthViewMatrix * depthModelMatrix;
	glm::mat4 biasMatrix(
		0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.5f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f);
	glm::mat4 depthBiasMVP = biasMatrix * depthMVP;

	_shader->SetUniformValue(0, _shadowMatrixLocation, glm::value_ptr(depthMVP));
	_shader->SetUniformValue(0, _shadowBiasLocation, glm::value_ptr(depthBiasMVP));
	_shader->SetUniformValue(0, _shadowDepthLocation, (int)depth);

	return 0;
}

Phage::PhageShader* Phage::Scene::CreateShader()
{
	_shader = new Phage::PhageShader();
	_shader->Init();

	return _shader;
}

Phage::PhageShader* Phage::Scene::GetShader()
{
	return _shader;
}

Phage::PhageCamera* Phage::Scene::CreateCamera()
{
	_camera = new Phage::PhageCamera();
	_camera->Init();

	return _camera;
}

Phage::PhageCamera* Phage::Scene::GetCamera()
{
	return _camera;
}

/*int Phage::Scene::AddEntity(Phage::PhageEntity* entity)
{
	_entityVector.push_back(entity);

	return 0;
}*/

int Phage::Scene::AddEntityToVector(Phage::PhageEntity* entity, int vector)
{
	switch (vector)
	{
	case UNIT_VECTOR: _entityUnitVector.push_back(entity); break;
	case BUILDING_VECTOR: _entityBuildingVector.push_back(entity); break;
	case OBJECT_VECTOR: _entityObjectVector.push_back(entity); break;
	}

	return 0;
}

int Phage::Scene::AddBoundingBoxInfo(glm::vec3 pos, glm::vec3 center, glm::vec3 size, float rotation, glm::vec4 bbRot)
{
	_boundingBoxInfoInfoVector.push_back(Phage::Scene::BoundingBoxInfo());
	_boundingBoxInfoInfoVector.back().pos = pos;
	_boundingBoxInfoInfoVector.back().center = center;
	_boundingBoxInfoInfoVector.back().size = size;
	_boundingBoxInfoInfoVector.back().rotation = rotation;
	_boundingBoxInfoInfoVector.back().bbRot = bbRot;

	return 0;
}

int Phage::Scene::AddTerrain(Phage::PhageTerrain* terrain)
{
	_terrainVector.push_back(terrain);

	return 0;
}

int Phage::Scene::AddTerrainTileDebug(glm::vec2 pos)
{
	_terrainTileDebugVector.push_back(TerrainTileDebug());
	_terrainTileDebugVector.back().pos = pos;

	return 0;
}

int Phage::Scene::AddLOSInfo(int** losInfo, unsigned int losTexture)
{
	_losInfo = losInfo;
	_losTexture = losTexture;

	return 0;
}

int Phage::Scene::AddTerrainDecal(Phage::PhageTerrainDecal* terrainDecal)
{
	_terrainDecalVector.push_back(terrainDecal);

	return 0;
}

int Phage::Scene::AddUIEntityToVector(Phage::PhageUIEntity* uiEntity)
{
	_uiEntityVector.push_back(uiEntity);

	return 0;
}

int Phage::Scene::AddUITextInfo(glm::vec2 pos, std::string str, int size)
{
	_uiTextInfoVector.push_back(Phage::Scene::TextInfo());
	_uiTextInfoVector.back().pos = pos;
	_uiTextInfoVector.back().str = str;
	_uiTextInfoVector.back().size = size;
	return 0;
}

int Phage::Scene::AddUITextIcon(char c, unsigned int texture)
{
	_uiTextIconVector.push_back(Phage::Scene::TextIcon());
	_uiTextIconVector.back().c = c;
	_uiTextIconVector.back().tex = texture;


	for (int i = 0;i < (int)_phageUITextSizeVector.size();i ++)
	{
		if (_phageUITextSizeVector[i])
			_phageUITextSizeVector[i]->AddUITextIcon(c, texture);
	}

	return 0;
}

int Phage::Scene::AddUITextureInfo(glm::vec4 points, unsigned int textureID, float depth)
{
	_uiTextureInfoVector.push_back(Phage::Scene::TextureInfo());
	_uiTextureInfoVector.back().points = points;
	_uiTextureInfoVector.back().depth = depth;
	_uiTextureInfoVector.back().textureID = textureID;

	return 0;
}

int Phage::Scene::AddUILineRectCoords(glm::vec4 points)
{
	_uiLineRectCoordsVector.push_back(points);

	return 0;
}

int Phage::Scene::AddLineDrawInfo(glm::vec3 pt1, glm::vec3 pt2)
{
	_lineDrawVector.push_back(LineDraw());
	_lineDrawVector.back().pt1 = pt1;
	_lineDrawVector.back().pt2 = pt2;

	return 0;
}

int Phage::Scene::Render(int renderPass, int renderVectorFlag, int renderFlag)
{
	if (!_mainFBUIEntityTexture)
	{
		_mainFBUIEntityTexture = new Phage::PhageUIEntity();
		_mainFBUIEntityTexture->Init();
		_mainFBUIEntityTexture->CreateAndAddUITextureQuad(0.0f, 0.0f, 1.0f, 1.0f, 0, true);
	}

	if (!_phageBoundingBox)
	{
		_phageBoundingBox = new Phage::PhageBoundingBox();
		_phageBoundingBox->Init();
	}

	if (!_phageTerrainTileDebug)
	{
		_phageTerrainTileDebug = new Phage::PhageTerrainTileDebug();
		_phageTerrainTileDebug->Init();
	}

	if (!_phageLine)
	{
		_phageLine = new Phage::PhageLine();
		_phageLine->Init();
	}

	int uniformValue = -1;

	if (_matrixLocation < 0)
	{
		_matrixLocation = _shader->LoadUniformLocation(0, "matrix"); uniformValue = 15;
	}

	if (_modelMatrixLocation < 0)
	{_modelMatrixLocation = _shader->LoadUniformLocation(0, "modelMatrix"); uniformValue = 14;
	}

	if (_cameraPositionLocaton < 0)
	{_cameraPositionLocaton = _shader->LoadUniformLocation(0, "cameraPosition"); uniformValue = 13;
	}

	if (_fbSizeLocation < 0)
	{_fbSizeLocation = _shader->LoadUniformLocation(0, "fbSize"); uniformValue = 12;
	}

	if (_fbShadowSizeLocation < 0)
	{_fbShadowSizeLocation = _shader->LoadUniformLocation(0, "fbShadowSize"); uniformValue = 11;
	}

	if (_animationFlagLocation < 0)
	{_animationFlagLocation = _shader->LoadUniformLocation(0, "animationFlag"); uniformValue = 10;
	}

	if (_boneMatrixLocation < 0)
	{_boneMatrixLocation = _shader->LoadUniformLocation(0, "boneMatrix"); uniformValue = 7;
	}

	if (_passValueLocation < 0)
	{_passValueLocation = _shader->LoadUniformLocation(0, "passValue"); uniformValue = 6;
	}

	if (_occlusionValueLocation < 0)
	{_occlusionValueLocation = _shader->LoadUniformLocation(0, "occlusionValue"); uniformValue = 5;
	}

	if (_shadowFlagLocation < 0)
	{_shadowFlagLocation = _shader->LoadUniformLocation(0, "shadowFlag"); uniformValue = 4;
	}

	//if (_shadowMatrixLocation < 0)
	//{_shadowMatrixLocation = _shader->LoadUniformLocation(0, "shadowMatrix"); uniformValue = 3;
	//}

	//if (_shadowBiasLocation < 0)
	//{_shadowBiasLocation = _shader->LoadUniformLocation(0, "shadowBias"); uniformValue = 2;
	//}

	if (_postProcLoc < 0)
	{_postProcLoc = _shader->LoadUniformLocation(0, "postProc"); uniformValue = 8;
	}

	if (_entityNumberLoc < 0)
	{_entityNumberLoc = _shader->LoadUniformLocation(0, "entityNumber"); uniformValue = 9;
	}

	if (uniformValue != -1)
		DebugOutput("ASD%i\n", uniformValue);

	float res[2] = {_resolution.x, _resolution.y};
	_shader->SetUniformValue(0, _fbSizeLocation, 2, res);
	float shadowres[2] = {_shadowResolution.x, _shadowResolution.y};
	_shader->SetUniformValue(0, _fbShadowSizeLocation, 2, shadowres);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glEnable(GL_ALPHA_TEST);
	//glAlphaFunc(GL_GREATER, 0.5f);

	glActiveTexture(GL_TEXTURE0 + 6);
	glBindTexture(GL_TEXTURE_2D, _seeThroughFrameBuffer->GetColorTextureID());
	glActiveTexture(GL_TEXTURE0);
	glActiveTexture(GL_TEXTURE0 + 7);
	glBindTexture(GL_TEXTURE_2D, _seeThroughFrameBuffer->GetDepthTextureID());
	glActiveTexture(GL_TEXTURE0);

	if (renderFlag & Phage::Scene::RENDER_FLAG_WIREFRAME)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	if (renderFlag & Phage::Scene::RENDER_FLAG_OCCLUSION_TEST)
	{
		_shader->SetUniformValue(0, _occlusionValueLocation, 2);
	}
	else if(renderFlag & Phage::Scene::RENDER_FLAG_OCCLUSION_SET  && !(renderVectorFlag & Phage::Scene::TERRAIN))
	{
		_shader->SetUniformValue(0, _occlusionValueLocation, 1);
	}
	else
	{
		_shader->SetUniformValue(0, _occlusionValueLocation, 0);
	}
	if (renderVectorFlag & Phage::Scene::UI_VECTOR)
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	else
		_mainFrameBuffer->BindFrameBuffer();//glBindFramebuffer(GL_FRAMEBUFFER, _mainFrameBuffer->GetFrameBufferID());

	if (renderPass & Phage::Scene::RENDER_PASS_SHADOW)
	{
		_shadowFrameBuffer->BindFrameBuffer();
		_shader->SetUniformValue(0, _shadowFlagLocation, 1);
		glViewport(0, 0, (int)_shadowResolution.x, (int)_shadowResolution.y);
		if (renderVectorFlag & Phage::Scene::TERRAIN)
			glCullFace(GL_FRONT);
	}
	else if (RendererSingleton->GetDebugFlag(Renderer::DEBUG_SHADOW))
	{
		_shader->SetUniformValue(0, _shadowFlagLocation, 0);
	}
	else
	{
		_shader->SetUniformValue(0, _shadowFlagLocation, -1);
	}

	//glBindFramebuffer(GL_FRAMEBUFFER, _FBO);
	//_framebuffer->BindFrameBuffer();

	_shader->SetUniformValue(0, _postProcLoc, 0);

	RenderScene(renderPass, renderVectorFlag, renderFlag);

	if (renderPass & Phage::Scene::RENDER_PASS_SHADOW)
	{
		glViewport(0, 0, (int)_resolution.x, (int)_resolution.y);
		if (renderVectorFlag & Phage::Scene::TERRAIN)
			glCullFace(GL_BACK);
	}

	if (renderFlag & Phage::Scene::RENDER_FLAG_OCCLUSION_SET && renderVectorFlag & Phage::Scene::TERRAIN)
	{
		_shader->SetUniformValue(0, _occlusionValueLocation, 1);
		_silhouetteFrameBuffer->BindFrameBuffer();
		glViewport(0, 0, (int)_silhouetteResolution.x, (int)_silhouetteResolution.y);
		glCullFace(GL_FRONT);
		RenderScene(renderPass, renderVectorFlag, renderFlag);
		glCullFace(GL_BACK);
		glViewport(0, 0, (int)_resolution.x, (int)_resolution.y);
	}

	glDisable(GL_ALPHA_TEST);
	glDisable(GL_BLEND);

	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glDepthMask(GL_TRUE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//_framebuffer->UnbindFrameBuffer();

	if (renderFlag & Phage::Scene::RENDER_FLAG_WIREFRAME)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	return 0;
}

int Phage::Scene::FinalizeOcclusion()
{
	_silhouetteFrameBuffer->BindFrameBuffer();
	_shader->SetUniformValue(0, _postProcLoc, -1);

	glViewport(0, 0, (int)_silhouetteResolution.x, (int)_silhouetteResolution.y);
	_camera->SetScreenResolution(_silhouetteResolution.x, _silhouetteResolution.y);

	glActiveTexture(GL_TEXTURE0 + 7);
	glBindTexture(GL_TEXTURE_2D, _mainFrameBuffer->GetDepthTextureID());
	glActiveTexture(GL_TEXTURE0 + 27);
	glBindTexture(GL_TEXTURE_2D, _mainFrameBuffer->GetGPosTextureID());
	glActiveTexture(GL_TEXTURE0);

	glm::vec4 uiPoints = glm::vec4(0.0f, 0.0f, _silhouetteResolution.x, _silhouetteResolution.y);
	_mainFBUIEntityTexture->SetTextureID(_mainFrameBuffer->GetDepthTextureID());
	_mainFBUIEntityTexture->SetTranslate(uiPoints.x, uiPoints.y);
	_mainFBUIEntityTexture->SetScale(uiPoints.z - uiPoints.x, uiPoints.w - uiPoints.y);

	mvp = _camera->GetOrthroMatrix().GetMatrix(0) * _mainFBUIEntityTexture->GetMatrix();
	_shader->SetUniformValue(0, _matrixLocation, glm::value_ptr(mvp));

	_mainFBUIEntityTexture->Draw();

	glViewport(0, 0, (int)_resolution.x, (int)_resolution.y);
	_camera->SetScreenResolution(_resolution.x, _resolution.y);

	_silhouetteFrameBuffer->UnbindFrameBuffer();

	glActiveTexture(GL_TEXTURE0 + 5);
	glBindTexture(GL_TEXTURE_2D, _silhouetteFrameBuffer->GetDepthTextureID());
	glActiveTexture(GL_TEXTURE0);

	return 0;
}

int Phage::Scene::PostProcess()
{
	bool _ssao = RendererSingleton->GetDebugFlag(Renderer::DEBUG_SSAO);
	//bool _blur = RendererSingleton->GetDebugFlag(Renderer::DEBUG_BLUR_SSAO);
	bool _outline = RendererSingleton->GetDebugFlag(Renderer::DEBUG_OUTLINE);
	bool _fxaa = RendererSingleton->GetDebugFlag(Renderer::DEBUG_FXAA);
	bool _sharpen = RendererSingleton->GetDebugFlag(Renderer::DEBUG_SHARPEN);

	//Bind the PostProcess Framebuffer. Everything will be drawn to this until the last draw to the screen
	//_postProcessFrameBuffer->BindFrameBuffer();

	//Bind all the textures from the Main Framebuffer
	glActiveTexture(GL_TEXTURE0 + 6);
	glBindTexture(GL_TEXTURE_2D, _mainFrameBuffer->GetColorTextureID());
	glActiveTexture(GL_TEXTURE0 + 7);
	glBindTexture(GL_TEXTURE_2D, _mainFrameBuffer->GetDepthTextureID());
	glActiveTexture(GL_TEXTURE0 + 27);
	glBindTexture(GL_TEXTURE_2D, _mainFrameBuffer->GetGPosTextureID());
	glActiveTexture(GL_TEXTURE0 + 28);
	glBindTexture(GL_TEXTURE_2D, _mainFrameBuffer->GetGNormTextureID());
	glActiveTexture(GL_TEXTURE0 + 29);
	glBindTexture(GL_TEXTURE_2D, _mainFrameBuffer->GetGMiscTextureID());
	glActiveTexture(GL_TEXTURE0);

	_mainFBUIEntityTexture->SetTextureID(_mainFrameBuffer->GetColorTextureID());

	glm::vec4 uiPoints;

	//SSAO implementation
	if (_ssao)
	{
		//Change viewport and camera resolution
		_ssaoFrameBuffer->BindFrameBuffer();
		glViewport(0, 0, (int)_ssaoResolution.x, (int)_ssaoResolution.y);
		_camera->SetScreenResolution(_ssaoResolution.x, _ssaoResolution.y);
		uiPoints = glm::vec4(0.0f, 0.0f, _ssaoResolution.x, _ssaoResolution.y);
		_mainFBUIEntityTexture->SetTranslate(uiPoints.x, uiPoints.y);
		_mainFBUIEntityTexture->SetScale(uiPoints.z - uiPoints.x, uiPoints.w - uiPoints.y);
		mvp = _camera->GetOrthroMatrix().GetMatrix(0) * _mainFBUIEntityTexture->GetMatrix();
		_shader->SetUniformValue(0, _matrixLocation, glm::value_ptr(mvp));

		//Set shader value and draw
		_shader->SetUniformValue(0, _postProcLoc, 4);
		_mainFBUIEntityTexture->Draw();

		//Set the SSAO color buffer for next PostProcess
		glActiveTexture(GL_TEXTURE0 + 6);
		glBindTexture(GL_TEXTURE_2D, _ssaoFrameBuffer->GetColorTextureID());
		glActiveTexture(GL_TEXTURE0);
	}

	//Change viewport and camera resolution back for PostProcess FB
	bool postProcessBack = true;
	_postProcessFrameBufferBack->BindFrameBuffer();
	glViewport(0, 0, (int)_resolution.x, (int)_resolution.y);
	_camera->SetScreenResolution(_resolution.x, _resolution.y);
	uiPoints = glm::vec4(0.0f, 0.0f, _resolution.x, _resolution.y);
	_mainFBUIEntityTexture->SetTranslate(uiPoints.x, uiPoints.y);
	_mainFBUIEntityTexture->SetScale(uiPoints.z - uiPoints.x, uiPoints.w - uiPoints.y);
	mvp = _camera->GetOrthroMatrix().GetMatrix(0) * _mainFBUIEntityTexture->GetMatrix();
	_shader->SetUniformValue(0, _matrixLocation, glm::value_ptr(mvp));

	//Blur SSAO
	/*if (_blur)
	{
		if (!_outline && !_fxaa && !_sharpen)
			glBindFramebuffer(GL_FRAMEBUFFER, 0);

		_shader->SetUniformValue(0, _postProcLoc, 5);
		_mainFBUIEntityTexture->Draw();

		glActiveTexture(GL_TEXTURE0 + 6);
		glBindTexture(GL_TEXTURE_2D, _postProcessFrameBuffer->GetColorTextureID());
		glActiveTexture(GL_TEXTURE0);
	}
	else
	{
		if (!_outline && !_fxaa && !_sharpen)
			glBindFramebuffer(GL_FRAMEBUFFER, 0);

		if (!_ssao)
			_shader->SetUniformValue(0, _postProcLoc, 7);
		else
			_shader->SetUniformValue(0, _postProcLoc, 6);
		_mainFBUIEntityTexture->Draw();

		glActiveTexture(GL_TEXTURE0 + 6);
		glBindTexture(GL_TEXTURE_2D, _postProcessFrameBuffer->GetColorTextureID());
		glActiveTexture(GL_TEXTURE0);
	}*/

	//_mainFBUIEntityTexture->SetTextureID(_seeThroughFrameBuffer->GetColorTextureID());

	//Outline
	if (_outline)
	{
		if (!_fxaa && !_sharpen)
			glBindFramebuffer(GL_FRAMEBUFFER, 0);

		_shader->SetUniformValue(0, _postProcLoc, 1);
		_mainFBUIEntityTexture->Draw();

		_mainFBUIEntityTexture->SetTextureID((postProcessBack ? _postProcessFrameBufferBack : _postProcessFrameBufferFront)->GetColorTextureID());
		(!postProcessBack ? _postProcessFrameBufferBack : _postProcessFrameBufferFront)->BindFrameBuffer();
		postProcessBack = !postProcessBack;
	}

	//FXAA
	if (_fxaa)
	{
		if (!_sharpen)
			glBindFramebuffer(GL_FRAMEBUFFER, 0);

		_shader->SetUniformValue(0, _postProcLoc, 2);
		_mainFBUIEntityTexture->Draw();

		_mainFBUIEntityTexture->SetTextureID((postProcessBack ? _postProcessFrameBufferBack : _postProcessFrameBufferFront)->GetColorTextureID());
		(!postProcessBack ? _postProcessFrameBufferBack : _postProcessFrameBufferFront)->BindFrameBuffer();
		postProcessBack = !postProcessBack;
	}

	//Sharpen
	if (_sharpen)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		if (_ssao)
			_shader->SetUniformValue(0, _postProcLoc, 3);
		else
			_shader->SetUniformValue(0, _postProcLoc, -3);
		_mainFBUIEntityTexture->Draw();
	}

	return 0;
}

int Phage::Scene::GenerateSeeThrough()
{
	if (!_mainFBUIEntityTexture)
	{
		_mainFBUIEntityTexture = new Phage::PhageUIEntity();
		_mainFBUIEntityTexture->Init();
		_mainFBUIEntityTexture->CreateAndAddUITextureQuad(0.0f, 0.0f, 1.0f, 1.0f, 0, true);
	}

	glm::vec4 uiPoints;
	_seeThroughFrameBuffer->BindFrameBuffer();
	glViewport(0, 0, (int)_seeThroughResolution.x, (int)_seeThroughResolution.y);
	_camera->SetScreenResolution(_seeThroughResolution.x, _seeThroughResolution.y);
	uiPoints = glm::vec4(0.0f, 0.0f, _seeThroughResolution.x, _seeThroughResolution.y);
	_mainFBUIEntityTexture->SetTranslate(uiPoints.x, uiPoints.y);
	_mainFBUIEntityTexture->SetScale(uiPoints.z - uiPoints.x, uiPoints.w - uiPoints.y);
	mvp = _camera->GetOrthroMatrix().GetMatrix(0) * _mainFBUIEntityTexture->GetMatrix();
	_shader->SetUniformValue(0, _matrixLocation, glm::value_ptr(mvp));

	//_shader->SetUniformValue(0, _passValueLocation, 0);
	_shader->SetUniformValue(0, _postProcLoc, -2);

	if (!_phageUIEntityTexture)
	{
		_phageUIEntityTexture = new Phage::PhageUIEntity();
		_phageUIEntityTexture->Init();
		_phageUIEntityTexture->CreateAndAddUITextureQuad(0.0f, 0.0f, 1.0f, 1.0f, 0);
	}
	_phageUIEntityTexture->SetTextureID(_seeThroughImage);

	glEnable(GL_BLEND);
	glBlendFunc(GL_DST_COLOR, GL_ZERO);

	float scale = _camera->GetPosition().y / 20.0f;
	float size = 64.0f / scale;
	float halfSize = size / 2.0f;

	_seeThroughQuad->GetMatrix().Init();
	Phage::Matrix seeThroughMatrix;
	seeThroughMatrix.Init();
	seeThroughMatrix.Identity();

	glActiveTexture(GL_TEXTURE0 + 6);
	glBindTexture(GL_TEXTURE_2D, _seeThroughFrameBuffer->GetColorTextureID());
	glActiveTexture(GL_TEXTURE0);
	glActiveTexture(GL_TEXTURE0 + 7);
	glBindTexture(GL_TEXTURE_2D, _seeThroughFrameBuffer->GetDepthTextureID());
	glActiveTexture(GL_TEXTURE0);
	//glDisable(GL_DEPTH_TEST);
	glDepthFunc(GL_ALWAYS);

	for (int i = 8; i < (int)_entityUnitVector.size(); i++)
	{
		glm::vec3 worldPos = glm::vec3(_entityUnitVector[i]->GetMatrix()[3]) +_entityUnitVector[i]->GetBBCenter();
		glm::vec4 screenPos = _camera->Calculate2DPoint(worldPos.x, worldPos.y, worldPos.z);

		_seeThroughQuad->GetMatrix().SetTranslate(worldPos.x, worldPos.y, worldPos.z);

		seeThroughMatrix.Identity();
		seeThroughMatrix.SetTranslate(worldPos.x, worldPos.y, worldPos.z);
		seeThroughMatrix.SetRotate(_camera->GetRotation(), 0.0f, -1.0f, 0.0f);
		seeThroughMatrix.SetScale(1.5f, _entityUnitVector[i]->GetBBCenter().y * 5.0f, 1.0f);

		//_phageUIEntityTexture->SetTranslate(screenPos.x - halfSize, screenPos.y - halfSize, screenPos.z);
		//if (i == 8)
			//printf("Screen pos: (%f, %f, %f, %f)\n", screenPos.x, screenPos.y, screenPos.z, screenPos.w);
		//_phageUIEntityTexture->SetTranslate(0.0f, 0.0f, 0.0f);
		//_phageUIEntityTexture->SetScale(size, size);

		//mvp = _camera->GetOrthroMatrix().GetMatrix(0) * _phageUIEntityTexture->GetMatrix();
		//_shader->SetUniformValue(0, _matrixLocation, glm::value_ptr(mvp));
		mvp = _camera->GetPerspectiveMatrix().GetMatrix(0) * _camera->GetMatrix().GetMatrix(0) * seeThroughMatrix.GetMatrix(1);

		_shader->SetUniformValue(0, _modelMatrixLocation, glm::value_ptr(seeThroughMatrix.GetMatrix(1)));
		_shader->SetUniformValue(0, _matrixLocation, glm::value_ptr(mvp));

		//_phageUIEntityTexture->Draw();
		_seeThroughQuad->Draw();
	}
	glDepthFunc(GL_LEQUAL);
	//glEnable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, (int)_resolution.x, (int)_resolution.y);
	_camera->SetScreenResolution(_resolution.x, _resolution.y);
	uiPoints = glm::vec4(0.0f, 0.0f, _resolution.x, _resolution.y);
	_mainFBUIEntityTexture->SetTranslate(uiPoints.x, uiPoints.y);
	_mainFBUIEntityTexture->SetScale(uiPoints.z - uiPoints.x, uiPoints.w - uiPoints.y);
	mvp = _camera->GetOrthroMatrix().GetMatrix(0) * _mainFBUIEntityTexture->GetMatrix();
	_shader->SetUniformValue(0, _matrixLocation, glm::value_ptr(mvp));

	return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//Clears all vectors, swaps and clears draw buffers
////////////////////////////////////////////////////////////////////////////////////////////////////
int Phage::Scene::FinalizeRender()
{
	for (int i = 0;i < (int)_entityUnitVector.size();i ++)
		_entityUnitVector[i]->ClearPhagePropDesc();
	for (int i = 0;i < (int)_entityBuildingVector.size();i ++)
		_entityBuildingVector[i]->ClearPhagePropDesc();
	for (int i = 0;i < (int)_entityObjectVector.size();i ++)
		_entityObjectVector[i]->ClearPhagePropDesc();

	//_entityVector.clear();
	_entityUnitVector.clear();
	_entityBuildingVector.clear();
	_entityObjectVector.clear();
	_boundingBoxInfoInfoVector.clear();
	_terrainVector.clear();
	_terrainTileDebugVector.clear();
	_losInfo = NULL;
	_losTexture = NULL;
	_terrainDecalVector.clear();
	_uiEntityVector.clear();
	_uiTextInfoVector.clear();
	_uiTextureInfoVector.clear();
	_uiLineRectCoordsVector.clear();
	_lineDrawVector.clear();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	SDL_GL_SwapWindow(_window);

	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	_mainFrameBuffer->ClearFrameBuffer();
	_silhouetteFrameBuffer->ClearFrameBuffer();
	_shadowFrameBuffer->ClearFrameBuffer();
	_ssaoFrameBuffer->ClearFrameBuffer();
	_postProcessFrameBufferFront->ClearFrameBuffer();
	_postProcessFrameBufferBack->ClearFrameBuffer();

	_seeThroughFrameBuffer->ClearFrameBuffer(1.0f, 1.0f, 1.0f);

	return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
//The actual render code that calls the Draw()/Render() command for each entity in the render vector
////////////////////////////////////////////////////////////////////////////////////////////////////
int Phage::Scene::RenderScene(int renderPass, int renderVectorFlag, int renderFlag)
{
	glm::mat4 persp = _camera->GetPerspectiveMatrix().GetMatrix(0);
	glm::mat4 ortho = _camera->GetOrthroMatrix().GetMatrix(0);
	float camPos[3] = { _camera->GetPosition().x, _camera->GetPosition().y, _camera->GetPosition().z };
	_shader->SetUniformValue(0, _cameraPositionLocaton, 3, camPos);

	Phage::PhageEntity* currentEntity = NULL;
	Phage::PhageUIEntity* currentUIEntity = NULL;
	std::vector<Phage::PhageEntity*>* renderVectorPointer = NULL;

	for (int vectorIndex = 0; vectorIndex < NUM_RENDER_VECTORS; vectorIndex++)
	{
		renderVectorPointer = NULL;
		bool renderTerrain = false;
		bool renderUI = false;
		bool renderLine3D = false;
		switch (vectorIndex)
		{
		case 0: if (renderVectorFlag & Phage::Scene::UNIT_VECTOR)
			renderVectorPointer = &_entityUnitVector; break;
		case 1: if (renderVectorFlag & Phage::Scene::BUILDING_VECTOR)
			renderVectorPointer = &_entityBuildingVector; break;
		case 2: if (renderVectorFlag & Phage::Scene::OBJECT_VECTOR)
			renderVectorPointer = &_entityObjectVector; break;
		case 3: if (renderVectorFlag & Phage::Scene::TERRAIN)
			renderTerrain = true; break;
		case 4: if (renderVectorFlag & Phage::Scene::UI_VECTOR)
			renderUI = true; break;
		case 5: if (renderVectorFlag & Phage::Scene::LINE3D_VECTOR)
			renderLine3D = true; break;
		}
		if (renderVectorPointer)
		{
			_shader->SetUniformValue(0, _passValueLocation, 0);
			//For the entityNumber, 0 is no outline at all, 1 is can be outlined against, and >1 is outline
			int testInt = 1;
			for (int i = 0; i < (int)renderVectorPointer->size(); i++)
			{
				currentEntity = renderVectorPointer->at(i);
				mvp = persp * _camera->GetMatrix().GetMatrix(0) * currentEntity->GetMatrix();

				_shader->SetUniformValue(0, _modelMatrixLocation, glm::value_ptr(currentEntity->GetMatrix()));
				_shader->SetUniformValue(0, _matrixLocation, glm::value_ptr(mvp));

				testInt++;

				//if (renderVectorFlag & Phage::Scene::UNIT_VECTOR || renderVectorFlag & Phage::Scene::BUILDING_VECTOR)
				if (vectorIndex == 0 || vectorIndex == 1 || vectorIndex == 2)
					_shader->SetUniformValue(0, _entityNumberLoc, testInt);
				else
					_shader->SetUniformValue(0, _entityNumberLoc, 1);

				if (currentEntity->HasAnimation(currentEntity->GetAnimationIndex()))
				{
					_shader->SetUniformValue(0, _animationFlagLocation, 1);
					std::vector<aiMatrix4x4> boneMat = currentEntity->GetBoneMatrices(currentEntity->GetAnimationIndex(), currentEntity->GetAnimationSeconds());
					_shader->SetUniformValue(0, _boneMatrixLocation, boneMat.size(), &boneMat.at(0).a1, Phage::PhageShader::UNIFORM_MATRIX);
				}

				currentEntity->Draw(_camera);

				_shader->SetUniformValue(0, _animationFlagLocation, 0);

				//Draw any props for the entity
				for (int propIndex = 0;propIndex < currentEntity->PropDescSize();propIndex ++)
				{
					Phage::PhagePropDesc propDesc = currentEntity->PropDescAt(propIndex);
					glm::mat4 propMat = propDesc.GetMatrix();
					mvp = persp * _camera->GetMatrix().GetMatrix(0) * propMat;

					_shader->SetUniformValue(0, _modelMatrixLocation, glm::value_ptr(propMat));
					_shader->SetUniformValue(0, _matrixLocation, glm::value_ptr(mvp));

					propDesc.GetPhageProp()->Draw(_camera);
				}
			}
		}
		if (renderTerrain)
		{
			_shader->SetUniformValue(0, _passValueLocation, 1);
			_shader->SetUniformValue(0, _entityNumberLoc, 1);
			for (int i = 0; i < (int)_terrainVector.size(); i++)
			{
				mvp = persp * _camera->GetMatrix().GetMatrix(0) * glm::mat4(1.0f);
				_shader->SetUniformValue(0, _modelMatrixLocation, glm::value_ptr(glm::mat4(1.0f)));
				_shader->SetUniformValue(0, _matrixLocation, glm::value_ptr(mvp));

				_terrainVector[i]->Render(_shader, _losInfo, _losTexture);
			}
			if (!(renderPass & Phage::Scene::RENDER_PASS_SHADOW))
			{
				glDisable(GL_ALPHA_TEST);
				_shader->SetUniformValue(0, _passValueLocation, 4);
				for (int i = 0; i < (int)_terrainVector.size(); i++)
				{
					mvp = persp * _camera->GetMatrix().GetMatrix(0) * glm::mat4(1.0f);
					_shader->SetUniformValue(0, _modelMatrixLocation, glm::value_ptr(glm::mat4(1.0f)));
					_shader->SetUniformValue(0, _matrixLocation, glm::value_ptr(mvp));

					_terrainVector[i]->RenderTerrainDecals();
					//for (int decalInd = 0;decalInd < _terrainDecalVector.size();decalInd ++)
						//_terrainDecalVector[i]->Draw();
				}
				glEnable(GL_ALPHA_TEST);
			}
		}
		if (renderUI)
		{
			//glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glAlphaFunc(GL_GREATER, 0.01f);
			_shader->SetUniformValue(0, _passValueLocation, 2);
			_shader->SetUniformValue(0, _entityNumberLoc, 1);
			for (int i = 0;i < (int)_uiEntityVector.size();i ++)
			{
				currentUIEntity = _uiEntityVector.at(i);
				mvp = ortho * currentUIEntity->GetMatrix();
				_shader->SetUniformValue(0, _matrixLocation, glm::value_ptr(mvp));

				currentUIEntity->Draw();
			}

			if (!_phageUILineRectTest)
			{
				_phageUILineRectTest = new Phage::PhageUILineRect();
				_phageUILineRectTest->CreateLineRect();
			}
			for (int i = 0;i < (int)_uiLineRectCoordsVector.size();i ++)
			{
				mvp = ortho;
				_shader->SetUniformValue(0, _matrixLocation, glm::value_ptr(mvp));

				_phageUILineRectTest->Draw(_uiLineRectCoordsVector[i]);
			}

			if (!_phageUIEntityTexture)
			{
				_phageUIEntityTexture = new Phage::PhageUIEntity();
				_phageUIEntityTexture->Init();
				_phageUIEntityTexture->CreateAndAddUITextureQuad(0.0f, 0.0f, 1.0f, 1.0f, 0);
			}
			std::sort(_uiTextureInfoVector.begin(), _uiTextureInfoVector.end());
			for (int i = 0;i < (int)_uiTextureInfoVector.size();i ++)
			{
				glm::vec4 uiPoints = _uiTextureInfoVector[i].points;
				_phageUIEntityTexture->SetTextureID(_uiTextureInfoVector[i].textureID);
				_phageUIEntityTexture->SetTranslate(uiPoints.x, uiPoints.y, _uiTextureInfoVector[i].depth);
				_phageUIEntityTexture->SetScale(uiPoints.z-uiPoints.x, uiPoints.w-uiPoints.y);

				mvp = ortho * _phageUIEntityTexture->GetMatrix();
				_shader->SetUniformValue(0, _matrixLocation, glm::value_ptr(mvp));

				_phageUIEntityTexture->Draw();
			}

			if (!_phageUITextTest)
			{
				_phageUITextTest = new Phage::PhageUIText();
				_phageUITextTest->Init();
				_phageUITextTest->CreateText("arialbd.ttf", 16, 0);
			}
			for (int i = 0;i < (int)_uiTextInfoVector.size();i ++)
			{
				mvp = ortho;
				_shader->SetUniformValue(0, _matrixLocation, glm::value_ptr(mvp));

				if (!_phageUITextSizeVector[_uiTextInfoVector[i].size])
				{
					_phageUITextSizeVector[_uiTextInfoVector[i].size] = new Phage::PhageUIText();
					_phageUITextSizeVector[_uiTextInfoVector[i].size]->Init();
					_phageUITextSizeVector[_uiTextInfoVector[i].size]->CreateText("arialbd.ttf", _uiTextInfoVector[i].size, 0);

					for (int k = 0;k < (int)_uiTextIconVector.size();k ++)
						_phageUITextSizeVector[_uiTextInfoVector[i].size]->AddUITextIcon(_uiTextIconVector[k].c, _uiTextIconVector[k].tex);
				}
				_phageUITextSizeVector[_uiTextInfoVector[i].size]->Draw((int)_uiTextInfoVector[i].pos.x, (int)_uiTextInfoVector[i].pos.y, _uiTextInfoVector[i].str);
			}
			glEnable(GL_BLEND);
			/*if (!_phageUIEntityTexture)
			{
				_phageUIEntityTexture = new Phage::PhageUIEntity();
				_phageUIEntityTexture->Init();
				_phageUIEntityTexture->CreateAndAddUITextureQuad(0.0f, 0.0f, 1.0f, 1.0f, 0);
			}*/
			/*for (int i = 0;i < _uiTextureInfoVector.size();i ++)
			{
				glm::vec4 uiPoints = _uiTextureInfoVector[i].points;
				_phageUIEntityTexture->SetTextureID(_uiTextureInfoVector[i].textureID);
				_phageUIEntityTexture->SetTranslate(uiPoints.x, uiPoints.y);
				_phageUIEntityTexture->SetScale(uiPoints.z-uiPoints.x, uiPoints.w-uiPoints.y);

				mvp = ortho * _phageUIEntityTexture->GetMatrix();
				_shader->SetUniformValue(0, _matrixLocation, glm::value_ptr(mvp));

				_phageUIEntityTexture->Draw();
			}*/
			glAlphaFunc(GL_GREATER, 0.5f);
			//glBindFramebuffer(GL_FRAMEBUFFER, _mainFrameBuffer->GetFrameBufferID());
		}
		if (renderLine3D)
		{
			_shader->SetUniformValue(0, _passValueLocation, 5);
			_shader->SetUniformValue(0, _entityNumberLoc, 0);
			for (int i = 0; i < (int)_boundingBoxInfoInfoVector.size(); i++)
			{
				_phageBoundingBox->SetTranslate(_boundingBoxInfoInfoVector[i].pos.x, _boundingBoxInfoInfoVector[i].pos.y, _boundingBoxInfoInfoVector[i].pos.z);
				_phageBoundingBox->Translate(_boundingBoxInfoInfoVector[i].center.x, _boundingBoxInfoInfoVector[i].center.y, _boundingBoxInfoInfoVector[i].center.z);
				_phageBoundingBox->SetScale(_boundingBoxInfoInfoVector[i].size.x, _boundingBoxInfoInfoVector[i].size.y, _boundingBoxInfoInfoVector[i].size.z);
				_phageBoundingBox->SetRotate(_boundingBoxInfoInfoVector[i].rotation);
				glm::vec4 bbRot = _boundingBoxInfoInfoVector[i].bbRot;
				_phageBoundingBox->Rotate(bbRot.x, bbRot.y, bbRot.z, bbRot.w);

				mvp = persp * _camera->GetMatrix().GetMatrix(0) * _phageBoundingBox->GetMatrix();
				_shader->SetUniformValue(0, _modelMatrixLocation, glm::value_ptr(_phageBoundingBox->GetMatrix()));
				_shader->SetUniformValue(0, _matrixLocation, glm::value_ptr(mvp));

				_phageBoundingBox->Draw();
			}

			for (int i = 0;i < (int)_terrainTileDebugVector.size();i ++)
			{
				_phageTerrainTileDebug->SetTranslate(_terrainTileDebugVector[i].pos.x, 4.5f, _terrainTileDebugVector[i].pos.y);
				mvp = persp * _camera->GetMatrix().GetMatrix(0) * _phageTerrainTileDebug->GetMatrix();
				_shader->SetUniformValue(0, _modelMatrixLocation, glm::value_ptr(_phageTerrainTileDebug->GetMatrix()));
				_shader->SetUniformValue(0, _matrixLocation, glm::value_ptr(mvp));

				_phageTerrainTileDebug->Draw();
			}

			for (int i = 0;i < (int)_lineDrawVector.size();i ++)
			{
				if (i == 0)
				{
					mvp = persp * _camera->GetMatrix().GetMatrix(0);
					_shader->SetUniformValue(0, _modelMatrixLocation, glm::value_ptr(glm::mat4(1.0f)));
					_shader->SetUniformValue(0, _matrixLocation, glm::value_ptr(mvp));
				}
				_phageLine->SetPoints(_lineDrawVector[i].pt1, _lineDrawVector[i].pt2);
				_phageLine->Draw();
			}
		}
	}

	return 0;
}
