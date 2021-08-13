#ifndef _FACTION_LIST_XML_H
#define _FACTION_LIST_XML_H

#include <string>
#include <vector>

class FactionXMLData;

class FactionListXML
{
public:
public:
	FactionListXML();
	~FactionListXML();

	int Init();
	std::vector<FactionXMLData*> GetFactionXMLDataVector();
private:
	friend class FactionListXMLParser;

	std::vector<FactionXMLData*> _factionXMLDataVector;
};
#endif
