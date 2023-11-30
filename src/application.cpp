/*
 * Suzu - (UML) diagram editor created with Qt
 *
 * (c) 2023 TophUwO <tophuwo01@gmail.com>. All rights reserved.
 *
 * For information on licensing, please refer to the LICENSE file in the project's
 * root directory.
 */

/**
 * \file  application.cpp
 * \brief implementation of application class
 */


/* sdk includes */
#include <sdk/log.hpp>

/* app includes */
#include <application.hpp>


namespace suzu {
    /**
     */
    namespace internal {
        /**
         * \brief  initializes suzu's global loggers
         *
         * This function is called before components are initialized. Loggers will be destroyed
         * after the application instance has been destroyed. Logging is therefore safe throughout
         * the entire lifetime of the application.
         *
         * \return C-array of constructed sinks
         * \note   The return value of this function should be passed to plug-ins as well upon invoking
         *         the plugin's initialization function.
         */
        inline std::pair<spdlog::sink_ptr const *, size_t> RetrieveGlobalLoggerSinks(char const *const logfile) noexcept {
            try {
                /* Create logger sinks. */
                static std::vector<spdlog::sink_ptr> const gl_sinks{
                    std::make_shared<spdlog::sinks::basic_file_sink_mt>(logfile, true),
                    std::make_shared<spdlog::sinks::ansicolor_stdout_sink_mt>(),
                };

                return { gl_sinks.data(), gl_sinks.size() };
            } catch (...) { }

            return { nullptr, 0 };
        }
    }


    Application::Application(int argc, char **argv)
        : QApplication(argc, argv)
    {
        /* Initialize logging facilities. */
        auto [sinks, len] = internal::RetrieveGlobalLoggerSinks("log.txt");
        suzu::sdk::InitializeInstanceLoggers(len, sinks);

        SZSDK_APP_INFO("Successfully initialized application instance.");
    }

    Application::~Application() { }


    bool Application::initialize() noexcept {
        return true;
    }

    int Application::run() {
        /* Initialize main components. */
        if (!initialize())
            return -1;

        /* Start main loop and run application. */
        return QApplication::exec();
    }
}


