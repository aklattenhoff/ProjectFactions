#include "TempEntityModelXMLCreator.h"

#include "loadGLE.h"
#include "Debug/Debug.h"

#include "Phage/Backend/Model.h"
#include "Phage/Backend/Animation.h"

#include "Phage/Phage.h"

#include "tinyxml2/tinyxml2.h"

TempEntityModelXMLCreator::TempEntityModelXMLCreator()
{
	_debug = false;
}

TempEntityModelXMLCreator::~TempEntityModelXMLCreator()
{

}

int TempEntityModelXMLCreator::Init(bool debug)
{
	_debug = debug;

	return 0;
}

Model* TempEntityModelXMLCreator::CreateFromFile(std::string fileName, int* errorOut)
{
	Model* modelOut = NULL;

	tinyxml2::XMLDocument modelXMLFile;

	//Try to load the file
	if (modelXMLFile.LoadFile(fileName.c_str()) != 0)
	{
		if (_debug) DebugOutput("Could not load file: %s\n", fileName.c_str());
		if (errorOut) (*errorOut) = NO_FILE_FOUND;
		return NULL;
	}

	//Try to find the first model element
	tinyxml2::XMLElement* modelElement = modelXMLFile.FirstChildElement("Model");
	if (!modelElement)
	{
		if (_debug) DebugOutput("Could not find Model tag in file: %s\n", fileName.c_str());
		if (errorOut) (*errorOut) = NO_MODEL_FOUND;
		return NULL;
	}

	//Create a new model since something exists in the xml
	modelOut = new Model();
	modelOut->Init();

	//Try to get name attribute
	//NOTE: Currently this is only for props
	if (modelElement->Attribute("name"))
	{
		modelOut->SetName(modelElement->Attribute("name"));
	}

	//Determine if we need to set a new orientation
	if (modelElement->FirstChildElement("Orientation"))
	{
		std::string orientationString = modelElement->FirstChildElement("Orientation")->GetText();
		if (orientationString.compare("z") == 0)
			modelOut->SetOrientation(Model::ORIENTATION_Z);
	}

	//Determine if we need to set a new scale
	if (modelElement->FirstChildElement("Scale"))
	{
		float scale = (float)atof(modelElement->FirstChildElement("Scale")->GetText());
		modelOut->SetScale(scale);
	}

	//There needs to be at least one model tag in the file
	//So, bail out if this fails
	if (modelElement->FirstChildElement("ModelFile"))
	{
		std::string modelFileString = modelElement->FirstChildElement("ModelFile")->GetText();
		if (modelOut->LoadScene(modelFileString) != 0)
		{
			if (_debug) DebugOutput("Could not load ModelFile tag in file: %s\n", fileName.c_str());
			if (errorOut) (*errorOut) = NO_FILE_FOUND;
			return NULL;
		}
	}
	else
	{
		if (_debug) DebugOutput("Could not find ModelFile tag in file: %s\n", fileName.c_str());
		if (errorOut) (*errorOut) = TAG_NOT_FOUND;
		return NULL;
	}

	//Load the 3 textures for the model

	if (modelElement->FirstChildElement("TextureFile"))
	{
		std::string textureFileString = modelElement->FirstChildElement("TextureFile")->GetText();
		modelOut->LoadMainTexture(textureFileString);
	}
	else
	{
		modelOut->LoadMainTexture("test.png");
	}

	if (modelElement->FirstChildElement("BumpTextureFile"))
	{
		std::string textureFileString = modelElement->FirstChildElement("BumpTextureFile")->GetText();
		modelOut->LoadBumpTexture(textureFileString);
	}
	else
	{
		modelOut->LoadBumpTexture("test.png");
	}

	if (modelElement->FirstChildElement("SBCTextureFile"))
	{
		std::string textureFileString = modelElement->FirstChildElement("SBCTextureFile")->GetText();
		modelOut->LoadSBCTexture(textureFileString);
	}
	else
	{
		modelOut->LoadSBCTexture("test.png");
	}

	//Load animations
	if (modelElement->FirstChildElement("Animations"))
	{
		tinyxml2::XMLElement* animationElement = modelElement->FirstChildElement("Animations")->FirstChildElement("Animation");
		while (animationElement)
		{
			//Get animation type
			int animType = GetAnimationType(animationElement->Attribute("name"));
			float animSpeed = 1.0f;
			if (animationElement->FirstChildElement("AnimSpeed"))
				animSpeed = (float)atof(animationElement->FirstChildElement("AnimSpeed")->GetText());
			//Get animation file
			if (animType != -1 && modelOut->GetAnimation(animType) == NULL)
				modelOut->AddAnimation(animationElement->FirstChildElement("AnimFile")->GetText(), animType, animSpeed);

			//Check for props in animation
			tinyxml2::XMLElement* propElement = animationElement->FirstChildElement("Prop");
			while (propElement)
			{
				Animation::PropDesc propToAdd;
				if (propElement->FirstChildElement("Name"))
					propToAdd.name = propElement->FirstChildElement("Name")->GetText();
				if (propElement->FirstChildElement("AnimTime"))
				{
					propToAdd.animTime.x = (float)atof(propElement->FirstChildElement("AnimTime")->Attribute("start"));
					propToAdd.animTime.y = (float)atof(propElement->FirstChildElement("AnimTime")->Attribute("end"));
				}
				else
				{
					propToAdd.animTime.x = -1.0f;
					propToAdd.animTime.y = -1.0f;
				}
				if (propElement->FirstChildElement("Bone"))
					propToAdd.boneAttachment = propElement->FirstChildElement("Bone")->GetText();
				if (propElement->FirstChildElement("Offset"))
				{
					if (propElement->FirstChildElement("Offset")->FirstChildElement("Translate"))
					{
						propToAdd.offsetTranslate.x = (float)atof(propElement->FirstChildElement("Offset")->FirstChildElement("Translate")->Attribute("x"));
						propToAdd.offsetTranslate.y = (float)atof(propElement->FirstChildElement("Offset")->FirstChildElement("Translate")->Attribute("y"));
						propToAdd.offsetTranslate.z = (float)atof(propElement->FirstChildElement("Offset")->FirstChildElement("Translate")->Attribute("z"));
					}
					if (propElement->FirstChildElement("Offset")->FirstChildElement("Rotate"))
					{
						propToAdd.offsetRotate.x = (float)atof(propElement->FirstChildElement("Offset")->FirstChildElement("Rotate")->Attribute("x"));
						propToAdd.offsetRotate.y = (float)atof(propElement->FirstChildElement("Offset")->FirstChildElement("Rotate")->Attribute("y"));
						propToAdd.offsetRotate.z = (float)atof(propElement->FirstChildElement("Offset")->FirstChildElement("Rotate")->Attribute("z"));
					}
					if (propElement->FirstChildElement("Offset")->FirstChildElement("Scale"))
					{
						propToAdd.offsetScale.x = (float)atof(propElement->FirstChildElement("Offset")->FirstChildElement("Scale")->Attribute("x"));
						propToAdd.offsetScale.y = (float)atof(propElement->FirstChildElement("Offset")->FirstChildElement("Scale")->Attribute("y"));
						propToAdd.offsetScale.z = (float)atof(propElement->FirstChildElement("Offset")->FirstChildElement("Scale")->Attribute("z"));
					}
				}
				modelOut->GetAnimation(animType)->AddProp(propToAdd);

				propElement = propElement->NextSiblingElement("Prop");
			}

			animationElement = animationElement->NextSiblingElement("Animation");
		}
	}

	if (errorOut) (*errorOut) = NONE;
	return modelOut;
}

