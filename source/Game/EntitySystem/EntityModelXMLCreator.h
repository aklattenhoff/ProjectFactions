#ifndef _ENTITY_MODEL_XML_CREATOR_H
#define _ENTITY_MODEL_XML_CREATOR_H

#include <string>

class Model;

class EntityModelXMLCreator
{
public:
	static const int NONE = 0;
	static const int NO_FILE_FOUND = 1;
	static const int NO_MODEL_FOUND = 2;
	static const int TAG_NOT_FOUND = 3;
public:
	EntityModelXMLCreator();
	~EntityModelXMLCreator();

	int Init(bool debug);

	Model* CreateFromFile(std::string fileName, int* errorOut = NULL);
private:
	bool _debug;
};

#endif
