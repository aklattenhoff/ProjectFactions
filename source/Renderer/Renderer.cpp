#include "Renderer.h"

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "Phage/Phage.h"

#include "Renderer/RS/GUIText.h"
#include "Renderer/RS/GUIRectangle.h"
#include "Renderer/RS/GUILineRect.h"
#include "Renderer/RS/GUITexture.h"
#include "Renderer/RS/GUIToolTip.h"
#include "Renderer/RS/GUIToolTipBackground.h"
#include "Renderer/RS/Entity.h"
#include "Renderer/RS/PhageEntity.h"
#include "Renderer/RS/TerrainDecal.h"
#include "Renderer/RS/PhageTerrainDecal.h"
#include "Renderer/RS/Prop.h"
//#include "Game/Terrain/Terrain.h"
#include "Game/LOSMap/LOSMap.h"
#include "Game/Entity/EntityBase.h"
#include "Game/LOS/LOS.h"

Renderer* Renderer::_instance = NULL;

Renderer::Renderer()
{
	_phageScene = NULL;
	_phageShader = NULL;
	_phageCamera = NULL;

	_phagePointerEntity = NULL;
	_phageFrustumEntity[0] = NULL;
	_phageFrustumEntity[1] = NULL;
	_phageFrustumEntity[2] = NULL;
	_phageFrustumEntity[3] = NULL;
	_phageFrustumEntity[4] = NULL;
	_phageFrustumEntity[5] = NULL;
	_phageFrustumEntity[6] = NULL;
	_phageFrustumEntity[7] = NULL;

	_phageMixer = NULL;


	_window = NULL;

	_cameraRotation = 0.0f;
	_bumpMapping = false;

	_losMap = NULL;

	_debugFlagVector.resize(100, false);
	_debugFlagVector[DEBUG_SSAO] = true;
	_debugFlagVector[DEBUG_BLUR_SSAO] = true;
	_debugFlagVector[DEBUG_OUTLINE] = true;
	_debugFlagVector[DEBUG_FXAA] = true;
	_debugFlagVector[DEBUG_SHARPEN] = true;
	_debugFlagVector[DEBUG_SHADOW] = true;

	_phageTerrain = NULL;

	_guiToolTipBackground = NULL;
}

Renderer::~Renderer()
{
	delete _phageScene;
	delete _phageShader;
	delete _phageCamera;
	delete _phageMixer;

	delete _losMap;
	delete _guiToolTipBackground;
}

int Renderer::CreateSingleton()
{
	_instance = new Renderer();

	return 0;
}

Renderer* Renderer::GetSingleton()
{
	return _instance;
}

int Renderer::DeleteSingleton()
{
	delete _instance;
	_instance = NULL;

	return 0;
}

int Renderer::InitGraphics()
{

	return 0;
}

