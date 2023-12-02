/*
 * Suzu - (UML) diagram editor created with Qt
 *
 * (c) 2023 TophUwO <tophuwo01@gmail.com>. All rights reserved.
 *
 * For information on licensing, please refer to the LICENSE file in the project's
 * root directory.
 */

/**
 * \file  editor.hpp
 * \brief definition of Suzu's DiagramEditor component
 */


#pragma once

/* external includes */
#include <QGraphicsView>


namespace suzu {
    /**
     * \class suzu::DiagramEditor
     * \brief represents Suzu's DiagramEditor component
     */
    class DiagramEditor final : public QGraphicsView {
        Q_OBJECT

        QGraphicsScene *m_scene; /**< scene graph */

    public:
        explicit DiagramEditor() noexcept = delete;
        /**
         * \brief constructs a new diagram editor
         * 
         * \param [in] parent parent widget
         */
        explicit DiagramEditor(QWidget *parent) noexcept;
        ~DiagramEditor();
    };
}


