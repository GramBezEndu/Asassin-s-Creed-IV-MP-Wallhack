#pragma once
#include "DirectOverlay.h"
#include <Psapi.h>
#include <TlHelp32.h>
#include <iostream>
#include <tchar.h>
using namespace std;
using std::string;
class Memory
{
private:
	HWND hwnd;
	DWORD proccessID;
	HANDLE hProcess;
	uintptr_t GetModuleBaseAddress();
public:
	bool Debug = true;
	DWORD ModuleBaseAddress;
	Memory(HWND handle);
	~Memory();
	int GetPointerAddress(int startAddress, int offsets[], int offsetCount);
	float ReadFloat(unsigned int address);
	int ReadInt(unsigned int address);
};