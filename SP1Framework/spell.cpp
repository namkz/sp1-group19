#include "spell.h"

#define _CLOSERTHAN(POS1, POS2, DISTANCE) (((POS1.X - POS2.X)*(POS1.X - POS2.X)) + ((POS1.Y - POS2.Y)*(POS1.Y - POS2*Y)) < DISTANCE * DISTANCE)

bool SSpellNode::addSpellToTree(SSpell * sNode, ESpellComponents aeSequence[])
{
	if(aeSequence[0] == SC_NONE)
	{
		if(m_sSpell == nullptr) 
		{
			m_sSpell = sNode;
			return true;
		}
		return false;
	}
	if(m_sNextSpells[aeSequence[0]] == nullptr) 
	{
		m_sNextSpells[aeSequence[0]] = new SSpellNode();
	}
	return m_sNextSpells[aeSequence[0]]->addSpellToTree(sNode, aeSequence + 1);
}

SSpell* SSpellNode::lookupSpell(ESpellComponents aeSequence[])
{
 	if(aeSequence[0] == SC_NONE) return m_sSpell;
	if(m_sNextSpells[aeSequence[0]] == nullptr) return nullptr;
	return m_sNextSpells[aeSequence[0]]->lookupSpell(aeSequence+1);
}

void SSpell::executeSpell()
{
}
