#include "schema_widget.h"
#include <QMenu>
#include <QPen>
#include <iostream>

namespace icp
{
    namespace ui
    {
        SchemaWidget::SchemaWidget(std::string nazov)
            : Schema(nazov)
        {
            init();
        }

        void SchemaWidget::init()
        {
            this->setContextMenuPolicy(Qt::CustomContextMenu);
            connect(this, &SchemaWidget::customContextMenuRequested, this, &SchemaWidget::show_context_menu);
        }

        void SchemaWidget::show_context_menu(const QPoint &pos)
        {
            QMenu contextMenu(tr("Context menu"), this);
            QAction action1("New Block", this);
            contextMenu.addAction(&action1);
            connect(&action1, &QAction::triggered, this, &SchemaWidget::new_block);
            contextMenu.exec(mapToGlobal(pos));
        }

        void SchemaWidget::new_block()
        {
            std::cout << "New Block" << std::endl;
        }

        void SchemaWidget::new_connection()
        {
            std::cout << "New Connection" << std::endl;
        }
    }
}