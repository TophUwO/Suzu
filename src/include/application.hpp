/*
 * Suzu - (UML) diagram editor created with Qt
 * 
 * (c) 2023 TophUwO <tophuwo01@gmail.com>. All rights reserved.
 * 
 * For information on licensing, please refer to the LICENSE file in the project's
 * root directory.
 */

/**
 * \file  application.hpp
 * \brief definition of application class
 */


#pragma once

/* external includes */
#include <QApplication>

/* sdk includes */
#include <sdk/config.hpp>

/* app includes */
#include <window.hpp>


/**
 * \namespace suzu
 * \brief     root namespace of suzu's code-base
 */
namespace suzu {
    constexpr std::string_view gl_glcfgpath = "data/config.json"; /**< path to global config file; relative to root directory */


    /**
     * \class suzu::Application
     * \brief main application class; singleton
     * 
     * The application class owns all of suzu's components. It behaves like a singleton and
     * cannot be instantiated more than once.
     */
    class Application final : public QApplication {
        Q_OBJECT

        sdk::Configuration  m_cfg; /**< global configuration */
        Window             *m_wnd; /**< main window */

        /**
         * \brief  formats a generic error message to use in the global exception handler
         * 
         * \param  [in] obj receiving object
         * \param  [in] ev event
         * \param  [in] exc exception description
         * 
         * \return formatted string
         * \note   This function never throws any exceptions.
         */
        static std::string int_fmtMessage(QObject *obj, QEvent *ev, char const *exc) noexcept;

    public:
        explicit Application() = delete;
        /**
         * \brief constructs a new application instance
         * 
         * \param [in] argc number of elements in *argv*
         * \param [in] argv command-line parameters
         * 
         * \note  *argc* and *argv* are supplied by the host environment.
         */
        explicit Application(int argc, char **argv);
        ~Application();

        /**
         * \brief  initializes the application's main components
         * 
         * \return *true* if all components could be initialized
         * 
         * \note   *false* is only returned in case of a fatal error
         *         that would require the application to exit.
         * \note   This function never throws any exceptions.
         */
        bool initialize();
        /**
         * \brief  starts the main-loop and runs the application
         * 
         * \return error code to be returned to host OS
         */
        int run();


        /**
         * \brief  overrides the default 'notify()' method in order to
         *         enable global exception handling
         * 
         * \param  [in] obj receiving object
         * \param  [in] ev event
         *
         * \return return value of event handler of *obj*
         */
        virtual bool notify(QObject *obj, QEvent *ev) override;
    };
}


