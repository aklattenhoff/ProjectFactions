#include "EntityRenderer.h"

#include "loadGLE.h"
#include "Debug/Debug.h"

#include "Game/EntitySystem/EntityBase.h"
#include "Game/Input/Input.h"

EntityRenderer::EntityRenderer()
{
	_healthIndicator = NULL;
	glow = false;
	bump = false;
	spec = false;
}

EntityRenderer::~EntityRenderer()
{
	delete _healthIndicator;
}

int EntityRenderer::Init()
{
	_healthIndicator = new HealthIndicator();

	return 0;
}

int EntityRenderer::RenderEntities(World* world, Camera* camera, EntityList* entityList)
{
	/*if (world->GetInput()->GetKeyboardInput(INPUT_KEYBOARD_PRESSED, 'g'))
			glow = !glow;
	if (world->GetInput()->GetKeyboardInput(INPUT_KEYBOARD_PRESSED, 'h'))
			bump = !bump;
	if (world->GetInput()->GetKeyboardInput(INPUT_KEYBOARD_PRESSED, 'n'))
			spec = !spec;

	if (bump)
		camera->GetShader()->SetBump(1);

	if (spec)
		camera->GetShader()->SetSpec(1);

	Entity* entity;

	camera->GetShader()->SetLighting(1);

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

		if (glow)
			camera->GetShader()->SetBloom(1);
		camera->GetShader()->UseProgram(SHADER_DEFAULT);
		camera->GetMVPMatrix()->Push(MODEL);
		camera->GetMVPMatrix()->translate(MODEL, entity->PosX(), entity->PosY(), entity->PosZ());
		camera->GetMVPMatrix()->rotate(MODEL, entity->RotY(), 0.0f, 1.0f, 0.0f);
		//camera->GetMVPMatrix()->rotate(MODEL, -90.0f, 1.0f, 0.0f, 0.0f);
		camera->UpdateMatrix(SHADER_DEFAULT);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, 0.0f);
		if (entity->IsMouseOver())
			camera->GetShader()->SetColor(1.5f, 1.5f, 1.5f, 1.0f);
		entity->GetEntityBase()->Model()->Draw(world, camera, 0, entity->GetAnimationSeconds());
		if (entity->IsMouseOver())
			camera->GetShader()->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
		glDisable(GL_ALPHA_TEST);
		glDisable(GL_BLEND);
		camera->GetMVPMatrix()->Pop(MODEL);
		if (entity->IsMouseOver() || entity->IsSelected())
			RenderHealthIndicator(camera, entity);
		camera->GetShader()->SetBloom(0);
	}

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
	
	//Iterate through all object instances and draw all unit models
	//These will be the occluded objects
	for (int i = 0;i < entityList->Size();i ++)
	{
		entity = entityList->Get(i);

		//Don't render unless it is a unit
		//We have already rendered all non-unit objects
		if (entity->GetEntityBase()->EntityTypeInt() != ENTITY_TYPE_UNIT)
			continue;

		if (glow)
			camera->GetShader()->SetBloom(1);
		camera->GetShader()->UseProgram(SHADER_DEFAULT);
		camera->GetMVPMatrix()->Push(MODEL);
		camera->GetMVPMatrix()->translate(MODEL, entity->PosX(), entity->PosY(), entity->PosZ());
		camera->GetMVPMatrix()->rotate(MODEL, entity->RotY(), 0.0f, 1.0f, 0.0f);
		//camera->GetMVPMatrix()->rotate(MODEL, -90.0f, 1.0f, 0.0f, 0.0f);
		camera->UpdateMatrix(SHADER_DEFAULT);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, 0.0f);
		if (entity->IsMouseOver())
			camera->GetShader()->SetColor(1.5f, 1.5f, 1.5f, 1.0f);
		entity->GetEntityBase()->Model()->Draw(world, camera, 0, entity->GetAnimationSeconds());
		if (entity->IsMouseOver())
			camera->GetShader()->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
		glDisable(GL_ALPHA_TEST);
		glDisable(GL_BLEND);
		camera->GetMVPMatrix()->Pop(MODEL);
		if (entity->IsMouseOver() || entity->IsSelected())
			RenderHealthIndicator(camera, entity);
		camera->GetShader()->SetBloom(0);
	}

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

	return 0;
}

int EntityRenderer::RenderHealthIndicator(Camera* camera, Entity* entity)
{
	float distance = sqrt(pow(camera->getCameraPosition().x-entity->PosX(), 2) + pow(camera->getCameraPosition().z-entity->PosZ(), 2));

	_healthIndicator->Draw(camera, entity->PosX(), entity->PosY(), entity->PosZ(), distance);

	return 0;
}
