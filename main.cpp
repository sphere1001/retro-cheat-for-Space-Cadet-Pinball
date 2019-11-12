// PinballCheat.cpp : Diese Datei enthält die Funktion "main". Hier beginnt und endet die Ausführung des Programms.
//

#include <iostream>
#include <Windows.h>


int main()
{
	HWND h = FindWindowA(0, "3D Pinball for Windows - Space Cadet");
	//we look for the window that says it all..
	if (h)
		printf("found window.\n");
	else
		printf("Window not found..\n");
	//error checking if the window got found
	DWORD p, pid;
	
	p = GetWindowThreadProcessId(h, &pid);
	//getting the process ID of the window
	if (p)
		printf("got process ID ,it is => %d\n", pid);
	else
		printf("could not get process ID.\n");
	//error checking if process ID got found
	HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS, false, pid);
	//open the process associated with the process ID
	if (handle)
		printf("opened process => %d\n", pid);
	else
		printf("could not open process.\n");
	//error checking again

	DWORD jmpaddress = 0x1017573;
	DWORD returnaddress = 0x1017578;
	//we make a jump on this address to our code cave and settle everything from returning to "returnaddress"
	BYTE jumpcode[] = { 0xe9,0x90,0x01,0xf7,0x00 };
	BYTE returncode[] = { 0x90 };
	//these are the bytes that translate into the actual jump instruction and returnaddress code
	DWORD caveAddress = 0x01f87708;
	DWORD jmpback = 0x01f8770e;
	//the address of the code cave and the address of the jump back point into regular execution flow
	BYTE codeCave[] = { 0x81,0xc6,0x10,0x27,0x00,0x00 };
	BYTE jmpbackcode[] = { 0xe9,0x65,0xfe,0x08,0xff };
	//the actual code cave bytes that are written to the memory at caveAddress and jmpback
	WriteProcessMemory(handle, (void*)jmpaddress, &jumpcode, sizeof(jumpcode), 0);
	WriteProcessMemory(handle, (void*)returnaddress, &returncode, sizeof(returncode), 0);
	WriteProcessMemory(handle, (void*)caveAddress, &codeCave, sizeof(codeCave), 0);
	WriteProcessMemory(handle, (void*)jmpback, &jmpbackcode, sizeof(jmpbackcode), 0);
	//the actual functions that execute writing to memory
	DWORD pinballbase = 0x01000000;
	DWORD offset = 0x00025040;
	//the base address of the pinball.exe and offset to the score value
	DWORD startaddress = pinballbase + offset;
	DWORD address;
	int score = 0;
	//preparing things
	while (true) {
		ReadProcessMemory(handle, (void*)startaddress, &address, sizeof(address), 0);
		printf("address at 0x%x is => 0x%x now.\n", startaddress, address);
		address += 0x52;
		//read memory at offset
		ReadProcessMemory(handle, (void*)address, &score, sizeof(score), 0);
		printf("Pinball Wizard score is at : %d\n", score);
		//finally read score value from address
		Sleep(1000);
	}

	return 0;
}