int Renderer::CreateScene()
{
	//Create Phage Scene
	_phageScene = new Phage::Scene();
	_phageScene->AttachToWindow(_window);
	_phageScene->Init();

	_phageMixer = new Phage::PhageMixer();
	_phageMixer->Init();

	//Create and populate Phage Shader
	_phageShader = _phageScene->CreateShader();
	if (_phageShader)
	{
		if (_phageShader->LoadShader("shaders/vert.fsl", "shaders/frag.fsl") == -1)
		{
			DebugOutput("Could not open shader files\n");
			return -1;
		}

		int samplerLoc = _phageShader->LoadUniformLocation(0, "sampler");
		_phageShader->SetUniformValue(0, samplerLoc, 0);
		int bumpSamplerLoc = _phageShader->LoadUniformLocation(0, "bumpSampler");
		_phageShader->SetUniformValue(0, bumpSamplerLoc, 3);
		int terrainMapSamplerLoc = _phageShader->LoadUniformLocation(0, "terrainMapSampler");
		int terrainMapSamplerData[2] = { 8, 9 };
		_phageShader->SetUniformValue(0, terrainMapSamplerLoc, 2, &terrainMapSamplerData[0]);
		int terrainTextureSamplerLoc = _phageShader->LoadUniformLocation(0, "terrainTextureSampler");
		int terrainTextureSamplerData[16] = { 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26 };
		_phageShader->SetUniformValue(0, terrainTextureSamplerLoc, 16, &terrainTextureSamplerData[0]);
		int terrainBumpSamplerLoc = _phageShader->LoadUniformLocation(0, "terrainBumpSampler");
		int terrainBumpSamplerData[16] = { 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46 };
		_phageShader->SetUniformValue(0, terrainBumpSamplerLoc, 16, &terrainBumpSamplerData[0]);
		int alphaTextureSamplerLoc = _phageShader->LoadUniformLocation(0, "alphaTextureSampler");
		_phageShader->SetUniformValue(0, alphaTextureSamplerLoc, 10);

		int fbShadowSamplerLoc = _phageShader->LoadUniformLocation(0, "fbShadowSampler");
		_phageShader->SetUniformValue(0, fbShadowSamplerLoc, 30);
		int fbSilhouetteSamplerLoc = _phageShader->LoadUniformLocation(0, "fbSilhouetteSampler");
		_phageShader->SetUniformValue(0, fbSilhouetteSamplerLoc, 5);
		int fbColorSamplerLoc = _phageShader->LoadUniformLocation(0, "fbColorSampler");
		_phageShader->SetUniformValue(0, fbColorSamplerLoc, 6);
		int fbDepthSamplerLoc = _phageShader->LoadUniformLocation(0, "fbDepthSampler");
		_phageShader->SetUniformValue(0, fbDepthSamplerLoc, 7);

		int gPositionSamplerLoc = _phageShader->LoadUniformLocation(0, "gPositionSampler");
		_phageShader->SetUniformValue(0, gPositionSamplerLoc, 27);
		int gNormalSamplerLoc = _phageShader->LoadUniformLocation(0, "gNormalSampler");
		_phageShader->SetUniformValue(0, gNormalSamplerLoc, 28);
		int gMiscSamplerLoc = _phageShader->LoadUniformLocation(0, "gMiscSampler");
		_phageShader->SetUniformValue(0, gMiscSamplerLoc, 29);
	}
	else
		return -1;

	//Create and populate Phage Camera
	_phageCamera = _phageScene->CreateCamera();
	if (_phageCamera)
	{
		_phageCamera->SetScreenResolution(800.0f, 600.0f);
		_phageCamera->SetPosition(16.0f + 50.0f, 28.0f, 48.0f + 50.0f);
		_phageCamera->SetAngle(52.5f);
		_phageCamera->SetRotation(0.0f);
	}

	//This is for the tooltip on menu
	//TODO: should probably be moved somewhere else
	 _guiToolTipBackground = new RS::GUIToolTipBackground();

	 Phage::PhageModel* pointerModel = new Phage::PhageModel();
	 pointerModel->Init();
	 _phagePointerEntity = new Phage::PhageEntity();
	 _phagePointerEntity->Init();
	 if (pointerModel->LoadCollada("models/pointer.dae") == -1)
		 return 0;
	 if (pointerModel->LoadMainTexture("models/pointer.png") == -1)
		 return 0;
	 if (pointerModel->LoadBumpTexture("models/pointer_bump.png") == -1)
		 return 0;
	 if (pointerModel->LoadSBCTexture("models/pointer_sbc.png") == -1)
		 return 0;
	 pointerModel->SetOrientation(Phage::PhageModel::ORIENTATION_Z);
	 pointerModel->SetScale(0.2f);
	 _phagePointerEntity->SetPhageModel(pointerModel);
	 _phagePointerEntity->SetBaseRotate(-90.0f, 1.0f, 0.0f, 0.0f);
	 _phagePointerEntity->AddScale(0.2f, 0.2f, 0.2f);

	 for (int i = 0; i < 8; i++)
	 {
		 _phageFrustumEntity[i] = new Phage::PhageEntity();
		 _phageFrustumEntity[i]->Init();
		 _phageFrustumEntity[i]->SetPhageModel(pointerModel);
		 _phageFrustumEntity[i]->SetBaseRotate(-90.0f, 1.0f, 0.0f, 0.0f);
		 _phageFrustumEntity[i]->AddScale(0.2f, 0.2f, 0.2f);
	 }

	return 0;
}

Phage::PhageCamera* Renderer::GetPhageCamera()
{
	return _phageCamera;
}

int Renderer::SetWindow(SDL_Window* window)
{
	_window = window;

	return 0;
}

int Renderer::SetResolution(int w, int h)
{
	_phageScene->SetResolution(w, h);
	_screenWidth = w;
	_screenHeight = h;

	return 0;
}

int Renderer::SetCameraPosition(float x, float y, float z)
{
	_phageCamera->SetPosition(x, y, z);

	return 0;
}

int Renderer::MoveCameraPosition(float x, float y, float z)
{
	_phageCamera->MovePosition(x, y, z);

	return 0;
}

glm::vec3 Renderer::GetCameraPosition()
{
	return _phageCamera->GetPosition();
}

