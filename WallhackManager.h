#pragma once
#include "Memory.h"
#include "Player.h"
#include "Vector2.h"
class WallhackManager
{
private:
	Memory* memoryManager;
	int windowWidth;
	int windowHeight;
	void UpdateViewMatrix();
	void UpdatePlayersPosition();
	Vector2 WorldToScreen(float x, float y, float z);
public:
	bool Debug = false;
	float ViewMatrix[16];
	Player players[8];
	WallhackManager(Memory* memoryManager);
	~WallhackManager();
	void Update();
};