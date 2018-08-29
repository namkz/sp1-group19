// This is the main file for the game logic and function
//
//
#include "game.h"
#include "spell.h"
#include "Framework\console.h"
#include "effect.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>

double  g_dElapsedTime;
double  g_dDeltaTime;
double  g_dNextRegen = 0;
bool    g_abKeyPressed[K_COUNT];

// Game specific variables here
SGameChar   g_sChar;
SMessage*	g_psMessages;
ESpellComponents g_aeSpell[4] = {SC_NONE, SC_NONE, SC_NONE, SC_NONE};
SSpellNode* g_sSpells;
SDungeonLevel * g_sLevel;
SRenderedEffectList* g_sEffects;
EGAMESTATES g_eGameState = S_SPLASHSCREEN;
SVisibilityMap * g_sVisible;
wchar_t * g_sBGM;

char g_cSpellSlot = 0;
bool g_bPlayerMoved = true;
bool g_bPlayerMovedLastTurn = false;
double  g_adBounceTime[K_COUNT] = {}; // this is to prevent key bouncing, so we won't trigger keypresses more than once
bool g_bifOver = false;

bool g_bFirst = true;
bool g_bSecond = false;
bool g_bThird = false;

std::string* g_asInventoryScreen[35];
std::string* g_asWinScreen[35];
std::string* g_asLeaderboard[35];
std::string* g_asGameOverscreen[35];
std::string* g_asTitle[35];
std::string* g_asHighscore[35];
std::string* g_asHowtoPlay[35];

// Console object
Console g_Console(80, 35, "Splash Screen Simulator");

//--------------------------------------------------------------
// Purpose  : Initialisation function
//            Initialize variables, allocate memory, load data from file, etc. 
//            This is called once before entering into your main loop
// Input    : void
// Output   : void
//--------------------------------------------------------------
void init( void )
{
	srand(time(0));
	g_sLevel = new SDungeonLevel ("Level.txt");
	
    // Set precision for floating point output
    g_dElapsedTime = 0.0;
	g_dNextRegen = 0.0;
	setBGM(L"title.wav");
    // sets the initial state for the game
    g_eGameState = S_SPLASHSCREEN;
    g_sChar.m_cLocation.X = 1;
    g_sChar.m_cLocation.Y = 2;
    g_sChar.m_bActive = true;
	g_sChar.m_iLevel = 1;
	g_sChar.m_iMaxEXP = 100;
	g_sChar.m_iExperience = 0;
	g_sChar.m_iMaxPlayerHealth = 100;
	g_sChar.m_iMaxPlayerMana = 100;
	g_sChar.m_iMaxPlayerAttack = 10;
	g_sChar.m_iMaxPlayerDefense = 10;
	g_sChar.m_iHealth = 100;
	g_sChar.m_iMana = 100;
	g_sChar.m_iAttack = 10;
	g_sChar.m_iDefense = 10;
	g_sChar.m_iInventoryIndex = 6;
	g_sChar.m_iInventoryPage = 1;
	g_sChar.m_iScore = 0;
	g_sChar.m_sInventory = new SInventory();
	g_sEffects = new SRenderedEffectList();
	g_sVisible = new SVisibilityMap();
	g_bPlayerMoved = true;
	SItem * temp = new SItemIntellectualWizardHat();
	g_sChar.m_sInventory->addItem(temp);
	//Test spell
	updateSpells();

	std::fstream instructGameFile;
	instructGameFile.open("instructions.txt");
	for (short i = 0; i < 35; i++)
	{
		g_asHowtoPlay[i] = new std::string;
		std::getline(instructGameFile, *g_asHowtoPlay[i]);
	}
	instructGameFile.close();

	std::fstream winGameFile;
	winGameFile.open("Win.txt");
	for (short i = 0; i < 35; i++)
	{
		g_asWinScreen[i] = new std::string;
		std::getline(winGameFile, *g_asWinScreen[i]);
	}
	winGameFile.close();

	std::fstream leaderboardFile;
	leaderboardFile.open("leaderboard.dat");
	for (short i = 0; i < 35; i++)
	{
		g_asLeaderboard[i] = new std::string;
		std::getline(leaderboardFile, *g_asLeaderboard[i]);
	}
	leaderboardFile.close();


	std::fstream inventoryFile;
	inventoryFile.open("inventory.txt");
	for (short i = 0; i < 35; i++)
	{
		g_asInventoryScreen[i] = new std::string;
		std::getline(inventoryFile, *g_asInventoryScreen[i]);
	}
	inventoryFile.close();

	std::fstream titleFile;
	titleFile.open("title.txt");
	for (short i = 0; i < 35; i++)
	{
		g_asTitle[i] = new std::string;
		std::getline(titleFile, *g_asTitle[i]);
	}
	titleFile.close();

	std::fstream GameOverFile;
	GameOverFile.open("GameOver.txt");
	for (short i = 0; i < 35; i++)
	{
		g_asGameOverscreen[i] = new std::string;
		std::getline(GameOverFile, *g_asGameOverscreen[i]);
	}
	GameOverFile.close();

    // sets the width, height and the font name to use in the console
    g_Console.setConsoleFont(0, 16, L"Consolas");
}

