#ifndef __SETTINGS_H
#define __SETTINGS_H

#include "../Include/types.h"

class Settings
{
public:
    char *UserName = "root";
    int UserNameLength = 4;
    char *UserPassword = "root";
    char* Hostname = "localhost";
    int HostnameLength = 10;

    bool IsNetworkEnabled = false;
    bool IsHDDEnabled = true;
};

#endif