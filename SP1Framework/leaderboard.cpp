#include "leaderboard.h"
#include "game.h"

extern SGameChar g_sChar;
//Leaderboard
void leaderboard(int newScore)
{
	for (int i = 0; i < 10; i++)//loops 10 times for top 10 rank
	{
		if (newScore <= iHighscore[i]) // if the new score is lesser or equals to current highscore in current rank continue
		{
			continue;
		}
		else
		{
			for (int t = 9; t > i ; t--)//start from 9 if its greater than i -- updating from the back
			{
				iHighscore[t] = iHighscore[t + 1]; //Shift rank to the right
			}
			iHighscore[i] = newScore; // New score becomes current rank in i
		}
	}
}