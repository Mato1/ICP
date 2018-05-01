#include "schema_widget.h"
#include <QMenu>
#include <QPen>
#include <iostream>
#include <QInputDialog>

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

            QAction action3("New schema", this);
            contextMenu.addAction(&action3);
            connect(&action3, &QAction::triggered, this, &SchemaWidget::new_schema);

            QAction action4("Save Schema", this);
            contextMenu.addAction(&action4);
            connect(&action4, &QAction::triggered, this, &SchemaWidget::save_schema);

            QAction action5("Load Schema", this);
            contextMenu.addAction(&action5);
            connect(&action5, &QAction::triggered, this, &SchemaWidget::load_schema);

            QAction action6("Close", this);
            contextMenu.addAction(&action6);
            connect(&action6, &QAction::triggered, this, &SchemaWidget::close_schema);
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

        void SchemaWidget::save_schema()
        {
            bool ok;
           QString text = QInputDialog::getText(this, tr("Save"),
                                         tr("Nazov suboru:"), QLineEdit::Normal,
                                            tr("Untitled_file"), &ok);
            std::cout << "Save schema" << std::endl;
        }

        void SchemaWidget::load_schema()
        {
            bool ok;
           QString text = QInputDialog::getText(this, tr("Load"),
                                         tr("Nazov suboru:"), QLineEdit::Normal,
                                            tr("Untitled_file"), &ok);
                                            
            std::cout << "Load schema" << std::endl;
        }

        void SchemaWidget::close_schema()
        {
            std::cout << "Close schema" << std::endl;
        }

        void SchemaWidget::play_schema()
        {
            std::cout << "Start play  schema" << std::endl;
        }

        void SchemaWidget::next_step()
        {
            std::cout << "Next step" << std::endl;
        }

        void SchemaWidget::new_schema()
        {
            std::cout << "New schema" << std::endl;
        }

    }
}