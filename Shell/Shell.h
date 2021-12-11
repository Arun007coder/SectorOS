// Copyright (C) 2021 Arun007coder
// 
// This file is part of SectorOS.
// 
// SectorOS is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// SectorOS is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with SectorOS.  If not, see <http://www.gnu.org/licenses/>.

#ifndef SHELL_H
#define SHELL_H

#include "../Drivers/Keyboard.h"
#include "../Include/types.h"
#include "../Include/Public_VAR.h"

class CShell : public CustomShell
{
    char CharBuffer[256];

public:
    CShell();
    ~CShell();
    virtual void Shell();
    virtual void clearBuffer();
    virtual void OnKeyDown(uint8_t key);
};

#endif