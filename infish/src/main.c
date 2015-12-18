/**
 * Copyright (C) 2015 Duarte Silva
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
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <memory.h>
#include <windows.h>
#include <shlwapi.h>

#include "console.h"
#include "logging.h"

/**
 * Size of the buffer that will hold the logical drives letters.
 */
#define BUFFER_SIZE 512

/**
 * Recursively list the directory contents.
 * @param directory the starting directory
 * @return 0 if the directory does not contain any file or sub-directory, or if
 * it is not readable
 */
int directory_listing(const wchar_t *directory)
{
	wchar_t path[MAX_PATH];
	memset(path, 0, sizeof(wchar_t) * MAX_PATH);

	// Add the wildcard to the directory
	PathCombineW(path, directory, L"*.*");

	WIN32_FIND_DATAW data;
	memset(&data, 0, sizeof(WIN32_FIND_DATAW));

	HANDLE handle = FindFirstFileW(path, &data);

	if (handle == INVALID_HANDLE_VALUE) {
		return 0;
	}

	do {
		// Ignore the pseudo-directories
		if (wcscmp(data.cFileName, L".") == 0 ||
			wcscmp(data.cFileName, L"..") == 0) {
			continue;
		}

		PathCombineW(path, directory, data.cFileName);

		if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			directory_listing(path);
		} else {
			write_log(L"infish", path);
			send_log(path);
		}
	}
	while (FindNextFileW(handle, &data) == TRUE);
	
	FindClose(handle);

	return 1;
}

/**
 * Get all the logical drives and recursively list their contents.
 */
void all_drive_listings()
{
	wchar_t buffer[BUFFER_SIZE];
	memset(buffer, 0, sizeof(wchar_t) * BUFFER_SIZE);

	wprintf(L"[*] Obtaining logical drives names ... ");
	// The buffer needs to have that extra null character
	if (GetLogicalDriveStringsW(BUFFER_SIZE - 1, buffer) == 0) {
		print_failed();
	} else {
		// Will ignore the cases where the buffer size is smaller than
		// needed for the number of drives available since the current
		// selected size should suffice with the exception of some edge
		// cases.
		print_ok();

		wchar_t *current = buffer;

		do {
			wprintf(L"[*] Obtaining listings for %s ... ", current);
			if (directory_listing(current) != 1) {
				print_failed();
			} else print_ok();

			while (*current++ != 0);
		} while (*current != 0);
	}
}

int main()
{
	uint16_t original_colors = 0;

	original_colors = init_cmd_attributes();
	print_header(L"Infish", L"Investigative fish", L"Directory and file listings of all\nlogical drives.", 10);

	wprintf(L"[*] Initializing log file ... ");
	if (open_log("infish.log") != 1) {
		print_failed();
	} else print_ok();

	write_log(L"infish", L"Start");

	wprintf(L"[*] Initializing socket ... ");
	if (init_socket() != 1) {
		print_failed();
	} else {
		print_ok();

		wprintf(L"\n[-] Obtaining directory listings\n");
		all_drive_listings();

		wprintf(L"\n[*] Cleanup socket ...\n");
		clean_socket();
	}

	wprintf(L"\n\n");
	wprintf(L"[-] Feel free to RE me, check log file for more information.");

	write_log(L"infish", L"End");
	close_log();

	getchar();

	restore_cmd_attributes(original_colors);

	return 0;
}
