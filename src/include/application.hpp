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


/* external includes */
#include <QApplication>


/**
 * \namespace suzu
 * \brief     root namespace of suzu's code-base
 */
namespace suzu {
    /**
     * \class suzu::Application
     * \brief main application class; singleton
     * 
     * The application class owns all of suzu's components. It behaves like a singleton and
     * cannot be instantiated more than once.
     */
    class Application final : public QApplication {
        Q_OBJECT

    public:
        explicit Application() noexcept = delete;
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
         */
        bool initialize() noexcept;
        /**
         * \brief  starts the main-loop and runs the application
         * 
         * \return error code to be returned to host OS
         */
        int run();
    };
}


