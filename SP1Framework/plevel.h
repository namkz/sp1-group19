#ifndef _PLEVEL_H
#define _PLEVEL_H

#include <iostream>
#include <string>

class SPlayer
{
public:
	int health;
	int mana;
	int exp;
	int level;

	SPlayer player(bool)
	{
		health = 30;
		mana = 10;
		level = 0;
		exp = 0;
	}
};

SPlayer player;

#endif