void updateSpells()
{
	delete g_sSpells;
	g_sSpells = new SSpellNode();
	{ESpellComponents aeTemp[4] = {SC_FIRE, SC_NONE};
	SSpell * psSpell = new SSpellElementalBasic(100000, E_FIRE, 0, "rekt bolt", 0x0F);
	g_sSpells->addSpellToTree(psSpell, aeTemp);}
	//Basic Fire
	//{ESpellComponents aeTemp[4] = { SC_FIRE, SC_NONE };
	//SSpell * psSpell = new SSpellElementalBasic(g_sChar.m_iAttack, E_FIRE, 4, "fireball", 0x0C);
	//g_sSpells->addSpellToTree(psSpell, aeTemp);}
	//Basic Water
	{ESpellComponents aeTemp[4] = { SC_WATER, SC_NONE };
	SSpell * psSpell = new SSpellElementalBasic(g_sChar.m_iAttack, E_WATER, 4, "waterbolt", 0x09);
	g_sSpells->addSpellToTree(psSpell, aeTemp);}
	//Basic Earth
	{ESpellComponents aeTemp[4] = { SC_EARTH, SC_NONE };
	SSpell * psSpell = new SSpellElementalBasic(g_sChar.m_iAttack, E_EARTH, 4, "dirt", 0x06);
	g_sSpells->addSpellToTree(psSpell, aeTemp);}
	//Basic Wind
	{ESpellComponents aeTemp[4] = { SC_AIR, SC_NONE };
	SSpell * psSpell = new SSpellElementalBasic(g_sChar.m_iAttack, E_AIR, 4, "piercing wind", 0x0F);
	g_sSpells->addSpellToTree(psSpell, aeTemp);}
	//Basic Lightning
	{ESpellComponents aeTemp[4] = { SC_LIGHTNING, SC_NONE };
	SSpell * psSpell = new SSpellElementalBasic(g_sChar.m_iAttack, E_LIGHTNING, 4, "spark", 0x0E);
	g_sSpells->addSpellToTree(psSpell, aeTemp);}
	//Basic Ice
	{ESpellComponents aeTemp[4] = { SC_ICE, SC_NONE };
	SSpell * psSpell = new SSpellElementalBasic(g_sChar.m_iAttack, E_ICE, 4, "snowball of death", 0x0A);
	g_sSpells->addSpellToTree(psSpell, aeTemp);}
	//DragonFlame
	{ESpellComponents aeTemp[4] = { SC_FIRE, SC_FIRE, SC_NONE };
	g_sSpells->addSpellToTree(new SSpellElementalDragonFlame((double)g_sChar.m_iAttack * 1.5, E_FIRE, 15 + ((double)g_sChar.m_iMaxPlayerMana / 100)  * 1.5), aeTemp);
	//void executeSpell();
	}
	//Steamed Hams
	{ESpellComponents aeTemp[4] = { SC_FIRE, SC_WATER, SC_NONE };
	SSpell * psSpell = new SSpellElementalSteamedHams((double)g_sChar.m_iAttack * 8, E_WATER, 20 + ((double)g_sChar.m_iMaxPlayerMana / 100) * 2);
	g_sSpells->addSpellToTree(psSpell, aeTemp);}
	//Firestorm
	{ESpellComponents aeTemp[4] = { SC_FIRE, SC_AIR, SC_NONE };
	SSpell * psSpell = new SSpellElementalFireStorm((double)g_sChar.m_iAttack * (1.5), E_FIRE, 40 + ((double)g_sChar.m_iMaxPlayerMana / 100) *4);
	g_sSpells->addSpellToTree(psSpell, aeTemp);}
	//Blazing Lightning
	{ESpellComponents aeTemp[4] = { SC_FIRE, SC_LIGHTNING, SC_NONE };
	SSpell * psSpell = new SSpellElementalBlazingLightning((double)g_sChar.m_iAttack*4, E_LIGHTNING, 30 + ((double)g_sChar.m_iMaxPlayerMana / 100) * 3);
	g_sSpells->addSpellToTree(psSpell, aeTemp);}
	//Frostfire
	{ESpellComponents aeTemp[4] = { SC_FIRE, SC_ICE, SC_NONE };
	SSpell * psSpell = new SSpellElementalFrostFire((double)g_sChar.m_iAttack*4, E_FIRE, 35+ ((double)g_sChar.m_iMaxPlayerMana / 100) * 3.5);
	g_sSpells->addSpellToTree(psSpell, aeTemp);}
	//Water Tree
	//Water Wave
	{ESpellComponents aeTemp[4] = { SC_WATER, SC_WATER, SC_NONE };
	SSpell * psSpell = new SSpellElementalWaterWave((double)g_sChar.m_iAttack*2, E_WATER, 30+ ((double)g_sChar.m_iMaxPlayerMana / 100) * 3);
	g_sSpells->addSpellToTree(psSpell, aeTemp);}
	//Quagmire
	{ESpellComponents aeTemp[4] = { SC_WATER, SC_EARTH, SC_NONE };
	SSpell * psSpell = new SSpellElementalQuagmire((double)g_sChar.m_iAttack, E_WATER, 40+ ((double)g_sChar.m_iMaxPlayerMana / 100) * 4);
	g_sSpells->addSpellToTree(psSpell, aeTemp);}
	//Hurricane
	{ESpellComponents aeTemp[4] = { SC_WATER, SC_AIR, SC_NONE};
	SSpell * psSpell = new SSpellElementalHurricane((double)g_sChar.m_iAttack*2, E_AIR, 40 + ((double)g_sChar.m_iMaxPlayerMana / 100) * 3);
	g_sSpells->addSpellToTree(psSpell, aeTemp);}
	//Shockwave
	{ESpellComponents aeTemp[4] = { SC_WATER, SC_LIGHTNING,SC_NONE };
	SSpell * psSpell = new SSpellElementalShockwave((double)g_sChar.m_iAttack*1.5, E_LIGHTNING, 25 + ((double)g_sChar.m_iMaxPlayerMana / 100) * 2.5);
	g_sSpells->addSpellToTree(psSpell, aeTemp);}
	//Ice Tomb
	{ESpellComponents aeTemp[4] = { SC_WATER, SC_ICE,SC_NONE};
	SSpell * psSpell = new SSpellElementalIceTomb((double)g_sChar.m_iAttack*3, E_WATER, 50 + ((double)g_sChar.m_iMaxPlayerMana / 100) * 5);
	 g_sSpells->addSpellToTree(psSpell, aeTemp);}
	//Earth Tree
	//Rock Armor // Buff
	{ESpellComponents aeTemp[4] = { SC_EARTH, SC_EARTH,SC_NONE };
	SSpell * psSpell = new SSpellElementalRockArmour(0, E_EARTH ,20 + ((double)g_sChar.m_iMaxPlayerMana / 100) * 2);
	g_sSpells->addSpellToTree(psSpell, aeTemp);}
	//Meteor Storm
	{ESpellComponents aeTemp[4] = { SC_EARTH, SC_AIR,SC_NONE };
	SSpell * psSpell = new SSpellElementalMeteorStorm((double)g_sChar.m_iAttack*1.5, E_EARTH, 40 + ((double)g_sChar.m_iMaxPlayerMana / 100) * 4);
	g_sSpells->addSpellToTree(psSpell, aeTemp);}
	//Effect Negation //Debuff
	{ESpellComponents aeTemp[4] = { SC_EARTH, SC_LIGHTNING,SC_NONE };
	SSpell * psSpell = new SSpellElementalEffectNegation(0, E_EARTH	, 20 + ((double)g_sChar.m_iMaxPlayerMana / 100) * 2);
	g_sSpells->addSpellToTree(psSpell, aeTemp);}
	//Pitfall
	{ESpellComponents aeTemp[4] = { SC_EARTH, SC_ICE,SC_NONE };
	SSpell * psSpell = new SSpellElementalEffectNegation((double)g_sChar.m_iAttack*5, E_EARTH, 30 + ((double)g_sChar.m_iMaxPlayerMana / 100) * 3);
	g_sSpells->addSpellToTree(psSpell, aeTemp);}
	//Wind Tree
	//Deflective Barrier // Buff
	{ESpellComponents aeTemp[4] = { SC_AIR, SC_AIR,SC_NONE };
	SSpell * psSpell = new SSpellElementalEffectNegation(0, E_AIR, 20 + ((double)g_sChar.m_iMaxPlayerMana / 100) * 2);
	g_sSpells->addSpellToTree(psSpell, aeTemp);}
	//Lightning Storm
	{ESpellComponents aeTemp[4] = { SC_AIR, SC_LIGHTNING,SC_NONE };
	SSpell * psSpell = new SSpellElementalEffectNegation((double)g_sChar.m_iAttack*2, E_AIR, 50 + ((double)g_sChar.m_iMaxPlayerMana / 100) * 5);
	g_sSpells->addSpellToTree(psSpell, aeTemp);}
	//Icicle Barrage
	{ESpellComponents aeTemp[4] = { SC_AIR, SC_LIGHTNING,SC_NONE };
	SSpell * psSpell = new SSpellElementalIcicleBarrage((double)g_sChar.m_iAttack * 2, E_AIR, 40 + ((double)g_sChar.m_iMaxPlayerMana / 100) * 4);
	g_sSpells->addSpellToTree(psSpell, aeTemp);}
	//Chain Lightning
	{ESpellComponents aeTemp[4] = { SC_LIGHTNING, SC_LIGHTNING,SC_NONE };
	SSpell * psSpell = new SSpellElementalChainLightning((double)g_sChar.m_iAttack * 2.5, E_LIGHTNING, 35 + ((double)g_sChar.m_iMaxPlayerMana / 100) * 3.5);
	g_sSpells->addSpellToTree(psSpell, aeTemp);}
	//Everlasting Paralysis // Debuff
	{ESpellComponents aeTemp[4] = { SC_LIGHTNING, SC_ICE,SC_NONE };
	SSpell * psSpell = new SSpellElementalEverlastingParalysis((double)g_sChar.m_iAttack * 1.5, E_LIGHTNING, 50 + ((double)g_sChar.m_iMaxPlayerMana / 100) * 5);
	g_sSpells->addSpellToTree(psSpell, aeTemp);}
	//Ice Tree
	//Ice Wall
	{ESpellComponents aeTemp[4] = { SC_LIGHTNING, SC_ICE,SC_NONE };
	SSpell * psSpell = new SSpellElementalIceWall((double)g_sChar.m_iAttack * 1.5, E_ICE, 50 + ((double)g_sChar.m_iMaxPlayerMana / 100) * 5);
	g_sSpells->addSpellToTree(psSpell, aeTemp);}
	//Ultimate Spell Tree
	//Sun God Fury
	{ESpellComponents aeTemp[4] = { SC_FIRE, SC_FIRE,SC_FIRE,SC_NONE };
	SSpell * psSpell = new SSpellElementalIceWall((double)g_sChar.m_iAttack * 15, E_FIRE, 500 + ((double)g_sChar.m_iMaxPlayerMana / 100) * 80);
	g_sSpells->addSpellToTree(psSpell, aeTemp);}
	//LandSlide
	{ESpellComponents aeTemp[4] = { SC_EARTH, SC_WATER,SC_AIR,SC_NONE };
	SSpell * psSpell = new SSpellElementalIceWall((double)g_sChar.m_iAttack * 10, E_EARTH, 50 + ((double)g_sChar.m_iMaxPlayerMana / 100) * 5);
	g_sSpells->addSpellToTree(psSpell, aeTemp);}
	//Ritual of Madness //Debuff
	{ESpellComponents aeTemp[4] = { SC_FIRE, SC_ICE,SC_EARTH,SC_NONE };
	SSpell * psSpell = new SSpellElementalIceWall(0, E_FIRE, g_sChar.m_iMaxPlayerMana);
	g_sSpells->addSpellToTree(psSpell, aeTemp);}
}

