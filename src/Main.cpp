#include "Main.h"
#include "Config.h"

#include <cstdio>

int main() {
    auto config = Config();
    if (config.load("config.ini")) {
        printf("failed to load config\n");
        system("pause");
        return -1;
    }

    auto name = NAME;
    auto iconPath = config.getIconPath();
    auto title = config.getTitle();
    auto cmd = config.getCmd();

    loadIcon(iconPath);
    regClass(name);
    initWnd(name, title);
    createNotify(title);

    workerThreadHandle = CreateThread(
        nullptr,
        0,
        reinterpret_cast<LPTHREAD_START_ROUTINE>(execCmd),
        LPVOID(cmd),
        0,
        &workerThreadId
    );
    if (workerThreadHandle == INVALID_HANDLE_VALUE) {
        releaseNotify();
        printf("failed to create thread\n");
        system("pause");
        return -1;
    }
    windowLoop();
    releaseNotify();
    TerminateThread(workerThreadHandle, 0);
    return 0;
}

void showLastError() {
    auto err = GetLastError();
    printf("last error: %d\n", err);
}

LRESULT CALLBACK wndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_COMMAND:
            if (wParam == IDM_SHOW) {
                ShowWindow(hConsoleWnd, SW_SHOW);
            } else if (wParam == IDM_HIDE) {
                ShowWindow(hConsoleWnd, SW_HIDE);
            } else if (wParam == IDM_EXIT) {
                DestroyWindow(hWnd);
            }
            break;
        case MY_NOTIFY:
            if (lParam == WM_RBUTTONUP) {
                POINT pt;
                GetCursorPos(&pt);
                TrackPopupMenu(hMenu, TPM_RIGHTBUTTON, pt.x, pt.y, NULL, hWnd, nullptr);
            } else if (lParam == WM_LBUTTONDBLCLK) {
                ShowWindow(hConsoleWnd, SW_SHOW);
            }
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
    }
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void loadIcon(LPCSTR path) {
    hIcon = static_cast<HICON>(LoadImage(
        nullptr,
        path,
        IMAGE_ICON,
        0, 0,
        LR_DEFAULTCOLOR |
        LR_CREATEDIBSECTION |
        LR_LOADFROMFILE
    ));
}

void loadIcon(WORD id) {
    hIcon = LoadIcon(GetModuleHandle(nullptr), MAKEINTRESOURCE(id));
}

void regClass(LPCSTR className) {
    hInstance = ::GetModuleHandle(nullptr);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.cbSize = sizeof(wcex);
    wcex.lpfnWndProc = wndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = hIcon;
    wcex.hCursor = nullptr;
    wcex.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = className;
    RegisterClassEx(&wcex);
}

void initWnd(LPCSTR className, LPCSTR title) {
    hConsoleWnd = GetForegroundWindow();
    ShowWindow(hConsoleWnd, SW_HIDE);
    SendMessage(hConsoleWnd, WM_SETICON, ICON_SMALL, (LPARAM) hIcon);
    SendMessage(hConsoleWnd, WM_SETICON, ICON_BIG, (LPARAM) hIcon);
    SendMessage(hConsoleWnd, WM_SETTEXT, NULL, (LPARAM) title);

    hWindowWnd = ::CreateWindow(
        className,
        title,
        WS_OVERLAPPEDWINDOW,
        0,
        0,
        0,
        0,
        nullptr,
        nullptr,
        hInstance,
        nullptr
    );
}

void createNotify(LPCSTR title) {
    lstrcpy(data.szTip, title);
    data.hWnd = hWindowWnd;
    data.uCallbackMessage = MY_NOTIFY;
    data.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    data.hIcon = wcex.hIcon;
    Shell_NotifyIcon(NIM_ADD, &data);

    hMenu = CreatePopupMenu();
    AppendMenu(hMenu, MF_STRING, IDM_SHOW, "Show");
    AppendMenu(hMenu, MF_STRING, IDM_HIDE, "Hide");
    AppendMenu(hMenu, MF_STRING, IDM_EXIT, "Exit");
}

void releaseNotify() {
    Shell_NotifyIcon(NIM_DELETE, &data);
}

void windowLoop() {
    ShowWindow(hWindowWnd, SW_HIDE);
    UpdateWindow(hWindowWnd);
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

void execCmd(LPCSTR cmd) {
    system((cmd));
}
