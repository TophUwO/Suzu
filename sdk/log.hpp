/*
 * Suzu - (UML) diagram editor created with Qt
 *
 * (c) 2023 TophUwO <tophuwo01@gmail.com>. All rights reserved.
 *
 * For information on licensing, please refer to the LICENSE file in the project's
 * root directory.
 */

/**
 * \file  log.hpp
 * \brief implementation of application class
 */


#pragma once

/* stdlib includes */
#include <initializer_list>

/* external includes */
#include <sdk/external/spdlog/spdlog.h>

#include <sdk/external/spdlog/sinks/basic_file_sink.h>
#include <sdk/external/spdlog/sinks/ansicolor_sink.h>


/**
 * \namespace suzu::sdk
 * \brief     root namespace of suzu's SDK
 */
namespace suzu::sdk {
    constexpr inline char const *gl_applog    = "suzu";        /**< application logger name */
    constexpr inline char const *gl_pluginlog = "suzu-plugin"; /**< plug-in logger name */


    /**
     * \brief  initializes the loggers for the current instance
     * 
     * Because spdlog is a header-only library, the logger registry has to be initialized for
     * each instance individually. Loggers can share sinks. Sinks are created by the main application
     * and then passed down to all plug-in instances upon initialization. Each plug-in instance owns
     * their loggers, but not the sinks.
     * Sinks are passed as a C-array due to ABI differences between debug and release builds for standard
     * C++ container classes when compiling with MSVC. 
     * 
     * \param  [in] nsinks number of sinks to register
     * \param  [in] sinks C-array, holding exactly *nsinks* sink pointers
     * \param  [in] minlvl minimum log level used by the application
     * 
     * \return *true* if all sinks could be initialized properly
     * \note   This function never throws any exceptions.
     */
    inline bool InitializeInstanceLoggers(size_t const nsinks, spdlog::sink_ptr const *sinks, spdlog::level::level_enum const minlvl = spdlog::level::trace) noexcept {
        /* Not initializing anything is not an error. */
        if (nsinks == 0 || sinks == nullptr)
            return true;

        try {
            /* Set global settings. When the loggers are initialized, global settings are inherited by them. */
            spdlog::set_pattern("[%D %r] %^%n::%l%$: %v");
            spdlog::set_level(minlvl);
            spdlog::flush_on(minlvl);

            /* Construct view on C-array. */
            std::initializer_list<spdlog::sink_ptr> const ilsinks(sinks, sinks + nsinks);

            /* Initialize and register loggers. */
            spdlog::initialize_logger(std::make_shared<spdlog::logger>(gl_applog, ilsinks));
            spdlog::initialize_logger(std::make_shared<spdlog::logger>(gl_pluginlog, ilsinks));
        } catch (...) { }

        return true;
    }
}


/**
 * \ingroup  Macros
 * \defgroup Logging
 * \brief    convenience macros for easy logging
 *
 * These macros should be used pretty much everywhere where logging is desired. They take no logger parameter,
 * the used logger is determined based on what macro is used. The logger used for each macro can be read from
 * the macro name, the schema is SZSDK_<logger-name>_<log-level>().
 *
 * \param    [in] format format string, allows fmtlib format specifiers
 * \param    [in] ... format arguments
 *
 * \note For fmtlib documentation, visit https://fmt.dev/latest/index.html.
 */
/** @{ */
#define SZSDK_APP_TRACE(format, ...)       spdlog::get(suzu::sdk::gl_applog)->trace(format, ##__VA_ARGS__)
#define SZSDK_APP_DEBUG(format, ...)       spdlog::get(suzu::sdk::gl_applog)->debug(format, ##__VA_ARGS__)
#define SZSDK_APP_INFO(format, ...)        spdlog::get(suzu::sdk::gl_applog)->info(format, ##__VA_ARGS__)
#define SZSDK_APP_WARNING(format, ...)     spdlog::get(suzu::sdk::gl_applog)->warn(format, ##__VA_ARGS__)
#define SZSDK_APP_ERROR(format, ...)       spdlog::get(suzu::sdk::gl_applog)->error(format, ##__VA_ARGS__)
#define SZSDK_APP_CRITICAL(format, ...)    spdlog::get(suzu::sdk::gl_applog)->critical(format, ##__VA_ARGS__)

#define SZSDK_PLUGIN_TRACE(format, ...)    spdlog::get(suzu::sdk::gl_pluginlog)->trace(format, ##__VA_ARGS__)
#define SZSDK_PLUGIN_DEBUG(format, ...)    spdlog::get(suzu::sdk::gl_pluginlog)->debug(format, ##__VA_ARGS__)
#define SZSDK_PLUGIN_INFO(format, ...)     spdlog::get(suzu::sdk::gl_pluginlog)->info(format, ##__VA_ARGS__)
#define SZSDK_PLUGIN_WARNING(format, ...)  spdlog::get(suzu::sdk::gl_pluginlog)->warn(format, ##__VA_ARGS__)
#define SZSDK_PLUGIN_ERROR(format, ...)    spdlog::get(suzu::sdk::gl_pluginlog)->error(format, ##__VA_ARGS__)
#define SZSDK_PLUGIN_CRITICAL(format, ...) spdlog::get(suzu::sdk::gl_pluginlog)->critical(format, ##__VA_ARGS__)
/** @} */


