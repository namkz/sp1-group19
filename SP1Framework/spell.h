#ifndef _SPELL_H
#define _SPELL_H
#include "game.h"
#include "entity.h"

extern SDungeonLevel g_sLevel;

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
	public:	
		SSpellElementalBasic(int iDamage, EElement eElement, int iMPCost)
		{
			 m_iDamage = iDamage;
			 m_eElement = eElement;
			 m_iMPCost = iMPCost;
		}

		void executeSpell()//Cast : 1 of any basic element spell
		{
			for(SEntity *sEntity : g_sLevel.m_sEnemies)
			{
				if(sEntity == nullptr) continue;
				SDamagePacket * sDamage = new SDamagePacket(m_iDamage, m_eElement, std::string("Your ice bolt"), sEntity->m_sTheName);
				sEntity->takeDamage(sDamage);
			}
		}
};



#endif
