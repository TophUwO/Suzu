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


/* app includes */
#include <application.hpp>


namespace suzu {
    Application::Application(int argc, char **argv)
        : QApplication(argc, argv)
    { }

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


