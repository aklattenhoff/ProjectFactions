#include "FactionsXMLParser.h"

#include "loadGLE.h"
#include "Debug/Debug.h"

#include "tinyxml2/tinyxml2.h"

#include "Parser/FactionsXML.h"

FactionsXMLParser::FactionsXMLParser()
{
	_factionsXML = NULL;
}

FactionsXMLParser::~FactionsXMLParser()
{
	delete _factionsXML;
}

int FactionsXMLParser::Init()
{
	_factionsXML = new FactionsXML();
	_factionsXML->Init();

	return 0;
}

int FactionsXMLParser::Parse(std::string fileName)
{
	if (!_factionsXML)
		return -1;

	//TODO: Add lots of error checking here!
	tinyxml2::XMLDocument xmlFactions;
	xmlFactions.LoadFile(fileName.c_str());

	_factionsXML->_screenWidth = atoi(xmlFactions.FirstChildElement("Factions")->FirstChildElement("ScreenWidth")->FirstChild()->ToText()->Value());
	_factionsXML->_screenHeight = atoi(xmlFactions.FirstChildElement("Factions")->FirstChildElement("ScreenHeight")->FirstChild()->ToText()->Value());

	_factionsXML->_fullScreen = atoi(xmlFactions.FirstChildElement("Factions")->FirstChildElement("FullScreen")->FirstChild()->ToText()->Value()) == 0 ? false : true;

	_factionsXML->_msaa = atoi(xmlFactions.FirstChildElement("Factions")->FirstChildElement("MSAA")->GetText());
	_factionsXML->_msaaNicest = atoi(xmlFactions.FirstChildElement("Factions")->FirstChildElement("MSAANicest")->GetText());

	_factionsXML->_vSync = atoi(xmlFactions.FirstChildElement("Factions")->FirstChildElement("VSync")->GetText());

	_factionsXML->_factionListFileName = xmlFactions.FirstChildElement("Factions")->FirstChildElement("FactionList")->GetText();

	_factionsXML->_bottomBarFileName = xmlFactions.FirstChildElement("Factions")->FirstChildElement("BottomBar")->GetText();

	_factionsXML->_shaderListFileName = xmlFactions.FirstChildElement("Factions")->FirstChildElement("ShaderList")->GetText();

	return 0;
}

FactionsXML* FactionsXMLParser::GetFactionsXML()
{
	return _factionsXML;
}
