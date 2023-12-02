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


#pragma once

/* external includes */
#include <QMainWindow>

#include <ui_window.h>

/* sdk includes */
#include <sdk/config.hpp>

/* app includes */
#include <editor.hpp>


namespace suzu {
    /**
     * \class suzu::Window
     * \brief represents Suzu's main window
     * 
     * Suzu only creates one main window at a time. Only modal and non-modal dialogs can
     * appear as top-level windows as children of Suzu's main window.
     */
    class Window final : public QMainWindow, private Ui_Window {
        Q_OBJECT

        sdk::Configuration &m_gcfg;   /**< reference to the global configuration */
        DiagramEditor      *m_editor; /**< diagram editor component */

    public:
        explicit Window() noexcept = delete;
        /**
         * \brief constructs the main window
         * 
         * \param [in] gcfg reference to the global configuration object
         */
        explicit Window(sdk::Configuration &gcfg) noexcept;
        ~Window();

    public slots:
        void on_actionExit_triggered();
    };
}


