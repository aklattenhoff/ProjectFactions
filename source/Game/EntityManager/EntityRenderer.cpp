#include "EntityRenderer.h"

#include "loadGLE.h"
#include "Debug/Debug.h"

#include "Game/Entity/EntityBase.h"
#include "Game/Entity/EntityBaseList.h"
#include "Game/Input/Input.h"
#include "Game/Player/Player.h"
#include "Game/Player/PlayerList.h"

//#include "Graphics/Model/Model.h"
//#include "Graphics/Model/Animation.h"
//#include "Graphics/Model/Skeleton.h"

#include "Renderer/Renderer.h"
#include "Renderer/RS/Entity.h"
#include "Renderer/RS/PhageEntity.h"
#include "Renderer/RS/Prop.h"
#include "Renderer/RS/GUITexture.h"

#include "Texturer/TextureLoader.h"

#include "Phage/Phage_Prop.h"
#include "Phage/Phage_PropDesc.h"

int DepthBuffSize = 128;

EntityRenderer::EntityRenderer()
{
	//_phageEntityRenderer = NULL;
	glow = false;
	bump = false;
	spec = false;
}

EntityRenderer::~EntityRenderer()
{
}

int EntityRenderer::Init()
{
	//tempModelCreator = new TempEntityModelXMLCreator();
	//tempModelCreator->Init(true);
	//swordModel = tempModelCreator->CreateFromFile("data/factions/kreytons/models/kreytons_props_swordsman_sword.xml");

	//Framebuffer
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	glGenTextures(1, &fbDepth);
	glBindTexture(GL_TEXTURE_2D, fbDepth);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, DepthBuffSize, DepthBuffSize, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, fbDepth, 0);

	printf("ASDASDASDASDFRAMEBUFFER STATUS: %i\n", (int)glCheckFramebufferStatus(GL_FRAMEBUFFER));

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glActiveTexture(GL_TEXTURE0+15);
	glBindTexture(GL_TEXTURE_2D, fbDepth);
	glActiveTexture(GL_TEXTURE0+0);

	guiScreen = new GUIIcon();
	guiScreen->Init();
	guiScreen->Generate(glm::vec2(DepthBuffSize, DepthBuffSize));

	_textureSelectionCircle = TextureLoader_LoadFromFile("textures/selection32x32.png");
	_rsSelectionCircle.SetTexture(_textureSelectionCircle);

	_selectionSquare64x64 = TextureLoader_LoadFromFile("textures/selection_square_64x64.png", TL_LINEAR);

	_farmDecalTexture = TextureLoader_LoadFromFile("models/market_decal.png", TL_MIPMAP);
	_farmBumpDecalTexture = TextureLoader_LoadFromFile("models/market_decal_bump.png", TL_LINEAR);

	_manorDecalTexture = TextureLoader_LoadFromFile("models/manor_decal.png", TL_MIPMAP);


	SDL_Surface *surface = NULL;
	surface = IMG_Load("textures/ui/build_progress_bar.png");
	if (!surface) return -1;

	GLenum textureFormat;
	if (surface->format->BytesPerPixel == 4)
		textureFormat = (surface->format->Rmask == 0x000000ff) ? GL_RGBA : GL_BGRA;
	else if (surface->format->BytesPerPixel == 3)
		textureFormat = (surface->format->Rmask == 0x000000ff) ? GL_RGB : GL_BGR;

	glGenTextures(1, &_buildProgressTextureID);
	glBindTexture(GL_TEXTURE_2D, _buildProgressTextureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, textureFormat, GL_UNSIGNED_BYTE, surface->pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (surface)
		SDL_FreeSurface(surface);

	surface = IMG_Load("textures/ui/build_progress_bar_back.png");
	if (!surface) return -1;

	if (surface->format->BytesPerPixel == 4)
		textureFormat = (surface->format->Rmask == 0x000000ff) ? GL_RGBA : GL_BGRA;
	else if (surface->format->BytesPerPixel == 3)
		textureFormat = (surface->format->Rmask == 0x000000ff) ? GL_RGB : GL_BGR;

	glGenTextures(1, &_buildProgressBackTextureID);
	glBindTexture(GL_TEXTURE_2D, _buildProgressBackTextureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, textureFormat, GL_UNSIGNED_BYTE, surface->pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (surface)
		SDL_FreeSurface(surface);

	/*_phageEntityRenderer = new Phage::PhageEntity();
	_phageEntityRenderer->Init();
	if (_phageEntityRenderer->LoadCollada("models/barracks.dae") == -1)
		return 0;
	if (_phageEntityRenderer->LoadMainTexture("models/barracks.png") == -1)
		return 0;
	if (_phageEntityRenderer->LoadBumpTexture("models/barracks_bump.png") == -1)
		return 0;
	if (_phageEntityRenderer->LoadSBCTexture("models/barracks_sbc.png") == -1)
		return 0;
	_phageEntityRenderer->Rotate(-90.0f, 1.0f, 0.0f, 0.0f);
	_phageEntityRenderer->Scale(1.0f, 1.0f, 1.0f);*/


	return 0;
}

int EntityRenderer::PhageModelCreate()
{
	//if (!_phageEntityRenderer)
	{
		/*_phageEntityRenderer = new Phage::PhageEntity();
		_phageEntityRenderer->Init();
		if (_phageEntityRenderer->LoadCollada("models/barracks.dae") == -1)
			return 0;
		if (_phageEntityRenderer->LoadMainTexture("models/barracks.png") == -1)
			return 0;
		if (_phageEntityRenderer->LoadBumpTexture("models/barracks_bump.png") == -1)
			return 0;
		if (_phageEntityRenderer->LoadSBCTexture("models/barracks_sbc.png") == -1)
			return 0;
		_phageEntityRenderer->Rotate(-90.0f, 1.0f, 0.0f, 0.0f);
		_phageEntityRenderer->Scale(1.0f, 1.0f, 1.0f);*/
	}

	return 0;
}

int EntityRenderer::RenderEntities(World* world, EntityList* entityList)
{
	/*if (!_phageEntityRenderer)
	{
		_phageEntityRenderer = new Phage::PhageEntity();
		_phageEntityRenderer->Init();
		if (_phageEntityRenderer->LoadCollada("models/barracks.dae") == -1)
			return 0;
		if (_phageEntityRenderer->LoadMainTexture("models/barracks.png") == -1)
			return 0;
		if (_phageEntityRenderer->LoadBumpTexture("models/barracks_bump.png") == -1)
			return 0;
		if (_phageEntityRenderer->LoadSBCTexture("models/barracks_sbc.png") == -1)
			return 0;
		_phageEntityRenderer->Rotate(-90.0f, 1.0f, 0.0f, 0.0f);
		_phageEntityRenderer->Scale(1.0f, 1.0f, 1.0f);
	}*/

	//RendererSingleton->AddPhageEntityToScene(_phageEntityRenderer);


	if (world->GetInput()->GetKeyboardInput(INPUT_KEYBOARD_PRESSED, 'g'))
			glow = !glow;
	if (world->GetInput()->GetKeyboardInput(INPUT_KEYBOARD_PRESSED, 'h'))
			bump = !bump;
	if (world->GetInput()->GetKeyboardInput(INPUT_KEYBOARD_PRESSED, 'n'))
			spec = !spec;

	//if (bump)
		//RendererSingleton->GetCamera()->GetShader()->SetBump(1);

	//if (spec)
		//RendererSingleton->GetCamera()->GetShader()->SetSpec(1);

	Entity* entity;

	//RendererSingleton->GetCamera()->GetShader()->SetLighting(1);

	//RendererSingleton->GetCamera()->GetShader()->SetPlayerColorFlag(1);

	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_ALWAYS, 254, 0xFF);
	glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);
	//Iterate through all entities and draw all non-unit models
	//These will be the occluding entities
	for (int i = 0;i < entityList->Size();i ++)
	{
		entity = entityList->Get(i);

		//Don't render if the entity is a unit
		//We'll render the units later because they can be occluded
		if (entity->GetEntityBase()->EntityTypeInt() == ENTITY_TYPE_UNIT)
			continue;

		/*if (glow)
			camera->GetShader()->SetBloom(1);
		if (entity->GetEntityBase()->ContainsClass("ReplaceTerrain"))
			camera->GetShader()->SetTerrainReplaceFlag(1);
		camera->GetShader()->UseProgram(SHADER_DEFAULT);
		camera->GetMVPMatrix()->Push(MODEL);
		camera->GetMVPMatrix()->translate(MODEL, entity->PosX(), entity->PosY(), entity->PosZ());
		camera->GetMVPMatrix()->rotate(MODEL, entity->RotY(), 0.0f, 1.0f, 0.0f);
		//camera->GetMVPMatrix()->rotate(MODEL, -90.0f, 1.0f, 0.0f, 0.0f);
		if (entity->GetModel()->GetOrientation() == Model::ORIENTATION_Z)
			camera->GetMVPMatrix()->rotate(MODEL, -90.0f, 1.0f, 0.0f, 0.0f);
		float scale = entity->GetModel()->GetScale();
		if (scale != 1.0f)
			camera->GetMVPMatrix()->scale(MODEL, scale, scale, scale);
		camera->UpdateMatrix(SHADER_DEFAULT);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, 0.0f);
		Player::Color color = world->GetPlayerList()->GetAt(entity->GetOwningPlayer())->GetColor();
		camera->GetShader()->SetPlayerColor(color.red, color.green, color.blue);
		if (entity->IsMouseOver())
			camera->GetShader()->SetColor(1.5f, 1.5f, 1.5f, 1.0f);*/
		//entity->GetEntityBase()->GetObjectModel()->Draw(world, camera, 0, entity->GetAnimationSeconds());
		if (entity->GetModel())
		{
			//entity->GetModel()->Draw(camera, entity->GetAnimationIndex(), entity->GetAnimationSeconds());
			RS::Entity rsEntity;
			rsEntity.SetModel(entity->GetModel(), entity->GetTranslationVector(), entity->RotY());
			rsEntity.SetAnimation(entity->GetAnimationIndex(), entity->GetAnimationSeconds());
			rsEntity.SetPlayer(world->GetPlayerList()->GetAt(entity->GetOwningPlayer()));
			rsEntity.SetEntityBase(entity->GetEntityBase());
			RendererSingleton->AddEntity(rsEntity);

			RS::PhageEntity rsPhageEntity;
			rsPhageEntity.SetModel(entity->GetPhageEntity(), entity->GetTranslationVector(), entity->RotY());
			rsPhageEntity.SetAnimation(entity->GetAnimationIndex(), entity->GetAnimationSeconds());
			rsPhageEntity.SetPlayer(world->GetPlayerList()->GetAt(entity->GetOwningPlayer()));
			rsPhageEntity.SetEntityBase(entity->GetEntityBase());
			//RendererSingleton->AddPhageEntityToScene(entity->GetEntityBase()->GetPhageEntity());
			RendererSingleton->AddPhageEntity(rsPhageEntity);

			if (!entity->IsBuilt())
			{
				glm::vec4 buildPoint = RendererSingleton->Calculate2DPoint(entity->PosX(), entity->PosY()+1.0f, entity->PosZ());
				int buildWidth = RendererSingleton->GetScreenHeight() / (1080 / 100);
				buildWidth *= (int)(28.0f / (RendererSingleton->GetCameraPosition().y - entity->PosY()));
				int buildHeight = RendererSingleton->GetScreenHeight() / (1080 / 10);
				buildHeight *= (int)(28.0f / (RendererSingleton->GetCameraPosition().y - entity->PosY()));

				RS::GUITexture rsBuildProgressBack;
				rsBuildProgressBack.SetTexture(_buildProgressBackTextureID);
				rsBuildProgressBack.SetPoints((int)buildPoint.x - buildWidth/2, (int)buildPoint.y, (int)buildPoint.x + buildWidth/2, (int)buildPoint.y + buildHeight);
				RendererSingleton->AddGUITexture(rsBuildProgressBack);

				RS::GUITexture rsBuildProgress;
				rsBuildProgress.SetTexture(_buildProgressTextureID);
				rsBuildProgress.SetPoints((int)buildPoint.x - buildWidth/2, (int)buildPoint.y, (int)buildPoint.x - buildWidth/2 + (int)(entity->GetCurrentBuildPoints() / entity->GetTotalBuildPoints()) * buildWidth, (int)buildPoint.y + buildHeight);
				RendererSingleton->AddGUITexture(rsBuildProgress);
			}

			if (entity->GetEntityBase()->EntityName().compare("kreytons_market") == 0)
			{
				RS::TerrainDecal farmDecal;
				farmDecal.SetOrigin(entity->PosX(), entity->PosZ());
				farmDecal.SetPoints(0.36736f, 0.1631f, 5.675f, 5.898f);
				farmDecal.SetTexture(_farmDecalTexture);
				farmDecal.SetRotation(entity->RotY());
				farmDecal.SetBumpTexture(_farmBumpDecalTexture);
				RendererSingleton->AddTerrainDecal(farmDecal);
			}
			else if (entity->GetEntityBase()->EntityName().compare("kreytons_manor") == 0)
			{
				RS::TerrainDecal manorDecal;
				manorDecal.SetOrigin(entity->PosX(), entity->PosZ());
				manorDecal.SetPoints(0.0f, -0.2221f, 5.0886f*1.1f, 4.4443f*1.1f);
				manorDecal.SetTexture(_manorDecalTexture);
				manorDecal.SetRotation(entity->RotY());
				manorDecal.SetBumpTexture(_farmBumpDecalTexture);
				RendererSingleton->AddTerrainDecal(manorDecal);
			}

			RS::TerrainDecal terrDecal;
			terrDecal.SetOrigin(entity->PosX(), entity->PosZ());
			terrDecal.SetPoints(0.0f, 0.0f, 1.0f, 1.0f);
			terrDecal.SetRotation(entity->RotY());
			terrDecal.SetTexture(_textureSelectionCircle);
			if (entity->IsMouseOver() || entity->IsSelected())
			{
				BoundingBox2DSet* bb2D = entity->GetEntityBase()->BoundingBox2D();
				if (bb2D->Size() > 0)
				{
					if (bb2D->Get(0).width > 2.0f)
						terrDecal.SetTexture(_selectionSquare64x64);
					terrDecal.SetPoints(bb2D->Get(0).midpoint.x, bb2D->Get(0).midpoint.z, (bb2D->Get(0).point2.x-bb2D->Get(0).point1.x)*1.185f, bb2D->Get(0).width*1.185f);
				}
				RendererSingleton->AddTerrainDecal(terrDecal);
			}
		}
		/*if (entity->IsMouseOver())
			camera->GetShader()->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
		glDisable(GL_ALPHA_TEST);
		glDisable(GL_BLEND);
		camera->GetMVPMatrix()->Pop(MODEL);
		if (entity->IsMouseOver() || entity->IsSelected())
			RenderHealthIndicator(camera, entity);
		camera->GetShader()->SetBloom(0);
		camera->GetShader()->SetTerrainReplaceFlag(0);*/
	}

	//Copy depth buffer to frame buffer
	//glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
	//glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FBO);
	//glBlitFramebuffer(0, 0, (GLint)RendererSingleton->GetCamera()->GetScreenResolution().x, (GLint)RendererSingleton->GetCamera()->GetScreenResolution().y, 0, 0, DepthBuffSize, DepthBuffSize, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	/*world->GetCamera()->GetShader()->SetLighting(0);
	world->GetCamera()->GetMVPMatrix()->SetProjectionMatrixType(ORTHO);
	world->GetCamera()->GetShader()->UseProgram(SHADER_DEFAULT);
	world->GetCamera()->UpdateMatrix(SHADER_DEFAULT);
	guiScreen->DrawAtLocation(world->GetCamera(), 0, 0, fbDepth);
	world->GetCamera()->GetMVPMatrix()->SetProjectionMatrixType(PROJECTION);*/

	//Runs the selection calculation process
	//NOTE: This is a bit of a misnomer because this calculates the mouse over objects
	//The selected objects are populated from the mouse over objects when the user releases the mouse button during the SetSelectionPoints() function
	//objectSelector->RunSelector(camera, objectInstanceArray);

	//TODO: Find a better way to do one-pass unit occlude testing
	//Currently this works just fine but will designate a fragment as an occluded unit if 255 fragments are rendered in near to far order
	//in a row. This is extremely unlikely and maybe if there are that many fragements drawn in that specific order, it might not be a
	//bad thing to designate them as occluded?
	glEnable(GL_STENCIL_TEST);
	glStencilMask(0xFF);
	glStencilFunc(GL_ALWAYS, 0, 0xFF);
	glStencilOp(GL_KEEP, GL_INCR, GL_ZERO);

	glLineWidth(1.0f);

	//RendererSingleton->GetCamera()->GetShader()->SetOccludeFlag(1);
	
	//Iterate through all object instances and draw all unit models
	//These will be the occluded objects
	for (int i = 0;i < entityList->Size();i ++)
	{
		entity = entityList->Get(i);

		//Don't render unless it is a unit
		//We have already rendered all non-unit objects
		if (entity->GetEntityBase()->EntityTypeInt() != ENTITY_TYPE_UNIT)
			continue;

		/*if (glow)
			camera->GetShader()->SetBloom(1);
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
		//camera->GetMVPMatrix()->scale(MODEL, 0.02027f, 0.02027f, 0.02027f);
		//camera->GetMVPMatrix()->rotate(MODEL, -90.0f, 1.0f, 0.0f, 0.0f);
		camera->UpdateMatrix(SHADER_DEFAULT);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, 0.0f);
		Player::Color color = world->GetPlayerList()->GetAt(entity->GetOwningPlayer())->GetColor();
		camera->GetShader()->SetPlayerColor(color.red, color.green, color.blue);
		if (entity->IsMouseOver())
			camera->GetShader()->SetColor(1.5f, 1.5f, 1.5f, 1.0f);*/
		//entity->GetEntityBase()->GetObjectModel()->Draw(world, camera, 0, entity->GetAnimationSeconds());
		if (entity->GetModel())
		{
			//entity->GetModel()->Draw(camera, entity->GetAnimationIndex(), entity->GetAnimationSeconds());
			RS::Entity rsEntity;
			rsEntity.SetModel(entity->GetModel(), entity->GetTranslationVector(), entity->RotY());
			rsEntity.SetAnimation(entity->GetAnimationIndex(), entity->GetAnimationSeconds());
			rsEntity.SetPlayer(world->GetPlayerList()->GetAt(entity->GetOwningPlayer()));
			rsEntity.SetEntityBase(entity->GetEntityBase());
			RendererSingleton->AddEntity(rsEntity);

			RS::PhageEntity rsPhageEntity;
			rsPhageEntity.SetModel(entity->GetPhageEntity(), entity->GetTranslationVector(), entity->RotY());
			rsPhageEntity.SetAnimation(entity->GetAnimationIndex(), entity->GetAnimationSeconds());
			rsPhageEntity.SetPlayer(world->GetPlayerList()->GetAt(entity->GetOwningPlayer()));
			rsPhageEntity.SetEntityBase(entity->GetEntityBase());
			//RendererSingleton->AddPhageEntityToScene(entity->GetEntityBase()->GetPhageEntity());
			RendererSingleton->AddPhageEntity(rsPhageEntity);

			_rsSelectionCircle.SetOrigin(entity->PosX(), entity->PosZ());
			_rsSelectionCircle.SetPoints(0.0f, 0.0f, 1.0f, 1.0f);
			if (entity->IsMouseOver() || entity->IsSelected())
				RendererSingleton->AddTerrainDecal(_rsSelectionCircle);

			//Add prop to phage entity
			Animation* currentAnim = entity->GetModel()->GetAnimation(entity->GetAnimationIndex());
			if (currentAnim && currentAnim->HasProps())
			{	
				for (int propIter = 0;propIter < currentAnim->NumberOfProps();propIter ++)
				{
					Animation::PropDesc propDesc = currentAnim->GetPropAtIndex(propIter);
					if ((propDesc.animTime.x >= 0.0f && entity->GetAnimationProgress() < propDesc.animTime.x) ||
						(propDesc.animTime.y >= 0.0f && entity->GetAnimationProgress() > propDesc.animTime.y))
						continue;

					Phage::PhageProp* phageProp = world->GetEntityBaseList()->PhagePropAt(propDesc.name);
					Phage::PhagePropDesc phagePropDesc;
					phagePropDesc.Init();
					phagePropDesc.SetPhageProp(phageProp);

					RS::Prop rsProp;
					phagePropDesc.Identity();
					phagePropDesc.Translate(entity->GetTranslationVector().x, entity->GetTranslationVector().y, entity->GetTranslationVector().z);
					phagePropDesc.Rotate(entity->RotY(), 0.0f, 1.0f, 0.0f);

					//Find bone translation
					aiVector3D aiBoneTrans = currentAnim->GetBoneTranslation(entity->GetModel()->GetSkeleton()->GetBoneAt(propDesc.boneAttachment), entity->GetAnimationSeconds());
					glm::vec3 glmBoneTrans;
					glmBoneTrans.x = aiBoneTrans.x; glmBoneTrans.y = aiBoneTrans.y; glmBoneTrans.z = aiBoneTrans.z;
					if (entity->GetModel()->GetOrientation() == Model::ORIENTATION_Z)
						glmBoneTrans = glm::rotate(glmBoneTrans, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
					float scale = entity->GetModel()->GetScale();
					if (scale != 1.0f)
						glmBoneTrans *= scale;
					phagePropDesc.Translate(glmBoneTrans.x, glmBoneTrans.y, glmBoneTrans.z);

					if (phageProp->GetPhageModel()->GetOrientation() == Phage::PhageModel::ORIENTATION_Z)
						phagePropDesc.Rotate(-90.0f, 1.0f, 0.0f, 0.0f);

					//Find bone rotation
					aiQuaternion aiBoneRot = currentAnim->GetBoneRotation(entity->GetModel()->GetSkeleton()->GetBoneAt(propDesc.boneAttachment), entity->GetAnimationSeconds());
					float boneAngle = 0.0f;
					glm::vec3 glmBoneRot;
					if (aiBoneRot.x < -1.01f || aiBoneRot.x > -0.99f)
					{
						boneAngle = glm::degrees(2.0f * glm::acos(aiBoneRot.w));
						glmBoneRot.x = aiBoneRot.x / glm::sqrt(1 - aiBoneRot.w * aiBoneRot.w);
						glmBoneRot.y = aiBoneRot.y / glm::sqrt(1 - aiBoneRot.w * aiBoneRot.w);
						glmBoneRot.z = aiBoneRot.z / glm::sqrt(1 - aiBoneRot.w * aiBoneRot.w);
					}
					phagePropDesc.Rotate(boneAngle, glmBoneRot.x, glmBoneRot.y, glmBoneRot.z);

					phagePropDesc.Translate(propDesc.offsetTranslate.x, propDesc.offsetTranslate.y, propDesc.offsetTranslate.z);
					phagePropDesc.Rotate(propDesc.offsetRotate.x, 1.0f, 0.0f, 0.0f);
					phagePropDesc.Rotate(propDesc.offsetRotate.y, 0.0f, 1.0f, 0.0f);
					phagePropDesc.Rotate(propDesc.offsetRotate.z, 0.0f, 0.0f, 1.0f);

					phagePropDesc.Scale(phageProp->GetPhageModel()->GetScale(), phageProp->GetPhageModel()->GetScale(), phageProp->GetPhageModel()->GetScale());

					entity->GetPhageEntity()->AddPhagePropDesc(phagePropDesc);
				}
			}

			//Add props
			/*Animation* currentAnim = entity->GetModel()->GetAnimation(entity->GetAnimationIndex());
			if (currentAnim && currentAnim->HasProps())
			{	
				for (int propIter = 0;propIter < currentAnim->NumberOfProps();propIter ++)
				{
					Animation::PropDesc propDesc = currentAnim->GetPropAtIndex(propIter);
					if ((propDesc.animTime.x >= 0.0f && entity->GetAnimationProgress() < propDesc.animTime.x) ||
						(propDesc.animTime.y >= 0.0f && entity->GetAnimationProgress() > propDesc.animTime.y))
						continue;

					Model* propModel = world->GetEntityBaseList()->PropAt(propDesc.name);

					//printf("FOUND PROP: %s\n%i\n%i\n", propDesc.name.c_str(),
						//entity->GetModel()->GetSkeleton()->GetBoneMap()->size(),
						//entity->GetModel()->GetSkeleton()->GetBoneAt("Bip001_Neck"));

					RS::Prop rsProp;
					rsProp.SetModel(propModel, entity->GetTranslationVector(), entity->RotY());

					//Find bone translation
					aiVector3D aiBoneTrans = currentAnim->GetBoneTranslation(entity->GetModel()->GetSkeleton()->GetBoneAt(propDesc.boneAttachment), entity->GetAnimationSeconds());
					glm::vec3 glmBoneTrans;
					glmBoneTrans.x = aiBoneTrans.x; glmBoneTrans.y = aiBoneTrans.y; glmBoneTrans.z = aiBoneTrans.z;
					if (entity->GetModel()->GetOrientation() == Model::ORIENTATION_Z)
						glmBoneTrans = glm::rotate(glmBoneTrans, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
					float scale = entity->GetModel()->GetScale();
					if (scale != 1.0f)
						glmBoneTrans *= scale;
					rsProp.SetBoneTranslation(glmBoneTrans.x, glmBoneTrans.y, glmBoneTrans.z);

					//Find bone rotation
					aiQuaternion aiBoneRot = currentAnim->GetBoneRotation(entity->GetModel()->GetSkeleton()->GetBoneAt(propDesc.boneAttachment), entity->GetAnimationSeconds());
					float boneAngle = 0.0f;
					glm::vec3 glmBoneRot;
					if (aiBoneRot.x < -1.01f || aiBoneRot.x > -0.99f)
					{
						boneAngle = glm::degrees(2.0f * glm::acos(aiBoneRot.w));
						glmBoneRot.x = aiBoneRot.x / glm::sqrt(1 - aiBoneRot.w * aiBoneRot.w);
						glmBoneRot.y = aiBoneRot.y / glm::sqrt(1 - aiBoneRot.w * aiBoneRot.w);
						glmBoneRot.z = aiBoneRot.z / glm::sqrt(1 - aiBoneRot.w * aiBoneRot.w);
					}
					rsProp.SetBoneRotation(glmBoneRot, boneAngle);

					//Set the offset
					rsProp.SetOffset(	glm::vec3(propDesc.offsetTranslate.x, propDesc.offsetTranslate.y, propDesc.offsetTranslate.z),
										glm::vec3(propDesc.offsetRotate.x, propDesc.offsetRotate.y, propDesc.offsetRotate.z),
										glm::vec3(propDesc.offsetScale.x, propDesc.offsetScale.y, propDesc.offsetScale.z));

					RendererSingleton->AddProp(rsProp);
				}
			}*/
		}
		/*if (entity->IsMouseOver())
			camera->GetShader()->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
		glDisable(GL_ALPHA_TEST);
		glDisable(GL_BLEND);
		camera->GetMVPMatrix()->Pop(MODEL);
		if (entity->IsMouseOver() || entity->IsSelected())
			RenderHealthIndicator(camera, entity);
		camera->GetShader()->SetBloom(0);*/
	}

	//TEST: REMOVE
	/*for (int i = 0;i < entityList->Size();i ++)
	{
		Entity* ent = entityList->Get(i);
		if (ent->GetEntityBase()->EntityName().compare("kreytons_swordsman") != 0)
			continue;
		Animation* anim = ent->GetEntityBase()->GetModel()->GetAnimation(ent->GetAnimationIndex());
		//TEST: REMOVE!!
		camera->GetShader()->UseProgram(SHADER_DEFAULT);
		camera->GetMVPMatrix()->Push(MODEL);
		camera->GetMVPMatrix()->translate(MODEL, ent->GetTranslationVector().x, ent->GetTranslationVector().y, ent->GetTranslationVector().z);
		camera->GetMVPMatrix()->rotate(MODEL, ent->RotY(), 0.0f, 1.0f, 0.0f);

		aiVector3D vec3D;
		glm::vec3 transVec;
		if (anim)
		{
			vec3D = anim->GetRHandTrans(ent->GetAnimationSeconds());
			if (vec3D.x < -1.01f || vec3D.x > -0.99f)
			{
				transVec.x = vec3D.x;
				transVec.y = vec3D.y;
				transVec.z = vec3D.z;
			}
			else
			{
				transVec = glm::vec3(0.0f);
			}
		}

		if (ent->GetModel()->GetOrientation() == Model::ORIENTATION_Z)
			transVec = glm::rotate(transVec, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		float scale = ent->GetModel()->GetScale();
		if (scale != 1.0f)
			transVec *= scale;

		camera->GetMVPMatrix()->translate(MODEL, transVec.x, transVec.y, transVec.z);
		//DebugOutput("Translating: %f, %f, %f\n", transVec.x, transVec.y, transVec.z);

		if (swordModel->GetOrientation() == Model::ORIENTATION_Z)
				camera->GetMVPMatrix()->rotate(MODEL, -90.0f, 1.0f, 0.0f, 0.0f);

		if (anim)
		{
			aiQuaternion rotQuat = anim->GetRHandRot(ent->GetAnimationSeconds());
			aiQuaternion orientQuat(aiVector3D(1.0f, 0.0, 0.0), glm::radians(-90.0f));
			//rotQuat = orientQuat * rotQuat;
			//vec3D = anim->GetRHandRot(ent->GetAnimationSeconds());
			if (rotQuat.x < -1.01f || rotQuat.x > -0.99f)
			{
				float angle = glm::degrees(2.0f * glm::acos(rotQuat.w));
				glm::vec3 rotAxis;
				rotAxis.x = rotQuat.x / glm::sqrt(1 - rotQuat.w * rotQuat.w);
				rotAxis.y = rotQuat.y / glm::sqrt(1 - rotQuat.w * rotQuat.w);
				rotAxis.z = rotQuat.z / glm::sqrt(1 - rotQuat.w * rotQuat.w);
				camera->GetMVPMatrix()->rotate(MODEL, angle, rotAxis.x, rotAxis.y, rotAxis.z);
				//camera->GetMVPMatrix()->rotate(MODEL, 90.0f, 0.0f, 1.0f, 0.0f);
				//DebugOutput("Rot: %f, %f, %f, %f\n", angle, rotAxis.x, rotAxis.y, rotAxis.z);
			}
		}
		camera->GetMVPMatrix()->translate(MODEL, 0.0f+0.05f, 0.0f, 0.0f);
		{
			float scale = swordModel->GetScale();
			if (scale != 1.0f)
				camera->GetMVPMatrix()->scale(MODEL, scale, scale, scale);
		}
		camera->UpdateMatrix(SHADER_DEFAULT);
		camera->GetShader()->SetColor(1.0f, 1.0f, 1.0f);
		//boundingBoxModel->DrawBoundingBoxModel();
		swordModel->Draw(camera, 0, 0.0f);
		camera->GetMVPMatrix()->Pop(MODEL);
		camera->GetShader()->UseProgram(SHADER_DEFAULT);
	}

	camera->GetShader()->SetOccludeFlag(0);

	camera->GetShader()->SetPlayerColorFlag(0);
	camera->GetShader()->SetBump(0);
	camera->GetShader()->SetSpec(0);

	//Draw silhouette of objects
	//This simply renders a rectangle to the screen with the stencil buffer only rendering pixels where occluded objects where marked
	glStencilFunc(GL_EQUAL, 255, 0xFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	camera->GetShader()->UseProgram(SHADER_NO_TEXTURE);
	camera->GetMVPMatrix()->SetProjectionMatrixType(ORTHO);
	camera->UpdateMatrix(SHADER_NO_TEXTURE);
	//camera->GetShader()->SetColor(0.0f, 0.0f, 1.0f);
	//guiRectangle->Draw();
	camera->GetMVPMatrix()->SetProjectionMatrixType(PROJECTION);
	camera->GetShader()->UseProgram(SHADER_DEFAULT);

	glLineWidth(1.0f);

	glDisable(GL_STENCIL_TEST);*/

	RS::TerrainDecal cursorRS;
	/*cursorRS.SetOrigin(RendererSingleton->GetCamera()->getMouseTerrain().x, RendererSingleton->GetCamera()->getMouseTerrain().z);
	cursorRS.SetPoints(0.0f, 0.0f, 1.0f, 1.0f);
	cursorRS.SetTexture(_textureSelectionCircle);
	RendererSingleton->AddTerrainDecal(cursorRS);*/

	return 0;
}

/*int EntityRenderer::RenderHealthIndicator(Camera* camera, Entity* entity)
{
	float distance = sqrt(pow(camera->getCameraPosition().x-entity->PosX(), 2) + pow(camera->getCameraPosition().z-entity->PosZ(), 2));

	return 0;
}*/
