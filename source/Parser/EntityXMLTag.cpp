#include "EntityXMLTag.h"

#include "loadGLE.h"
#include "Debug/Debug.h"

EntityXMLTag::Resource::Resource(std::string t, float r)
{
	type = t;
	resource = r;
}

EntityXMLTag::Cost::Cost(std::string t, float c)
{
	type = t;
	cost = c;
}
