#include "TempEntityBaseListCreator.h"

#include "loadGLE.h"
#include "Debug/Debug.h"

#include "Game/TempEntityCreator/TempEntityBaseCreator.h"
#include "Game/Entity/EntityBaseList.h"

//Parser
#include "Parser/Parser.h"
#include "Parser/FactionListXML.h"
#include "Parser/FactionXMLData.h"
#include "Parser/EntityListXML.h"
#include "Parser/EntityXML.h"
#include "Parser/ProjectileXML.h"
#include "Parser/ProjectileListXML.h"

#include "Game/ProjectileSystem/ProjectileBase.h"

#include "Phage/Phage_Prop.h"

TempEntityBaseListCreator::TempEntityBaseListCreator()
{
	_entityBaseCreator = NULL;
}

TempEntityBaseListCreator::~TempEntityBaseListCreator()
{
	delete _entityBaseCreator;
}

int TempEntityBaseListCreator::Init()
{
	_entityBaseCreator = new TempEntityBaseCreator();
	_entityBaseCreator->Init();

	return 0;
}

EntityBaseList* TempEntityBaseListCreator::CreateEntityBaseList(Parser* parser)
{
	EntityBaseList* entityBaseList = new EntityBaseList();
	
	std::vector<FactionXMLData*> factionXMLDataVector = parser->GetFactionListXML()->GetFactionXMLDataVector();
	for (unsigned int i = 0;i < factionXMLDataVector.size();i ++)
	{
		std::vector<EntityXML*> entityXMLVector = factionXMLDataVector[i]->GetEntityListXML()->GetEntityXMLVector();
		for (unsigned int j = 0;j < entityXMLVector.size();j ++)
		{
			if (!entityBaseList->At(entityXMLVector[j]->name))
			{
				EntityBase* entityBase = _entityBaseCreator->CreateEntityBase(entityXMLVector[j]);
				entityBaseList->Add(entityBase);
				DebugOutput("Created EntityBase: %s\n", entityXMLVector[j]->name.c_str());
			}
		}

		std::vector<std::string> propFileVector = factionXMLDataVector[i]->GetEntityListXML()->GetPropFileVector();
		for (unsigned int j = 0;j < propFileVector.size();j ++)
		{
			//if (!entityBaseList->PropAt(propFileVector[i]))
			{
				Model* model = _entityBaseCreator->CreateProp(propFileVector[j]);

				//Don't re-add a duplicate prop to the prop list
				if (entityBaseList->PropAt(model->GetName()))
				{
					delete model;
				}
				else
				{
					entityBaseList->AddProp(model, model->GetName());
					DebugOutput("Created Prop: %s\n", model->GetName().c_str());
				}
			}
			Phage::PhageModel* phageModel = _entityBaseCreator->CreatePropPhage(propFileVector[j]);
			if (entityBaseList->PhagePropAt(phageModel->GetName()))
			{
				delete phageModel;
			}
			else
			{
				Phage::PhageProp* phageProp = new Phage::PhageProp();
				phageProp->Init();
				phageProp->SetPhageModel(phageModel);
				entityBaseList->AddPhageProp(phageProp, phageModel->GetName());
				DebugOutput("Created Phage Prop: %s\n", phageModel->GetName().c_str());
			}
		}

		/*std::vector<std::string> projectileFileVector = factionXMLDataVector[i]->GetEntityListXML()->GetProjectileFileVector();
		for (unsigned int j = 0;j < projectileFileVector.size();j ++)
		{
			//if (!entityBaseList->PropAt(propFileVector[i]))
			{
				Model* model = _entityBaseCreator->CreateProjectile(projectileFileVector[j]);

				//Don't re-add a duplicate prop to the prop list
				if (entityBaseList->ProjectileAt(model->GetName()))
				{
					delete model;
				}
				else
				{
					entityBaseList->AddProjectile(model, model->GetName());
					DebugOutput("Created Prop: %s\n", model->GetName().c_str());
				}
			}
		}*/

		std::vector<ProjectileXML*> projectileFileVector = factionXMLDataVector[i]->GetProjectileListXML()->GetProjectileXMLVector();
		for (unsigned int j = 0;j < projectileFileVector.size();j ++)
		{
			//if (!entityBaseList->PropAt(propFileVector[i]))
			{
				ProjectileBase* projectileBase = _entityBaseCreator->CreateProjectileBase(projectileFileVector[j]);

				//Don't re-add a duplicate prop to the prop list
				if (entityBaseList->ProjectileBaseAt(projectileFileVector[j]->_projectileName))
				{
					//Delete projectile
				}
				else
				{
					entityBaseList->AddProjectileBase(projectileBase);
					DebugOutput("Created Projectile: %s\n", projectileFileVector[j]->_projectileName.c_str());
				}
			}
		}
	}

	return entityBaseList;
}
