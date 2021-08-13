#include "ShaderListXMLParser.h"

#include "loadGLE.h"
#include "Debug/Debug.h"

#include "tinyxml2/tinyxml2.h"

#include "Parser/ShaderListXML.h"

ShaderListXMLParser::ShaderListXMLParser()
{
}

ShaderListXMLParser::~ShaderListXMLParser()
{
}

int ShaderListXMLParser::Init()
{
	_shaderListXML = new ShaderListXML();
	_shaderListXML->Init();

	return 0;
}

int ShaderListXMLParser::Parse(std::string fileName)
{
	if (!_shaderListXML)
		return -1;

	//TODO: Add lots of error checking here!
	tinyxml2::XMLDocument xmlShaderList;
	xmlShaderList.LoadFile(fileName.c_str());

	tinyxml2::XMLElement* shaderListElement = xmlShaderList.FirstChildElement("ShaderList");

	if (!shaderListElement)
	{
		DebugOutput("Could not find <ShaderList> in file: %s\n", fileName.c_str());
		return -1;
	}

	_shaderListXML->_defaultVertex = shaderListElement->FirstChildElement("DefaultVertex")->GetText();
	_shaderListXML->_defaultFragment = shaderListElement->FirstChildElement("DefaultFragment")->GetText();

	return 0;
}

ShaderListXML* ShaderListXMLParser::GetShaderListXML()
{
	return _shaderListXML;
}
