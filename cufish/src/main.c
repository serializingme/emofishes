/**
 * Copyright (C) 2015 Duarte Silva
 *
 * This file is part of Cufish.
 *
 * Cufish is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Cufish is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with Cufish. If not, see <http://www.gnu.org/licenses/>.
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <windows.h>
#include <wbemidl.h>

#include "console.h"
#include "logging.h"
#include "wmi.h"
#include "fingerprint.h"

int main()
{
	uint16_t original_colors = 0;
	IWbemServices *services = NULL;
	int index = 0;

	write_log("cufish.log", L"cufish", L"Start");

	original_colors = init_cmd_attributes();
	print_header(L"Cufish", L"Curious fish", L"Fingerprinting malware execution\nenvironments.", 10);

	wprintf(L"[*] Initializing socket ... ");
	if (init_socket() != 1) {
		print_failed();
	} else {
		print_ok();

		wprintf(L"\n[-] Obtaining data using WMI\n");
		wprintf(L"[*] Starting WMI client ... ");
		if (wmi_initialize(&services) != 1) {
			print_failed();
		} else {
			print_ok();

			for (index = 0; wmitargets[index].caption != NULL; index++) {
				wprintf(L"[*] Obtaining %s data ... ", wmitargets[index].caption);
				if (wmi_execute_query(services, wmitargets[index].caption, wmitargets[index].classname,
					wmitargets[index].properties) != 1) {
					print_failed();
				} else print_ok();
			}

			wprintf(L"[*] Cleanup WMI client ... \n");
			wmi_cleanup(services);
		}

		wprintf(L"\n[*] Cleanup socket ...\n");
		clean_socket();
	}

	wprintf(L"\n\n");
	wprintf(L"[-] Feel free to RE me, check log file for more information.");

	write_log("cufish.log", L"cufish", L"End");

	getchar();

	restore_cmd_attributes(original_colors);

	return 0;
}
