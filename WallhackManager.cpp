#include "WallhackManager.h"



void WallhackManager::UpdateViewMatrix()
{
	int viewMatrixOffsets[2];
	viewMatrixOffsets[0] = 0x200;
	viewMatrixOffsets[1] = 0xB0;
	int viewMatrixAddressBeginning = memoryManager->GetPointerAddress(memoryManager->ModuleBaseAddress + 0x016E162C, viewMatrixOffsets, 2);
	for (int i = 0; i < 16; i++)
	{
		ViewMatrix[i] = memoryManager->ReadFloat(viewMatrixAddressBeginning + i * 0x04);
	}
}

void WallhackManager::UpdatePlayersPosition()
{
	for (int i = 0; i < 8; i++)
	{
		int offsets[4];
		offsets[0] = 0x24;
		offsets[1] = 0x0;
		offsets[2] = 0xC0;
		offsets[3] = 0x0 + 0xF0 * i;
		int positionBeginning = memoryManager->GetPointerAddress(memoryManager->ModuleBaseAddress + 0x01619FE0, offsets, 4);
		players[i].X = memoryManager->ReadFloat(positionBeginning);
		players[i].Y = memoryManager->ReadFloat(positionBeginning + 0x04);
		players[i].Z = memoryManager->ReadFloat(positionBeginning + 0x08);
		//Update on screen position
		players[i].OnScreenPosition = WorldToScreen(players[i].X, players[i].Y, players[i].Z);
	}
}

Vector2 WallhackManager::WorldToScreen(float x, float y, float z)
{
	int pointX, pointY;
	float lx = x * ViewMatrix[0] + y * ViewMatrix[4] + z * ViewMatrix[8] + ViewMatrix[12];
	float ly = x * ViewMatrix[1] + y * ViewMatrix[5] + z * ViewMatrix[9] + ViewMatrix[13];
	float lz = x * ViewMatrix[2] + y * ViewMatrix[6] + z * ViewMatrix[10] + ViewMatrix[14];
	float lw = x * ViewMatrix[3] + y * ViewMatrix[7] + z * ViewMatrix[11] + ViewMatrix[15];

	if (lw < 0.1f)
	{
		return Vector2(-500, -500);
	}

	//perspective division, dividing by clip.W = Normalized Device Coordinates
	//vec3_t NDC;
	float x2 = lx / lw;
	float y2 = ly / lw;
	float z2 = lz / lw;

	pointX = (int)((windowWidth / 2 * x2) + (x2 + windowWidth / 2));
	pointY = (int)(-(windowHeight / 2 * y2) + (y2 + windowHeight / 2));

	return Vector2(pointX, pointY);
}

WallhackManager::WallhackManager(Memory* memoryManager)
{
	this->memoryManager = memoryManager;
	this->windowWidth = memoryManager->ReadInt(memoryManager->ModuleBaseAddress + 0x16FF4CC);
	this->windowHeight = memoryManager->ReadInt(memoryManager->ModuleBaseAddress + 0x16FF4D0);
	if (Debug)
	{
		//Print window dimensions
		cout << "[DEBUG] AC4 Window dimensions: " << windowWidth << " x " << windowHeight << "\n";
	}
}


WallhackManager::~WallhackManager()
{

}

void WallhackManager::Update()
{
	UpdateViewMatrix();
	UpdatePlayersPosition();
}
