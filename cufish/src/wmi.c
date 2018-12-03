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
#include <stdio.h>
#include <wchar.h>
#include <windows.h>
#include <wbemidl.h>

#include "logging.h"
#include "wmi.h"

/**
 * WMI namespace where queries will be executed.
 */
static const wchar_t *query_namespace = L"root\\cimv2";
/**
 * WMI query format.
 */
static const wchar_t *query_format = L"SELECT %s FROM %s";
/**
 * WMI query properties separator.
 */
static const wchar_t *query_separator = L",";
/**
 * WMI query language.
 */
static const wchar_t *query_language = L"WQL";

/**
 * Construct a WMI query with the provided class name and properties. The query
 * will be returned as a BSTR. The calling code is responsible for freeing the
 * allocated memory for it.
 */
BSTR construct_query(wchar_t *class_name, wchar_t *properties[]) {
    wchar_t *expression;
    wchar_t *query;
    size_t count;
    size_t length;
    BSTR result;

    // Count the number of properties and calculate the combined length of
    // them all.
    for (count = 0, length = 0; properties[count] != NULL; count++) {
        length = length + wcslen(properties[count]);
    }

    // Account for the comma separators (should be "count - 1" but since
    // the string needs to be null terminated, it allocates that extra
    // character).
    length = length + count;

    // Get a nice zero filled string.
    expression = (wchar_t *) malloc(length * sizeof (wchar_t));
    memset(expression, 0, length * sizeof (wchar_t));

    for (count = 0; properties[count] != NULL; count++) {
        wcscat(expression, properties[count]);

        // If there is a next element, add the comma separator.
        if (properties[count + 1] != NULL) {
            wcscat(expression, query_separator);
        }
    }

    // The length for the query is calculated based on the expression, the
    // class name, and the query format. The extra character for string
    // termination isn't necessary since the query format will give extra
    // characters (4 in total).
    length = wcslen(expression) + wcslen(class_name) + wcslen(query_format);

    query = (wchar_t *) malloc(length * sizeof (wchar_t));
    memset(query, 0, length * sizeof (wchar_t));

    snwprintf(query, length, query_format, expression, class_name);

    result = SysAllocString(query);

    // Free allocated memory.
    free(expression);
    free(query);

    return result;
}

/**
 * Report to the application log and the UDP socket. Maximum size for each line
 * is 2048 wide characters (4096 bytes).
 */
void wmi_report(wchar_t *format, ...) {
    wchar_t formated[2048] = {0};
    va_list arglist;

    va_start(arglist, format);
    vswprintf(formated, 2048, format, arglist);
    va_end(arglist);

    write_log(L"cufish", formated);
    send_log(formated);
}

/**
 * Handle a VARIANT that is an array. Note that this function only supports
 * VARIANT's that are strings.
 */
void wmi_handle_array(wchar_t *property, VARIANT *value) {
    wchar_t *element;
    long index, count, upper_bound = 0, lower_bound = 0;

    SAFEARRAY *safe_array = V_ARRAY(value);

    SafeArrayGetUBound(safe_array, 1, &upper_bound);
    SafeArrayGetLBound(safe_array, 1, &lower_bound);

    count = upper_bound - lower_bound + 1;
    wmi_report(L"- %s:", property);

    for (index = 0; index < count; ++index) {
        SafeArrayGetElement(safe_array, &index, &element);

        wmi_report(L"  - %s", element);
    }
}

/**
 * Function responsible for extracting from the instance the property value and
 * reporting it. Note that 64 bits CIM properties in a 32 bits application (at
 * least) are encoded as strings, so V_BSTR is used instead of V_UI8.
 */
