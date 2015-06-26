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
#include <windows.h>
#include <wbemidl.h>

#include "types.h"
#include "common.h"
#include "wmi.h"
#include "fingerprint.h"

int main(void)
{
	unsigned short original_colors = 0;
	IWbemServices *services = NULL;
	int index = 0;

	write_log(L"Start");

	original_colors = init_cmd_colors();
	print_header();

	printf("[*] Initializing socket ... ");
	if (init_socket() != TRUE) {
		print_failed();
	}
	else {
		print_ok();

		printf("\n[-] Obtaining data using WMI\n");
		printf("[*] Starting WMI client ... ");
		if (wmi_initialize(&services) != TRUE) {
			print_failed();
		}
		else {
			print_ok();

			for (index = 0; wmitargets[index].caption != NULL; index++) {
				wprintf(L"[*] Obtaining %s data ... ", wmitargets[index].caption);
				if (wmi_execute_query(services, wmitargets[index].caption, wmitargets[index].classname,
						wmitargets[index].properties) != TRUE) {
					print_failed();
				}
				else print_ok();
			}

			printf("[*] Cleanup WMI client ... \n");
			wmi_cleanup(services);
		}

		printf("\n[*] Cleanup socket ...\n");
		clean_socket();
	}

	printf("\n\n");
	printf("[-] Feel free to RE me, check log file for more information.");

	write_log(L"End");

	getchar();

	/* Restore Original Console Colors */
	restore_cmd_colors(original_colors);

	return 0;
}
