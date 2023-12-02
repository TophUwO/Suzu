/*
 * Suzu - (UML) diagram editor created with Qt
 *
 * (c) 2023 TophUwO <tophuwo01@gmail.com>. All rights reserved.
 *
 * For information on licensing, please refer to the LICENSE file in the project's
 * root directory.
 */

/**
 * \file  config.hpp
 * \brief implementation of integrated config reader and writer
 */


#pragma once

/* external includes */
#include <QReadWriteLock>

#include <sdk/external/json/nlohmann/json.hpp>

/* sdk includes */
#include <sdk/util.hpp>


namespace suzu::sdk {
    using JSON = nlohmann::json; /**< raw JSON values; not serialized */


    /**
     * \class suzu::sdk::Configuration
     * \brief class interfacing with configuration objects and files
     * 
     * Suzu uses JSON as its primary configuration format. This class allows the application
     * to load, read, and write values to these configuration files.
     * 
     * \note  Instances of this class are thread-safe after being constructed. File flushes
     *        are NOT thread-safe.
     */
    class Configuration {
        mutable QReadWriteLock m_lock;       /**< synchronization object */
                JSON           m_dict;       /**< raw JSON key-value store */
                std::string    m_path;       /**< (optional) file path */
                bool           m_isOk;       /**< whether or not the config state is normal */
                bool           m_writeOnDel; /**< whether or not to flush the file when the object is deleted */

    public:
        /**
         * \brief constructs a new configuration object
         *
         * The configuration supports two forms of input. A new configuration object can
         * be generated from an empty object, or from a file path.
         *
         * \param [in] path (optional) file path, must be *nullptr* if the config object
         *        should be initially empty
         * \param [in] writedest whether or not to flush the file when the object is destroyed;
         *        ignored if *path* is *nullptr*
         */
        explicit Configuration(char const *const path = nullptr, bool writedest = false) noexcept
            : m_lock(QReadWriteLock::RecursionMode::Recursive), m_isOk(true), m_writeOnDel(writedest)
        {
            try {
                /* If *path* is not *nullptr*, copy the filename. */
                if (path != nullptr) {
                    m_path = std::string{ path };

                    /* Load config file from the given path. */
                    util::FileBuffer buf;
                    if (util::ReadFile(path, buf) != ErrorCode::Ok)
                        return;

                    m_dict = JSON::parse(buf.data(), nullptr, false, true);
                    if (m_dict.is_discarded())
                        reset();
                }
            } catch (...) { m_isOk = false; }
        }

        virtual ~Configuration() {
            /*
             * If the path is not empty and the corresponding flag is set, write the current
             * state to the file at the saved file path.
             */
            if (m_writeOnDel && !m_path.empty())
                writeToFile(m_path.c_str());
        }


        /**
         * \brief retrieves the healthy flag of the underlying JSON document
         * 
         * \return *true* if the state is healthy
         */
        bool isOk() const noexcept { return m_isOk; }


        /**
         * \brief  retrieves the raw JSON value at the given path
         * 
         * The path must be of the form /path/to/val where / denotes the root
         * element.
         * 
         * \param  [in] path path to the desired value, in the above form
         * 
         * \return raw JSON value; on error this return value's *is_discarded()*
         *         method will return *true*
         * \note   To get the actual underlying primitive value, use *suzu::sdk::JSONCVT*'s
         */
        JSON getValue(char const *const path) const noexcept {
            /* Initialize discarded value. */
            static JSON const gl_discval = JSON::parse("{" /* invalid JSON */, nullptr, false, true);

            try {
                QReadLocker lock(&m_lock);

                /* Initialize path. */
                nlohmann::json_pointer<std::string> ptr(path);
                if (!m_isOk || !m_dict.contains(ptr))
                    return gl_discval;

                return m_dict[ptr].get<JSON>();
            } catch (...) { }

            return gl_discval;
        }

        /**
         * \brief updates or insertes the given value *val* at *path*
         * 
         * \param [in] path path of the JSON value
         * \param [in] val raw JSON value
         * 
         * \note  A copy of *val* will be inserted.
         */
        void setValue(char const *const path, JSON const &val) noexcept {
            try {
                QWriteLocker lock(&m_lock);
                if (!m_isOk)
                    return;

                /* Initialize path. */
                nlohmann::json_pointer<std::string> ptr(path);

                /* Update or insert JSON value. */
                m_dict[ptr] = val;
            } catch (...) { }
        }


