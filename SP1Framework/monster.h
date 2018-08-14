#ifndef _MONSTER_H
#define _MONSTER_H

#include "dlevel.h"
#include "entity.h"
#include "game.h"
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
	SEntityList* m_psNext;
	SEntity* m_psThis;
	// Do as you will for this one. Don't have to use a linked list but 
	// try to keep max number of entities on a floor at least 200 if  
	// there's a cap.
};

class SEntityFlamerTroll : public SEntity
{
	public:
		char m_cMonsterClass = 'T';
		char m_cColor = 0x0c;
		std::string m_sName = "flamer troll";
		std::string m_sTheName = "the flamer troll";
		std::string m_sAName = "a flamer troll";
		std::string m_sCTheName = "The flamer troll";
		std::string m_sCAName = "A flamer troll";
		COORD m_cLocation;
		int m_iHealth = 23;
		int m_iAttack = 12;
		int m_iMana = 0;
		int m_iDefense = 5;
		void takeTurn();
};

#endif