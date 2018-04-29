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

            QAction action2("New connection", this);
            contextMenu.addAction(&action2);
            connect(&action2, &QAction::triggered, this, &SchemaWidget::new_connection);

            QAction action3("New port", this);
            contextMenu.addAction(&action3);
            connect(&action3, &QAction::triggered, this, &SchemaWidget::new_port);

            QAction action4("New expression", this);
            contextMenu.addAction(&action4);
            connect(&action4, &QAction::triggered, this, &SchemaWidget::new_expression);
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

        void SchemaWidget::new_port()
        {
            std::cout << "New Port" << std::endl;
        }

        void SchemaWidget::new_expression()
        {
            std::cout << "New expression" << std::endl;
        }

        void SchemaWidget::save_schema()
        {
            std::cout << "Save schema" << std::endl;
        }

        void SchemaWidget::load_schema()
        {
            std::cout << "Load schema" << std::endl;
        }

        void SchemaWidget::close_schema()
        {
            std::cout << "Close schema" << std::endl;
        }

    }
}