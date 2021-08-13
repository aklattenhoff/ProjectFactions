#ifndef _PLAYER_H
#define _PLAYER_H

#include <string>

class EntityList;

class Player
{
public:
	struct Resources
	{
		float food;
		float wood;
		float stone;
		float alloy;
		float gold;
		float mana;
	};
	struct Color
	{
		float red;
		float green;
		float blue;
	};
public:
	Player();
	~Player();

	int Init();

	EntityList* GetEntityList();

	int SetColor(float r, float g, float b);
	int SetColor(Color color);
	Color GetColor();

	int SetTeam(int t);
	int GetTeam();

	Resources GetResources();
	int ModifyResource(int resource, float value);
	static int GetResourceIndex(std::string str);
	float GetResourceValue(std::string str);
private:
	EntityList* entityList;
	Resources _resources;
	Color _color;
	int _team;
};

#endif
