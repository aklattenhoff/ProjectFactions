#ifndef _SHADER_LIST_XML_H
#define _SHADER_LIST_XML_H

#include <string>
#include <vector>

class ShaderListXML
{
public:
	ShaderListXML();
	~ShaderListXML();

	int Init();
	
	std::string GetDefaultVertex();
private:
	friend class ShaderListXMLParser;

	std::string _defaultVertex;
	std::string _defaultFragment;
};
#endif
