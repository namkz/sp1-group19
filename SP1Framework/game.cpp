// This is the main file for the game logic and function
//
//
#include "game.h"
#include "spell.h"
#include "Framework\console.h"
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
ESpellComponents g_aeSpell[3];
SDungeonLevel g_sLevel = {"Level.txt"};
EGAMESTATES g_eGameState = S_SPLASHSCREEN;
double  g_adBounceTime[K_COUNT] = {}; // this is to prevent key bouncing, so we won't trigger keypresses more than once
char g_bSpellSlot = 0;
std::string* g_asInventoryScreen[35];

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
    // Set precision for floating point output
    g_dElapsedTime = 0.0;

    // sets the initial state for the game
    g_eGameState = S_SPLASHSCREEN;

    g_sChar.m_cLocation.X = 9;
    g_sChar.m_cLocation.Y = 11;
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
	
	std::fstream inventoryFile;
	inventoryFile.open("inventory.txt");
	for (short i = 0; i < 35; i++)
	{
		g_asInventoryScreen[i] = new std::string;
		std::getline(inventoryFile, *g_asInventoryScreen[i]);
	}
	inventoryFile.close();

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
    if (g_abKeyPressed[K_SPACE]) // wait for 3 seconds to switch to game mode, else do nothing
        g_eGameState = S_GAME;
}

void gameplay()            // gameplay logic
{
    processUserInput(); // checks if you should change states or do something else with the game, e.g. pause, exit
    moveCharacter();    // moves the character, collision detection, physics, etc
	entityTurns();
                        // sound can be played here too.
}

void entityTurns()
{
	for(SEntity *ppsCurrent : g_sLevel.m_sEnemies)
	{
		if(ppsCurrent == nullptr) continue;
		if(ppsCurrent->m_dNextTurn > g_dElapsedTime) continue;
		ppsCurrent->takeTurn();
	}
}

void playerMove(COORD *cNewLocation)
{
	if(!g_sLevel.hasEnemy(*cNewLocation) && g_sLevel.getFeatureAt(cNewLocation)->onMovedInto()) g_sChar.m_cLocation = *cNewLocation;
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
	if (g_adBounceTime[K_U] < g_dElapsedTime && g_abKeyPressed[K_U] && g_bSpellSlot <= 2)
	{
		g_aeSpell[g_bSpellSlot] = SC_U;
		g_bSpellSlot++;
		bSomethingHappened = true;
	}
	if (g_adBounceTime[K_I] < g_dElapsedTime && g_abKeyPressed[K_I] && g_bSpellSlot <= 2)
	{
		g_aeSpell[g_bSpellSlot] = SC_I;
		g_bSpellSlot++;
		bSomethingHappened = true;
		
	}
	if (g_adBounceTime[K_O] < g_dElapsedTime && g_abKeyPressed[K_O] && g_bSpellSlot <= 2)
	{
		g_aeSpell[g_bSpellSlot] = SC_O;
		g_bSpellSlot++;
		bSomethingHappened = true;
		
	}
	if (g_adBounceTime[K_J] < g_dElapsedTime && g_abKeyPressed[K_J] && g_bSpellSlot <= 2)
	{
		g_aeSpell[g_bSpellSlot] = SC_J;
		g_bSpellSlot++;
		bSomethingHappened = true;
		
	}
	if (g_adBounceTime[K_K] < g_dElapsedTime && g_abKeyPressed[K_K] && g_bSpellSlot <= 2)
	{
		g_aeSpell[g_bSpellSlot] = SC_K;
		g_bSpellSlot++;
		bSomethingHappened = true;
		
	}
	if (g_adBounceTime[K_L] < g_dElapsedTime && g_abKeyPressed[K_L] && g_bSpellSlot <= 2)
	{
		
		g_aeSpell[g_bSpellSlot] = SC_L;
		g_bSpellSlot++;
		bSomethingHappened = true;
	}
	if (g_adBounceTime[K_SPACE] < g_dElapsedTime && g_abKeyPressed[K_SPACE])
	{

		g_aeSpell[0] = SC_NONE;
		g_aeSpell[1] = SC_NONE;
		g_aeSpell[2] = SC_NONE;
		g_bSpellSlot = 0;
		bSomethingHappened = true;
	}
    if (bSomethingHappened)
    {
        // set the bounce time to some time in the future to prevent accidental triggers
        for(int i = 0; i < K_COUNT; i++)
		{
			if(g_abKeyPressed[i]) g_adBounceTime[i] = g_dElapsedTime + (i >= K_U?1/8.0:1/12.0);
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
    g_Console.writeToBuffer(COORD {c.X - 14, c.Y}, "Welcome to [game name here]!", 0x03);
    c.Y += 1;
    g_Console.writeToBuffer(COORD {c.X - 17, c.Y}, "Still in early alpha. Don't judge.", 0x09);
    c.Y += 1;
	g_Console.writeToBuffer(COORD {c.X - 10, c.Y}, "Press <Esc> to quit", 0x09);
    c.Y += 1;
	g_Console.writeToBuffer(COORD {c.X - 12, c.Y}, "Press <Space> to start", 0x09);
}

void renderItems()
{

}

void renderEnemies()
{
	for(SEntity *ppsCurrent : g_sLevel.m_sEnemies)
	{
		if(ppsCurrent == nullptr) continue;
		g_Console.writeToBuffer(ppsCurrent->m_cLocation, ppsCurrent->m_cMonsterClass, ppsCurrent->m_cColor);  
	}

}

unsigned char getSpellColor(ESpellComponents eComponent)
{
	switch(eComponent)
	{
	case SC_U:
		return 0xC0;
	case SC_I:
		return 0xE0;
	case SC_J:
		return 0x90;
	case SC_K:
		return 0xA0;
	case SC_O:
		return 0xF0;
	case SC_L:
		return 0x80;
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


void renderGame()
{
    renderMap();        // renders the map to the buffer first
	renderItems();      // then overwrites item locations to buffer next
	renderEnemies();    // then renders enemies
    renderCharacter();  // then renders the character into the buffer
	renderStatus();		// then renders the status
	renderMessages();   // then renders messages
	renderSpell();
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
}


void renderMap()
{
	for(SHORT i = 0; i < 80 * 28; i++)
	{
		g_Console.writeToBuffer(COORD{i%80, i/80}, g_sLevel.getFeatureAt(i%80,i/80)->getMapChar(), g_sLevel.getFeatureAt(i%80,i/80)->getMapColor());
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
