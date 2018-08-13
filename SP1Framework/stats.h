#ifndef _STATS_H
#define _STATS_H

struct SEnemyStats
{
	int sEnemyHealth;
	int sEnemyAttack;
	int sEnemyDefense;
};

void enemyHealthUpdate(int spellDamage, int enemyIndex);
void enemyAttackDebuff(int atkDebuff, int enemyIndex);
void enemyDefenseDebuff(int defDebuff, int enemyIndex);
void playerLevelUp(int healthIncrease, int manaIncrease, int AttackIncrease, int defenseIncrease);

#endif