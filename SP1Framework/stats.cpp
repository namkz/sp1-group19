#include "stats.h"
#include "game.h"

extern SGameChar g_sChar;

void levelUp()
{
	g_sChar.m_iLevel++;
	g_sChar.m_iMaxPlayerAttack = (((g_sChar.m_iLevel * g_sChar.m_iLevel) / 40) + (g_sChar.m_iLevel / 10) + 3);
	g_sChar.m_iMaxPlayerDefense = (((g_sChar.m_iLevel * g_sChar.m_iLevel) / 44) + (g_sChar.m_iLevel / 12) + 3);
	g_sChar.m_iMaxPlayerMana = (((g_sChar.m_iLevel * g_sChar.m_iLevel) / 30) + (g_sChar.m_iLevel / 5) + 10) + 100;
	g_sChar.m_iMaxPlayerHealth = g_sChar.m_iMaxPlayerMana;
	// Set player's health and mana as the new max
}
