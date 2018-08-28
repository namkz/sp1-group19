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
		levelUp();
		updateSpells();
	} 
}

void levelUp()
{
	mciSendString(L"close \"level_up.wav\"", NULL, 0, NULL);
	mciSendString(L"open \"level_up.wav\" type waveaudio", NULL, 0, NULL);
	mciSendString(L"play \"level_up.wav\"", NULL, 0, NULL);  
	g_sChar.m_iLevel++;
	g_sChar.m_iMaxPlayerAttack = ((pow(g_sChar.m_iLevel, 2) / 2) + (g_sChar.m_iLevel * 5) + 10); 
	g_sChar.m_iMaxPlayerDefense = (g_sChar.m_iMaxPlayerAttack - 10) / 1.5 + 10;
	g_sChar.m_iMaxPlayerMana = (5* pow((g_sChar.m_iLevel),2) + 100);
	g_sChar.m_iMaxPlayerHealth = g_sChar.m_iMaxPlayerMana;
	updateEquipmentStats();
	// Set player's health and mana as the new max
} 