float Renderer::GetCameraX()
{
	return _phageCamera->GetPosition().x;
}
float Renderer::GetCameraY()
{
	return _phageCamera->GetPosition().y;
}
float Renderer::GetCameraZ()
{
	return _phageCamera->GetPosition().z;
}

int Renderer::SetCameraRotation(float r)
{
	_phageCamera->SetRotation(r);

	return 0;
}

int Renderer::MoveCameraPitch(float p)
{
	_phageCamera->SetAngle(_phageCamera->GetAngle() + p);

	return 0;
}

int Renderer::SetCameraPitch(float p)
{
	_phageCamera->SetAngle(p);

	return 0;
}

float Renderer::GetCameraPitch()
{
	return _phageCamera->GetAngle();
}

int Renderer::SetMousePosition(float x, float y)
{
	_mousePosition.x = x;
	_mousePosition.y = y;

	//_phageCamera->Set2DMousePosition(x, y);

	return 0;
}

glm::vec2 Renderer::GetMouse()
{
	return _mousePosition;
}

glm::vec3 Renderer::GetMouseNear()
{
	return _phageCamera->GetMouseNear();
}

glm::vec3 Renderer::GetMouseFar()
{
	return _phageCamera->GetMouseFar();
}

glm::vec3 Renderer::GetMouseTerrain()
{
	return _mouseTerrainPosition;
}

int Renderer::GetScreenWidth()
{
	return (int)_phageScene->GetResolution().x;
}

int Renderer::GetScreenHeight()
{
	return (int)_phageScene->GetResolution().y;
}

int Renderer::CalculateScene()
{
	_phageCamera->Set2DMousePosition(_mousePosition.x, _mousePosition.y);

	glm::vec3 nearPlane[4];
	nearPlane[0] = glm::vec3(_phageCamera->Calculate3DPointFromScreenPos(0.0f, 0.0f, -1.0f));
	nearPlane[1] = glm::vec3(_phageCamera->Calculate3DPointFromScreenPos(_phageScene->GetResolution().x, 0.0f, -1.0f));
	nearPlane[2] = glm::vec3(_phageCamera->Calculate3DPointFromScreenPos(0.0f, _phageScene->GetResolution().y, -1.0f));
	nearPlane[3] = glm::vec3(_phageCamera->Calculate3DPointFromScreenPos(_phageScene->GetResolution().x, _phageScene->GetResolution().y, -1.0f));
	glm::vec3 farPlane[4];
	farPlane[0] = _phageCamera->Calculate3DPointAtZero(0.0, 0.0);
	farPlane[1] = _phageCamera->Calculate3DPointAtZero(_phageScene->GetResolution().x, 0.0);
	farPlane[2] = _phageCamera->Calculate3DPointAtZero(0.0, _phageScene->GetResolution().y);
	farPlane[3] = _phageCamera->Calculate3DPointAtZero(_phageScene->GetResolution().x, _phageScene->GetResolution().y);
	_phageScene->SetShadowMapBounds(nearPlane, farPlane);

	_phageFrustumEntity[0]->SetTranslate(nearPlane[0].x, nearPlane[0].y, nearPlane[0].z);
	_phageFrustumEntity[1]->SetTranslate(nearPlane[1].x, nearPlane[1].y, nearPlane[1].z);
	_phageFrustumEntity[2]->SetTranslate(nearPlane[2].x, nearPlane[2].y, nearPlane[2].z);
	_phageFrustumEntity[3]->SetTranslate(nearPlane[3].x, nearPlane[3].y, nearPlane[3].z);
	_phageFrustumEntity[4]->SetTranslate(farPlane[0].x, farPlane[0].y, farPlane[0].z);
	_phageFrustumEntity[5]->SetTranslate(farPlane[1].x, farPlane[1].y, farPlane[1].z);
	_phageFrustumEntity[6]->SetTranslate(farPlane[2].x, farPlane[2].y, farPlane[2].z);
	_phageFrustumEntity[7]->SetTranslate(farPlane[3].x, farPlane[3].y, farPlane[3].z);


	_phageScene->AddEntityToVector(_phageFrustumEntity[0], Phage::Scene::UNIT_VECTOR);
	_phageScene->AddEntityToVector(_phageFrustumEntity[1], Phage::Scene::UNIT_VECTOR);
	_phageScene->AddEntityToVector(_phageFrustumEntity[2], Phage::Scene::UNIT_VECTOR);
	_phageScene->AddEntityToVector(_phageFrustumEntity[3], Phage::Scene::UNIT_VECTOR);
	_phageScene->AddEntityToVector(_phageFrustumEntity[4], Phage::Scene::UNIT_VECTOR);
	_phageScene->AddEntityToVector(_phageFrustumEntity[5], Phage::Scene::UNIT_VECTOR);
	_phageScene->AddEntityToVector(_phageFrustumEntity[6], Phage::Scene::UNIT_VECTOR);
	_phageScene->AddEntityToVector(_phageFrustumEntity[7], Phage::Scene::UNIT_VECTOR);

	if (_phageTerrain)
	{
		_mouseTerrainPosition = _phageTerrain->CalculateMouseOnTerrain(_phageCamera->GetMouseNear(), _phageCamera->GetMouseZero());
	}

	//_phagePointerEntity->SetTranslate(pNW.x, 0.0f, pNW.z);
	//_phagePointerEntity->SetTranslate(_phageCamera->GetMouseZero().x, _phageTerrain->GetMouseOnTerrain().y, _phageCamera->GetMouseZero().z);
	//_phageScene->AddEntityToVector(_phagePointerEntity, Phage::Scene::UNIT_VECTOR);

	return 0;
}

