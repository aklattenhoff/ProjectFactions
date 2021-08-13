#include "EntityModelXMLCreator.h"

#include "loadGLE.h"
#include "Debug/Debug.h"

#include "Graphics/Model/Model.h"

#include "tinyxml2/tinyxml2.h"

EntityModelXMLCreator::EntityModelXMLCreator()
{
	_debug = false;
}

EntityModelXMLCreator::~EntityModelXMLCreator()
{

}

int EntityModelXMLCreator::Init(bool debug)
{
	_debug = debug;

	return 0;
}

Model* EntityModelXMLCreator::CreateFromFile(std::string fileName, int* errorOut)
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

	if (modelElement->FirstChildElement("BumpTextureFile"))
	{
		std::string textureFileString = modelElement->FirstChildElement("BumpTextureFile")->GetText();
		modelOut->LoadBumpTexture(textureFileString);
	}

	if (modelElement->FirstChildElement("SBCTextureFile"))
	{
		std::string textureFileString = modelElement->FirstChildElement("SBCTextureFile")->GetText();
		modelOut->LoadSBCTexture(textureFileString);
	}

	if (errorOut) (*errorOut) = NONE;
	return modelOut;
}