Phage::PhageModel* TempEntityModelXMLCreator::CreatePhageModelFromFile(std::string fileName, int* errorOut)
{
	Phage::PhageModel* modelOut = NULL;

	tinyxml2::XMLDocument modelXMLFile;

	//Try to load the file
	if (modelXMLFile.LoadFile(fileName.c_str()) != 0)
	{
		if (_debug) DebugOutput("Could not load file: %s\n", fileName.c_str());
		if (errorOut) (*errorOut) = NO_FILE_FOUND;
		return NULL;
	}

	//Try to find the first model element
	tinyxml2::XMLElement* modelElement = modelXMLFile.FirstChildElement("Model");
	if (!modelElement)
	{
		if (_debug) DebugOutput("Could not find Model tag in file: %s\n", fileName.c_str());
		if (errorOut) (*errorOut) = NO_MODEL_FOUND;
		return NULL;
	}

	//Create a new model since something exists in the xml
	modelOut = new Phage::PhageModel();
	modelOut->Init();

	//Try to get name attribute
	//NOTE: Currently this is only for props
	if (modelElement->Attribute("name"))
	{
		modelOut->SetName(modelElement->Attribute("name"));
	}

	//There needs to be at least one model tag in the file
	//So, bail out if this fails
	if (modelElement->FirstChildElement("ModelFile"))
	{
		std::string modelFileString = modelElement->FirstChildElement("ModelFile")->GetText();
		if (modelOut->LoadCollada(modelFileString) != 0)
		{
			if (_debug) DebugOutput("Could not load ModelFile tag in file: %s\n", fileName.c_str());
			if (errorOut) (*errorOut) = NO_FILE_FOUND;
			return NULL;
		}
	}
	else
	{
		if (_debug) DebugOutput("Could not find ModelFile tag in file: %s\n", fileName.c_str());
		if (errorOut) (*errorOut) = TAG_NOT_FOUND;
		return NULL;
	}

	//Load the 3 textures for the model

	if (modelElement->FirstChildElement("TextureFile"))
	{
		std::string textureFileString = modelElement->FirstChildElement("TextureFile")->GetText();
		modelOut->LoadMainTexture(textureFileString);
	}
	else
	{
		modelOut->LoadMainTexture("test.png");
	}

	if (modelElement->FirstChildElement("BumpTextureFile"))
	{
		std::string textureFileString = modelElement->FirstChildElement("BumpTextureFile")->GetText();
		modelOut->LoadBumpTexture(textureFileString);
	}
	else
	{
		modelOut->LoadBumpTexture("test.png");
	}

	if (modelElement->FirstChildElement("SBCTextureFile"))
	{
		std::string textureFileString = modelElement->FirstChildElement("SBCTextureFile")->GetText();
		modelOut->LoadSBCTexture(textureFileString);
	}
	else
	{
		modelOut->LoadSBCTexture("test.png");
	}

	//Determine if we need to set a new orientation
	if (modelElement->FirstChildElement("Orientation"))
	{
		std::string orientationString = modelElement->FirstChildElement("Orientation")->GetText();
		if (orientationString.compare("z") == 0)
			modelOut->SetOrientation(Phage::PhageModel::ORIENTATION_Z);
	}

	//Determine if we need to set a new scale
	if (modelElement->FirstChildElement("Scale"))
	{
		float scale = (float)atof(modelElement->FirstChildElement("Scale")->GetText());
		modelOut->SetScale(scale);
	}

	//Load Sound Effect Files
	tinyxml2::XMLElement* sfxFileElement = modelElement->FirstChildElement("SoundEffectFile");
	std::map<std::string, Phage::PhageSound*> sfxFileMap;
	while (sfxFileElement)
	{
		std::string sfxName = sfxFileElement->Attribute("name");
		std::string sfxFileName = sfxFileElement->GetText();

		Phage::PhageSound* newSfx = new Phage::PhageSound();
		newSfx->Init(sfxFileName);
		sfxFileMap[sfxName] = newSfx;

		sfxFileElement = sfxFileElement->NextSiblingElement("SoundEffectFile");
	}
	modelOut->SetSfxFileMap(sfxFileMap);

	//Load animations
	if (modelElement->FirstChildElement("Animations"))
	{
		tinyxml2::XMLElement* animationElement = modelElement->FirstChildElement("Animations")->FirstChildElement("Animation");
		while (animationElement)
		{
			//Get animation type
			int animType = GetAnimationType(animationElement->Attribute("name"));
			float animSpeed = 1.0f;
			if (animationElement->FirstChildElement("AnimSpeed"))
				animSpeed = (float)atof(animationElement->FirstChildElement("AnimSpeed")->GetText());
			//Get animation file
			if (animType != -1 && modelOut->GetAnimation(animType) == NULL)
				modelOut->AddAnimation(animationElement->FirstChildElement("AnimFile")->GetText(), animType, animSpeed);

			//Check for props in animation
			tinyxml2::XMLElement* propElement = animationElement->FirstChildElement("Prop");
			while (propElement)
			{
				Animation::PropDesc propToAdd;
				if (propElement->FirstChildElement("Name"))
					propToAdd.name = propElement->FirstChildElement("Name")->GetText();
				if (propElement->FirstChildElement("AnimTime"))
				{
					propToAdd.animTime.x = (float)atof(propElement->FirstChildElement("AnimTime")->Attribute("start"));
					propToAdd.animTime.y = (float)atof(propElement->FirstChildElement("AnimTime")->Attribute("end"));
				}
				else
				{
					propToAdd.animTime.x = -1.0f;
					propToAdd.animTime.y = -1.0f;
				}
				if (propElement->FirstChildElement("Bone"))
					propToAdd.boneAttachment = propElement->FirstChildElement("Bone")->GetText();
				if (propElement->FirstChildElement("Offset"))
				{
					if (propElement->FirstChildElement("Offset")->FirstChildElement("Translate"))
					{
						propToAdd.offsetTranslate.x = (float)atof(propElement->FirstChildElement("Offset")->FirstChildElement("Translate")->Attribute("x"));
						propToAdd.offsetTranslate.y = (float)atof(propElement->FirstChildElement("Offset")->FirstChildElement("Translate")->Attribute("y"));
						propToAdd.offsetTranslate.z = (float)atof(propElement->FirstChildElement("Offset")->FirstChildElement("Translate")->Attribute("z"));
					}
					if (propElement->FirstChildElement("Offset")->FirstChildElement("Rotate"))
					{
						propToAdd.offsetRotate.x = (float)atof(propElement->FirstChildElement("Offset")->FirstChildElement("Rotate")->Attribute("x"));
						propToAdd.offsetRotate.y = (float)atof(propElement->FirstChildElement("Offset")->FirstChildElement("Rotate")->Attribute("y"));
						propToAdd.offsetRotate.z = (float)atof(propElement->FirstChildElement("Offset")->FirstChildElement("Rotate")->Attribute("z"));
					}
					if (propElement->FirstChildElement("Offset")->FirstChildElement("Scale"))
					{
						propToAdd.offsetScale.x = (float)atof(propElement->FirstChildElement("Offset")->FirstChildElement("Scale")->Attribute("x"));
						propToAdd.offsetScale.y = (float)atof(propElement->FirstChildElement("Offset")->FirstChildElement("Scale")->Attribute("y"));
						propToAdd.offsetScale.z = (float)atof(propElement->FirstChildElement("Offset")->FirstChildElement("Scale")->Attribute("z"));
					}
				}
				modelOut->GetAnimation(animType)->AddProp(propToAdd);

				propElement = propElement->NextSiblingElement("Prop");
			}

			//Sound Effects
			tinyxml2::XMLElement* sfxElement = animationElement->FirstChildElement("SoundEffect");
			while (sfxElement)
			{
				std::string sfxName = sfxElement->Attribute("name");
				float sfxTime = (float)atof(sfxElement->GetText());

				DebugOutput("Adding %s sfx to anim %i: %s, %f\n", fileName.c_str(), animType, sfxName.c_str(), sfxTime);
				modelOut->GetAnimation(animType)->AddSoundEffect(sfxName, sfxTime);

				sfxElement = sfxElement->NextSiblingElement("SoundEffect");
			}

			animationElement = animationElement->NextSiblingElement("Animation");
		}
	}

	if (errorOut) (*errorOut) = NONE;
	return modelOut;
}

