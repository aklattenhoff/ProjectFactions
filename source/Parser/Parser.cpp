#include "Parser.h"

#include "loadGLE.h"
#include "Debug/Debug.h"

#include "Parser/FactionsXML.h"
#include "Parser/FactionsXMLParser.h"

#include "Parser/shaderListXML.h"
#include "Parser/shaderListXMLParser.h"

#include "Parser/FactionListXML.h"
#include "Parser/FactionListXMLParser.h"

Parser::Parser()
{
	_factionsXMLParser = NULL;
	_shaderListXMLParser = NULL;
	_factionListXMLParser = NULL;
}

Parser::~Parser()
{
	delete _factionsXMLParser;
	delete _shaderListXMLParser;
	delete _factionListXMLParser;
}

int Parser::Init()
{
	_factionsXMLParser = new FactionsXMLParser();
	_factionsXMLParser->Init();

	_shaderListXMLParser = new ShaderListXMLParser();
	_shaderListXMLParser->Init();

	_factionListXMLParser = new FactionListXMLParser();
	_factionListXMLParser->Init();

	return 0;
}

int Parser::Parse(std::string factionXMLFileName)
{
	//Parse factions.xml
	_factionsXMLParser->Parse(factionXMLFileName);

	//Parse the shader list
	_shaderListXMLParser->Parse(_factionsXMLParser->GetFactionsXML()->GetShaderListFileName());

	//Parse the factionlist.xml file that was parsed from factions.xml
	_factionListXMLParser->Parse(_factionsXMLParser->GetFactionsXML()->GetFactionListFileName());

	return 0;
}

FactionsXML* Parser::GetFactionsXML()
{
	return _factionsXMLParser->GetFactionsXML();
}

ShaderListXML* Parser::GetShaderListXML()
{
	return _shaderListXMLParser->GetShaderListXML();
}

FactionListXML* Parser::GetFactionListXML()
{
	return _factionListXMLParser->GetFactionListXML();
}
