#ifndef _TEMP_ENTITY_MODEL_XML_CREATOR_H
#define _TEMP_ENTITY_MODEL_XML_CREATOR_H

#include <string>

class Model;

namespace Phage
{
	class PhageEntity;
	class PhageModel;
}

class TempEntityModelXMLCreator
{
public:
	static const int NONE = 0;
	static const int NO_FILE_FOUND = 1;
	static const int NO_MODEL_FOUND = 2;
	static const int TAG_NOT_FOUND = 3;
public:
	TempEntityModelXMLCreator();
	~TempEntityModelXMLCreator();

	int Init(bool debug);

	Model* CreateFromFile(std::string fileName, int* errorOut = NULL);
	Phage::PhageModel* CreatePhageModelFromFile(std::string fileName, int* errorOut = NULL);
private:
	int GetAnimationType(std::string str);
private:
	bool _debug;
};

#endif
