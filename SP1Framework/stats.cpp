#include "stats.h"
#include "game.h"
extern SGameChar g_sChar;

void levelUp()
{
	g_sChar.m_iLevel++;
	g_sChar.m_iMaxPlayerAttack = (((g_sChar.m_iLevel ^ 2) / 40) + (g_sChar.m_iLevel / 10) + 5);
	g_sChar.m_iMaxPlayerDefense = (((g_sChar.m_iLevel ^ 2) / 44) + (g_sChar.m_iLevel / 12) + 5);
	g_sChar.m_iMaxPlayerMana = (((g_sChar.m_iLevel ^ 2) / 30) + (g_sChar.m_iLevel / 5) + 10);
	g_sChar.m_iMaxPlayerHealth = (((g_sChar.m_iLevel ^ 2) / 30) + (g_sChar.m_iLevel / 5) + 10);
}


