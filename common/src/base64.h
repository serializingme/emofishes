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
#ifndef BASE64_H
#define	BASE64_H

/**
 * Calculate the length of the resulting string of encoding a source buffer so
 * that memory can be allocated correctly. Since the resulting encoded string
 * should be null terminated, this will return an extra character.
 * @param length the length of the buffer to be encoded
 * @return the resulting base64 encoded string length
 */
inline size_t base64_encode_len(size_t length)
{
	return((length + 2) / 3 * 4) + 1;
}

#ifdef	__cplusplus
extern "C" {
#endif

	/**
	 * Encode the supplied buffer in base64.
         * @param encoded string that will hold the result
         * @param source the source of data to encode
         * @param length the length of data to encode
         */
	void base64_encode(char *encoded, const uint8_t *source, size_t length);

#ifdef	__cplusplus
}
#endif

#endif	/* BASE64_H */
