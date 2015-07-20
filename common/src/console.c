/**
 * Copyright (C) 2015 Duarte Silva
 *
 * This file is part of Emofishes.
 *
 * Emofishes is a collection of free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as published by
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
#include <stdint.h>
#include <stdio.h>
#include <wchar.h>
#include <windows.h>

uint16_t init_cmd_attributes()
{
	HANDLE handler = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_SCREEN_BUFFER_INFO information;
	GetConsoleScreenBufferInfo(handler, &information);
	SetConsoleTextAttribute(handler, FOREGROUND_INTENSITY);

	return information.wAttributes;
}

void restore_cmd_attributes(uint16_t original)
{
	HANDLE handler = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(handler, original);
}

void print_header(const wchar_t *title, const wchar_t *subtitle,
	const wchar_t *description, uint16_t attributes)
{
	HANDLE handler = GetStdHandle(STD_OUTPUT_HANDLE);
	wprintf(L"* %s (", title);
	SetConsoleTextAttribute(handler, attributes);
	wprintf(L"%s", subtitle);
	SetConsoleTextAttribute(handler, FOREGROUND_INTENSITY);
	wprintf(L") *\n\n%s\n\n", description);
}

void print_ok()
{
	HANDLE handler = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(handler, 10);
	wprintf(L"OK\n");
	SetConsoleTextAttribute(handler, FOREGROUND_INTENSITY);
}

void print_failed()
{
	HANDLE handler = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(handler, 207);
	wprintf(L"failed!\n");
	SetConsoleTextAttribute(handler, FOREGROUND_INTENSITY);
}
