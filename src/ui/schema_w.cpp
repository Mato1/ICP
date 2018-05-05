#include "ui/schema_w.h"
#include "model/schema.h"
#include <QMenu>
#include <QPen>
#include <iostream>
#include <QInputDialog>
#include <QFileDialog>
#include "main_window.h"
#include "ui/block_w.h"
#include "ui/valid_dialog.h"
#include "ui/add_connection_dialog.h"

namespace icp
{
namespace ui
{
SchemaW::SchemaW(std::string nazov)
    : model::Schema(nazov)
{
    this->setContextMenuPolicy(Qt::CustomContextMenu);

    context_menu        = new QMenu(tr("Context menu"),        this);
    action_insert_block = new QAction(tr("Insert Block"),      this);
    action_insert_con   = new QAction(tr("Insert Connection"), this);
    context_menu->addAction(action_insert_block);
    context_menu->addAction(action_insert_con);

    connect(this, &SchemaW::customContextMenuRequested, this, &SchemaW::show_context_menu);
    connect(action_insert_block, &QAction::triggered,   this, &SchemaW::new_block);
    connect(action_insert_con,   &QAction::triggered,   this, &SchemaW::new_connection);
}

void SchemaW::show_context_menu(const QPoint &pos)
{
    context_menu->exec(mapToGlobal(pos));
}

void SchemaW::new_block()
{
    QString text = tr("Untitled");
    int ok;

    do
    {

        ValidDialog dialog("^\\w*$", "Novy blok", "Nazov:", text.toStdString());
        ok = dialog.exec();

        if (ok == QDialog::Accepted)
        {
            text = dialog.get_value();
        }

    }
    while (get_block(text.toStdString()) != nullptr && ok == QDialog::Accepted);


    if (ok == QDialog::Accepted)
    {
        BlockW * bw = new BlockW(text.toStdString());
        add_block(bw);
        bw->setParent(this);
        bw->show();
    }
}


void SchemaW::new_connection()
{
   AddConnectionDialog dialog(blocks, this);

   if (dialog.exec() == QDialog::Accepted)
   {

   }
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
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                "/home",
                                                tr("Untitled (*.txt)"));

    std::cout << "Load schema" << std::endl;
}

void SchemaW::close_schema()
{
    QWidget::close();
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

void SchemaW::paintEvent(QPaintEvent * event)
{
    QWidget::paintEvent(event);

    QPainter painter(this);
    QPen pen;  // creates a default pen

    pen.setStyle(Qt::DotLine);
    pen.setWidth(1);
    pen.setBrush(Qt::gray);


    painter.setPen(pen);

    for (int x = 0; x < this->width(); x += MainWindow::GRID_SQUARE_SIZE)
    {
        painter.drawLine(x, 0, x, height());
    }

    for (int y = 0; y < this->height(); y += MainWindow::GRID_SQUARE_SIZE)
    {
        painter.drawLine(0, y, width(), y);
    }
}
}
}