//--------------------------------------------------------------
// Purpose  : Reset before exiting the program
//            Do your clean up of memory here
//            This is called once just before the game exits
// Input    : Void
// Output   : void
//--------------------------------------------------------------
void shutdown( void )
{
    // Reset to white text on black background
    colour(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);

    g_Console.clearBuffer();
}

//--------------------------------------------------------------
// Purpose  : Getting all the key press states
//            This function checks if any key had been pressed since the last time we checked
//            If a key is pressed, the value for that particular key will be true
//
//            Add more keys to the enum in game.h if you need to detect more keys
//            To get other VK key defines, right click on the VK define (e.g. VK_UP) and choose "Go To Definition" 
//            For Alphanumeric keys, the values are their ascii values (uppercase).
// Input    : Void
// Output   : void
//--------------------------------------------------------------
void getInput( void )
{    
    g_abKeyPressed[K_SPACE]  = isKeyPressed(VK_SPACE);
    g_abKeyPressed[K_ESCAPE] = isKeyPressed(VK_ESCAPE);
    g_abKeyPressed[K_W]  = isKeyPressed('W') || isKeyPressed(VK_UP);
    g_abKeyPressed[K_A]  = isKeyPressed('A') || isKeyPressed(VK_LEFT);
    g_abKeyPressed[K_S]  = isKeyPressed('S') || isKeyPressed(VK_DOWN);
    g_abKeyPressed[K_D]  = isKeyPressed('D') || isKeyPressed(VK_RIGHT);
	g_abKeyPressed[K_E]  = isKeyPressed('E');
    g_abKeyPressed[K_U]  = isKeyPressed('U');
    g_abKeyPressed[K_I]  = isKeyPressed('I');
    g_abKeyPressed[K_O]  = isKeyPressed('O');
    g_abKeyPressed[K_J]  = isKeyPressed('J');
    g_abKeyPressed[K_K]  = isKeyPressed('K');
	g_abKeyPressed[K_L]  = isKeyPressed('L');
	g_abKeyPressed[K_SHIFT] = isKeyPressed(VK_SHIFT);
	g_abKeyPressed[K_ENTER] = isKeyPressed(VK_RETURN);
	g_abKeyPressed[K_ESCAPE] = isKeyPressed(VK_ESCAPE);
}

void setBGM(wchar_t * eBGM)
{
	if(g_sBGM != eBGM)
	{
		PlaySound(eBGM, NULL, SND_LOOP | SND_ASYNC);
		g_sBGM = eBGM;
	}
}

//--------------------------------------------------------------
// Purpose  : Update function
//            This is the update function
//            double dt - This is the amount of time in seconds since the previous call was made
//
//            Game logic should be done here.
//            Such as collision checks, determining the position of your game characters, status updates, etc
//            If there are any calls to write to the console here, then you are doing it wrong.
//
//            If your game has multiple states, you should determine the current state, and call the relevant function here.
//
// Input    : dt = deltatime
// Output   : void
//--------------------------------------------------------------
void update(double dt)
{
    // get the delta time
    g_dElapsedTime += dt;
    g_dDeltaTime = dt;

    switch (g_eGameState)
    {
        case S_SPLASHSCREEN : splashScreenWait(); // game logic for the splash screen
            break;
		case S_GAME: gameplay(); // gameplay logic when we are in the game
			break;
		case S_INVENTORY: gameplayInventory();  // gameplay logic when we are in the inventory
            break;
		case S_GAMEEND: case S_GAMEWIN: gameEnd(); // Spacebar ends program
			break;
		case S_GAMEINSTRUCT:resetMain();
			break;
    }
}
//--------------------------------------------------------------
// Purpose  : Render function is to update the console screen
//            At this point, you should know exactly what to draw onto the screen.
//            Just draw it!
//            To get an idea of the values for colours, look at console.h and the URL listed there
// Input    : void
// Output   : void
//--------------------------------------------------------------
void render()
{
    clearScreen();      // clears the current screen and draw from scratch 
    switch (g_eGameState)
    {
        case S_SPLASHSCREEN: renderSplashScreen();
            break;
        case S_GAME: renderGame();
            break;
		case S_INVENTORY: renderInventory();
			break;
		case S_GAMEEND: renderGameOver();
			break;
		case S_GAMEWIN: renderWin();
			break;
		case S_GAMEINSTRUCT: renderInstruct();
			break;
    }
    renderFramerate();  // renders debug information, frame rate, elapsed time, etc
    renderToScreen();   // dump the contents of the buffer to the screen, one frame worth of game
}

