#ifndef _ACTION_LIST_XML_H
#define _ACTION_LIST_XML_H

#include <string>
#include <vector>

class ActionXML;

class ActionListXML
{
public:
	ActionListXML();
	~ActionListXML();

	int Init();
	std::vector<ActionXML*> GetActionXMLVector();
private:
	friend class ActionListXMLCreator;

	std::vector<ActionXML*> _actionXMLVector;
};
#endif
