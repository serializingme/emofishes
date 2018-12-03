/**
 * Copyright (C) 2019 Duarte Silva
 *
 * This file is part of Emofishes.
 *
 * Emofishes is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Emofishes is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Emofishes. If not, see <http://www.gnu.org/licenses/>.
 */
#include <windows.h>

#include "wow64.h"

/**
 * Prototypes for the Wow64 API's since they aren't available in all Windows
 * versions, most notably Windows XP 32 bits.
 */
typedef BOOL(WINAPI *IsWow64ProcessProto) (HANDLE, BOOL*);
typedef BOOL(WINAPI *DisableWow64FsRedirectionProto) (void**);
typedef BOOL(WINAPI *RevertWow64FsRedirectionProto) (void*);

int is_wow64() {
    IsWow64ProcessProto fniswow = (IsWow64ProcessProto) GetProcAddress(
            GetModuleHandleA("kernel32"), "IsWow64Process");

    int result = 0;
    return (fniswow != NULL) &&
            (fniswow(GetCurrentProcess(), &result) != FALSE) ? result : 0;
}

int disable_wow64_fs_redirection(void **old) {
    DisableWow64FsRedirectionProto fndisable = (DisableWow64FsRedirectionProto)
            GetProcAddress(GetModuleHandleA("kernel32"), "Wow64DisableWow64FsRedirection");

    return (fndisable != NULL) && (fndisable(old) != FALSE) ? 1 : 0;
}

int revert_wow64_fs_redirection(void *old) {
    RevertWow64FsRedirectionProto fnrevert = (RevertWow64FsRedirectionProto)
            GetProcAddress(GetModuleHandleA("kernel32"), "Wow64RevertWow64FsRedirection");

    return (fnrevert != NULL) && (fnrevert(old) != FALSE) ? 1 : 0;
}
