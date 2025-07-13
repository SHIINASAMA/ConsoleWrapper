#include "Main.h"
#include "Config.h"

#include <cstdio>

HICON hIcon;
HWND hWindowWnd;
HWND hConsoleWnd;
HINSTANCE hInstance;
WNDCLASSEX wcex{0};
NOTIFYICONDATA data;

PROCESS_INFORMATION processInfo;
// HANDLE workerThreadHandle;
// DWORD workerThreadId;

HMENU hMenu;

bool isDebug = false;

int main() {
    cw::checkDebug();

    auto config = Config();
    if (!config.load("./config.ini")) {
        printf("failed to load config\n");
        system("pause");
        return -1;
    }

    auto name = cw::NAME;
    auto iconPath = config.getIconPath();
    auto title = config.getTitle();
    auto cmd = config.getCmd();

    cw::loadIcon(iconPath);
    cw::regClass(name);
    cw::initWnd(name, title);
    cw::createNotify(title);

    // workerThreadHandle = CreateThread(
    //     nullptr,
    //     0,
    //     reinterpret_cast<LPTHREAD_START_ROUTINE>(cw::execCmd),
    //     LPVOID(cmd),
    //     0,
    //     &workerThreadId
    // );
    // if (workerThreadHandle == INVALID_HANDLE_VALUE) {
    //     cw::releaseNotify();
    //     printf("failed to create thread\n");
    //     system("pause");
    //     return -1;
    // }
    cw::execCmd(cmd);

    cw::windowLoop();
    cw::releaseNotify();
    // TerminateThread(workerThreadHandle, 0);
    TerminateProcess(processInfo.hProcess, 0);
    return 0;
}

namespace cw {
    void checkDebug() {
        char string[16]{};
        GetEnvironmentVariable("KAORU_DEBUG", string, 16);
        isDebug = strcmp(string, "1") == 0;
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
            default:
                DefWindowProc(hWnd, uMsg, wParam, lParam);
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
        wcex.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
        wcex.lpszMenuName = nullptr;
        wcex.lpszClassName = className;
        RegisterClassEx(&wcex);
    }

    void initWnd(LPCSTR className, LPCSTR title) {
        hConsoleWnd = GetForegroundWindow();
        if (!isDebug) {
            ShowWindow(hConsoleWnd, SW_HIDE);
        }
        SendMessage(hConsoleWnd, WM_SETICON, ICON_SMALL, reinterpret_cast<LPARAM>(hIcon));
        SendMessage(hConsoleWnd, WM_SETICON, ICON_BIG, reinterpret_cast<LPARAM>(hIcon));
        SendMessage(hConsoleWnd, WM_SETTEXT, NULL, reinterpret_cast<LPARAM>(title));

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

    void execCmd(LPSTR cmd) {
        // system(cmd);
        STARTUPINFO si = {sizeof(si)};
        auto rt = CreateProcess(
            nullptr,
            cmd,
            nullptr,
            nullptr,
            FALSE,
            0,
            nullptr,
            nullptr,
            &si,
            &processInfo
        );
        if (!rt) {
            printf("failed to create the process\n");
            system("pause");
            exit(-1);
        }
    }
}
