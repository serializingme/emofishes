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
#ifndef WMI_H
#define WMI_H

/**
 * Structure that contains the name of the class to query and the respective
 * properties. Note that the maximum number of properties is forty.
 */
struct wmitarget {
	wchar_t *caption;
	wchar_t *classname;
	wchar_t *properties[40];
};

/**
 * Initialise the WMI client that will connect to the local machine WMI
 * namespace. It will return TRUE if the connection was successful, FALSE
 * otherwise.
 */
int wmi_initialize(IWbemServices **);

/**
 * Cleanup WMI.
 */
void wmi_cleanup(IWbemServices *);

/**
 * Create the WMI query based on the class name and the selected properties,
 * execute the query and report for each instance its properties values. 
 */
int wmi_execute_query(IWbemServices *, wchar_t *, wchar_t *, wchar_t *[]);

#endif	/* WMI_H */
