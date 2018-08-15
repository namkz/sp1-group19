#include "spell.h"

bool addSpellToTree(SSpellNode *sRoot, SSpell *sSpell, enum ESpellComponents aeSequence[])
{
	short sNComponents = sizeof(aeSequence) / sizeof(aeSequence[0]);
	SSpellNode * sCurrentNode = sRoot;
	for (short i = 0; i < sNComponents && aeSequence[i] != SC_NONE; i++)
	{
		if(sCurrentNode->m_sNextSpells[aeSequence[i]] == nullptr) sCurrentNode->m_sNextSpells[aeSequence[i]] = new SSpellNode;
		sCurrentNode = sCurrentNode->m_sNextSpells[aeSequence[i]];
	}
	if(sCurrentNode->m_sSpell != nullptr) return false;
	sCurrentNode->m_sSpell = sSpell;

}