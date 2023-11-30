/*
 * Suzu - (UML) diagram editor created with Qt
 *
 * (c) 2023 TophUwO <tophuwo01@gmail.com>. All rights reserved.
 *
 * For information on licensing, please refer to the LICENSE file in the project's
 * root directory.
 */

 /**
  * \file  main.cpp
  * \brief definition of application entry point
  */


#include <application.hpp>


int main(int argc, char **argv) {
    /* Construct and initialize application instance. */
    suzu::Application app(argc, argv);

    /* Run app. */
    return app.run();
}


