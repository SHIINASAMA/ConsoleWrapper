#include "Main.h"

int main(int argc, char** argv) {
	Config::ptr config = ConfigHelper::BuildConfig();

	LPCWSTR name = NAME;
	LPCWSTR iconPath = config->get(L"icon");
	LPCWSTR title = config->get(L"title");
	LPCWSTR cmd = config->get(L"cmd");

	loadIcon(iconPath);
	regClass(name);
	initWnd(name, title);
	createNotify(title);

	workerThreadHandle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)execCmd, (LPVOID)cmd, 0, &workerThreadId);
	if (workerThreadHandle == INVALID_HANDLE_VALUE) {
		printf_s("create thread failed");
		releaseNotify();
		return -1;
	}
	windowLoop();
	releaseNotify();
	TerminateThread(workerThreadHandle, 0);
	return 0;
}

void showLastError() {
	auto err = GetLastError();
	printf_s("last error: %d\n", err);
}

LRESULT CALLBACK wndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg)
	{
	case WM_COMMAND:
		if (wParam == IDM_SHOW) {
			ShowWindow(hConsoleWnd, SW_SHOW);
		}
		else if (wParam == IDM_HIDE) {
			ShowWindow(hConsoleWnd, SW_HIDE);
		}
		else if (wParam == IDM_EXIT) {
			DestroyWindow(hWnd);
		}
		break;
	case MY_NOTIFY:
		if (lParam == WM_RBUTTONUP)
		{
			POINT pt;
			GetCursorPos(&pt);
			TrackPopupMenu(hMenu, TPM_RIGHTBUTTON, pt.x, pt.y, NULL, hWnd, NULL);
		}
		else if (lParam == WM_LBUTTONDBLCLK) {
			ShowWindow(hConsoleWnd, SW_SHOW);
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void loadIcon(LPCWSTR path) {
	hIcon = (HICON)LoadImage(
		NULL,
		path,
		IMAGE_ICON,
		0, 0,
		LR_DEFAULTCOLOR |
		LR_CREATEDIBSECTION |
		LR_LOADFROMFILE
	);
}

void loadIcon(WORD id) {
	hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(id));
}

void regClass(LPCWSTR className) {
	hInstance = ::GetModuleHandle(NULL);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.cbSize = sizeof(wcex);
	wcex.lpfnWndProc = wndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = hIcon;
	wcex.hCursor = NULL;
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = className;
	RegisterClassEx(&wcex);
}

void initWnd(LPCWSTR className, LPCWSTR title) {
	hConsoleWnd = GetForegroundWindow();
	ShowWindow(hConsoleWnd, SW_HIDE);
	SendMessage(hConsoleWnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
	SendMessage(hConsoleWnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
	SendMessage(hConsoleWnd, WM_SETTEXT, NULL, (LPARAM)title);

	hWindowWnd = ::CreateWindow(
		className,
		title,
		WS_OVERLAPPEDWINDOW,
		0, 
		0, 
		0, 
		0, 
		NULL,
		NULL,
		hInstance,
		NULL
	);
}

void createNotify(LPCWSTR title) {
	lstrcpy(data.szTip, title);
	data.hWnd = hWindowWnd;
	data.uCallbackMessage = MY_NOTIFY;
	data.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	data.hIcon = wcex.hIcon;
	Shell_NotifyIcon(NIM_ADD, &data);

	hMenu = CreatePopupMenu();
	AppendMenu(hMenu, MF_STRING, IDM_SHOW, L"Show");
	AppendMenu(hMenu, MF_STRING, IDM_HIDE, L"Hide");
	AppendMenu(hMenu, MF_STRING, IDM_EXIT, L"Exit");
}

void releaseNotify() {
	Shell_NotifyIcon(NIM_DELETE, &data);
}

void windowLoop() {
	ShowWindow(hWindowWnd, SW_HIDE);
	UpdateWindow(hWindowWnd);
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

void execCmd(LPCWSTR cmd) {
	USES_CONVERSION;
	system(T2A(cmd));
}