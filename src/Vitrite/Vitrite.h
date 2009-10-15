#ifndef VITRITE_H
#define VITRITE_H

// Prototypes
BOOL AddIconToSystemTray(HWND hWnd, UINT uID, LPSTR lpszTip); 
BOOL RemoveIconFromSystemTray(HWND hWnd, UINT uID);
BOOL ShowPopupMenu(HWND hWnd, POINT pOint);
BOOL APIENTRY MainDlgProc(HWND hDlg, UINT Msg, UINT wParam, LONG lParam);
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine, int nCmdShow);

// Constants
#define TRAY_CALLBACK	10001

#define IDM_MEXIT		500
#define IDM_MMAIN		501

#define VITRITE_VERSION	"1.1.1"


#endif	// VITRITE_H
