#ifndef _STATS_
#define _STATS_

struct SEnemyStats
{
	int sEnemyHealth;
	int sEnemyAttack;
	int sEnemyDefense;
};
struct SPlayerStats
{
	int sPlayerHealth;
	int sPlayerMana;
	int sPlayerAttack;
	int sPlayerDefense;
};

void enemyHealthUpdate(int spellDamage, int enemyIndex);
void enemyAttackDebuff(int atkDebuff, int enemyIndex);
void enemyDefenseDebuff(int defDebuff, int enemyIndex);
void playerLevelUp(int healthIncrease, int manaIncrease, int AttackIncrease, int defenseIncrease);

#endif