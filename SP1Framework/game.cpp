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
char g_cSpellSlot = 0;
bool g_bPlayerMoved = true;
double  g_adBounceTime[K_COUNT] = {}; // this is to prevent key bouncing, so we won't trigger keypresses more than once

std::string* g_asInventoryScreen[35];
std::string* g_asTitle[35];

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
	//g_sChar.m_asInventory[16];
	g_sChar.m_iScore = 0;
	g_sEffects = new SRenderedEffectList();
	g_sVisible = new SVisibilityMap();
	g_bPlayerMoved = true;
	//Test spell
	g_sSpells = new SSpellNode();
	{ESpellComponents aeTemp[4] = {SC_FIRE, SC_NONE};
	SSpell * psSpell = new SSpellElementalBasic(100, E_FIRE, 1);
	g_sSpells->addSpellToTree(psSpell, aeTemp);}
	//Basic Fire
	{ESpellComponents aeTemp[4] = { SC_FIRE, SC_NONE };
	SSpell * psSpell = new SSpellElementalBasic(g_sChar.m_iAttack, E_FIRE, 4);
	g_sSpells->addSpellToTree(psSpell, aeTemp);}
	//Basic Water
	{ESpellComponents aeTemp[4] = { SC_WATER, SC_NONE };
	SSpell * psSpell = new SSpellElementalBasic(g_sChar.m_iAttack, E_WATER, 4);
	g_sSpells->addSpellToTree(psSpell, aeTemp);}
	//Basic Earth
	{ESpellComponents aeTemp[4] = { SC_EARTH, SC_NONE };
	SSpell * psSpell = new SSpellElementalBasic(g_sChar.m_iAttack, E_EARTH, 4);
	g_sSpells->addSpellToTree(psSpell, aeTemp);}
	//Basic Wind
	{ESpellComponents aeTemp[4] = { SC_AIR, SC_NONE };
	SSpell * psSpell = new SSpellElementalBasic(g_sChar.m_iAttack, E_AIR, 4);
	g_sSpells->addSpellToTree(psSpell, aeTemp);}
	//Basic Lightning
	{ESpellComponents aeTemp[4] = { SC_LIGHTNING, SC_NONE };
	SSpell * psSpell = new SSpellElementalBasic(g_sChar.m_iAttack, E_LIGHTNING, 4);
	g_sSpells->addSpellToTree(psSpell, aeTemp);}
	//Basic Ice
	{ESpellComponents aeTemp[4] = { SC_ICE, SC_NONE };
	SSpell * psSpell = new SSpellElementalBasic(g_sChar.m_iAttack, E_ICE, 4);
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
	SSpell * psSpell = new SSpellElementalBasic((double)g_sChar.m_iAttack*4, E_FIRE, 35+ ((double)g_sChar.m_iMaxPlayerMana / 100) * 3.5);
	g_sSpells->addSpellToTree(psSpell, aeTemp);}
	//Water Wave
	{ESpellComponents aeTemp[4] = { SC_WATER, SC_WATER, SC_NONE };
	SSpell * psSpell = new SSpellElementalBasic((double)g_sChar.m_iAttack*2, E_WATER, 30+ ((double)g_sChar.m_iMaxPlayerMana / 100) * 3);
	g_sSpells->addSpellToTree(psSpell, aeTemp);}
	//Quagmire
	{ESpellComponents aeTemp[4] = { SC_WATER, SC_EARTH, SC_NONE };
	SSpell * psSpell = new SSpellElementalBasic((double)g_sChar.m_iAttack, E_WATER, 40+ ((double)g_sChar.m_iMaxPlayerMana / 100) * 4);
	g_sSpells->addSpellToTree(psSpell, aeTemp);}
	//Hurricane
	{ESpellComponents aeTemp[4] = { SC_WATER, SC_AIR, SC_NONE};
	SSpell * psSpell = new SSpellElementalBasic((double)g_sChar.m_iAttack*2, E_AIR, 40 + ((double)g_sChar.m_iMaxPlayerMana / 100) * 3);
	g_sSpells->addSpellToTree(psSpell, aeTemp);}
	//Shockwave
	{ESpellComponents aeTemp[4] = { SC_WATER, SC_LIGHTNING,SC_NONE };
	SSpell * psSpell = new SSpellElementalBasic((double)g_sChar.m_iAttack*1.5, E_LIGHTNING, 25 + ((double)g_sChar.m_iMaxPlayerMana / 100) * 2.5);
	g_sSpells->addSpellToTree(psSpell, aeTemp);}
	//Ice Tomb
	{ESpellComponents aeTemp[4] = { SC_WATER, SC_ICE,SC_NONE};
	SSpell * psSpell = new SSpellElementalBasic((double)g_sChar.m_iAttack*3, E_WATER, 50 + ((double)g_sChar.m_iMaxPlayerMana / 100) * 5);
	g_sSpells->addSpellToTree(psSpell, aeTemp);}

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

    // sets the width, height and the font name to use in the console
    g_Console.setConsoleFont(0, 16, L"Consolas");
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
        case S_GAME: case S_INVENTORY: gameplay(); // gameplay logic when we are in the game
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
    }
    renderFramerate();  // renders debug information, frame rate, elapsed time, etc
    renderToScreen();   // dump the contents of the buffer to the screen, one frame worth of game
}

