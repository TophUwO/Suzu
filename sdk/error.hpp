/*
 * Suzu - (UML) diagram editor created with Qt
 *
 * (c) 2023 TophUwO <tophuwo01@gmail.com>. All rights reserved.
 *
 * For information on licensing, please refer to the LICENSE file in the project's
 * root directory.
 */

/**
 * \file  error.hpp
 * \brief SDK error codes and string representations
 */


#pragma once

/* stdlib includes */
#include <array>
#include <string_view>


namespace suzu::sdk {
    /**
     * \enum  ErrorCode
     * \brief describes all possible error states that may be returned by Suzu
     *
     * Suzu does not use exceptions. Therefore, most functions return an integer value indicating
     * whether or not the function finished successfully. Based on the information given by the error
     * code, the application may choose to recover from the failure.
     * Plug-ins should return the same error codes and should also adhere to the specification of the
     * plug-in interface regarding error codes in case certain conditions apply.
     *
     * \note  When a plug-in callback returns an error, Suzu will by default roll-back the current
     *        operation and print the error to the console.
     */
    enum ErrorCode {
        Ok                = 0, /**< Ok; no error */
        Unknown,               /**< unknown error */
        NoOperation,           /**< no operation was carried-out; not necessarily an error */

        InvalidParameter,      /**< invalid function parameter */
        InvalidState,          /**< invalid object state */
        CriticalResource,      /**< error locating an operation-critical resource */
        CriticalComponent,     /**< error initializing an operation-critical component */
        CriticalError,         /**< application exit due to fatal error */
        OpenFile,              /**< error while opening file */
        ReadFile,              /**< error while reading file buffer */
        WriteFile,             /**< error while writing file */

        __NumErrors__          /**< (only used internally) */
    };
}


