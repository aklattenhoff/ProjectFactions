#ifndef _PROP_XML_H
#define _PROP_XML_H

#include <string>

class PropXML
{
public:
	PropXML();
	~PropXML();
private:
	friend class PropXMLCreator;

	std::string _actionName;
	int _actionType;

	std::string _iconFileName;

	std::string _buildObjectName;
	std::string _trainObjectName;
};
#endif
