#ifndef _GAME_H
#define _GAME_H

#include "Framework\timer.h"
#include "dlevel.h"
#include "entity.h"
#include <string>

extern class CStopWatch g_swTimer;
extern double g_dElapsedTime;
extern bool g_bQuitGame;
static int iHighscore[10] = { 0,0,0,0,0,0,0,0,0,0 };

// Enumeration to store the control keys that your game will have
enum EKEYS
{
    K_UP,
    K_DOWN,
    K_LEFT,
    K_RIGHT,
    K_ESCAPE,
	K_ENTER,
	K_SHIFT,
    K_SPACE,
	K_W,
	K_A,
	K_S,
	K_D,
	K_E,
	K_U,
	K_I,
	K_O,
	K_J,
	K_K,
	K_L,
    K_COUNT,
};

// Enumeration for the different screen states
enum EGAMESTATES
{
    S_SPLASHSCREEN,
    S_GAME,
	S_INVENTORY,
	S_GAMEEND,
    S_COUNT
};

// struct for the game character
struct SGameChar : public SEntity
{
    COORD m_cLocation;
    bool  m_bActive;
	int m_iLevel;
	int m_iMaxEXP;
	int m_iExperience;
	int m_iMaxPlayerHealth;
	int m_iMaxPlayerMana;
	int m_iMaxPlayerAttack;
	int m_iMaxPlayerDefense;
	int m_iFacingX;
	int m_iFacingY;
};

struct SMessage
{
	SMessage *m_psNext;
	std::string m_sStringMessage;
	double m_dSentTime;
	double m_dExpiryTime;

	SMessage * addNewMessage(std::string sStringMessage, double dTimeout)
	{
		SMessage * newMessageObject = new SMessage(sStringMessage, dTimeout);
		newMessageObject->m_psNext = this;

		return newMessageObject;
	};

	SMessage(std::string sStringMessage, double dTimeout)
	{
		m_psNext = nullptr;
		m_sStringMessage = sStringMessage;
		m_dSentTime = g_dElapsedTime;
		m_dExpiryTime = g_dElapsedTime + dTimeout;
	}

	~SMessage()
	{
		if(m_psNext != nullptr) delete m_psNext;
	}
};

void init        ( void );      // initialize your variables, allocate memory, etc
void getInput    ( void );      // get input from player
void update      ( double dt ); // update the game and the state of the game
void render      ( void );      // renders the current state of the game to the console
void shutdown    ( void );      // do clean up, free memory

void splashScreenWait();    // waits for time to pass in splash screen
void gameplay();            // gameplay logic
void moveCharacter();       // moves the character, collision detection, physics, etc
void processUserInput();    // checks if you should change states or do something else with the game, e.g. pause, exit
void clearScreen();         // clears the current screen and draw from scratch 
void renderSplashScreen();  // renders the splash screen
void renderGame();          // renders the game stuff
void renderMap();           // renders the map to the buffer first
void entityTurns();
void regen();
void updateSpells();
bool playerMove(COORD * cNewLocation);
void renderItems();
void renderEnemies();
void sendMessage(std::string); 
void renderCharacter();     // renders the character into the buffer
void writeToBuffer(COORD sA, char cChar, unsigned char cColor); // renders a codepoint into the buffer
void renderStatus();
void renderMessages();     
void renderInventory();
void renderFramerate();     // renders debug information, frame rate, elapsed time, etc
void renderToScreen();      // dump the contents of the buffer to the screen, one frame worth of game
//void renderLeaderboard();	// renders the high score the player has

#endif // _GAME_H