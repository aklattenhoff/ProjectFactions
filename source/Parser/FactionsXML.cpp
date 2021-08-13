#include "FactionsXML.h"

#include "loadGLE.h"
#include "Debug/Debug.h"

FactionsXML::FactionsXML()
{
}

FactionsXML::~FactionsXML()
{
}

int FactionsXML::Init()
{
	return 0;
}

int FactionsXML::GetScreenWidth()
{
	return _screenWidth;
}

int FactionsXML::GetScreenHeight()
{
	return _screenHeight;
}

int FactionsXML::GetFullScreen()
{
	return _fullScreen;
}

int FactionsXML::GetMSAA()
{
	return _msaa;
}

int FactionsXML::GetMSAANicest()
{
	return _msaaNicest;
}

int FactionsXML::GetVSync()
{
	return _vSync;
}

std::string FactionsXML::GetFactionListFileName()
{
	return _factionListFileName;
}

std::string FactionsXML::GetBottomBarFileName()
{
	return _bottomBarFileName;
}

std::string FactionsXML::GetShaderListFileName()
{
	return _shaderListFileName;
}