void splashScreenWait()    // waits for time to pass in splash screen
{
    /*if (g_abKeyPressed[K_SPACE]) 
	{
        g_eGameState = S_GAME;
		g_adBounceTime[K_SPACE] = g_dElapsedTime + 0.4;
	}*/
}

void gameplay()            // gameplay logic
{
	if(g_sChar.m_iHealth < g_sChar.m_iMaxPlayerHealth / 3) setBGM(L"creepy_low.wav");
	else setBGM(L"creepy.wav");
    processUserInput(); // checks if you should change states or do something else with the game, e.g. pause, exit
    if(g_eGameState != S_INVENTORY) moveCharacter();    // moves the character, collision detection, physics, etc
	if(g_bPlayerMoved) 
	{
		g_sVisible = g_sLevel->tilesWithLineOfSight(g_sChar.m_cLocation);
		g_sLevel->m_sExplored->assimilate(g_sVisible);
		g_bPlayerMoved = false;
	}
	if(g_eGameState != S_INVENTORY) 
	{
		entityTurns();
		regen();
	}
	if (g_sChar.m_iHealth <= 0)//If health is 0 transition to end game screen.
	{
		g_eGameState = S_GAMEEND;
	}
}


void gameEnd()
{
	if (g_abKeyPressed[K_ENTER] && g_dElapsedTime > g_adBounceTime[K_ENTER])
	{
		if (g_eGameState == S_GAMEEND)
		{
			g_bQuitGame = true;
		}
	}
}
void resetMain()
{
	if (g_abKeyPressed[K_SPACE] && g_dElapsedTime > g_adBounceTime[K_SPACE])
	{
		if (g_eGameState == S_GAMEINSTRUCT)
		{
			g_eGameState = S_SPLASHSCREEN;
		}
	}
}


void regen()
{
	if(g_dNextRegen > g_dElapsedTime) return;
	g_sChar.m_iMana += (g_sChar.m_iMana * 3 + g_sChar.m_iMaxPlayerMana * 2) / 100;
	g_sChar.m_iHealth += (g_sChar.m_iMaxPlayerHealth * 2) / 100;
	if(g_sChar.m_iMana > g_sChar.m_iMaxPlayerMana) g_sChar.m_iMana = g_sChar.m_iMaxPlayerMana;
	if(g_sChar.m_iHealth > g_sChar.m_iMaxPlayerHealth) g_sChar.m_iHealth = g_sChar.m_iMaxPlayerHealth;
	g_dNextRegen = g_dElapsedTime + 1;
}

void gameplayInventory()
{
	processUserInput();
	processEquipment(g_sChar.m_iInventoryIndex);
	g_Console.writeToBuffer({ 26,16 }, std::to_string(g_sChar.m_iInventoryPage), 0x0f);
	g_Console.writeToBuffer(moveInventoryCursor(), ">", 0x0f);
}
void mainMenuCursor()
{
	g_Console.writeToBuffer(moveMainMenuCursor(), ">", 0x0f);
}

COORD moveMainMenuCursor()
{
	COORD cCursorPos;
	bool b_input = true;
	if (g_bFirst == true)
	{
		cCursorPos = { 37, 15 };
		if (g_adBounceTime[K_ENTER] < g_dElapsedTime && g_abKeyPressed[K_ENTER])
		{
			g_eGameState = S_GAME;
		}
	}
	if (g_adBounceTime[K_S] < g_dElapsedTime && g_abKeyPressed[K_S] && g_bFirst == true)
	{
		cCursorPos = { 34, 18 };
		g_bFirst = false;
		g_bSecond = true;
		b_input = true;
	}
	else if (g_adBounceTime[K_S] < g_dElapsedTime && g_abKeyPressed[K_S] && g_bSecond == true)
	{
		cCursorPos = { 37,21 };
		g_bSecond = false;
		g_bThird = true;
		b_input = true;
	}
	else if (g_adBounceTime[K_W] < g_dElapsedTime && g_abKeyPressed[K_W] && g_bThird == true)
	{
		cCursorPos = { 34,18 };
		g_bThird = false;
		g_bSecond = true;
	}
	else if (g_adBounceTime[K_W] < g_dElapsedTime && g_abKeyPressed[K_W] && g_bSecond == true)
	{
		cCursorPos = { 37,15 };
		b_input = true;
		g_bFirst = true;
		g_bSecond = false;
	}
	if (g_bSecond == true)
	{
		if (g_adBounceTime[K_ENTER] < g_dElapsedTime && g_abKeyPressed[K_ENTER])
		{
			g_eGameState = S_GAMEINSTRUCT;
		}
		b_input = true;
		cCursorPos = { 34, 18 };
	}
	if ( g_bThird == true)
	{
		if (g_adBounceTime[K_ENTER] < g_dElapsedTime && g_abKeyPressed[K_ENTER])
		{
			exit(1);
		}
		b_input = true;
		cCursorPos = { 37, 21 };
	}
	if (b_input = true)
	{
		// set the bounce time to some time in the future to prevent accidental triggers
		for (int i = 0; i < K_COUNT; i++)
		{
			if (g_abKeyPressed[i]) g_adBounceTime[i] = g_dElapsedTime + (i >= K_U ? 1 / 8.0 : 1 / 15.0);
		}
	}
	return cCursorPos;
}

