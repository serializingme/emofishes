/* 
 * File:   logging.h
 * Author: castrdu
 *
 * Created on 20 July 2015, 10:35
 */

#ifndef LOGGING_H
#define	LOGGING_H

#ifdef	__cplusplus
extern "C" {
#endif

	/**
	 * Initialise the socket used to send logs over UDP (raw string in ANSI
	 * encoding sent over the network). By default, this will setup the
	 * socket with a destination IP of 192.168.1.1 on port 9876. This needs
	 * to be called before using the send functions. The objective is to
	 * create network traffic that contain the log messages since many if
	 * not all malware sand boxes do not allow the download of created
	 * files but it is usually allowed the download of the network packet
	 * captures.
	 * @return 0 in case of failure, 1 otherwise
	 */
	int init_socket();

	/**
	 * Send the log message over the UDP socket. This function will convert
	 * the message from a wide character string to a multiple byte string
	 * and call it's ANSI counterpart.
	 * @param message
	 */
	void send_log(const wchar_t *message);

	/**
	 * Send the log message over the UDP socket. This function will send the
	 * message encoded in ANSI to make it easier to read while inspecting
	 * the network packet capture.
	 * @param message
	 */
	void send_loga(const char *message);
	
	/**
	 * Cleanup the UDP socket.
	 */
	void clean_socket();

	/**
	 * Write a log message into a file. The file will be opened (in append
	 * mode) and closed every time a message is to be written.
	 * @param filename the file to write the log message to
	 * @param tag log entry tag
	 * @param message the message to be written
	 */
	void write_log(const char *filename, const wchar_t *tag,
		const wchar_t *message);

#ifdef	__cplusplus
}
#endif

#endif	/* LOGGING_H */