int Renderer::Render()
{
	for (int i = 0;i < (int)_phageUIEntityVector.size();i ++)
	{
		_phageScene->AddUIEntityToVector(_phageUIEntityVector[i]);
	}

	for (int i = 0;i < (int)_rsGUITextureVector.size();i ++)
	{
		_phageScene->AddUITextureInfo(_rsGUITextureVector[i].GetPoints(), _rsGUITextureVector[i].GetTexture(), _rsGUITextureVector[i].GetDepth());
	}

	for (int i = 0;i < (int)_debugConsoleTextVector.size();i ++)
	{
		_phageScene->AddUITextInfo(glm::vec2(10, i*20+30), _debugConsoleTextVector[i].str);

		_debugConsoleTextVector[i].timer ++;
		if (_debugConsoleTextVector[i].timer > 300)
		{
			_debugConsoleTextVector.erase(_debugConsoleTextVector.begin()+i);
			i --;
		}
	}
	for (int i = 0;i < (int)_rsGUITextVector.size();i ++)
	{
		int textSize = _screenHeight / (1080 / _rsGUITextVector[i].GetHeight());
		if (textSize < 6) textSize = 6;
		_phageScene->AddUITextInfo(glm::vec2(_rsGUITextVector[i].GetX(), _rsGUITextVector[i].GetY()), _rsGUITextVector[i].GetString(), textSize);
	}
	for (int i = 0;i < (int)_rsGUILineRectVector.size();i ++)
	{
		_phageScene->AddUILineRectCoords(_rsGUILineRectVector[i].GetPoints());
	}

	//Tooltip
	for (int i = 0;i < (int)_rsGUIToolTipVector.size();i ++)
	{
		glm::vec2 ttPos = glm::vec2(_rsGUIToolTipVector[i].GetX(), _rsGUIToolTipVector[i].GetY());
		glm::vec4 backP = glm::vec4(ttPos, 0.0f, 0.0f);
		float maxLineNum = 0.0f;
		float height = (float)_rsGUIToolTipVector[i].GetHeight();
		float paddedHeight = (float) (_rsGUIToolTipVector[i].GetHeight()*4/3);
		ttPos.x += paddedHeight;
		for (int k = 0;k < _rsGUIToolTipVector[i].GetNumberOfLines();k ++)
		{
			if (_rsGUIToolTipVector[i].GetString(k).size() > maxLineNum) maxLineNum = (float)_rsGUIToolTipVector[i].GetString(k).size();
			_phageScene->AddUITextInfo(ttPos, _rsGUIToolTipVector[i].GetString(k));
			ttPos.y += paddedHeight;
		}
		if (_guiToolTipBackground)
		{
			backP.z = ttPos.x + maxLineNum*(height*3.0f)/4.0f;
			backP.w = ttPos.y;
			float borderWidth = (float)_guiToolTipBackground->GetBorderWidth();
			_phageScene->AddUITextureInfo(backP, _guiToolTipBackground->GetBorderBackground()._backTex, 0.9f);

			_phageScene->AddUITextureInfo(	glm::vec4(backP.x-borderWidth, backP.y-borderWidth, backP.x, backP.y),
											_guiToolTipBackground->GetBorderBackground()._cornerTexNW, 0.9f);
			_phageScene->AddUITextureInfo(	glm::vec4(backP.z, backP.y-borderWidth, backP.z+borderWidth, backP.y),
											_guiToolTipBackground->GetBorderBackground()._cornerTexNE, 0.9f);
			_phageScene->AddUITextureInfo(	glm::vec4(backP.z, backP.w, backP.z+borderWidth, backP.w+borderWidth),
											_guiToolTipBackground->GetBorderBackground()._cornerTexSE, 0.9f);
			_phageScene->AddUITextureInfo(	glm::vec4(backP.x-borderWidth, backP.w, backP.x, backP.w+borderWidth),
											_guiToolTipBackground->GetBorderBackground()._cornerTexSW, 0.9f);

			_phageScene->AddUITextureInfo(	glm::vec4(backP.x, backP.y-borderWidth, backP.z, backP.y),
											_guiToolTipBackground->GetBorderBackground()._edgeTexN, 0.9f);
			_phageScene->AddUITextureInfo(	glm::vec4(backP.x, backP.w, backP.z, backP.w+borderWidth),
											_guiToolTipBackground->GetBorderBackground()._edgeTexS, 0.9f);
			_phageScene->AddUITextureInfo(	glm::vec4(backP.z, backP.y, backP.z+borderWidth, backP.w),
											_guiToolTipBackground->GetBorderBackground()._edgeTexE, 0.9f);
			_phageScene->AddUITextureInfo(	glm::vec4(backP.x-borderWidth, backP.y, backP.x, backP.w),
											_guiToolTipBackground->GetBorderBackground()._edgeTexW, 0.9f);
		}
	}

	for (int i = 0; i < (int)_phageEntityVector.size(); i++)
	{
		_phageScene->AddEntityToVector(_phageEntityVector[i], Phage::Scene::OBJECT_VECTOR);
	}

	if (HoverOverEntity)
	{
		glm::vec3 pos = HoverOverEntity->GetTranslationVector();
		float rot = HoverOverEntity->RotY();
		for (int bbI = 0; bbI < HoverOverEntity->GetEntityBase()->BoundingBox3D()->Size(); bbI++)
		{
			const BoundingBox3D* bb3D = HoverOverEntity->GetEntityBase()->BoundingBox3D()->GetBoundingBox(bbI);
			glm::vec3 bbPos = bb3D->midpoint;
			glm::vec3 bbSize = glm::vec3(bb3D->point2.x, bb3D->size.y, bb3D->size.x);
			glm::vec4 bbRot = glm::vec4(bb3D->dot, bb3D->cross.x, bb3D->cross.y, bb3D->cross.z);

			_phageScene->AddBoundingBoxInfo(pos, bbPos, bbSize, rot, bbRot);
		}
		HoverOverEntity = NULL;
	}

	//_phageScene->AddEntity(_phageTestEntity);
	for (int i = 0;i < (int)_rsPhageEntityVector.size();i ++)
	{
		glm::vec3 pos = _rsPhageEntityVector[i].GetPosition();
		_rsPhageEntityVector[i].GetModel()->SetTranslate(pos.x, pos.y, pos.z);
		_rsPhageEntityVector[i].GetModel()->SetRotate(_rsPhageEntityVector[i].GetRotation(), 0.0f, 0.0f, 1.0f);
		_rsPhageEntityVector[i].GetModel()->SetAnimationIndex(_rsPhageEntityVector[i].GetAnimationIndex());
		_rsPhageEntityVector[i].GetModel()->SetAnimationSeconds(_rsPhageEntityVector[i].GetAnimationTime());

		_rsPhageEntityVector[i].GetModel()->SetBBCenter(_rsPhageEntityVector[i].GetEntityBase()->BoundingBox3D()->CalcAverageMidpoint());

		if (_rsPhageEntityVector[i].GetEntityBase()->EntityTypeInt() == ENTITY_TYPE_UNIT)
			_phageScene->AddEntityToVector(_rsPhageEntityVector[i].GetModel(), Phage::Scene::UNIT_VECTOR);
		else if (_rsPhageEntityVector[i].GetEntityBase()->EntityTypeInt() == ENTITY_TYPE_BUILDING)
			_phageScene->AddEntityToVector(_rsPhageEntityVector[i].GetModel(), Phage::Scene::BUILDING_VECTOR);
		else
			_phageScene->AddEntityToVector(_rsPhageEntityVector[i].GetModel(), Phage::Scene::OBJECT_VECTOR);
		//DebugOutput(":%i\n", (int)_rsPhageEntityVector[i].GetModel());
	}
	if (_phageTerrain)
	{
		_phageScene->AddTerrain(_phageTerrain);
		for (int i = 0;i < (int)_rsPhageTerrainDecalVector.size();i ++)
		{
			_phageScene->AddTerrainDecal(_rsPhageTerrainDecalVector[i].GetTerrainDecal());
		}
	}
	if (_los)
	{
		_phageScene->AddLOSInfo(_los->GetLOSPointer(), _los->GetLOSTexture());
	}

	if (_debugFlagVector[DEBUG_FLAG_RENDER_BOUNDING_BOXES])
	{
		for (int i = 0;i < (int)_rsPhageEntityVector.size();i ++)
		{
			glm::vec3 pos = _rsPhageEntityVector[i].GetPosition();
			float rot = _rsPhageEntityVector[i].GetRotation();
			for (int bbI = 0;bbI < _rsPhageEntityVector[i].GetEntityBase()->BoundingBox3D()->Size();bbI ++)
			{
				const BoundingBox3D* bb3D = _rsPhageEntityVector[i].GetEntityBase()->BoundingBox3D()->GetBoundingBox(bbI);
				glm::vec3 bbPos = bb3D->midpoint;
				glm::vec3 bbSize = glm::vec3(bb3D->point2.x, bb3D->size.y, bb3D->size.x);
				glm::vec4 bbRot = glm::vec4(bb3D->dot, bb3D->cross.x, bb3D->cross.y, bb3D->cross.z);

				_phageScene->AddBoundingBoxInfo(pos, bbPos, bbSize, rot, bbRot);
			}
		}
	}

	/*if (_debugFlagVector[DEBUG_FLAG_RENDER_TERRAIN_MAP])
	{
		if (_terrain)
		{
			glm::vec2 mapSize = _terrain->getTerrainMap()->GetMapSize();
			float mapRes = _terrain->getTerrainMap()->GetResolution();
			for (int j = 0;j < mapSize.y;j ++)
			{
				for (int i = 0;i < mapSize.x;i ++)
				{
					if (_terrain->getTerrainMap()->GetMapValue(glm::vec2(i, j)) != 0)
					{

						//_phageScene->AddTerrainTileDebug(glm::vec2(i, j) / mapRes);
						float tX = (float)i / mapRes;
						float tY = (float)j / mapRes;
						float tD = 1.0f / mapRes;
						glm::vec3 terrP1 = glm::vec3(tX, _phageTerrain->CalculateHeight(tX, tY), tY);
						glm::vec3 terrP2 = glm::vec3(tX + tD, _phageTerrain->CalculateHeight(tX + tD, tY), tY);
						glm::vec3 terrP3 = glm::vec3(tX + tD, _phageTerrain->CalculateHeight(tX + tD, tY + tD), tY + tD);
						glm::vec3 terrP4 = glm::vec3(tX, _phageTerrain->CalculateHeight(tX, tY + tD), tY + tD);
						_phageScene->AddLineDrawInfo(terrP1, terrP2);
						_phageScene->AddLineDrawInfo(terrP2, terrP3);
						_phageScene->AddLineDrawInfo(terrP3, terrP4);
						_phageScene->AddLineDrawInfo(terrP4, terrP1);
					}
				}
			}
		}
	}*/

	//Debug On-screen Menu
	{
		glm::vec2 textPos = glm::vec2(_screenWidth - 180, 320);
		std::string textStr = "Debug Menu";
		_phageScene->AddUITextInfo(textPos, textStr, 10);

		textPos.y += 10; textStr = "SSAO "; textStr.append(_debugFlagVector[DEBUG_SSAO] ? "+" : "-");
		_phageScene->AddUITextInfo(textPos, textStr, 10);
		textPos.y += 10; textStr = "Blur SSAO "; textStr.append(_debugFlagVector[DEBUG_BLUR_SSAO] ? "+" : "-");
		_phageScene->AddUITextInfo(textPos, textStr, 10);
		textPos.y += 10; textStr = "Outline "; textStr.append(_debugFlagVector[DEBUG_OUTLINE] ? "+" : "-");
		_phageScene->AddUITextInfo(textPos, textStr, 10);
		textPos.y += 10; textStr = "FXAA "; textStr.append(_debugFlagVector[DEBUG_FXAA] ? "+" : "-");
		_phageScene->AddUITextInfo(textPos, textStr, 10);
		textPos.y += 10; textStr = "Sharpen "; textStr.append(_debugFlagVector[DEBUG_SHARPEN] ? "+" : "-");
		_phageScene->AddUITextInfo(textPos, textStr, 10);
		textPos.y += 10; textStr = "Shadow "; textStr.append(_debugFlagVector[DEBUG_SHADOW] ? "+" : "-");
		_phageScene->AddUITextInfo(textPos, textStr, 10);
	}

	for (int i = 0;i < (int)_lineDrawVector.size();i ++)
		_phageScene->AddLineDrawInfo(_lineDrawVector[i].pt1, _lineDrawVector[i].pt2);

	int wireframe = _debugFlagVector[DEBUG_FLAG_RENDER_WIREFRAME] ? Phage::Scene::RENDER_FLAG_WIREFRAME : 0;

	_phageScene->GenerateSeeThrough();

	//_phageTestUIEntity->SetTranslate(_mousePosition.x, _mousePosition.y, 0.99f);
	//_phageScene->AddUIEntityToVector(_phageTestUIEntity);

	_phageScene->Render(Phage::Scene::RENDER_PASS_NORMAL, Phage::Scene::LINE3D_VECTOR, 0x00);

	//DebugOutput("Num Phage Entities: %i\n", (int)_rsPhageEntityVector.size());
	if (_debugFlagVector[DEBUG_SHADOW])
	{
		//_phageScene->Render(Phage::Scene::RENDER_PASS_SHADOW, Phage::Scene::TERRAIN, 0x00 | wireframe);
		_phageScene->Render(Phage::Scene::RENDER_PASS_SHADOW, Phage::Scene::BUILDING_VECTOR | Phage::Scene::OBJECT_VECTOR | Phage::Scene::UNIT_VECTOR, 0x00 | wireframe);
	}

	//_phageScene->Render(Phage::Scene::RENDER_PASS_NORMAL, Phage::Scene::TERRAIN, Phage::Scene::RENDER_FLAG_OCCLUSION_SET | wireframe);
	_phageScene->Render(Phage::Scene::RENDER_PASS_NORMAL, Phage::Scene::TERRAIN, Phage::Scene::RENDER_FLAG_OCCLUSION_SET | wireframe);
	_phageScene->Render(Phage::Scene::RENDER_PASS_NORMAL, Phage::Scene::UNIT_VECTOR, Phage::Scene::RENDER_FLAG_OCCLUSION_TEST | wireframe);
	_phageScene->Render(Phage::Scene::RENDER_PASS_NORMAL, Phage::Scene::BUILDING_VECTOR | Phage::Scene::OBJECT_VECTOR, Phage::Scene::RENDER_FLAG_OCCLUSION_SET | wireframe);
	_phageScene->FinalizeOcclusion();
	//_phageScene->Render(Phage::Scene::RENDER_PASS_NORMAL, Phage::Scene::UNIT_VECTOR, Phage::Scene::RENDER_FLAG_OCCLUSION_TEST | wireframe);

	//_phageScene->Render(Phage::Scene::RENDER_PASS_NORMAL, Phage::Scene::LINE3D_VECTOR, 0x00);
	_phageScene->PostProcess();
	//_phageScene->Render(Phage::Scene::RENDER_PASS_NORMAL, Phage::Scene::UNIT_VECTOR | Phage::Scene::BUILDING_VECTOR | Phage::Scene::OBJECT_VECTOR | Phage::Scene::TERRAIN);
	_phageScene->Render(Phage::Scene::RENDER_PASS_ORTHO, Phage::Scene::UI_VECTOR);
	_phageScene->FinalizeRender();

	ClearRenderVectors();

	//_losMap->Clear();

	return 0;
}

