#ifndef _SPELL_H
#define _SPELL_H
#include "game.h"

enum ESpellComponents{SC_U, SC_I, SC_O, SC_J, SC_K, SC_L, SC_COUNT, SC_NONE=-1};

class SSpell
{
	public:
		int m_iMPCost;
		EElement m_eElement;
		int m_iDamage;

		virtual SDamagePacket executeSpell() {};
};

class SSpellNode 
{
	public:
		SSpellNode *m_sNextSpells[SC_COUNT];
		SSpell *m_sSpell;
		~SSpellNode()
		{
			for(SSpellNode *sDeleteSpell : m_sNextSpells)
			{
				delete sDeleteSpell;
			}
			delete m_sSpell;
		};
};

bool addSpellToTree(SSpellNode *sRoot, SSpell *sNode, enum ESpellComponents aeSequence[]);


class SSpellElementalBasic : public SSpell
{
	public:	
		SSpellElementalBasic(int iDamage, EElement eElement, int iMPCost)
		{
			 m_iDamage = iDamage;
			 m_eElement = eElement;
			 m_iMPCost = iMPCost;
		}

		SDamagePacket executespell()//Cast : 1 of any basic element spell
		{

			SDamagePacket sDamage = SDamagePacket(m_iDamage, m_eElement, targetEntity(), true);
		}
};



#endif