        /**
         * \brief  serializes the current state of the underlying JSON document
         * 
         * \param  [in] pretty whether or not to serialize the document in a
         *         pretty form (i.e. indented, etc.)
         * 
         * \return serialized JSON document as UTF-8 string
         * \note   If there was an error, the function returns a valid empty 
         *         serialized JSON document ('{}')
         */
        std::string writeToString(bool pretty = true) const noexcept {
            static std::string const gl_emptydoc = std::string{ "{}" };

            try {
                QReadLocker lock(&m_lock);
                if (!m_isOk)
                    return gl_emptydoc;

                return m_dict.dump(pretty ? 4 : -1);
            } catch (...) { }

            return gl_emptydoc;
        }

        /**
         * \brief  writes the serialization of the current state of the internal JSON document
         *         to the file pointed to by *path*
         * 
         * If the *path* parameter is *nullptr*, the function will use the saved path that was
         * passed, if any is present. If *path* is *nullptr* and no path was saved, the function
         * does nothing.
         * 
         * \param  [in] path to the file to override the saved path
         * \param  [in] append whether or not to append the contents to the file
         * 
         * \return *suzu::sdk::ErrorCode::Ok* on success
         */
        ErrorCode writeToFile(char const *const path = nullptr, bool append = false) const noexcept {
            if (path == nullptr)
                return ErrorCode::InvalidParameter;

            ErrorCode code = ErrorCode::Ok;
            try {
                QReadLocker lock(&m_lock);
                if (!m_isOk)
                    return ErrorCode::InvalidState;

                /* Write serialized JSON document. */
                std::string const serjson = writeToString(false);
                code = util::WriteFile(path == nullptr ? m_path.c_str() : path, serjson.c_str(), serjson.length());
            } catch (...) { }

            return code;
        }

        /**
         * \brief clears the current state of the config object and deletes all keys
         * 
         * If the source of the configuration object is a file, the disk file will
         * not be updated.
         * 
         * \note  All references to values from within this object are invalidated.
         */
        void reset() noexcept {
            try {
                QWriteLocker lock(&m_lock);

                /* Reset value by replacing it with an empty JSON document. */
                m_dict = JSON::parse("{}", nullptr, false, true);
                m_isOk = true;
            } catch (...) { }
        }
    };


    /**
     * \class suzu::sdk::JSONValueConverter
     * \brief converts raw values to concrete, mostly primitive types
     */
    struct JSONValueConverter {
        /**
         * \brief  converts a given JSON value to the desired type
         * 
         * \param  [in] val raw JSON value to convert
         * \param  [in] fallback fallback value in case of errors
         * 
         * \return 
         */
        template<class TargetVal> static TargetVal to(JSON const &val, TargetVal fallback) noexcept {
            /*
             * Generic conversion function. The purpose of this return statement is to cause
             * a compilation error. This is a bit ugly, but it does work.
             */
            return TargetVal::not_implemented_handler;
        }

        template<> static int32_t      to(JSON const &val, int32_t fallback)     noexcept { if (!val.is_number_integer()) return fallback; return val.get<int32_t>(); }
        template<> static int64_t      to(JSON const &val, int64_t fallback)     noexcept { if (!val.is_number_integer()) return fallback; return val.get<int64_t>(); }
        template<> static float        to(JSON const &val, float fallback)       noexcept { if (!val.is_number_float()) return fallback; return val.get<float>(); }
        template<> static double       to(JSON const &val, double fallback)      noexcept { if (!val.is_number_float()) return fallback; return val.get<double>(); }
        template<> static std::string  to(JSON const &val, std::string fallback) noexcept { if (!val.is_string()) return fallback; return val.get<std::string>(); }


        /**
         * \brief converts mostly primitive values to a JSON value which can be fed into
         *        the suzu::sdk::Configuration's methods
         */
        template<class SourceVal> static JSON from(SourceVal val) noexcept {
            /*
             * Generic conversion function. The purpose of this return statement is to cause
             * a compilation error. This is a bit ugly, but it does work.
             */
            return SourceVal::not_implemented_handler;
        }

        template<> static JSON from(int32_t val)     noexcept { return JSON(val); }
        template<> static JSON from(int64_t val)     noexcept { return JSON(val); }
        template<> static JSON from(float val)       noexcept { return JSON(val); }
        template<> static JSON from(double val)      noexcept { return JSON(val); }
        template<> static JSON from(char const *val) noexcept { return JSON(val); }
        template<> static JSON from(std::string val) noexcept { return JSON(val); }
    };
    using JSONCVT = JSONValueConverter;
}


