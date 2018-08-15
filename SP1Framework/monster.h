#ifndef _MONSTER_H
#define _MONSTER_H

#include "entity.h"
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
		SEntity *m_asEntities[200];
	public:
		bool addEntity(SEntity* sAddEntity)
		{
			for(int i = 0; i < 199; i++)
			{
				if(m_asEntities[i] == nullptr)
				{
					m_asEntities[i] = sAddEntity;
					return true;
				}
			}
			return false;
		}
		SEntity** begin()
		{
			return &m_asEntities[0];
		}
		SEntity** end()
		{
			return &m_asEntities[199];
		}
};

class SEntityFlamerTroll : public SEntity
{
	public:
		SEntityFlamerTroll()
		{
			m_cMonsterClass = 'T';
			m_cColor = 0x0c;
			m_sName = "flamer troll";
			m_sTheName = "the flamer troll";
			m_sAName = "a flamer troll";
			m_sCTheName = "The flamer troll";
			m_sCAName = "A flamer troll";
			m_iHealth = 23;
			m_iAttack = 12;
			m_iMana = 0;
			m_iDefense = 5;
			m_dTurnInterval = 0.500;
			m_dAttackInterval = 1.500;
		}
		void takeTurn();
};


#endif