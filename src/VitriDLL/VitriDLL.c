/*
Copyright 2002 Ryan VanMiddlesworth

This file is part of Vitrite.

Vitrite is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

Vitrite is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Vitrite; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

// This DLL requires Windows 2000 or newer
#define _WIN32_WINNT 0x0500
#define WINVER 0x0500

#include <windows.h>
#include <winuser.h>
#include "vitridll.h"


// This little magic sets up a globally shared variable - one
// which all instances of the .DLL can read and write to.
#pragma data_seg(".shared")
HHOOK  hCurrentHook=0;
#pragma data_seg()
#pragma comment(linker, "/SECTION:.shared,RWS")

// Globals
BOOL  g_bHookSet;
HANDLE  g_hInstance;

LRESULT CALLBACK KbHookProc(int nCode, WPARAM wParam, LPARAM lParam) {
	int  iTenths=-1;
	HWND  hActiveWindow;
	WINDOWINFO  wInfo;

	char cString[155]="";	// DEBUG
	if (nCode >= 0)	{
		if ((!(HIWORD(lParam) & KF_UP)) &&		// Is this a key down situation?
			(!(HIWORD(lParam) & KF_REPEAT)) &&	// Or a repeating key?
			(!(HIWORD(lParam) & KF_EXTENDED))) {// Or one on the numpad?
			if ((GetKeyState(VK_SHIFT) < 0) && (GetKeyState(VK_CONTROL) < 0)) {
				if ((wParam>=0x030) && (wParam<=0x039))	{// Is this a number key?
					iTenths = wParam - 0x030;
				} else if (wParam == VK_OEM_PLUS) {	// Is this the plus key?
					memset(&wInfo, 0, sizeof(wInfo));
					if ((hActiveWindow = GetForegroundWindow()) == NULL) {
						return (CallNextHookEx(hCurrentHook, nCode, wParam, lParam));
					}
					if (GetWindowInfo(hActiveWindow, &wInfo) == 0) {
						return (CallNextHookEx(hCurrentHook, nCode, wParam, lParam));
					}
					if (wInfo.dwExStyle &  WS_EX_TOPMOST) {	// Is this window topmost?
                        SetWindowPos(hActiveWindow, HWND_NOTOPMOST, 0, 0, 0, 0,
							SWP_NOMOVE | SWP_NOSIZE);
					} else {
						SetWindowPos(hActiveWindow, HWND_TOPMOST, 0, 0, 0, 0,
							SWP_NOMOVE | SWP_NOSIZE);
					}
				}
			}
		}
	} else {
		return (CallNextHookEx(hCurrentHook, nCode, wParam, lParam));
	}

	// Okay, let's change the active window's alphaness (is that a word?)
	if (iTenths >= 0) {
		if ((hActiveWindow = GetForegroundWindow()) == NULL) {
			return (CallNextHookEx(hCurrentHook, nCode, wParam, lParam));
		}		
		if (iTenths > 0) {
			// Set WS_EX_LAYERED on it
			SetWindowLong(hActiveWindow, GWL_EXSTYLE,
				GetWindowLong(hActiveWindow, GWL_EXSTYLE) | WS_EX_LAYERED);
			// Set the alpha value on it
			SetLayeredWindowAttributes(hActiveWindow, 0, 
				(BYTE)(((float)iTenths/10)*255), LWA_ALPHA);
			/*
			Remember to come back and play with the second parameter
			in SetLayeredWindowAttributes - it has to do with setting the 
			transparency color. Sounds fun.
			*/
		} else {	// iTenths == 0
			// Remove the transparency bit.
			// This seemed like a smarter use for the '0' key 
			// than making the window 100% transparent.
			SetWindowLong(hActiveWindow, GWL_EXSTYLE,
				GetWindowLong(hActiveWindow, GWL_EXSTYLE) & ~WS_EX_LAYERED);
			RedrawWindow(hActiveWindow, NULL, NULL, RDW_ERASE | RDW_INVALIDATE |
				RDW_FRAME | RDW_ALLCHILDREN);
		}
	}
	// Return 1 if we want to remove the key from the queue
	return (CallNextHookEx(hCurrentHook, nCode, wParam, lParam));
}

__declspec(dllexport) int APIENTRY InstallHook() { 
	// We return 0 on success; 1 or GetLastError() on failure
	// We can't set the hook more than once
	if (g_bHookSet)	{
		MessageBox(NULL, "You can't set the keyboard hook more than once.", 
			"Warning", MB_OK | MB_ICONEXCLAMATION);
		return 1;
	} else {
		hCurrentHook = SetWindowsHookEx(WH_KEYBOARD, KbHookProc,
			g_hInstance, 0);
		if (hCurrentHook == NULL)
			return GetLastError();
		else
			g_bHookSet = TRUE;
	}
	return 0;
}

__declspec(dllexport) int APIENTRY RemoveHook() {
// We return 0 on success; 1 or GetLastError() on failure
	if (g_bHookSet)	{
		if (UnhookWindowsHookEx(hCurrentHook) == 0)
			return GetLastError();
		else
			g_bHookSet = FALSE;
	} else {
		MessageBox(NULL, "You can't remove a hook which isn't set.",
			"Warning", MB_OK | MB_ICONEXCLAMATION);
		return 1;
	}
	return 0;
}

BOOL APIENTRY DllMain(HANDLE hMod, DWORD  fdwReason, LPVOID lpvReserved) {
	g_bHookSet = FALSE;
	g_hInstance = hMod;
	return TRUE;
}