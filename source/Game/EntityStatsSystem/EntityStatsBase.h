#ifndef _ENTITY_STATS_BASE_H
#define _ENTITY_STATS_BASE_H

class EntityStatsBase
{
public:
	EntityStatsBase();
	~EntityStatsBase();

	int Init();

public:
	float MovementSpeed();
private:
	int _foodCost;
	int _woodCost;
	int _stoneCost;
	int _allowCost;
	int _goldCost;
	int _manaCost;
	int _populationCount;
	float _movementSpeed;
	float _movementTurnRate;
	float _hitpoints;
	float _trainpoints;
	float _los;
	float _hackArmor;
	float _pierceArmor;
	float _magicArmor;
};
#endif
