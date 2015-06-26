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
#ifndef _COMMON_H_
#define _COMMON_H_

unsigned short init_cmd_colors();

void restore_cmd_colors(unsigned short);

void print_header();

void print_failed();

void print_ok();

int init_socket();

void write_log(wchar_t *);

void send_log(wchar_t *);

int init_socket();

void clean_socket();

#endif
