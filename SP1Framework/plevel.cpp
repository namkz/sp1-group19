#include "plevel.h"
#include "game.h"

extern SGameChar g_sChar;

void playerLevel(int iXpGain)
{
	g_sChar.m_iExperience = g_sChar.m_iExperience + iXpGain; // Adding xp
	if (g_sChar.m_iExperience >= g_sChar.m_iMaxEXP) // Levelup check
	{
		g_sChar.m_iExperience = g_sChar.m_iExperience - g_sChar.m_iMaxEXP;
		g_sChar.m_iMaxEXP = pow((g_sChar.m_iLevel-1), 2) + ((g_sChar.m_iLevel-1) * 10) + 100;
		++g_sChar.m_iLevel;
	}
}