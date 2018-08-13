#ifndef _MONSTER_H
#define _MONSTER_H

#include "stats.h"

enum EAttackType {E_RANGED, E_MELEE};
enum EElement {E_FIRE, E_WATER, E_EARTH, E_AIR, E_PHYSICAL, E_MEME};
struct SAttack
{
	enum EAttackType eAttack;
	enum EElement eElement;
	unsigned short sMaxRange;
	unsigned int iBaseDamage;
	unsigned int iDamageVariance;
};

class SEntity
{
	char cMonsterClass = '@';
	char cColor = 0x0e;
	SAttack asAttacks[6];
	SEnemyStats stats;
};


struct SDamagePacket
{
	int iDamage;
	enum EElement eElement;
};

const SEntity BLANK = {};

struct SEntityList
{
	SEntityList *sNextEntity;
	SEntity sEntity;
};


#endif