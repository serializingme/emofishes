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
#ifndef WOW64_H
#define WOW64_H

#ifdef __cplusplus
extern "C" {
#endif

    /**
     * Wrapper function for IsWow64Process. The function returns 1 if the
     * current process is running under Wow64, 0 otherwise or if the
     * invocation failed.
     * @return 1 if the process is Wow64, 0 otherwise
     */
    int is_wow64();

    /**
     * Wrapper function for Wow64DisableWow64FsRedirection. The function
     * returns 0 if the Wow64DisableWow64FsRedirection is not found or the
     * invocation failed. The old value is returned in the argument old.
     * @param old old status of the file system redirection to store
     * @return 0 if failed, 1 otherwise
     */
    int disable_wow64_fs_redirection(void **old);

    /**
     * Wrapper function for Wow64RevertWow64FsRedirection. The function
     * returns 0 if the Wow64RevertWow64FsRedirection is not found or the
     * invocation failed. The old value is to be provided using the
     * argument old.
     * @param old the old status of the file system redirection to restore
     * @return 0 if failed, 1 otherwise
     */
    int revert_wow64_fs_redirection(void *old);

#ifdef __cplusplus
}
#endif

#endif /* WOW64_H */
