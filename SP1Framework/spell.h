#ifndef _SPELL_H
#define _SPELL_H
#include "game.h"

enum ESpellComponents{SC_U, SC_I, SC_O, SC_J, SC_K, SC_L, SC_COUNT, SC_NONE=-1};

class SSpell
{
	public:
		int iMPCost;
		virtual void executeSpell() {} ;
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


class SSpellElementalExplosion : public SSpell
{
	public:	
		void executeSpell()
		{
			
		}
};

#endif
