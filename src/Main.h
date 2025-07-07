#pragma once

#include <Windows.h>

#define MY_NOTIFY (WM_USER+1)

#define IDM_SHOW (WM_USER+2)
#define IDM_HIDE (WM_USER+3)
#define IDM_EXIT (WM_USER+4)

#define NAME "Console Wrapper"

inline HICON hIcon;
inline HWND hWindowWnd;
inline HWND hConsoleWnd;
inline HINSTANCE hInstance;
inline WNDCLASSEX wcex{0};
inline NOTIFYICONDATA data;

inline HANDLE workerThreadHandle;
inline DWORD workerThreadId;

inline HMENU hMenu;

void showLastError();

LRESULT CALLBACK wndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

void loadIcon(LPCSTR path);

void loadIcon(WORD id);

void regClass(LPCSTR className);

void initWnd(LPCSTR className, LPCSTR title);

void createNotify(LPCSTR title);

void releaseNotify();

void windowLoop();

void execCmd(LPCSTR cmd);