COORD moveInventoryCursor()
{
	bool bInput = false;
	short sEquipCalculationNumber = 64;
	COORD cCursorPos;
	// Moving the cursor position when user input is detected
	if (g_sChar.m_iInventoryIndex >= 0 && g_sChar.m_iInventoryIndex <= 5)
	{
		cCursorPos = { 9, 11 };
		if (g_adBounceTime[K_A] < g_dElapsedTime && g_abKeyPressed[K_A] && g_sChar.m_iInventoryIndex != 5)
		{
			++g_sChar.m_iInventoryIndex;
			bInput = true;
		}
		if (g_adBounceTime[K_D] < g_dElapsedTime && g_abKeyPressed[K_D] && g_sChar.m_iInventoryIndex != 0)
		{
			--g_sChar.m_iInventoryIndex;
			bInput = true;
		}
		cCursorPos.X = (sEquipCalculationNumber - g_sChar.m_iInventoryIndex * 11);
		if (g_adBounceTime[K_S] < g_dElapsedTime && g_abKeyPressed[K_S])
		{
			cCursorPos = { 7, 17 };
			g_sChar.m_iInventoryIndex = (g_sChar.m_iInventoryPage * 8) - 2;
			bInput = true;
		}
	}
	else if (g_sChar.m_iInventoryIndex >= 6 && g_sChar.m_iInventoryIndex <= 38)
	{
		cCursorPos = { 7, 17 };
		if (g_adBounceTime[K_W] < g_dElapsedTime && g_abKeyPressed[K_W])
		{
			if ((g_sChar.m_iInventoryIndex + 2) % 8 == 0)
			{
				cCursorPos = { 9, 11 };
				g_sChar.m_iInventoryIndex = 5;
				bInput = true;
			}
			else
			{
				--g_sChar.m_iInventoryIndex;
				bInput = true;
			}
		}
		if (g_adBounceTime[K_S] < g_dElapsedTime && g_abKeyPressed[K_S])
		{
			if ((g_sChar.m_iInventoryIndex - 5) % 8 != 0)
			{
				++g_sChar.m_iInventoryIndex;
				bInput = true;
			}
		}
		if (g_adBounceTime[K_A] < g_dElapsedTime && g_abKeyPressed[K_A])
		{
			if (g_sChar.m_iInventoryPage != 1)
			{
				g_sChar.m_iInventoryPage -= 1;
				g_sChar.m_iInventoryIndex -= 8;
				bInput = true;
			}
		}
		if (g_adBounceTime[K_D] < g_dElapsedTime && g_abKeyPressed[K_D])
		{
			if (g_sChar.m_iInventoryPage != 4)
			{
				g_sChar.m_iInventoryPage += 1;
				g_sChar.m_iInventoryIndex += 8;
				bInput = true;
			}
		}
		
		if (g_sChar.m_iInventoryIndex >= 6)
		{
			cCursorPos.Y += ((g_sChar.m_iInventoryIndex + 2) - g_sChar.m_iInventoryPage * 8) * 2;
		}
	}
	if (bInput)
	{
		// set the bounce time to some time in the future to prevent accidental triggers
		for (int i = 0; i < K_COUNT; i++)
		{
			if (g_abKeyPressed[i]) g_adBounceTime[i] = g_dElapsedTime + (i >= K_U ? 1 / 8.0 : 1 / 15.0);
		}
	}
	return cCursorPos;
}

void processEquipment(short sIndex)
{
	if (sIndex < 6 && g_adBounceTime[K_ENTER] < g_dElapsedTime && g_abKeyPressed[K_ENTER])
	{
		g_sChar.m_sInventory->unequipItemFromSlot(sIndex);
	}
	else if (g_adBounceTime[K_ENTER] < g_dElapsedTime && g_abKeyPressed[K_ENTER])
	{
		g_sChar.m_sInventory->equipItemToSlot(sIndex);
	}
}

void resetPlayerMaxStats()
{
	g_sChar.m_iMaxPlayerAttack = ((pow(g_sChar.m_iLevel, 2) / 2) + (g_sChar.m_iLevel * 5) + 5);
	g_sChar.m_iMaxPlayerDefense = (g_sChar.m_iMaxPlayerAttack - 10) / 1.5 + 10;
	g_sChar.m_iMaxPlayerMana = (5 * pow((g_sChar.m_iLevel), 2) + 95);
	g_sChar.m_iMaxPlayerHealth = g_sChar.m_iMaxPlayerMana;
}

void updateEquipmentStats()
{
	resetPlayerMaxStats();
	int health = g_sChar.m_iMaxPlayerHealth;
	int mana = g_sChar.m_iMaxPlayerMana;
	int attack = g_sChar.m_iMaxPlayerAttack;
	int defense = g_sChar.m_iMaxPlayerDefense;
	for (int i = 0; i < 6; i++)
	{
		if (g_sChar.m_sInventory->m_asContents[i] == nullptr)
		{
			continue;
		}
		else
		{
			health += g_sChar.m_sInventory->m_asContents[i]->processHealth(g_sChar.m_iMaxPlayerHealth);
			mana += g_sChar.m_sInventory->m_asContents[i]->processMana(g_sChar.m_iMaxPlayerMana);
			attack += g_sChar.m_sInventory->m_asContents[i]->processAttack(g_sChar.m_iMaxPlayerAttack);
			defense += g_sChar.m_sInventory->m_asContents[i]->processDefense(g_sChar.m_iMaxPlayerDefense);
		}
	}
	g_sChar.m_iMaxPlayerHealth = health;
	g_sChar.m_iMaxPlayerMana = mana;
	g_sChar.m_iMaxPlayerAttack = attack;
	g_sChar.m_iMaxPlayerDefense = defense;
	g_sChar.m_iAttack = g_sChar.m_iMaxPlayerAttack;
	g_sChar.m_iDefense = g_sChar.m_iMaxPlayerDefense;
}

void entityTurns()
{
 	g_sLevel->m_sEnemies.cleanDeadEntities();
	for(SEntity *ppsCurrent : g_sLevel->m_sEnemies)
	{
		if(ppsCurrent == nullptr) continue;
		if(ppsCurrent->m_dNextTurn > g_dElapsedTime) continue;
 		if(ppsCurrent) ppsCurrent->takeTurn();
	}
}

bool playerMove(COORD *cNewLocation)
{
	if((!g_sLevel->hasEnemy(*cNewLocation) || g_sLevel->canPlayerSeeEnemy(*cNewLocation)) && g_sLevel->getFeatureAt(cNewLocation)->onMovedInto()) 
	{
		g_sChar.m_cLocation = *cNewLocation;
		g_bPlayerMoved = true;
		return true;
	}
	return false;
}