void splashScreenWait()    // waits for time to pass in splash screen
{
    if (g_abKeyPressed[K_SPACE]) 
	{
        g_eGameState = S_GAME;
		g_adBounceTime[K_SPACE] = g_dElapsedTime + 0.4;
	}
}

void gameplay()            // gameplay logic
{
    processUserInput(); // checks if you should change states or do something else with the game, e.g. pause, exit
    moveCharacter();    // moves the character, collision detection, physics, etc
	if(g_bPlayerMoved) 
	{
		g_sVisible = g_sLevel->tilesWithLineOfSight(g_sChar.m_cLocation);
		g_sLevel->m_sExplored->assimilate(g_sVisible);
		g_bPlayerMoved = false;
	}
	entityTurns();
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

void playerMove(COORD *cNewLocation)
{
	if(g_sLevel->canPlayerSeeEnemy(*cNewLocation) && g_sLevel->getFeatureAt(cNewLocation)->onMovedInto()) 
	{
		g_sChar.m_cLocation = *cNewLocation;
		g_bPlayerMoved = true;
	}
}

void moveCharacter()
{
    bool bSomethingHappened = false;
    // Updating the location of the character based on the key press
    // providing a beep sound whenver we shift the character
    if (g_adBounceTime[K_W] < g_dElapsedTime && g_abKeyPressed[K_W] && g_sChar.m_cLocation.Y > 0)
    {
        //Beep(1440, 30);
		COORD cNewLocation = {g_sChar.m_cLocation.X, g_sChar.m_cLocation.Y};
		cNewLocation.Y--;
		playerMove(&cNewLocation);
        bSomethingHappened = true;
    }
    if (g_adBounceTime[K_A] < g_dElapsedTime && g_abKeyPressed[K_A] && g_sChar.m_cLocation.X > 0)
    {
        //Beep(1440, 30);
		COORD cNewLocation = {g_sChar.m_cLocation.X, g_sChar.m_cLocation.Y};
		cNewLocation.X--;
		playerMove(&cNewLocation);
        bSomethingHappened = true;
    }
    if (g_adBounceTime[K_S] < g_dElapsedTime && g_abKeyPressed[K_S] && g_sChar.m_cLocation.Y < 27)
    {
        //Beep(1440, 30);
		COORD cNewLocation = {g_sChar.m_cLocation.X, g_sChar.m_cLocation.Y};
		cNewLocation.Y++;
		playerMove(&cNewLocation);
        bSomethingHappened = true;
    }
    if (g_adBounceTime[K_D] < g_dElapsedTime && g_abKeyPressed[K_D] && g_sChar.m_cLocation.X < 79)
    {
        //Beep(1440, 30);
		COORD cNewLocation = {g_sChar.m_cLocation.X, g_sChar.m_cLocation.Y};
		cNewLocation.X++;
		playerMove(&cNewLocation);
        bSomethingHappened = true;
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
        // set the bounce time to some time in the future to prevent accidental triggers
        for(int i = 0; i < K_COUNT; i++)
		{
			if(g_abKeyPressed[i]) g_adBounceTime[i] = g_dElapsedTime + (i >= K_U?1/8.0:1/15.0);
		}
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
		if(g_eGameState == S_INVENTORY)
		{
			g_eGameState = S_GAME;
		}
		else if(g_eGameState == S_GAME)
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
		g_Console.writeToBuffer(COORD{0, s}, *(g_asTitle[s]), 0x04);
	}
	c.Y += 3;
    g_Console.writeToBuffer(COORD {c.X - 11, c.Y}, "Welcome to Slash!", 0x03);
    c.Y += 1;
	g_Console.writeToBuffer(COORD {c.X - 13, c.Y}, "Press <Space> to start", 0x09);
    c.Y += 1;
	g_Console.writeToBuffer(COORD {c.X - 12, c.Y}, "Press <Esc> to quit", 0x09);
}

void renderItems()
{
	if (g_eGameState == S_INVENTORY)
	{
		COORD c = { 32, 13 };
		for (int i = 0; i < 16; i++)
		{
		/*	g_Console.writeToBuffer(COORD{ c.X, c.Y }, placeholderItem[i].m_cDroppedIcon, placeholderItem[i].m_cDroppedColour);
			c.X++;
			g_Console.writeToBuffer(COORD{ c.X, c.Y }, placeholderItem[i].m_sName);
			c.Y++;
			c.X--;*/
		}
	}
}

void renderItemStats(int itemIndex)
{
	COORD c = { 48, 17 };
	/*g_Console.writeToBuffer(COORD{ c.X, c.Y }, placeholderItem[itemIndex].healthModifier); // Writes health modifier of the currently selected equipment in the inventory
	COORD c = { 64, 17 };
	g_Console.writeToBuffer(COORD{ c.X, c.Y }, placeholderItem[itemIndex].manaModifier);
	COORD c = { 48, 19 };
	g_Console.writeToBuffer(COORD{ c.X, c.Y }, placeholderItem[itemIndex].attackModifier);
	COORD c = { 64, 19 };
	g_Console.writeToBuffer(COORD{ c.X, c.Y }, placeholderItem[itemIndex].defenseModifier);*/
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

void renderEffects()
{
	g_sEffects->renderAllEffects();
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
	renderItems();      // then overwrites item locations to buffer next
	renderEffects();
	renderEnemies();    // then renders enemies
    renderCharacter();  // then renders the character into the buffer
	renderStatus();		// then renders the status
	renderMessages();   // then renders messages
	renderSpell();
	renderNonVisibility();
	renderHighScore();  // renders the high score the player has
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

void renderHighScore()
{
	g_Console.writeToBuffer(COORD{45, 34}, "High Score:");
}

void renderStatus()
{
	// [!] TODO: draw the player's health and stats in the bottom right part of the screen
	// [!] NICE TO HAVE: a health BAR

	g_Console.writeToBuffer(COORD{45, 28}, "Level:");
	g_Console.writeToBuffer(COORD{45, 29}, "Health:");
	g_Console.writeToBuffer(COORD{45, 30}, "Mana:");
	g_Console.writeToBuffer(COORD{45, 31}, "Attack:");
	g_Console.writeToBuffer(COORD{45, 32}, "Defense:");
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
	for(short s = 0; s < 35; s++)
	{
		g_Console.writeToBuffer(COORD{0,s}, *(g_asInventoryScreen[s]), 0x0F);
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
