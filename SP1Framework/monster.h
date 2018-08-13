#ifndef _MONSTER_H
#define _MONSTER_H

enum EAttackType {E_RANGED, E_MELEE};
enum EElement {E_PHYSICAL};
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

const SEntity BLANK = {};

struct SEntityList
{
	SEntityList *sNextEntity;
	SEntity sEntity;
};


#endif