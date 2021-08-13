#include "GUIBottomBarParser.h"

#include "Phage/Phage.h"

GUIBottomBarParser::GUIBottomBarParser()
{
}

GUIBottomBarParser::~GUIBottomBarParser()
{
}

int GUIBottomBarParser::Parse(const char* fileName, GUIBottomBar* guiBottomBar)
{
	//TODO: Add error checking
	DebugOutput("Parsing bottom bar file: %s...\n", fileName);

	tinyxml2::XMLDocument xmlDoc;
	if (xmlDoc.LoadFile(fileName) != 0)
		DebugOutputFatalError("Unable to open bottom bar file: %s\n", fileName);

	tinyxml2::XMLElement* elementBottomBar = xmlDoc.FirstChildElement("BottomBar");

	//Action panel
	if (elementBottomBar->FirstChildElement("ActionPanel"))
	{
		guiBottomBar->actionPanelInfo.position = atoi(elementBottomBar->FirstChildElement("ActionPanel")->Attribute("position"));
		guiBottomBar->actionPanelInfo.texture = TextureLoader_LoadFromFile(elementBottomBar->FirstChildElement("ActionPanel")->GetText());

		guiBottomBar->_phageActionPanel = new Phage::PhageUIEntity();
		guiBottomBar->_phageActionPanel->Init();
		guiBottomBar->_phageActionPanel->CreateAndAddUITextureQuad(0.0f, 0.0f, 1.0f, 1.0f, guiBottomBar->actionPanelInfo.texture);
	}
	else
		DebugOutputFatalError("No ActionPanel tag found in bottom bar file: %s\n", fileName);

	//Object panel
	if (elementBottomBar->FirstChildElement("ObjectPanel"))
	{
		guiBottomBar->objectPanelInfo.position = atoi(elementBottomBar->FirstChildElement("ObjectPanel")->Attribute("position"));
		guiBottomBar->objectPanelInfo.texture = TextureLoader_LoadFromFile(elementBottomBar->FirstChildElement("ObjectPanel")->GetText());

		guiBottomBar->_phageObjectPanel = new Phage::PhageUIEntity();
		guiBottomBar->_phageObjectPanel->Init();
		guiBottomBar->_phageObjectPanel->CreateAndAddUITextureQuad(0.0f, 0.0f, 1.0f, 1.0f, guiBottomBar->objectPanelInfo.texture);
	}
	else
		DebugOutputFatalError("No ObjectPanel tag found in bottom bar file: %s\n", fileName);

	//Resource panel
	if (elementBottomBar->FirstChildElement("ResourcePanel"))
	{
		guiBottomBar->resourcePanelInfo.position = atoi(elementBottomBar->FirstChildElement("ResourcePanel")->Attribute("position"));
		guiBottomBar->resourcePanelInfo.texture = TextureLoader_LoadFromFile(elementBottomBar->FirstChildElement("ResourcePanel")->GetText());

		guiBottomBar->_phageResourcePanel = new Phage::PhageUIEntity();
		guiBottomBar->_phageResourcePanel->Init();
		guiBottomBar->_phageResourcePanel->CreateAndAddUITextureQuad(0.0f, 0.0f, 1.0f, 1.0f, guiBottomBar->resourcePanelInfo.texture);
	}
	else
		DebugOutputFatalError("No ResourcePanel tag found in bottom bar file: %s\n", fileName);

	//Minimap panel
	if (elementBottomBar->FirstChildElement("MinimapPanel"))
	{
		guiBottomBar->minimapPanelInfo.position = atoi(elementBottomBar->FirstChildElement("MinimapPanel")->Attribute("position"));
		guiBottomBar->minimapPanelInfo.texture = TextureLoader_LoadFromFile(elementBottomBar->FirstChildElement("MinimapPanel")->GetText());

		guiBottomBar->_phageMinimapPanel = new Phage::PhageUIEntity();
		guiBottomBar->_phageMinimapPanel->Init();
		guiBottomBar->_phageMinimapPanel->CreateAndAddUITextureQuad(0.0f, 0.0f, 1.0f, 1.0f, guiBottomBar->minimapPanelInfo.texture);
	}
	else
		DebugOutputFatalError("No MinimapPanel tag found in bottom bar file: %s\n", fileName);

	return 0;
}
