#ifndef _MONSTER_H
#define _MONSTER_H

#include "stats.h"
#include "Framework\console.h"

enum EAttackType {E_RANGED, E_MELEE};
enum EElement {E_FIRE, E_WATER, E_EARTH, E_AIR, E_PHYSICAL, E_MEME};
struct SAttack
{
	enum EAttackType m_eAttack;
	enum EElement m_eElement;
	unsigned short m_sMaxRange;
	unsigned int m_iBaseDamage;
	unsigned int m_iDamageVariance;
};

class SEntityList
{
	// Do as you will for this one. Don't have to use a linked list but 
	// try to keep max number of entities on a floor at least 200 if  
	// there's a cap.
};

class SEntity
{
	public:
		char m_cMonsterClass = '@';
		char m_cColor = 0x0e;
		COORD m_cLocation;
		SAttack m_asAttacks[6];
		int m_iHealth;
		int m_iAttack;
		int m_iMana;
		int m_iDefense;
};

struct SDamagePacket
{
	int iDamage;
	enum EElement eElement;
};

#endif