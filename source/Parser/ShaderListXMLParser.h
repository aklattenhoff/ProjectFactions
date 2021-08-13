#ifndef _SHADER_LIST_XML_PARSER_H
#define _SHADER_LIST_XML_PARSER_H

#include <string>

class ShaderListXML;

class ShaderListXMLParser
{
public:
	ShaderListXMLParser();
	~ShaderListXMLParser();

	int Init();
	int Parse(std::string fileName);
	ShaderListXML* GetShaderListXML();
private:
	ShaderListXML* _shaderListXML;
};
#endif
