#include "Main.h"
#include "Memory.h"
#include "DirectOverlay.h"
#include "WallhackManager.h"

HWND hwnd;
Memory* memory;
WallhackManager* wallhackManager;

const int MENU_FONT_SIZE = 18;

void main()
{
	Main* m = new Main();
	m->main();
}

void Main::main()
{
	hwnd = FindWindowW(NULL, L"Assassin's Creed IV Black Flag");
	if (hwnd == NULL)
	{
		std::cout << "Could not find Assassin's Creed IV Black Flag window\n";
		std::cout << "Open the game first, then restart this program";
		getchar();
	}
	else
	{
		createMemoryManager();
		createWallhackManager();
		initWindowDrawing();
		std::cout << "Have fun!\n";
		//The overlay operates in it's own thread, ours will continue as normal
		while (true)
		{
			wallhackManager->Update();
			Sleep(15);
		}
	}
}

void Main::createMemoryManager()
{
	memory = new Memory(hwnd);
}

void Main::createWallhackManager()
{
	wallhackManager = new WallhackManager(memory);
}

// our loop to render in
void drawLoop(int width, int height)
{
	//Draw menu
	DrawString("Simple Wallhack by GramBezEndu", MENU_FONT_SIZE, 0, 0, 255, 255, 0);
	//DrawString("[F1] Toggle WH", MENU_FONT_SIZE, 0, 20, 255, 255, 0);
	//DrawString("[F2] Debug info", MENU_FONT_SIZE, 0, 40, 255, 255, 0);

	//[DEBUG] Print view matrix info
	if (wallhackManager->Debug)
	{
		DrawString("View matrix:\n", MENU_FONT_SIZE, 0, 60, 255, 255, 0);
		for (int i = 0; i < 16; i++)
		{
			DrawString(std::to_string(wallhackManager->ViewMatrix[i]), MENU_FONT_SIZE, 0, 80 + i * 20, 255, 255, 0);
		}

	}
	//Draw WH
	for (int i = 0; i < 8; i++)
	{
		DrawString("PLAYER", MENU_FONT_SIZE, wallhackManager->players[i].OnScreenPosition.X, wallhackManager->players[i].OnScreenPosition.Y, 255, 255, 0);
	}
}

void Main::initWindowDrawing()
{
	DWORD windowStyle = (DWORD)GetWindowLong(hwnd, GWL_STYLE);
	//Add WS_DISABLED, WS_CHILD
	SetWindowLongPtr(hwnd, GWL_STYLE, windowStyle | WS_DISABLED | WS_CHILD);

	DirectOverlaySetOption(D2DOV_DRAW_FPS | D2DOV_FONT_IMPACT);	// set the font, and draw the fps
	DirectOverlaySetup(drawLoop, hwnd);		// initialize our overlay
}