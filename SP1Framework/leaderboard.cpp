#include "leaderboard.h"
#include "game.h"

extern SGameChar g_sChar;

void leaderboard(int newScore)
{
	for (int i = 0; i < 10; i++)
	{
		if (newScore <= iHighscore[i])
		{
			continue;
		}
		else
		{
			for (int t = 9; t > i ; t--)
			{
				iHighscore[t] = iHighscore[t + 1];
			}
			iHighscore[i] = newScore;
		}
	}
}