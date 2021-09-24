#include "ConfigHelper.h"
#include <Windows.h>
#include <iostream>
#include <atlconv.h>

#define MY_NOTIFY WM_USER+1

#define IDM_SHOW WM_USER+2
#define IDM_HIDE WM_USER+3
#define IDM_EXIT WM_USER+4

#define NAME L"Console Wrapper"

HICON hIcon;
HWND hWindowWnd;
HWND hConsoleWnd;
HINSTANCE hInstance;
WNDCLASSEX wcex{ 0 };
NOTIFYICONDATA data;

HANDLE workerThreadHandle;
DWORD workerThreadId;

HMENU hMenu;

void showLastError();

LRESULT CALLBACK wndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

void loadIcon(LPCWSTR path);

void loadIcon(WORD id);

void regClass(LPCWSTR className);

void initWnd(LPCWSTR className, LPCWSTR title);

void createNotify(LPCWSTR title);

void releaseNotify();

void windowLoop();

void execCmd(LPCWSTR cmd);