void moveCharacter()
{
    bool bSomethingHappened = false;
	if (g_adBounceTime[K_W] < g_dElapsedTime && g_abKeyPressed[K_W] 
	 || g_adBounceTime[K_A] < g_dElapsedTime && g_abKeyPressed[K_A] 
	 || g_adBounceTime[K_S] < g_dElapsedTime && g_abKeyPressed[K_S] 
	 || g_adBounceTime[K_D] < g_dElapsedTime && g_abKeyPressed[K_D]) 
	{
		g_sChar.m_sFacingX = 0;
		g_sChar.m_sFacingY = 0;
		g_bPlayerMoved = true;
	}
    // Updating the location of the character based on the key press
    // providing a beep sound whenver we shift the character
    if (g_adBounceTime[K_W] < g_dElapsedTime && g_abKeyPressed[K_W] && g_sChar.m_cLocation.Y > 0)
    {
        bSomethingHappened = true;
		g_sChar.m_sFacingY = -1;
    }
    if (g_adBounceTime[K_A] < g_dElapsedTime && g_abKeyPressed[K_A] && g_sChar.m_cLocation.X > 0)
    {
        bSomethingHappened = true;
		g_sChar.m_sFacingX = -1;
    }
    if (g_adBounceTime[K_S] < g_dElapsedTime && g_abKeyPressed[K_S] && g_sChar.m_cLocation.Y < 27)
    {
        bSomethingHappened = true;
		g_sChar.m_sFacingY = 1;
    }
    if (g_adBounceTime[K_D] < g_dElapsedTime && g_abKeyPressed[K_D] && g_sChar.m_cLocation.X < 79)
    {
        bSomethingHappened = true;
		g_sChar.m_sFacingX = 1;
    }
	if (g_adBounceTime[K_U] < g_dElapsedTime && g_abKeyPressed[K_U] && g_cSpellSlot <= 2)
	{
		g_aeSpell[g_cSpellSlot] = SC_FIRE;
		g_cSpellSlot++;
		bSomethingHappened = true;
	}
	if (g_adBounceTime[K_I] < g_dElapsedTime && g_abKeyPressed[K_I] && g_cSpellSlot <= 2)
	{
		g_aeSpell[g_cSpellSlot] = SC_LIGHTNING;
		g_cSpellSlot++;
		bSomethingHappened = true;
		
	}
	if (g_adBounceTime[K_O] < g_dElapsedTime && g_abKeyPressed[K_O] && g_cSpellSlot <= 2)
	{
		g_aeSpell[g_cSpellSlot] = SC_AIR;
		g_cSpellSlot++;
		bSomethingHappened = true;
		
	}
	if (g_adBounceTime[K_J] < g_dElapsedTime && g_abKeyPressed[K_J] && g_cSpellSlot <= 2)
	{
		g_aeSpell[g_cSpellSlot] = SC_WATER;
		g_cSpellSlot++;
		bSomethingHappened = true;
		
	}
	if (g_adBounceTime[K_K] < g_dElapsedTime && g_abKeyPressed[K_K] && g_cSpellSlot <= 2)
	{
		g_aeSpell[g_cSpellSlot] = SC_EARTH;
		g_cSpellSlot++;
		bSomethingHappened = true;
		
	}
	if (g_adBounceTime[K_L] < g_dElapsedTime && g_abKeyPressed[K_L] && g_cSpellSlot <= 2)
	{
		
		g_aeSpell[g_cSpellSlot] = SC_ICE;
		g_cSpellSlot++;
		bSomethingHappened = true;
	}
	if (g_adBounceTime[K_SPACE] < g_dElapsedTime && g_abKeyPressed[K_SPACE])
	{
		if(g_aeSpell[0] !=  SC_NONE)
		{
			SSpell *sSpell = g_sSpells->lookupSpell(g_aeSpell);
			if(sSpell == nullptr)
			{
				sendMessage("Your spell fizzles into nothing.");
				_PLAY_SOUND(L"hiss.wav")
			}
			else
			{
				sSpell->executeSpell();
			}
			g_aeSpell[0] = SC_NONE;
			g_aeSpell[1] = SC_NONE;
			g_aeSpell[2] = SC_NONE;
			g_aeSpell[3] = SC_NONE;
			g_cSpellSlot = 0;
			bSomethingHappened = true;
		}
	}
    if (bSomethingHappened)
    {
		if (g_bPlayerMoved && (g_sChar.m_sFacingX || g_sChar.m_sFacingY))
		{
			COORD cNewLocation = {g_sChar.m_cLocation.X, g_sChar.m_cLocation.Y};
			cNewLocation.X += g_sChar.m_sFacingX;
			cNewLocation.Y += g_sChar.m_sFacingY;
			if(!playerMove(&cNewLocation) && g_bPlayerMovedLastTurn)
			{
				COORD cNewLocationX = {g_sChar.m_cLocation.X, g_sChar.m_cLocation.Y};
				COORD cNewLocationY = {g_sChar.m_cLocation.X, g_sChar.m_cLocation.Y};
				cNewLocationY.Y += g_sChar.m_sFacingY;
				playerMove(&cNewLocationY);
				cNewLocationX.X += g_sChar.m_sFacingX;
				playerMove(&cNewLocationX);
			}
		}
        // set the bounce time to some time in the future to prevent accidental triggers
        for(int i = 0; i < K_COUNT; i++)
		{
			if(g_abKeyPressed[i]) g_adBounceTime[i] = g_dElapsedTime + (i >= K_U?1/8.0:1/15.0);
		}
		g_bPlayerMovedLastTurn = g_bPlayerMoved;
	}
}

void processUserInput()
{
	bool bSomethingHappened = false ;
    // quits the game if player hits the escape key
    if (g_abKeyPressed[K_ESCAPE])
        g_bQuitGame = true;    
	if (g_abKeyPressed[K_E] && g_dElapsedTime > g_adBounceTime[K_E])
	{
		if (g_eGameState == S_INVENTORY)
		{
			updateEquipmentStats();
			g_eGameState = S_GAME;
		}
		else if (g_eGameState == S_GAME)
		{
			g_eGameState = S_INVENTORY;
		}
		g_adBounceTime[K_E] = g_dElapsedTime + 0.2;
	}
}

void clearScreen()
{
    // Clears the buffer with this colour attribute
    g_Console.clearBuffer(0x0F);
}

void renderSplashScreen()  // renders the splash screen
{
    COORD c = g_Console.getConsoleSize();
    c.Y /= 3;
    c.X /= 2;
	for (short s = 0; s < 35; s++)
	{
		if (s < 10)
		{
			g_Console.writeToBuffer(COORD{ 0, s }, *(g_asTitle[s]), 0x0C);
		}
		else
		{
			g_Console.writeToBuffer(COORD{ 0, s }, *(g_asTitle[s]), 0x09);
		}
	}
	g_Console.writeToBuffer(COORD{ 38, 15 }, "START", 0x0F);
	g_Console.writeToBuffer(COORD{ 35, 18 }, "HOW TO PLAY", 0x0F);
	g_Console.writeToBuffer(COORD{ 38,21 }, "EXIT", 0x0F);
	mainMenuCursor();
}

void renderItems()
{
}

void renderEnemies()
{
	for(SEntity *ppsCurrent : g_sLevel->m_sEnemies)
	{
		if(ppsCurrent == nullptr) continue;
		if(g_sVisible->getTileVisibility(ppsCurrent->m_cLocation)) g_Console.writeToBuffer(ppsCurrent->m_cLocation, ppsCurrent->m_cMonsterClass, ppsCurrent->m_cColor);  
	}
}

unsigned char getSpellColor(ESpellComponents eComponent)
{
	switch(eComponent)
	{
	case SC_FIRE:
		return 0xC0;
	case SC_LIGHTNING:
		return 0xE0;
	case SC_WATER:
		return 0x90;
	case SC_EARTH:
		return 0xA0;
	case SC_AIR:
		return 0xF0;
	case SC_ICE:
		return 0xB0;
	default:
		return 0x00;
	}
}

void renderSpell()
{
	g_Console.writeToBuffer(COORD {0, 28}, "Spell:", 0x07);
	g_Console.writeToBuffer(COORD {7, 28}, "O", getSpellColor(g_aeSpell[0]));
	g_Console.writeToBuffer(COORD {8, 28}, "O", getSpellColor(g_aeSpell[1]));
	g_Console.writeToBuffer(COORD {9, 28}, "O", getSpellColor(g_aeSpell[2]));
}

void writeToBuffer(COORD sA, char cChar, unsigned char cColor)
{
	g_Console.writeToBuffer(sA, cChar, cColor);
}

void renderEffects(int i)
{
	g_sEffects->renderAllEffects(i);
	g_sEffects->clearExpiredEffects();
}

void renderNonVisibility()
{	
	for(short i = 0; i < 80 * 28; i++)
	{
		if(!(g_sLevel->m_sExplored->getTileVisibility(COORD{i%80, i/80}))) writeToBuffer(COORD{i % 80, i / 80}, ' ', 0x08);
	}
}

