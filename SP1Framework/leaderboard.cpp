#include "leaderboard.h"
#include "game.h"

extern SGameChar g_sChar;
int g_iHighscore[10];
//Leaderboard
void leaderboard(int newScore)
{
	std::fstream leaderboardFile;
	int iDispose = 0;
	leaderboardFile.open("leaderboard.dat");
	for (int i = 0; i < 10; i++)//Loops 10 times for the 10 highscores
	{
		leaderboardFile >> iDispose; // Index
		leaderboardFile >> g_iHighscore[i]; // Highscores from file to game
	}
	leaderboardFile.close();
	for (int i = 0; i < 10; i++)//loops 10 times for top 10 rank
	{
		if (newScore <= g_iHighscore[i]) // if the new score is lesser or equals to current highscore in current rank continue
		{
			continue;
		}
		else
		{
			for (int t = 9; t > i ; t--)//start from 9 if its greater than i -- updating from the back
			{
				g_iHighscore[t] = g_iHighscore[t - 1]; //Shift rank to the right
			}
			g_iHighscore[i] = newScore; // New score becomes current rank in i
			break;
		}
	}
	std::ofstream outsaveFile("leaderboard.dat", std::ios::out);
	if (!outsaveFile)
	{
		exit(1);//If save file cant be found exit program
	}
	for (int i = 0; i < 10; i++)
	{
		outsaveFile << i+1 << " " << g_iHighscore[i] << "\n"; //Writes highscores into file
	}
	outsaveFile.close();
}
