#ifndef _PARSER_H
#define _PARSER_H

#include <string>

class FactionsXML;
class FactionsXMLParser;

class ShaderListXML;
class ShaderListXMLParser;

class FactionListXML;
class FactionListXMLParser;

class EntityListXML;
class EntityListXMLParser;

class Parser
{
public:
	Parser();
	~Parser();

	int Init();

	//Parses everything from the xml file hierarchy
	int Parse(std::string factionXMLFileName);

	FactionsXML* GetFactionsXML();
	ShaderListXML* GetShaderListXML();
	FactionListXML* GetFactionListXML();
private:
	FactionsXMLParser* _factionsXMLParser;
	ShaderListXMLParser* _shaderListXMLParser;
	FactionListXMLParser* _factionListXMLParser;
};
#endif
