#include "Memory.h"

Memory::Memory(HWND hwnd)
{
	this->hwnd = hwnd;
	//Get process id
	GetWindowThreadProcessId(this->hwnd, &proccessID);
	if (proccessID == 0)
	{
		std::cout << GetLastError();
	}
	if (Debug)
	{
		std::cout << "[DEBUG] Process ID: " << proccessID << "\n";
	}

	//Get process handle
	this->hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, proccessID);
	if (hProcess == NULL)
	{
		std::cout << GetLastError();
	}
	//Get base address
	string processName = "AC4BFMP.exe";
	ModuleBaseAddress = GetModuleBaseAddress();
}


Memory::~Memory()
{
}

uintptr_t Memory::GetModuleBaseAddress()
{
	HMODULE hMods[1024];
	DWORD cbNeeded;

	// Get a list of all the modules in process
	if (EnumProcessModules(this->hProcess, hMods, sizeof(hMods), &cbNeeded))
	{
		for (int i = 0; i < (cbNeeded / sizeof(HMODULE)); i++)
		{
			TCHAR szModName[MAX_PATH];

			// Get the module name

			if (GetModuleBaseName(hProcess, hMods[i], szModName,
				sizeof(szModName) / sizeof(TCHAR)))
			{
				// Print the module name and handle value.
				if (_tcscmp(szModName, "AC4BFMP.exe") == 0)
				{
					if (Debug)
					{
						cout << "[DEBUG] Module name: " << szModName << "\n";
						cout << "[DEBUG] Module base address: " << hMods[i] << "\n";
						return (uintptr_t)hMods[i];
					}
				}
			}
		}
	}
}

int Memory::ReadInt(unsigned int address)
{
	int buffer = 0;
	SIZE_T NumberOfBytesToRead = sizeof(buffer); //this is equal to 4
	SIZE_T NumberOfBytesActuallyRead;
	BOOL success = ReadProcessMemory(this->hProcess, (LPCVOID)address, &buffer, NumberOfBytesToRead, &NumberOfBytesActuallyRead);
	if (!success || NumberOfBytesActuallyRead != NumberOfBytesToRead) 
	{
		if (Debug)
		{
			std::cout << "Memory Error!" << std::endl;
		}
		return -1;
	}
	return buffer;
}

int Memory::GetPointerAddress(int startAddress, int offsets[], int offsetCount)
{
	if (startAddress == -1)
		return -1;
	int ptr = ReadInt(startAddress);
	for (int i = 0; i < offsetCount - 1; i++) {
		ptr += offsets[i];
		ptr = ReadInt(ptr);
	}
	ptr += offsets[offsetCount - 1];
	return ptr;
}

float Memory::ReadFloat(unsigned int address) {
	if (address == -1)
		return -1;
	float buffer = 0.0;
	SIZE_T NumberOfBytesToRead = sizeof(buffer); //this is equal to 4
	SIZE_T NumberOfBytesActuallyRead;
	BOOL success = ReadProcessMemory(this->hProcess, (LPCVOID)address, &buffer, NumberOfBytesToRead, &NumberOfBytesActuallyRead);
	if (!success || NumberOfBytesActuallyRead != NumberOfBytesToRead)
		return -1;
	return buffer;
}