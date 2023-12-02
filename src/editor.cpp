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
 * \brief implementation of Suzu's DiagramEditor component
 */


/* external includes */
#include <QGraphicsItem>

/* app includes */
#include <editor.hpp>


namespace suzu {
    DiagramEditor::DiagramEditor(QWidget *parent) noexcept
        : QGraphicsView(parent)
    {
        /* Initialize scene. */
        m_scene = new QGraphicsScene(this);
    }

    DiagramEditor::~DiagramEditor() { }
}


