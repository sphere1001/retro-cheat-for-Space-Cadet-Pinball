// PinballCheat.cpp : Diese Datei enthält die Funktion "main". Hier beginnt und endet die Ausführung des Programms.
//

#include <iostream>
#include <Windows.h>


int main()
{
	HWND h = FindWindowA(0, "3D Pinball for Windows - Space Cadet");

	if (h)
		printf("found window.\n");
	else
		printf("Window not found..\n");

	DWORD p, pid;

	p = GetWindowThreadProcessId(h, &pid);

	if (p)
		printf("got process ID ,it is => %d\n", pid);
	else
		printf("could not get process ID.\n");

	HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS, false, pid);

	if (handle)
		printf("opened process => %d\n", pid);
	else
		printf("could not open process.\n");


	DWORD jmpaddress = 0x1017573;
	DWORD returnaddress = 0x1017578;

	BYTE jumpcode[] = { 0xe9,0x90,0x01,0xf7,0x00 };
	BYTE returncode[] = { 0x90 };

	DWORD caveAddress = 0x01f87708;
	DWORD jmpback = 0x01f8770e;

	BYTE codeCave[] = { 0x81,0xc6,0x10,0x27,0x00,0x00 };
	BYTE jmpbackcode[] = { 0xe9,0x65,0xfe,0x08,0xff };

	WriteProcessMemory(handle, (void*)jmpaddress, &jumpcode, sizeof(jumpcode), 0);
	WriteProcessMemory(handle, (void*)returnaddress, &returncode, sizeof(returncode), 0);
	WriteProcessMemory(handle, (void*)caveAddress, &codeCave, sizeof(codeCave), 0);
	WriteProcessMemory(handle, (void*)jmpback, &jmpbackcode, sizeof(jmpbackcode), 0);

	DWORD pinballbase = 0x01000000;
	DWORD offset = 0x00025040;

	DWORD startaddress = pinballbase + offset;
	DWORD address;
	int score = 0;

	while (true) {
		ReadProcessMemory(handle, (void*)startaddress, &address, sizeof(address), 0);
		printf("address at 0x%x is => 0x%x now.\n", startaddress, address);
		address += 0x52;

		ReadProcessMemory(handle, (void*)address, &score, sizeof(score), 0);
		printf("Pinball Wizard score is at : %d\n", score);

		Sleep(1000);
	}


}


