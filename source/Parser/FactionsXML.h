#ifndef _FACTIONS_XML_H
#define _FACTIONS_XML_H

#include <string>

class FactionsXML
{
public:
	FactionsXML();
	~FactionsXML();

	int Init();
	int GetScreenWidth();
	int GetScreenHeight();
	int GetFullScreen();
	int GetMSAA();
	int GetMSAANicest();
	int GetVSync();
	std::string GetFactionListFileName();
	std::string GetBottomBarFileName();
	std::string GetShaderListFileName();
private:
	friend class FactionsXMLParser;

	int _screenWidth;
	int _screenHeight;
	int _fullScreen;
	int _msaa;
	int _msaaNicest;
	int _vSync;
	std::string _factionListFileName;
	std::string _bottomBarFileName;
	std::string _shaderListFileName;
};
#endif
