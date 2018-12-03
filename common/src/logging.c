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
#include <stdint.h>
#include <stdio.h>
#include <wchar.h>
#include <winsock2.h>
#include <windows.h>
#include <memory.h>

#include "logging.h"

/**
 * The default server address to send the logs to.
 */
static const char *server_addr = "192.168.1.1";
/**
 * The default server port to use.
 */
static const int server_port = 9876;
/**
 * Socket to be used to send the logs from.
 */
SOCKET client_socket = INVALID_SOCKET;
/**
 * Socket address to send the logs to.
 */
struct sockaddr_in socket_addr;
/**
 * File descriptor.
 */
FILE *log_file = NULL;

int init_socket() {
    WSADATA data;
    memset(&data, 0, sizeof (WSADATA));

    if (WSAStartup(MAKEWORD(2, 2), &data) != NO_ERROR) {
        return 0;
    }

    if ((client_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == INVALID_SOCKET) {
        WSACleanup();
        return 0;
    }

    socket_addr.sin_family = AF_INET;
    socket_addr.sin_port = htons(server_port);
    socket_addr.sin_addr.s_addr = inet_addr(server_addr);

    return 1;
}

void send_log(const wchar_t *message) {
    size_t length = wcslen(message) + 1;

    // Get a nice zero filled string.
    char *final = (char *) malloc(length * sizeof (char));
    memset(final, 0, length * sizeof (char));

    wcstombs(final, message, length);

    send_loga(final);

    free(final);
}

void send_loga(const char *message) {
    // Make sure there is space for the line feed and the null character
    size_t length = strlen(message) + 2;

    // Get a nice zero filled string.
    char *final = (char *) malloc(length * sizeof (char));
    memset(final, 0, length * sizeof (char));

    sprintf(final, "%s\n", message);

    // While sending ignore the null character
    sendto(client_socket, final, (length - 1) * sizeof (char), 0,
            (SOCKADDR *) & socket_addr, sizeof (struct sockaddr_in));

    free(final);
}

void clean_socket() {
    if (client_socket == INVALID_SOCKET) {
        closesocket(client_socket);
    }

    WSACleanup();
}

int open_log(const char *filename) {
    return ((log_file = fopen(filename, "a")) != NULL) ? 1 : 0;
}

void write_log(const wchar_t *tag, const wchar_t *message) {
    if (log_file == NULL) {
        return;
    }

    fwprintf(log_file, L"[%s] %s\n", tag, message);
}

void close_log() {
    if (log_file != NULL) {
        fclose(log_file);
    }

    log_file = NULL;
}
