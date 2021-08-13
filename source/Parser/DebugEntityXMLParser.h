#ifndef _DEBUG_ENTITY_XML_PARSER_H
#define _DEBUG_ENTITY_XML_PARSER_H

#include <string>

class DebugEntityXMLParser
{
public:
	DebugEntityXMLParser();
	~DebugEntityXMLParser();

	int OpenFile(const char* fileName);
	int CloseFile();

	int FoundEntity(const char* type, const char* name);
	int AddedTag(const char* tag, const char* value);
	int AddedTag(const char* tag, float value);
	int AddedTagType(const char* tag, float value, const char* type);

	int NoFileFound();
	int NoTagFound(const char* tag);
	int NoAttributeInTag(const char* attribute, const char* tag);
private:
	std::string _fileName;
};

#endif
