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
#ifndef CONSOLE_H
#define CONSOLE_H

#ifdef __cplusplus
extern "C" {
#endif

    /**
     * Setup the console and return the current console attributes.
     * @return the original console attributes
     */
    uint16_t init_cmd_attributes();

    /**
     * Restore the original console attributes saved from the initialise
     * colours function.
     * @param the original console attributes
     */
    void restore_cmd_attributes(uint16_t original);

    /**
     * Print a formated header.
     * @param title the application title
     * @param subtitle the subtitle
     * @param description the application description
     * @param attributes for the subtitle part of the header
     */
    void print_header(const wchar_t *title, const wchar_t *subtitle,
            const wchar_t *description, uint16_t attributes);

    /**
     * Output to the console a green "OK" string (with a newline character
     * in the end).
     */
    void print_ok();

    /**
     * Output to the console a red background and white foreground "failed!"
     * string (with a newline character in the end).
     */
    void print_failed();

#ifdef __cplusplus
}
#endif

#endif /* CONSOLE_H */
