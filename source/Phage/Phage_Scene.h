#ifndef _PHAGE_SCENE_H
#define _PHAGE_SCENE_H

#include <stdlib.h>
#include <vector>
#include <string>
#include <algorithm>
#include <random>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "Phage_Math.h"
#include "Phage_Const.h"

struct SDL_Window;
typedef void* SDL_GLContext;

namespace Phage
{
	class Window;
	class MVP;
	class PhageShader;
	class PhageCamera;
	class PhageEntity;
	class PhageTerrain;
	class PhageTerrainDecal;
	class PhageUIEntity;
	class PhageUITextureQuad;
	class PhageUIText;
	class PhageUILineRect;
	class PhageFrameBuffer;
	class PhageBoundingBox;
	class PhageTerrainTileDebug;
	class PhageLine;
	class PhageQuad;
	class Scene
	{
	public:
		static const int UNIT_VECTOR = 0x01;
		static const int BUILDING_VECTOR = 0x02;
		static const int OBJECT_VECTOR = 0x04;
		static const int TERRAIN = 0x08;
		static const int UI_VECTOR = 0x10;
		static const int LINE3D_VECTOR = 0x20;

		static const int NUM_RENDER_VECTORS = 6;

		static const int RENDER_PASS_NORMAL = 0;
		static const int RENDER_PASS_ORTHO = 1;
		static const int RENDER_PASS_SHADOW = 2;

		static const int RENDER_FLAG_WIREFRAME = 0x01;
		static const int RENDER_FLAG_OCCLUSION_SET = 0x02;
		static const int RENDER_FLAG_OCCLUSION_TEST = 0x04;
	public:
		Scene();
		~Scene();

		int Init();
	private:
		int InitGraphics();
		int LoadExtensions();
	public:

		//Window functions
		int AttachToWindow(SDL_Window* window);
		int AttachToWindow(Phage::Window* window);
		int SetResolution(int w, int h);
		Phage::vec2 GetResolution();
		int SetShadowMapBounds(glm::vec3 nearPlane[4], glm::vec3 farPlane[4]);

		Phage::PhageShader* CreateShader();
		Phage::PhageShader* GetShader();

		Phage::PhageCamera* CreateCamera();
		Phage::PhageCamera* GetCamera();

		//int AddEntity(Phage::PhageEntity* entity);
		int AddEntityToVector(Phage::PhageEntity* entity, int vector);
		int AddBoundingBoxInfo(glm::vec3 pos, glm::vec3 center, glm::vec3 size, float rotation, glm::vec4 bbRot);
		int AddTerrain(Phage::PhageTerrain* terrain);
		int AddTerrainTileDebug(glm::vec2 pos);
		int AddLOSInfo(int** losInfo, unsigned int losTexture);
		int AddTerrainDecal(Phage::PhageTerrainDecal* terrainDecal);
		int AddUIEntityToVector(Phage::PhageUIEntity* uiEntity);
		int AddUITextInfo(glm::vec2 pos, std::string str, int size = 12);
		int AddUITextIcon(char c, unsigned int texture);
		int AddUITextureInfo(glm::vec4 points, unsigned int textureID, float depth = 0.0f);
		int AddUILineRectCoords(glm::vec4 points);
		int AddLineDrawInfo(glm::vec3 pt1, glm::vec3 pt2);
		//int Render();
		int Render(int renderPass, int renderVectorFlag, int renderFlag = 0x00);
		int FinalizeOcclusion();
		int PostProcess();
		int GenerateSeeThrough();
		int FinalizeRender();
	private:
		int RenderScene(int renderPass, int renderVectorFlag, int renderFlag);
	private:
		//Init flags
		bool _initGraphicsFlag;
		bool _loadExtensionsFlag;

		SDL_Window* _window;
		MVP* _mvp;
		Phage::vec2 _resolution;
		Phage::PhageShader* _shader;
		Phage::PhageCamera* _camera;
		SDL_GLContext _glContext;

		//std::vector<Phage::PhageEntity*> _entityVector;
		std::vector<Phage::PhageEntity*> _entityUnitVector;
		std::vector<Phage::PhageEntity*> _entityBuildingVector;
		std::vector<Phage::PhageEntity*> _entityObjectVector;
		struct BoundingBoxInfo {glm::vec3 pos; glm::vec3 center; glm::vec3 size; float rotation; glm::vec4 bbRot;};
		std::vector<BoundingBoxInfo> _boundingBoxInfoInfoVector;
		std::vector<Phage::PhageTerrain*> _terrainVector;
		struct TerrainTileDebug {glm::vec2 pos;};
		std::vector<TerrainTileDebug> _terrainTileDebugVector;
		int** _losInfo;
		unsigned int _losTexture;
		std::vector<Phage::PhageTerrainDecal*> _terrainDecalVector;
		std::vector<Phage::PhageUIEntity*> _uiEntityVector;
		struct TextInfo {glm::vec2 pos; std::string str; int size;};
		std::vector<TextInfo> _uiTextInfoVector;
		struct TextIcon {char c; unsigned int tex;};
		std::vector<TextIcon> _uiTextIconVector;
		struct TextureInfo
		{
			glm::vec4 points; float depth; unsigned int textureID;
			//Overwrite less than operator so we can sort by depth
			bool operator < (const TextureInfo &ti) const
			{
				return (depth < ti.depth);
			}
		};
		std::vector<TextureInfo> _uiTextureInfoVector;
		std::vector<glm::vec4> _uiLineRectCoordsVector;
		struct LineDraw {glm::vec3 pt1; glm::vec3 pt2;};
		std::vector<LineDraw> _lineDrawVector;

		int _matrixLocation;
		int _modelMatrixLocation;
		int _cameraPositionLocaton;
		int _fbSizeLocation;
		int _fbShadowSizeLocation;
		int _animationFlagLocation;
		int _boneMatrixLocation;
		int _passValueLocation;
		int _occlusionValueLocation;
		int _shadowFlagLocation;
		int _shadowMatrixLocation;
		int _shadowBiasLocation;
		int _postProcessValueLocation;
		int _shadowDepthLocation;
		glm::mat4 mvp;
		float _modelRotation;

		PhageUIText* _phageUITextTest;
		std::vector<Phage::PhageUIText*> _phageUITextSizeVector;
		PhageUILineRect* _phageUILineRectTest;
		Phage::PhageUIEntity* _phageUIEntityTexture;
		Phage::PhageUITextureQuad* _phageTextureQuad;
		Phage::PhageBoundingBox* _phageBoundingBox;
		Phage::PhageTerrainTileDebug* _phageTerrainTileDebug;
		Phage::PhageLine* _phageLine;

		//Framebuffer
		Phage::PhageFrameBuffer* _mainFrameBuffer;
		Phage::PhageUIEntity* _mainFBUIEntityTexture;
		Phage::PhageFrameBuffer* _silhouetteFrameBuffer;
		glm::vec2 _silhouetteResolution;
		Phage::PhageFrameBuffer* _shadowFrameBuffer;
		glm::vec2 _shadowResolution;
		Phage::PhageFrameBuffer* _ssaoFrameBuffer;
		glm::vec2 _ssaoResolution;
		Phage::PhageFrameBuffer* _postProcessFrameBufferFront;
		Phage::PhageFrameBuffer* _postProcessFrameBufferBack;
		Phage::PhageFrameBuffer* _seeThroughFrameBuffer;
		glm::vec2 _seeThroughResolution;
		unsigned int _seeThroughImage;
		Phage::PhageQuad* _seeThroughQuad;

		int _postProcLoc;
		int _entityNumberLoc;
	};
}

#endif
