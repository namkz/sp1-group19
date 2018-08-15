#include "spell.h"


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