int TempEntityModelXMLCreator::GetAnimationType(std::string str)
{
	int returnInt = -1;

	if (str.compare("Idle_1") == 0)
		returnInt = Model::ANIMATION_IDLE_1;
	else if (str.compare("Idle_2") == 0)
		returnInt = Model::ANIMATION_IDLE_2;
	else if (str.compare("Bored_1") == 0)
		returnInt = Model::ANIMATION_BORED_1;
	else if (str.compare("Walking") == 0)
		returnInt = Model::ANIMATION_WALKING;
	else if (str.compare("Attack_1") == 0)
		returnInt = Model::ANIMATION_ATTACK_1;
	else if (str.compare("Build") == 0)
		returnInt = Model::ANIMATION_BUILD;
	else if (str.compare("Gather_Food") == 0)
		returnInt = Model::ANIMATION_GATHERING_FOOD;
	else if (str.compare("Gather_Wood") == 0)
		returnInt = Model::ANIMATION_GATHERING_WOOD;
	else if (str.compare("Gather_Stone") == 0)
		returnInt = Model::ANIMATION_GATHERING_STONE;
	else if (str.compare("Gather_Alloy") == 0)
		returnInt = Model::ANIMATION_GATHERING_ALLOY;
	else if (str.compare("Gather_Huntable") == 0)
		returnInt = Model::ANIMATION_GATHERING_HUNTABLE;
	else if (str.compare("Gather_Herdable") == 0)
		returnInt = Model::ANIMATION_GATHERING_HERDABLE;
	else if (str.compare("Gather_Farm") == 0)
		returnInt = Model::ANIMATION_GATHERING_FARM;
	else if (str.compare("Gather_Tree") == 0)
		returnInt = Model::ANIMATION_GATHERING_TREE;
	else if (str.compare("Death") == 0)
		returnInt = Model::ANIMATION_DEATH;
	else if (str.compare("Dead") == 0)
		returnInt = Model::ANIMATION_DEAD;

	return returnInt;
}
