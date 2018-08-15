#ifndef _ENTITY_H
#define _ENTITY_H

#include <string>
#include "Framework\console.h"

class SEntity
{
	public:
		char m_cMonsterClass = '@';
		char m_cColor = 0x0e;
		std::string m_sName = "";
		std::string m_sTheName = "";
		std::string m_sAName = "";
		COORD m_cLocation;
		int m_iHealth;
		int m_iAttack;
		int m_iMana;
		int m_iDefense;
		virtual void takeTurn() {};
};

struct SDamagePacket
{
	int m_iDamage;
	enum EElement m_eElement;
	std::string m_sHitMessage, m_sMissMessage;

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

	SDamagePacket(int iDamage, EElement eElement, std::string sHittingEntityName, std::string sHitEntityName)
	{
		m_iDamage = iDamage;
		m_eElement = eElement;
		m_sHitMessage = sHittingEntityName + " hits " + sHitEntityName + "!";
		m_sMissMessage = sHittingEntityName + " misses " + sHitEntityName + "!";
	}

};

#endif