void renderGame()
{
    renderMap();        // renders the map to the buffer first
	renderEffects(-1);
	renderItems();      // then overwrites item locations to buffer next
	renderEffects(0);
	renderEnemies();    // then renders enemies
	renderEffects(1);
    renderCharacter();  // then renders the character into the buffer
	renderEffects(2);
	renderMessages();   // then renders messages
	renderSpell();
	renderNonVisibility();
	renderStatus();		// then renders the status
}

char messageColourFromTime(double dTimeDiff)
{
	if(dTimeDiff <= 0)
		return 0x00;
	if(dTimeDiff < 2.5)
		return 0x08;
	if(dTimeDiff < 5.0)
		return 0x07;
	return 0x0F;
}

void sendMessage(std::string sStringMessage)
{
	if(g_psMessages == nullptr) g_psMessages = new SMessage(sStringMessage, 10.0);
	else g_psMessages = g_psMessages->addNewMessage(sStringMessage, 10.0);

}

void renderMessages()
{
	SMessage *psCurrentMessage = g_psMessages;
	for(unsigned char i = 0; i < 6; i++)
	{
		if(psCurrentMessage == nullptr) return;
		g_Console.writeToBuffer(COORD{0, 34-i}, psCurrentMessage->m_sStringMessage, messageColourFromTime(psCurrentMessage->m_dExpiryTime - g_dElapsedTime)); 
		psCurrentMessage = psCurrentMessage->m_psNext;
	}
}  

void renderStatus()
{
	// [!] TODO: draw the player's health and stats in the bottom right part of the screen
	// [!] NICE TO HAVE: a health BAR
	std::stringstream ss;

	g_Console.writeToBuffer(COORD{45, 28}, "Level:");
	ss << g_sChar.m_iLevel;
	g_Console.writeToBuffer(COORD{55, 28}, ss.str());
	ss.str("");
	g_Console.writeToBuffer(COORD{55, 27}, "EXP:");
	ss << g_sChar.m_iExperience << " / " << g_sChar.m_iMaxEXP;
	g_Console.writeToBuffer(COORD{60, 27}, ss.str());
	ss.str("");
	//Health
	g_Console.writeToBuffer(COORD{13, 27}, "Health:");
	std::string sTemp;
	sTemp = "";
	ss.str("");
	ss << g_sChar.m_iHealth << " / " << g_sChar.m_iMaxPlayerHealth;
	sTemp = ss.str();
	ss.str("");
	ss << std::left<< std::setw(10) << sTemp;
	g_Console.writeToBuffer(COORD{ 21 , 27 }, ss.str(), 0x0F);
	g_Console.writeToBuffer(COORD{ 21 , 27 }, ss.str().substr(0, (10 * g_sChar.m_iHealth / g_sChar.m_iMaxPlayerHealth)), 0xCF);
	ss.str("");
	//Mana
	g_Console.writeToBuffer(COORD{35, 27}, "Mana:");
	std::string sManaT;
	sManaT = "";
	ss.str("");
	ss << g_sChar.m_iMana << " / " << g_sChar.m_iMaxPlayerMana;
	sManaT = ss.str();
	ss.str("");
	ss << std::left << std::setw(10) << sManaT;
	g_Console.writeToBuffer(COORD{41, 27}, ss.str(),0x0F);
	g_Console.writeToBuffer(COORD{ 41 , 27 }, ss.str().substr(0, (10 * g_sChar.m_iMana/ g_sChar.m_iMaxPlayerMana)), 0x1F);
	ss.str("");
	g_Console.writeToBuffer(COORD{45, 29}, "Attack:");
	ss << g_sChar.m_iAttack << " (Base: " << g_sChar.m_iMaxPlayerAttack << ")";
	g_Console.writeToBuffer(COORD{55, 29}, ss.str());
	ss.str("");
	g_Console.writeToBuffer(COORD{45, 30}, "Defense:");
	ss << g_sChar.m_iDefense << " (Base: " << g_sChar.m_iMaxPlayerDefense << ")";
	g_Console.writeToBuffer(COORD{55, 30}, ss.str());
	ss.str("");
	g_Console.writeToBuffer(COORD{ 45,31 }, "Score:");
	ss << g_sChar.m_iScore;
	g_Console.writeToBuffer(COORD{ 55,31 }, ss.str());
	ss.str("");
}

void renderInstruct()
{
	for (short s = 0;s < 35;s++)
	{
		g_Console.writeToBuffer(COORD{ 0,s }, *(g_asHowtoPlay[s]), 0x0F);
	}
}

void renderGameOver()
{
	if (g_bifOver == false)
	{
		leaderboard(g_sChar.m_iScore);
		g_bifOver = true;
	}
	for (short s = 0; s < 35; s++)
	{
		g_Console.writeToBuffer(COORD{ 10,s + 3 }, *(g_asGameOverscreen[s]), 0x0C);
	}
	g_Console.writeToBuffer(COORD{ 33, 10 }, "LEADERBOARD", 0x0C);
	for (short i = 0; i < 10; i++)
	{
		g_Console.writeToBuffer(COORD{ i==9?9:10,i*2 + 11 }, std::to_string(i+1), 0x0C);
		g_Console.writeToBuffer(COORD{ 60,i*2 + 11 }, std::to_string(g_iHighscore[i]), 0x0C);
	}
	g_Console.writeToBuffer(COORD{ 30,32 }, "PRESS ENTER TO EXIT", 0x0C);
}
void renderWin()
{
	for (short s = 0; s < 35; s++)
	{
		g_Console.writeToBuffer(COORD{ 10,s + 3 }, *(g_asWinScreen[s]), 0x0A);
	}
	g_Console.writeToBuffer(COORD{ 33,10 }, "LEADERBOARD", 0X0A);
	for (short i = 0; i < 10; i++)
	{
		g_Console.writeToBuffer(COORD{ i == 9 ? 9 : 10,i * 2 + 11 }, std::to_string(i + 1), 0x0A);
		g_Console.writeToBuffer(COORD{ 60,i * 2 + 11 }, std::to_string(g_iHighscore[i]), 0x0A);
	}
	g_Console.writeToBuffer(COORD{ 30,32 }, "PRESS ENTER TO EXIT", 0x0A);
}

void renderMap()
{
	for(SHORT i = 0; i < 80 * 28; i++)
	{
		g_Console.writeToBuffer(COORD{i%80, i/80}, g_sLevel->getFeatureAt(i%80,i/80)->getMapChar(), g_sVisible->getTileVisibility(COORD{i%80,i/80})?g_sLevel->getFeatureAt(i%80,i/80)->getMapColor():(g_sLevel->getFeatureAt(i%80,i/80)->getMapColor()%16 != 0?0x08:0x80));
	}
}

void renderCharacter()
{
    // Draw the location of the character
    WORD charColor = 0x0C;
    if (g_sChar.m_bActive)
    {
        charColor = 0x0A;
    }
    g_Console.writeToBuffer(g_sChar.m_cLocation, '@', charColor);
}

