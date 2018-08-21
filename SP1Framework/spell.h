#ifndef _SPELL_H
#define _SPELL_H
#include "game.h"
#include <string>
#include "entity.h"
#include "effect.h"
#define _CLOSERTHAN(POS1, POS2, DISTANCE) (((POS1.X - POS2.X)*(POS1.X - POS2.X)) + ((POS1.Y - POS2.Y)*(POS1.Y - POS2*Y)) < DISTANCE * DISTANCE)


extern SDungeonLevel * g_sLevel;
extern SGameChar g_sChar;
extern SRenderedEffectList *g_sEffects;

enum ESpellComponents{SC_FIRE, SC_LIGHTNING, SC_AIR, SC_WATER, SC_EARTH, SC_ICE, SC_COUNT, SC_NONE=-1};

class SSpell
{
	public:
		int m_iMPCost;
		EElement m_eElement;
		int m_iDamage;

		virtual void executeSpell();
};

class SSpellNode 
{
	public:
		SSpellNode *m_sNextSpells[SC_COUNT];
		SSpell *m_sSpell;
		SSpellNode()
		{
			for(int i = 0; i < SC_COUNT; i++)
			{
				m_sNextSpells[i] = nullptr;
			}
			m_sSpell = nullptr;
		};
		~SSpellNode()
		{
			for(SSpellNode *sDeleteSpell : m_sNextSpells)
			{
				delete sDeleteSpell;
			}
			delete m_sSpell;
		};
		bool addSpellToTree(SSpell *sNode, enum ESpellComponents aeSequence[]);
		SSpell* lookupSpell(ESpellComponents aeSequence[]);
};


class SSpellElementalBasic : public SSpell
{
	std::string m_sName;
	char m_cColor;
	public:	
		SSpellElementalBasic(int iDamage, EElement eElement, int iMPCost, std::string sName, unsigned char cColor)
		{
			 m_iDamage = iDamage;
			 m_eElement = eElement;
			 m_iMPCost = iMPCost;
			 m_sName = sName;
			 m_cColor = cColor;
		}

		void executeSpell()//Cast : 1 of any basic element spell
		{
			for(SEntity *sEntity : g_sLevel->m_sEnemies) // loop through all enemies on the map
			{
				if(sEntity == nullptr) continue; // if entity is nonexistent / empty entity slot, skip. to avoid referencing a property of a nullptr this should always be first
				if(!g_sLevel->lineOfSight(sEntity->m_cLocation, g_sChar.m_cLocation)) continue; // if the entity is not in line of sight, skip
				SDamagePacket * sDamage = new SDamagePacket(m_iDamage, m_eElement, std::string("Your " + m_sName), sEntity->m_sTheName); // construct damage packet
				sEntity->takeDamage(sDamage); // deal damage packet
				g_sEffects->addEffect(new SEffectLine(sEntity->m_cLocation, g_sChar.m_cLocation, '*', m_cColor, 0.3)); // draw effect. if you need an effect @ me on discord lmao
				break; // this for single-target (break after first hit)
			}
		}
};

class SSpellElementalDragonFlame : public SSpell
{
public:
	SSpellElementalDragonFlame(double iDamage, EElement eElement, int iMPCost)
	{
		m_iDamage = iDamage;
		m_eElement = eElement;
		m_iMPCost = iMPCost;
	}

	void executeSpell()//Cast : 1 of any Dual element spell
	{
		for (SEntity *sEntity : g_sLevel->m_sEnemies) // loop through all enemies on the map
		{
			if (sEntity == nullptr) continue; // if entity is nonexistent / empty entity slot, skip. to avoid referencing a property of a nullptr this should always be first
			if (!g_sLevel->lineOfSight(sEntity->m_cLocation, g_sChar.m_cLocation)) continue; // if the entity is not in line of sight, skip
			SDamagePacket * sDamage = new SDamagePacket(m_iDamage, m_eElement, std::string("Your Dragonflame"), sEntity->m_sTheName); // construct damage packet
			sEntity->takeDamage(sDamage); // deal damage packet
			g_sEffects->addEffect(new SEffectLine(sEntity->m_cLocation, g_sChar.m_cLocation, '#', 0x04, 0.3)); // draw effect. if you need an effect @ me on discord lmao
			break; // this for single-target (break after first hit)
		}
	}
};