glm::vec4 Renderer::Calculate2DPoint(float x, float y, float z)
{
	return _phageCamera->Calculate2DPoint(x, y, z);
}

int Renderer::SetGameSpeed(int speed)
{
	_gameSpeed = speed;

	return 0;
}

int Renderer::GetGameSpeed()
{
	return _gameSpeed;
}

int Renderer::AddGUIText(RS::GUIText guiText)
{
	_rsGUITextVector.push_back(guiText);

	return 0;
}

int Renderer::AddGUIRectangle(RS::GUIRectangle guiRectangle)
{
	_rsGUIRectangleVector.push_back(guiRectangle);

	return 0;
}

int Renderer::AddGUILineRect(RS::GUILineRect guiLineRect)
{
	_rsGUILineRectVector.push_back(guiLineRect);

	return 0;
}

int Renderer::AddGUITexture(RS::GUITexture guiTexture)
{
	_rsGUITextureVector.push_back(guiTexture);

	return 0;
}

int Renderer::AddGUIToolTip(RS::GUIToolTip guiToolTip)
{
	_rsGUIToolTipVector.push_back(guiToolTip);

	return 0;
}

int Renderer::AddEntity(RS::Entity entity)
{
	_rsEntityVector.push_back(entity);

	return 0;
}

int Renderer::AddPhageEntity(RS::PhageEntity entity)
{
	//entity.GetModel()->SetTranslate(entity.GetPosition().x, entity.GetPosition().y, entity.GetPosition().z);
	//_phageScene->AddEntity(entity.GetModel());
	_rsPhageEntityVector.push_back(entity);

	return 0;
}

