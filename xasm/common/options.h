/*  Copyright 2008-2017 Carsten Elton Sorensen

    This file is part of ASMotor.

    ASMotor is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    ASMotor is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with ASMotor.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef XASM_COMMON_OPTIONS_H_INCLUDED_
#define XASM_COMMON_OPTIONS_H_INCLUDED_

#include "lists.h"
#include "xasm.h"

#define MAX_DISABLED_WARNINGS 32

struct MachineOptions;

typedef struct Options {
    list_Data(struct Options);

    EEndianness endianness;
    uint8_t binaryLiteralCharacters[2];
    uint8_t uninitializedValue;

    uint8_t disabledWarningsCount;
    uint16_t disabledWarnings[MAX_DISABLED_WARNINGS];

    struct MachineOptions* machineOptions;
    bool allowReservedKeywordLabels;
} SOptions;

extern void
opt_Push(void);

extern void
opt_Pop(void);

extern void
opt_Parse(char* s);

extern void
opt_Open(void);

extern void
opt_Close(void);

extern SOptions* opt_Current;

#endif /* XASM_COMMON_OPTIONS_H_INCLUDED_ */
