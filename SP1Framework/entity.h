#ifndef _ENTITY_H
#define _ENTITY_H

#include <string>
#include "Framework\console.h"

//SDamagePacket stores an instance of damage being dealt
struct SDamagePacket
{
	int m_iDamage;
	enum EElement m_eElement;
	std::string m_sHitMessage, m_sMissMessage;

	//this constructor is for damage involving the player
	SDamagePacket(int iDamage, EElement eElement, std::string sEntityName, bool bFromPlayer)
	{
		m_iDamage = iDamage;
		m_eElement = eElement;
		if(bFromPlayer)
		{
			m_sHitMessage = "You hit " + sEntityName + "!";
			m_sMissMessage = "You miss " + sEntityName + "!";
		}
		else
		{
			m_sHitMessage = sEntityName + " hits!";
			m_sMissMessage = sEntityName + " misses!";
		}
	}

	// this constructor is for when an entity hits another entity
	SDamagePacket(int iDamage, EElement eElement, std::string sHittingEntityName, std::string sHitEntityName)
	{
		m_iDamage = iDamage;
		m_eElement = eElement;
		m_sHitMessage = sHittingEntityName + " hits " + sHitEntityName + "!";
		m_sMissMessage = sHittingEntityName + " misses " + sHitEntityName + "!";
	}

};

class SEntity
{
	public:
		char m_cMonsterClass;
		char m_cColor;
		std::string m_sName = "";
		std::string m_sTheName = "";
		std::string m_sAName = "";
		std::string m_sCTheName = "";
		std::string m_sCAName = "";
		COORD m_cLocation;
		bool m_bAlive;
		int m_iHealth;
		int m_iAttack;
		int m_iMana;
		int m_iDefense;
		double m_dTurnInterval;
		double m_dAttackInterval;
		double m_dNextTurn = 0;
		double m_dNextAttack = 0;
		virtual void takeTurn() {};
		void takeDamage(SDamagePacket *sDamage);
		void moveTowards(COORD c, bool bTryOtherPaths);
		void attack(SEntity *sTarget) {};
		void die();
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
		void cleanDeadEntities()
		{
			for(int i = 0; i < 200; i++)
			{
				if(m_asEntities[i] != nullptr && !m_asEntities[i]->m_bAlive)
				{
					delete m_asEntities[i];
					m_asEntities[i] = nullptr;
				}
			}
		};
};


#endif