void renderInventory()
{
	for (short s = 0; s < 35; s++)
	{
		g_Console.writeToBuffer({ 0,s }, *(g_asInventoryScreen[s]), 0x0F);
	}
	COORD c = { 11, 17 };
	for (int i = ((g_sChar.m_iInventoryPage - 1) * 8) + 6; i < g_sChar.m_iInventoryPage * 8 + 6; i++)
	{
		if (g_sChar.m_sInventory->m_asContents[i] == nullptr) continue;
		g_Console.writeToBuffer(c, g_sChar.m_sInventory->m_asContents[i]->m_cDroppedIcon, g_sChar.m_sInventory->m_asContents[i]->m_cDroppedColour);
		c.X++;
		g_Console.writeToBuffer(c, g_sChar.m_sInventory->m_asContents[i]->m_sName);
		c.Y += 2;
 		c.X--;
	}
	for (int i = 0; i < 6; i++)
	{
		if (g_sChar.m_sInventory->m_asContents[i] == nullptr)
		{
			continue;
		}
		else
		{
			COORD c = { 67, 12 };
			g_Console.writeToBuffer({ (SHORT)(c.X - (i * 11) - (g_sChar.m_sInventory->m_asContents[i]->m_sEquippedName1.length() / 2)), c.Y }, g_sChar.m_sInventory->m_asContents[i]->m_sEquippedName1);
			g_Console.writeToBuffer({ (SHORT)(c.X - (i * 11) - (g_sChar.m_sInventory->m_asContents[i]->m_sEquippedName2.length() / 2)), c.Y + 1 }, g_sChar.m_sInventory->m_asContents[i]->m_sEquippedName2);
		}
	}
	for (int i = (((g_sChar.m_iInventoryPage - 1) * 8)+1); i < (g_sChar.m_iInventoryPage * 8)+1; i++)
	{
		COORD c = { 9, 17 };
		std::string sInventoryNumber = std::to_string(i) + ".";
		c.Y += ((i-1)%8)*2;
		if (i < 10)
		{
			g_Console.writeToBuffer(c, sInventoryNumber, 0x0f);
		}
		else
		{
			g_Console.writeToBuffer(c.X - 1, c.Y, sInventoryNumber, 0x0f);
		}
	}
	if (g_sChar.m_sInventory->m_asContents[g_sChar.m_iInventoryIndex] != nullptr)
	{
		renderItemStats(g_sChar.m_iInventoryIndex);
	}
	gameplayInventory();
}

void renderItemStats(int itemIndex)
{
	COORD c = { 52, 19 };
	c.X = c.X - g_sChar.m_sInventory->m_asContents[itemIndex]->m_sHealth.length() / 2; // Centering the text under the stats
	g_Console.writeToBuffer(c, g_sChar.m_sInventory->m_asContents[itemIndex]->m_sHealth); // Writes health modifier of the currently selected equipment in the inventory
	c.X = 68;
	c.X = c.X - g_sChar.m_sInventory->m_asContents[itemIndex]->m_sMana.length() / 2;
	g_Console.writeToBuffer(c, g_sChar.m_sInventory->m_asContents[itemIndex]->m_sMana);
	c = { 52, 21 };
	c.X = c.X - g_sChar.m_sInventory->m_asContents[itemIndex]->m_sAttack.length() / 2;
	g_Console.writeToBuffer(c, g_sChar.m_sInventory->m_asContents[itemIndex]->m_sAttack);
	c.X = 68;
	c.X = c.X - g_sChar.m_sInventory->m_asContents[itemIndex]->m_sDefense.length() / 2;
	g_Console.writeToBuffer(c, g_sChar.m_sInventory->m_asContents[itemIndex]->m_sDefense);
	c = { 59, 23 };
	c.X = c.X - g_sChar.m_sInventory->m_asContents[itemIndex]->m_sSpecial1.length() / 2;
	g_Console.writeToBuffer(c, g_sChar.m_sInventory->m_asContents[itemIndex]->m_sSpecial1);
	c = { 59, 24 };
	c.X = c.X - g_sChar.m_sInventory->m_asContents[itemIndex]->m_sSpecial2.length() / 2;
	g_Console.writeToBuffer(c, g_sChar.m_sInventory->m_asContents[itemIndex]->m_sSpecial2);
	c = { 59, 25 };
	c.X = c.X - g_sChar.m_sInventory->m_asContents[itemIndex]->m_sSpecial3.length() / 2;
	g_Console.writeToBuffer(c, g_sChar.m_sInventory->m_asContents[itemIndex]->m_sSpecial3);
	c = { 59, 26 };
	c.X = c.X - g_sChar.m_sInventory->m_asContents[itemIndex]->m_sSpecial4.length() / 2;
	g_Console.writeToBuffer(c, g_sChar.m_sInventory->m_asContents[itemIndex]->m_sSpecial4);
	c = { 60, 28 };
	c.X = c.X - g_sChar.m_sInventory->m_asContents[itemIndex]->m_sDescription1.length() / 2;
	g_Console.writeToBuffer(c, g_sChar.m_sInventory->m_asContents[itemIndex]->m_sDescription1);
	c = { 60, 29 };
	c.X = c.X - g_sChar.m_sInventory->m_asContents[itemIndex]->m_sDescription2.length() / 2;
	g_Console.writeToBuffer(c, g_sChar.m_sInventory->m_asContents[itemIndex]->m_sDescription2);
	c = { 60, 30 };
	c.X = c.X - g_sChar.m_sInventory->m_asContents[itemIndex]->m_sDescription3.length() / 2;
	g_Console.writeToBuffer(c, g_sChar.m_sInventory->m_asContents[itemIndex]->m_sDescription3);
	if (itemIndex <= 5)
	{
		g_Console.writeToBuffer({ (SHORT)(40 - (g_sChar.m_sInventory->m_asContents[itemIndex]->m_sName.length() / 2)), 3 }, g_sChar.m_sInventory->m_asContents[itemIndex]->m_sName);
	}
}

void renderFramerate()
{
    COORD c;
    // displays the framerate
    std::ostringstream ss;
    ss << std::fixed << std::setprecision(3);
    ss << 1.0 / g_dDeltaTime << "fps";
    c.X = g_Console.getConsoleSize().X - 9;
    c.Y = 0;
    g_Console.writeToBuffer(c, ss.str());
	// displays the elapsed time
    ss.str("");
    ss << g_dElapsedTime << "secs";
    c.X = 0;
    c.Y = 0;
    g_Console.writeToBuffer(c, ss.str(), 0x59);
}
void renderToScreen()
{
    // Writes the buffer to the console, hence you will see what you have written
    g_Console.flushBufferToConsole();
}

SGameChar::~SGameChar()
{
	if (this->m_sInventory != nullptr)
	{
		delete this->m_sInventory;
	}
}
void SGameChar::takeDamage(SDamagePacket * sDamage)
{	
	m_iHealth -= sDamage->m_iDamage; 
	sDamage->printHitMessage();
	if(m_iHealth <= 0) die();
	_PLAY_SOUND(L"hit.wav")
}