void wmi_handle_property(IWbemClassObject *instance, wchar_t *property) {
    CIMTYPE type = CIM_ILLEGAL;
    VARIANT value;

    HRESULT hresult = instance->lpVtbl->Get(instance, property, 0, &value, &type, 0);

    if (FAILED(hresult) || V_VT(&value) == VT_NULL) {
        wmi_report(L"- %s: Null", property);
    } else {
        switch (type) {
            case CIM_ILLEGAL:
                wmi_report(L"- %s: Illegal", property);
                break;
            case CIM_EMPTY:
                wmi_report(L"- %s: Empty", property);
                break;
            case CIM_SINT8:
                wmi_report(L"- %s: %i", property, V_I1(&value));
                break;
            case CIM_UINT8:
                wmi_report(L"- %s: %u", property, V_UI1(&value));
                break;
            case CIM_SINT16:
                wmi_report(L"- %s: %i", property, V_I2(&value));
                break;
            case CIM_UINT16:
                wmi_report(L"- %s: %u", property, V_UI2(&value));
                break;
            case CIM_SINT32:
                wmi_report(L"- %s: %i", property, V_I4(&value));
                break;
            case CIM_UINT32:
                wmi_report(L"- %s: %u", property, V_UI4(&value));
                break;
            case CIM_SINT64:
                wmi_report(L"- %s: %s", property, V_BSTR(&value));
                break;
            case CIM_UINT64:
                wmi_report(L"- %s: %s", property, V_BSTR(&value));
                break;
            case CIM_BOOLEAN:
                wmi_report(L"- %s: %s", property, V_BOOL(&value) ? L"True" : L"False");
                break;
            case CIM_STRING:
                wmi_report(L"- %s: %s", property, V_BSTR(&value));
                break;
            case CIM_DATETIME:
                wmi_report(L"- %s: %s", property, V_BSTR(&value));
                break;
            case CIM_CHAR16:
                wmi_report(L"- %s: %i", property, V_I2(&value));
                break;
            case CIM_STRING | CIM_FLAG_ARRAY:
                wmi_handle_array(property, &value);
                break;
            default:
                wmi_report(L"- %s: Unsupported", property);
                break;
        }
    }
}

/**
 * Handle a WMI class instance and report the selected properties value.
 */
void wmi_handle_instance(IWbemClassObject *instance, wchar_t *properties[]) {
    ULONG index;
    VARIANT value;

    for (index = 0; properties[index] != NULL; index++) {
        wmi_handle_property(instance, properties[index]);
    }

    VariantClear(&value);
}

int wmi_execute_query(IWbemServices *services, wchar_t *caption, wchar_t *classname,
        wchar_t *properties[]) {
    int status = 1;
    IEnumWbemClassObject *queryrows = NULL;
    IWbemClassObject * batchrows[10];
    BSTR language = SysAllocString(query_language);
    BSTR wmiquery;

    wmiquery = construct_query(classname, properties);

    // Execute the query.
    HRESULT result = services->lpVtbl->ExecQuery(
            services, language, wmiquery, WBEM_FLAG_BIDIRECTIONAL, NULL, &queryrows);

    if (!FAILED(result) && (queryrows != NULL)) {
        ULONG index, count = 0;
        result = WBEM_S_NO_ERROR;

        while (WBEM_S_NO_ERROR == result) {
            // Retrieve 10 rows (instances) each time.
            result = queryrows->lpVtbl->Next(queryrows, WBEM_INFINITE, 10,
                    batchrows, &count);

            if (!SUCCEEDED(result)) {
                continue;
            }
            // For each instance returned, report the properties
            // values.
            for (index = 0; index < count; index++) {
                wmi_report(L"Start %s", caption);
                wmi_handle_instance(batchrows[index], properties);
                wmi_report(L"End", caption);

                batchrows[index]->lpVtbl->Release(batchrows[index]);
            }
        }

        queryrows->lpVtbl->Release(queryrows);
    } else {
        status = 0;
    }

    SysFreeString(wmiquery);
    SysFreeString(language);

    return status;
}

int wmi_initialize(IWbemServices **services) {
    BSTR namespace;
    IWbemLocator *locator = NULL;
    int result;

    HRESULT hresult = CoInitializeEx(0, COINIT_MULTITHREADED);

    if (FAILED(hresult)) {
        return 0;
    }

    hresult = CoInitializeSecurity(NULL, -1, NULL, NULL, RPC_C_AUTHN_LEVEL_DEFAULT,
            RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE, NULL);

    if (FAILED(hresult)) {
        CoUninitialize();

        return 0;
    }

    hresult = CoCreateInstance(&CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER,
            &IID_IWbemLocator, (LPVOID *) & locator);

    if (FAILED(hresult)) {
        CoUninitialize();

        return 0;
    }

    namespace = SysAllocString(query_namespace);

    // Connect to the namespace with the current user and obtain pointer
    // services to make IWbemServices calls.
    hresult = locator->lpVtbl->ConnectServer(locator, namespace, NULL, NULL, NULL, 0,
            NULL, NULL, services);

    result = FAILED(hresult) ? 0 : 1;

    SysFreeString(namespace);
    locator->lpVtbl->Release(locator);

    return result;
}

void wmi_cleanup(IWbemServices *services) {
    if (services != NULL) {
        services->lpVtbl->Release(services);
    }

    CoUninitialize();
}
