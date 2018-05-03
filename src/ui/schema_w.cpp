#include "ui/schema_w.h"
#include "model/schema.h"
#include <QMenu>
#include <QPen>
#include <iostream>
#include <QInputDialog>
#include <QFileDialog>

namespace icp
{
    namespace ui
    {
        SchemaW::SchemaW(std::string nazov)
            : model::Schema(nazov)
        {
            // schema = new model::Schema(nazov);
            this->setContextMenuPolicy(Qt::CustomContextMenu);
            connect(this, &SchemaW::customContextMenuRequested, this, &SchemaW::show_context_menu);
        }

        SchemaW::~SchemaW()
        {
            // delete schema;
        }

        void SchemaW::show_context_menu(const QPoint &pos)
        {
            QMenu contextMenu(tr("Context menu"), this);
            QAction action1("Insert Block", this);
            contextMenu.addAction(&action1);
            connect(&action1, &QAction::triggered, this, &SchemaW::new_block);

            QAction action2("Insert Connection", this);
            contextMenu.addAction(&action2);
            connect(&action2, &QAction::triggered, this, &SchemaW::new_connection);

            QAction action3("New schema", this);
            contextMenu.addAction(&action3);
            connect(&action3, &QAction::triggered, this, &SchemaW::new_schema);

            QAction action4("Save Schema", this);
            contextMenu.addAction(&action4);
            connect(&action4, &QAction::triggered, this, &SchemaW::save_schema);

            QAction action5("Load Schema", this);
            contextMenu.addAction(&action5);
            connect(&action5, &QAction::triggered, this, &SchemaW::load_schema);

            QAction action6("Close", this);
            contextMenu.addAction(&action6);
            connect(&action6, &QAction::triggered, this, &SchemaW::close_schema);
            contextMenu.exec(mapToGlobal(pos));
        }

        void SchemaW::new_block()
        {
            bool ok;
            QString text = tr("Untitled");
            
            do {
                text = QInputDialog::getText(this, tr("Novy blok"),
                                             tr("Nazov:"), QLineEdit::Normal,
                                             text, &ok);
            
            } while (get_block(text.toStdString()) != nullptr && ok == true);

            if (ok == false)
            {
                return;
            }
            
            BlockW * bw = new BlockW(text.toStdString());
            add_block(bw);
            bw->setParent(this);
            bw->show();
        }
        

        void SchemaW::new_connection()
        {
            bool ok;
            QStringList ports_output;
            QStringList ports_input;

            for (unsigned i=0; i < get_all_ports().size(); i++) 
            {
                for (unsigned j=0; j < get_all_ports()[i]->get_output_ports().size(); j++) 
                {
                    ports_output << tr (get_all_ports()[i]->get_output_ports()[j]->get_nazov().c_str());
                }
                for (unsigned j=0; j < get_all_ports()[i]->get_input_ports().size(); j++) 
                {
                    ports_input << tr (get_all_ports()[i]->get_input_ports()[j]->get_nazov().c_str());
                }
            }

            QString port_output = QInputDialog::getItem(this, tr("Output port"),
                                         tr("Name port"), ports_output, 0, false, &ok);

            QString port_input = QInputDialog::getItem(this, tr("Input port"),
                                         tr("Name port"), ports_input, 0, false, &ok);


        }

        void SchemaW::save_schema()
        {
            QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                                                            "/home/untitled.txt",
                                                            tr("Untitled (*.txt)"));
            save(fileName.toStdString());
            // std::cout << "Save schema" << std::endl;
            // std::cout << fileName.toStdString()<< std::endl;
        }

        void SchemaW::load_schema()
        {
           bool ok;
           QString text = QInputDialog::getText(this, tr("Load"),
                                                tr("Nazov suboru:"), QLineEdit::Normal,
                                                tr("Untitled_file"), &ok);
                                            
           std::cout << "Load schema" << std::endl;
        }

        void SchemaW::close_schema()
        {
            std::cout << "Close schema" << std::endl;
        }

        void SchemaW::play_schema()
        {
            std::cout << "Start play  schema" << std::endl;
        }

        void SchemaW::next_step()
        {
            std::cout << "Next step" << std::endl;
        }

        void SchemaW::new_schema()
        {
            std::cout << "New schema" << std::endl;
        }

    }
}