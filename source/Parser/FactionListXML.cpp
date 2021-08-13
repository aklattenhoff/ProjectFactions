#include "FactionListXML.h"

#include "loadGLE.h"
#include "Debug/Debug.h"

#include "Parser/EntityListXML.h"

FactionListXML::FactionListXML()
{
}

FactionListXML::~FactionListXML()
{
}

int FactionListXML::Init()
{

	return 0;
}

std::vector<FactionXMLData*> FactionListXML::GetFactionXMLDataVector()
{
	return _factionXMLDataVector;
}
