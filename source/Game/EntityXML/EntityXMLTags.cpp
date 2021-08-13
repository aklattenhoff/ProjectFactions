#include "EntityXMLTags.h"

#include "loadGLE.h"
#include "Debug/Debug.h"

EntityXMLTags::Resource::Resource(std::string t, float r)
{
	type = t;
	resource = r;
}

EntityXMLTags::Cost::Cost(std::string t, float c)
{
	type = t;
	cost = c;
}
