#ifndef _FACTIONS_XML_PARSER_H
#define _FACTIONS_XML_PARSER_H

#include <string>

class FactionsXML;

class FactionsXMLParser
{
public:
	FactionsXMLParser();
	~FactionsXMLParser();

	int Init();
	int Parse(std::string fileName);
	FactionsXML* GetFactionsXML();
private:
	FactionsXML* _factionsXML;
};
#endif
