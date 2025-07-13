#pragma once

#include <Windows.h>

struct Config {
    char iconPath[1024]{};
    char title[256]{};
    char cmd[1024]{};

    Config() = default;

    bool load(const char *filePath) {
        if (GetPrivateProfileString(
                "DEFAULT",
                "icon",
                "",
                iconPath,
                sizeof(iconPath),
                filePath
            ) == 0)
            return false;
        if (GetPrivateProfileString(
                "DEFAULT",
                "title",
                "",
                title,
                sizeof(title),
                filePath
            ) == 0)
            return false;
        if (GetPrivateProfileString(
                "DEFAULT",
                "cmd",
                "",
                cmd,
                sizeof(cmd),
                filePath
            ) == 0)
            return false;
        return true;
    }

    [[nodiscard]] const char *getIconPath() const { return iconPath; }
    [[nodiscard]] const char *getTitle() const { return title; }
    [[nodiscard]] char *getCmd() { return cmd; }
};