class SSpellElementalSteamedHams : public SSpell
{
public:
	SSpellElementalSteamedHams(double iDamage, EElement eElement, int iMPCost)
	{
		m_iDamage = iDamage;
		m_eElement = eElement;
		m_iMPCost = iMPCost;
	}

	void executeSpell()//Cast : 1 of Steamed Hams
	{
		for (SEntity *sEntity : g_sLevel->m_sEnemies) // loop through all enemies on the map
		{
			if (sEntity == nullptr) continue; // if entity is nonexistent / empty entity slot, skip. to avoid referencing a property of a nullptr this should always be first
			if (!g_sLevel->lineOfSight(sEntity->m_cLocation, g_sChar.m_cLocation)) continue; // if the entity is not in line of sight, skip
			SDamagePacket * sDamage = new SDamagePacket(m_iDamage, m_eElement, std::string("Your Steamed Hams "), sEntity->m_sTheName); // construct damage packet
			sEntity->takeDamage(sDamage); // deal damage packet
			g_sEffects->addEffect(new SEffectLine(sEntity->m_cLocation, g_sChar.m_cLocation, '%', 0x08, 0.3)); // draw effect. if you need an effect @ me on discord lmao
			break; // this for single-target (break after first hit)
		}
	}
};

class SSpellElementalFireStorm : public SSpell
{
public:
	SSpellElementalFireStorm(double iDamage, EElement eElement, int iMPCost)
	{
		m_iDamage = iDamage;
		m_eElement = eElement;
		m_iMPCost = iMPCost;
	}

	void executeSpell()//Cast : 1 of Firestorm
	{
		for (SEntity *sEntity : g_sLevel->m_sEnemies) // loop through all enemies on the map
		{
			if (sEntity == nullptr) continue; // if entity is nonexistent / empty entity slot, skip. to avoid referencing a property of a nullptr this should always be first
			if (!g_sLevel->lineOfSight(sEntity->m_cLocation, g_sChar.m_cLocation)) continue; // if the entity is not in line of sight, skip
			SDamagePacket * sDamage = new SDamagePacket(m_iDamage, m_eElement, std::string("Your Firestorm"), sEntity->m_sTheName); // construct damage packet
			sEntity->takeDamage(sDamage); // deal damage packet
			g_sEffects->addEffect(new SEffectLine(sEntity->m_cLocation, g_sChar.m_cLocation, '#', 0x04, 0.3)); // draw effect. if you need an effect @ me on discord lmao
			break; // this for single-target (break after first hit)
		}
	}
};


class SSpellElementalBlazingLightning : public SSpell
{
public:
	SSpellElementalBlazingLightning(double iDamage, EElement eElement, int iMPCost)
	{
		m_iDamage = iDamage;
		m_eElement = eElement;
		m_iMPCost = iMPCost;
	}

	void executeSpell()//Cast : 1 of Blazing Lightning
	{
		for (SEntity *sEntity : g_sLevel->m_sEnemies) // loop through all enemies on the map
		{
			if (sEntity == nullptr) continue; // if entity is nonexistent / empty entity slot, skip. to avoid referencing a property of a nullptr this should always be first
			if (!g_sLevel->lineOfSight(sEntity->m_cLocation, g_sChar.m_cLocation)) continue; // if the entity is not in line of sight, skip
			SDamagePacket * sDamage = new SDamagePacket(m_iDamage, m_eElement, std::string("Your Blazing Lightning"), sEntity->m_sTheName); // construct damage packet
			sEntity->takeDamage(sDamage); // deal damage packet
			g_sEffects->addEffect(new SEffectLine(sEntity->m_cLocation, g_sChar.m_cLocation, 'z', 0x0E, 0.3)); // draw effect. if you need an effect @ me on discord lmao
			break; // this for single-target (break after first hit)
		}
	}
};
#endif
