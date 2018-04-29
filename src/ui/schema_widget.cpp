#include "schema_widget.h"
#include <QMenu>
#include <QPen>
#include <iostream>

namespace icp
{
    namespace ui
    {
        SchemaWidget::SchemaWidget(std::string nazov)
        {
            schema = new model::Schema(nazov);
            this->setContextMenuPolicy(Qt::CustomContextMenu);
            connect(this, &SchemaWidget::customContextMenuRequested, this, &SchemaWidget::show_context_menu);
        }

        SchemaWidget::~SchemaWidget()
        {
            delete schema;
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
            BlockWidget * bw = new BlockWidget("Untitled_Block");
            schema->add_block(bw->get_block());
            bw->setParent(this);
            bw->show();
        }

        void SchemaWidget::new_connection()
        {
            std::cout << "New Connection" << std::endl;
        }
    }
}