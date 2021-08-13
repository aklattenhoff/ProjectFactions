#include "DebugEntityXMLParser.h"

#include "Debug/Debug.h"

DebugEntityXMLParser::DebugEntityXMLParser()
{
}

DebugEntityXMLParser::~DebugEntityXMLParser()
{
}

int DebugEntityXMLParser::OpenFile(const char* fileName)
{
	_fileName = fileName;

	DebugOutput("Begin Parsing: %s...\n", fileName);

	return 0;
}

int DebugEntityXMLParser::CloseFile()
{
	DebugOutput("Done Parsing: %s\n", _fileName);

	return 0;
}


int DebugEntityXMLParser::FoundEntity(const char* type, const char* name)
{
	DebugOutput("     added entity class type: %s\n", type);
	DebugOutput("     added entity class name: %s\n", name);

	return 0;
}

int DebugEntityXMLParser::AddedTag(const char* tag, const char* value)
{
	DebugOutput("     added %s: %s\n", tag, value);

	return 0;
}

int DebugEntityXMLParser::AddedTag(const char* tag, float value)
{
	DebugOutput("     added %s: %f\n", tag, value);

	return 0;
}

int DebugEntityXMLParser::AddedTagType(const char* tag, float value, const char* type)
{
	DebugOutput("     added %s: %s, %f\n", tag, type, value);

	return 0;
}


int DebugEntityXMLParser::NoFileFound()
{
	DebugOutput("No File Found: %s\n", _fileName.c_str());

	return 0;
}

int DebugEntityXMLParser::NoTagFound(const char* tag)
{
	DebugOutput("No %s tag found\n", tag);

	return 0;
}

int DebugEntityXMLParser::NoAttributeInTag(const char* attribute, const char* tag)
{
	DebugOutput("No %s attribute found in %s tag found\n", attribute, tag);

	return 0;
}
