#ifndef VITRIDLL_H
#define VITRIDLL_H

// Prototypes
LRESULT CALLBACK KbHookProc(int nCode, WPARAM wParam, LPARAM lParam);
__declspec(dllexport) int APIENTRY InstallHook();
__declspec(dllexport) int APIENTRY RemoveHook();
BOOL APIENTRY DllMain(HANDLE hMod, DWORD  fdwReason, LPVOID lpvReserved);

#endif	// VITRIDLL_H