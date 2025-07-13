#pragma once

#include <Windows.h>


namespace cw {
    const auto NAME = "Console Wrapper";
    const int MY_NOTIFY(WM_USER + 1);

    const int IDM_SHOW(WM_USER + 2);
    const int IDM_HIDE(WM_USER + 3);
    const int IDM_EXIT(WM_USER + 4);

    void checkDebug();

    void showLastError();

    LRESULT CALLBACK wndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    void loadIcon(LPCSTR path);

    void loadIcon(WORD id);

    void regClass(LPCSTR className);

    void initWnd(LPCSTR className, LPCSTR title);

    void createNotify(LPCSTR title);

    void releaseNotify();

    void windowLoop();

    void execCmd(LPSTR cmd);
}