/*int Renderer::AddPhageEntityToScene(Phage::PhageEntity* entity)
{
	_phageScene->AddEntity(entity);

	return 0;
}*/

int Renderer::AddPhageEntityNonRS(Phage::PhageEntity* ent)
{
	_phageEntityVector.push_back(ent);

	return 0;
}

int Renderer::AddModel(Model* model)
{
	_modelVector.push_back(model);

	return 0;
}

int Renderer::AddTerrain(Terrain* terrain)
{
	_terrain = terrain;

	return 0;
}

int Renderer::AddPhageTerrain(Phage::PhageTerrain* terrain)
{
	_phageTerrain = terrain;

	return 0;
}

int Renderer::AddTerrainDecal(RS::TerrainDecal terrainDecal)
{
	_rsTerrainDecalVector.push_back(terrainDecal);

	return 0;
}

int Renderer::AddPhageTerrainDecal(RS::PhageTerrainDecal terrainDecal)
{
	_rsPhageTerrainDecalVector.push_back(terrainDecal);

	return 0;
}

int Renderer::AddProp(RS::Prop prop)
{
	_rsPropVector.push_back(prop);

	return 0;
}

int Renderer::AddLOS(LOS* los)
{
	_los = los;

	return 0;
}

int Renderer::AddPhageUIEntity(Phage::PhageUIEntity* uiEntity)
{
	_phageUIEntityVector.push_back(uiEntity);

	return 0;
}

