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
    using namespace std::string_literals;


    /**
     * \namespace suzu::internal
     * \brief     internal functions used by Suzu
     *
     * This namespace is not to be used by plug-ins. Plug-ins use the namespace natsu::sdk::internal for
     * internal functions.
     */
    namespace internal {
        /**
         * \brief  initializes suzu's global loggers
         *
         * This function is called before components are initialized. Loggers will be destroyed
         * after the application instance has been destroyed. Logging is therefore safe throughout
         * the entire lifetime of the application.
         *
         * \param  [in] logfile pointer to a C-string which holds the path of the main logfile
         *
         * \return 2-tuple of a statically-allocated array holding sink pointers and the number of
         *         initialized sinks
         * \note   The return value of this function should be passed to plug-ins as well upon invoking
         *         the plugin's initialization function.
         */
        static std::pair<spdlog::sink_ptr const *, size_t> RetrieveGlobalLoggerSinks(char const *const logfile) noexcept {
            if (logfile == nullptr || *logfile == '\0')
                return { nullptr, 0 };

            try {
                /* Create logger sinks. */
                static std::vector<spdlog::sink_ptr> const gl_sinks{
                    std::make_shared<spdlog::sinks::basic_file_sink_mt>(logfile, true),
                    std::make_shared<spdlog::sinks::ansicolor_stdout_sink_mt>(),
                };

                return { gl_sinks.data(), gl_sinks.size() };
            }
            catch (...) {}

            return { nullptr, 0 };
        }
    }


    Application::Application(int argc, char **argv)
        : QApplication(argc, argv), m_cfg(gl_glcfgpath.data(), true), m_wnd(nullptr)
    {
        /* Apply modern style. */
        QApplication::setStyle("fusion");

        /* If global config could not be read, exit the application. */
        if (!m_cfg.isOk()) {
            QApplication::exit(sdk::ErrorCode::CriticalResource);

            return;
        }

        /* Initialize logging facilities. */
        auto [sinks, len] = internal::RetrieveGlobalLoggerSinks(sdk::JSONCVT::to(m_cfg.getValue("/logfile"), ""s).c_str());
        if ((sinks == nullptr && len == 0) || !suzu::sdk::InitializeInstanceLoggers(len, sinks)) {
            QApplication::exit(sdk::ErrorCode::CriticalComponent);

            return;
        }

        SZSDK_APP_INFO("Successfully initialized application instance.");
    }

    Application::~Application() {
        SZSDK_APP_INFO("Shutdown application instance.");
    }


    bool Application::initialize() noexcept {
        try {
            m_wnd = new Window(m_cfg);
        } catch (...) { return false; }

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


