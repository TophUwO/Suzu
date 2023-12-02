/*
 * Suzu - (UML) diagram editor created with Qt
 *
 * (c) 2023 TophUwO <tophuwo01@gmail.com>. All rights reserved.
 *
 * For information on licensing, please refer to the LICENSE file in the project's
 * root directory.
 */

/**
 * \file  util.hpp
 * \brief various utilities used by the SDK
 */


#pragma once

/* stdlib includes */
#include <vector>
#include <fstream>
#include <sstream>

/* sdk includes */
#include <sdk/error.hpp>


/**
 * \namespace suzu::sdk::util
 * \brief     contains utility functions that can be invoked by both the application and
 *            the SDK itself
 */
namespace suzu::sdk::util {
    using FileBuffer = std::vector<char>; /**< used for result/input values of file I/O operations */


    /**
     * \brief   reads the file at the given file path
     * 
     * The file can be either read as a binary or as a text file. Reading the file as text will
     * transform line-break characters to reflect the system's default. Reading as binary will not
     * perform any transformations.
     * 
     * \param   [in] path file path of the file that is to be read
     * \param   [out] result reference to a *suzu::sdk::util::FileBuffer* that will be populated with
     *          the result bytes
     * \param   [in] binary whether or not to read the file in binary mode
     * 
     * \return  *suzu::sdk::ErrorCode::Ok* on success
     * \note    If the function returns non-zero, there was an error. In this case, *result* will remain
     *          in an undeterminate state.
     * \warning Data that is already inside *result* prior to calling this function may be lost!
     */
    inline ErrorCode ReadFile(char const *const path, FileBuffer &result, bool binary = false) noexcept {
        if (path == nullptr)
            return ErrorCode::InvalidParameter;

        try {
            /* Open file stream. */
            std::fstream stream(path, std::ios_base::in | (binary ? std::ios_base::binary : 0));
            if (stream.fail())
                return ErrorCode::OpenFile;

            /* Read file contents into buffer. */
            result.assign(std::istreambuf_iterator<char>(stream), {});
            if (!binary)
                result.push_back('\0');

            stream.close();
        } catch (...) {
            result.clear();

            return ErrorCode::ReadFile;
        }

        return ErrorCode::Ok;
    }

    /**
     * \brief   writes *len* bytes from the beginning of *data* into the file at *path*
     * 
     * Just like for reading, files can also be written in binary or non-binary mode with similar effects
     * to the reading operation.
     * 
     * \param   [in] path file path of the file that is to be written
     * \param   [in] data pointer to a buffer of binary data with a size of at least *len* bytes
     * \param   [in] len number of bytes that are to be written
     * \param   [in] binary whether or not to write the data as binary data
     * \param   [in] append whether or not to append the data to the end of the file rather than discarding
     *          the existing file's contents
     * 
     * \return  *suzu::sdk::ErrorCode::Ok* on success
     * \note    If the function returns non-zero, there was an error.
     * \warning Unless *append* is true, data that is already inside the file at *path* prior to calling this
     *          function may be lost!
     */
    inline ErrorCode WriteFile(char const *const path, char const *data, size_t len, bool binary = false, bool append = false) noexcept {
        if (path == nullptr || data == nullptr)
            return ErrorCode::InvalidParameter;
        else if (len == 0)
            return ErrorCode::NoOperation;

        try {
            /* Open file stream. */
            std::fstream stream(path, std::ios_base::out | (binary ? std::ios_base::binary : 0) | (append ? std::ios_base::app : 0));
            if (stream.fail())
                return ErrorCode::OpenFile;

            /* Write buffer. */
            stream.write(data, len);
            stream.close();
        } catch (...) {
            return ErrorCode::WriteFile;
        }

        return ErrorCode::Ok;
    }
}


