#ifndef _RENDERER_H
#define _RENDERER_H

#include <vector>
#include <random>

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/ext.hpp>

namespace Phage
{
	class Scene;
	class PhageShader;
	class PhageCamera;
	class PhageModel;
	class PhageEntity;
	class PhageUIEntity;
	class PhageTerrain;
	class PhageSound;
	class PhageMixer;
}

namespace RS
{
	class GUIText;
	class GUIRectangle;
	class GUILineRect;
	class GUITexture;
	class GUIToolTip;
	class GUIToolTipBackground;
	class Entity;
	class PhageEntity;
	class TerrainDecal;
	class PhageTerrainDecal;
	class Prop;
	class RSLOS;
}
class Model;
class Terrain;
class LOSMap;
class LOS;

//class Parser;
//class Camera;
//class ModelRenderer;
//class GUIRenderer;

class Entity;

#define RendererSingleton Renderer::GetSingleton()

class Renderer
{
public:
	static const int DEBUG_FLAG_RENDER_BOUNDING_BOXES = 0;
	static const int DEBUG_FLAG_RENDER_WIREFRAME = 1;
	static const int DEBUG_FLAG_RENDER_TERRAIN_MAP = 2;

	static const int DEBUG_SSAO = 10;
	static const int DEBUG_BLUR_SSAO = 11;
	static const int DEBUG_OUTLINE = 12;
	static const int DEBUG_FXAA = 13;
	static const int DEBUG_SHARPEN = 14;
	static const int DEBUG_SHADOW = 15;

	Entity* HoverOverEntity = NULL;
public:
	Renderer();
	~Renderer();

	static int CreateSingleton();
	static Renderer* GetSingleton();
	static int DeleteSingleton();

	int InitGraphics();
	int CreateScene();
	Phage::PhageCamera* GetPhageCamera();
	int SetWindow(SDL_Window* window);
	int SetResolution(int w, int h);
	int SetCameraPosition(float x, float y, float z);
	int MoveCameraPosition(float x, float y, float z);
	glm::vec3 GetCameraPosition();
	float GetCameraX();
	float GetCameraY();
	float GetCameraZ();
	int SetCameraRotation(float r);
	int MoveCameraPitch(float p);
	int SetCameraPitch(float p);
	float GetCameraPitch();
	int SetMousePosition(float x, float y);
	glm::vec2 GetMouse();
	glm::vec3 GetMouseNear();
	glm::vec3 GetMouseFar();
	glm::vec3 GetMouseTerrain();
	int GetScreenWidth();
	int GetScreenHeight();
	int CalculateScene();
	int Render();

	glm::vec4 Calculate2DPoint(float x, float y, float z);
	int SetGameSpeed(int speed);
	int GetGameSpeed();

	int AddGUIText(RS::GUIText guiText);
	int AddGUIRectangle(RS::GUIRectangle guiRectangle);
	int AddGUILineRect(RS::GUILineRect guiLineRect);
	int AddGUITexture(RS::GUITexture guiTexture);
	int AddGUIToolTip(RS::GUIToolTip guiToolTip);
	int AddEntity(RS::Entity entity);
	int AddPhageEntity(RS::PhageEntity entity);
	//int AddPhageEntityToScene(Phage::PhageEntity* entity);
	int AddPhageEntityNonRS(Phage::PhageEntity* ent);
	int AddModel(Model* model);
	int AddTerrain(Terrain* terrain);
	int AddPhageTerrain(Phage::PhageTerrain* terrain);
	int AddTerrainDecal(RS::TerrainDecal terrainDecal);
	int AddPhageTerrainDecal(RS::PhageTerrainDecal terrainDecal);
	int AddProp(RS::Prop prop);
	int AddLOS(LOS* los);

	int AddPhageUIEntity(Phage::PhageUIEntity* uiEntity);
	int AddPhageUITextIcon(char c, unsigned int texture);
	int DebugConsole(std::string text);
	int AddLineDrawInfo(glm::vec3 pt1, glm::vec3 pt2);

	int SetGUIToolTipBackground(RS::GUIToolTipBackground background);

	int SetBumpMapping(bool bump);
	int ToggleBumpMapping();

	int SetDebugFlag(int flag, bool value);
	int ToggleDebugFlag(int flag);
	bool GetDebugFlag(int flag);

	//Sound
	int SetBackgroundMusic(Phage::PhageSound* sound);
	int PlaySound(Phage::PhageSound* sound);
private:
	int ClearRenderVectors();
private:
	static Renderer* _instance;

	Phage::Scene* _phageScene;
	Phage::PhageShader* _phageShader;
	Phage::PhageCamera* _phageCamera;
	//Phage::PhageEntity* _phageTestEntity;
	//Phage::PhageUIEntity* _phageTestUIEntity;
	Phage::PhageEntity* _phagePointerEntity;
	Phage::PhageEntity* _phageFrustumEntity[8];

	Phage::PhageMixer* _phageMixer;

	//Camera* _camera;
	float _cameraRotation;

	SDL_Window* _window;
	int _screenWidth;
	int _screenHeight;

	int _gameSpeed;

	std::vector<RS::GUIText> _rsGUITextVector;
	std::vector<RS::GUIRectangle> _rsGUIRectangleVector;
	std::vector<RS::GUILineRect> _rsGUILineRectVector;
	std::vector<RS::GUITexture> _rsGUITextureVector;
	std::vector<RS::GUIToolTip> _rsGUIToolTipVector;
	std::vector<RS::Entity> _rsEntityVector;
	std::vector<RS::PhageEntity> _rsPhageEntityVector;
	std::vector<RS::TerrainDecal> _rsTerrainDecalVector;
	std::vector<RS::PhageTerrainDecal> _rsPhageTerrainDecalVector;
	std::vector<RS::Prop> _rsPropVector;
	std::vector<Model*> _modelVector;
	std::vector<Phage::PhageEntity*> _phageEntityVector;
	Terrain* _terrain;
	Phage::PhageTerrain* _phageTerrain;
	LOS* _los;

	std::vector<Phage::PhageUIEntity*> _phageUIEntityVector;
	struct DebugConsoleText {std::string str; int timer;};
	std::vector<DebugConsoleText> _debugConsoleTextVector;
	struct LineDraw {glm::vec3 pt1; glm::vec3 pt2;};
	std::vector<LineDraw> _lineDrawVector;

	RS::GUIToolTipBackground* _guiToolTipBackground;

	glm::vec2 _mousePosition;
	glm::vec3 _mouseTerrainPosition;

	bool _bumpMapping;

	LOSMap* _losMap;

	//Debug flags
	std::vector<bool> _debugFlagVector;
};

#endif
