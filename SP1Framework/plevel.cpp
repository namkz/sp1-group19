#include "plevel.h"

void PlayerLevel()
{
	if (player.exp >= 0 && player.exp < 99)
	{
		player.level = 0;
	}
	if (player.exp >= 100 && player.exp < 199)
	{
		player.level = 1;
	}
	if (player.exp >= 200 && player.exp < 299)
	{
		player.level = 2;
	}
	if (player.exp >= 300 && player.exp < 399)
	{
		player.level = 3;
	}
	if (player.exp >= 400 && player.exp < 499)
	{
		player.level = 4;
	}
	if (player.exp >= 500 && player.exp < 599)
	{
		player.level = 5;
	}
	if (player.exp >= 600 && player.exp < 699)
	{
		player.level = 6;
	}
	if (player.exp >= 700 && player.exp < 799)
	{
		player.level = 7;
	}
	if (player.exp >= 800 && player.exp < 899)
	{
		player.level = 8;
	}
	if (player.exp >= 900 && player.exp < 999)
	{
		player.level = 9;
	}
	if (player.exp >= 1000 && player.exp < 1999)
	{
		player.level = 10;
	}

	/*static const int required_exp[] = { 0, 100, 200, 300, 400, 500, 600, 700, 800, 900, 1000 };
	//When player.exp reaches on one of these integers, player.level will increase by 1

	while (player.exp >= required_exp[player.level])
	{
	++player.level;
	}*/
}