int Renderer::AddPhageUITextIcon(char c, unsigned int texture)
{
	_phageScene->AddUITextIcon(c, texture);

	return 0;
}

int Renderer::DebugConsole(std::string text)
{
	_debugConsoleTextVector.insert(_debugConsoleTextVector.begin(), DebugConsoleText());
	_debugConsoleTextVector.front().str = text;
	_debugConsoleTextVector.front().timer = 0;

	if (_debugConsoleTextVector.size() > 20) _debugConsoleTextVector.erase(_debugConsoleTextVector.begin()+20);

	return 0;
}

int Renderer::AddLineDrawInfo(glm::vec3 pt1, glm::vec3 pt2)
{
	_lineDrawVector.push_back(LineDraw());
	_lineDrawVector.back().pt1 = pt1;
	_lineDrawVector.back().pt2 = pt2;

	return 0;
}

int Renderer::SetGUIToolTipBackground(RS::GUIToolTipBackground background)
{
	(*_guiToolTipBackground) = background;

	return 0;
}

int Renderer::SetBumpMapping(bool bump)
{
	_bumpMapping = bump;

	return 0;
}

int Renderer::ToggleBumpMapping()
{
	_bumpMapping = !_bumpMapping;

	return 0;
}

int Renderer::SetBackgroundMusic(Phage::PhageSound* sound)
{
	_phageMixer->SetBackgroundMusic(sound);

	return 0;
}

int Renderer::PlaySound(Phage::PhageSound* sound)
{
	_phageMixer->PlaySound(sound);

	return 0;
}

int Renderer::ClearRenderVectors()
{
	_rsEntityVector.clear();
	_rsPhageEntityVector.clear();
	_modelVector.clear();
	_rsGUITextVector.clear();
	_rsGUIRectangleVector.clear();
	_rsGUILineRectVector.clear();
	_rsGUITextureVector.clear();
	_rsGUIToolTipVector.clear();
	_rsTerrainDecalVector.clear();
	_rsPhageTerrainDecalVector.clear();
	_rsPropVector.clear();
	_phageEntityVector.clear();
	_terrain = NULL;
	_phageTerrain = NULL;
	_los = NULL;

	_phageUIEntityVector.clear();

	_lineDrawVector.clear();

	return 0;
}

int Renderer::SetDebugFlag(int flag, bool value)
{
	_debugFlagVector[flag] = value;

	return 0;
}

int Renderer::ToggleDebugFlag(int flag)
{
	_debugFlagVector[flag] = !_debugFlagVector[flag];

	return 0;
}

bool Renderer::GetDebugFlag(int flag)
{
	return _debugFlagVector[flag];
}
