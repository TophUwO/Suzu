/*
 * Suzu - (UML) diagram editor created with Qt
 *
 * (c) 2023 TophUwO <tophuwo01@gmail.com>. All rights reserved.
 *
 * For information on licensing, please refer to the LICENSE file in the project's
 * root directory.
 */

/**
 * \file  window.hpp
 * \brief definition of main window class
 */


/* external includes */
#include <QMessageBox>

/* sdk includes */
#include <sdk/log.hpp>

/* app includes */
#include <window.hpp>


namespace suzu {
    Window::Window(suzu::sdk::Configuration &gcfg) noexcept
        : QMainWindow(), m_gcfg(gcfg)
    {
        /* Initialize UI. */
        setupUi(this);

        showNormal();
        SZSDK_APP_INFO("Successfully initialized main window.");
    }

    Window::~Window() {
        SZSDK_APP_INFO("Shutdown main window.");
    }


    void Window::on_actionExit_triggered() {
        throw std::exception("This function is currently not implemented.");

        QApplication::exit(sdk::ErrorCode::Ok);
    }
}


