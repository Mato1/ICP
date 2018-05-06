/**
 * @brief 
 * 
 * @file schema_w.cpp
 * @author Libor Moravcik, Matej Mrazik
 * @date 2018-05-06
 */

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
#include "ui/port_w.h"
#include "ui/connection_w.h"
#include <QToolTip>
#include <QMessageBox>

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

SchemaW::~SchemaW()
{

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
    ConnectionW * connectionW = nullptr;

    if (dialog.exec() == QDialog::Accepted)
    {
        QString s_oblock = dialog.get_selected_output_block();
        QString s_oport = dialog.get_selected_output_port();
        QString s_iblock = dialog.get_selected_input_block();
        QString s_iport = dialog.get_selected_input_port();

        BlockW * out_block = static_cast<BlockW *>(get_block(s_oblock.toStdString()));
        PortW  * out_port  = static_cast<PortW *>(out_block->get_port((s_oport.toStdString())));
        BlockW * in_block  = static_cast<BlockW *>(get_block(s_iblock.toStdString()));
        PortW  * in_port   = static_cast<PortW *>(in_block->get_port((s_iport.toStdString())));

        if (out_block != in_block && out_port != in_port && out_block != nullptr && out_port != nullptr && in_block != nullptr && in_port != nullptr)
        {
            connectionW = new ConnectionW(out_block, out_port, in_block, in_port);
            add_prepoj(connectionW);
        }
    }

    dialog.close();

    if (this->detect_cycles())
    {
        QMessageBox msgBox;
        msgBox.setText("Cykly v scheme nie su povolene.");
        msgBox.exec();
        remove_prepoj(connectionW);
        delete connectionW;
    }
}

void SchemaW::play_schema()
{

    // for (model::Block * b : blocks)
    // {
    //     BlockW * bw = static_cast<BlockW *>(b);
    //     bw->set_input_ports_ready(true);
    //     bw->set_ouput_ports_ready(true);
    //     bw->set_data_to_input_ports();
    // }
    current_blocks = get_root_blocks();
    this->eval();
}

void SchemaW::next_step()
{   
    std::vector<model::Block *> temp;
    for (model::Block * b : current_blocks)
    {
        BlockW * bw = static_cast<BlockW *>(b);
        bw->set_data_to_input_ports();
        bw->set_input_ports_ready(true);
        bw->set_ouput_ports_ready(true);

        for(auto var : b->get_next_blocks())
        {
            temp.push_back(var);
        }
        bw->update();
    }

    current_blocks = temp;

  
}

void SchemaW::play_all_schema()
{

    for (model::Block * b : blocks)
    {
        BlockW * bw = static_cast<BlockW *>(b);
        bw->set_input_ports_ready(true);
        bw->set_ouput_ports_ready(true);
        bw->set_data_to_input_ports();
        bw->update();
    }

    this->eval();
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

    for (auto p : prepojenia)
    {
        QPainter painter(this);
        QPen pen;

        pen.setStyle(Qt::SolidLine);
        pen.setWidth(4);
        pen.setBrush(QColor(255, 175, 0));
        painter.setPen(pen);

        PortW * out_p = static_cast<PortW *>(p->get_output_port());
        PortW * in_p = static_cast<PortW *>(p->get_input_port());

        QRect out_geom = out_p->geometry();
        QRect in_geom = in_p->geometry();

        QPoint b_point = out_geom.topRight();
        QPoint e_point = in_geom.topLeft();
        b_point.setY(b_point.y() + 2);
        e_point.setY(e_point.y() + 2);

        QPoint middle_point = ((b_point + e_point) / 2);
        middle_point.setX(middle_point.x() + 20);

        painter.drawLine(b_point.x(), b_point.y(), middle_point.x(), b_point.y());

        QPainterPath path;
        path.addRect(b_point.x(), b_point.y(), abs(middle_point.x()-b_point.x()), 4);

        painter.drawLine(middle_point.x(), b_point.y(), middle_point.x(), e_point.y());

        if (middle_point.y() < e_point.y())
        {
            path.addRect(middle_point.x(), b_point.y(), 4, abs(b_point.y()-e_point.y()));
        }
        else
        {
            path.addRect(middle_point.x(), e_point.y(), 4, abs(b_point.y()-e_point.y()));
        }

        painter.drawLine(middle_point.x(), e_point.y(), e_point.x(), e_point.y());
        path.addRect(middle_point.x(), e_point.y(), abs(middle_point.x()-e_point.x()), 4);

        ConnectionW * pp = static_cast<ConnectionW *>(p);
        pp->set_painter_path(path);

    }
}

void SchemaW::mouseMoveEvent(QMouseEvent * event)
{
    for (auto p : prepojenia)
    {
        ConnectionW * pp = static_cast<ConnectionW *>(p);

        if (pp->get_painter_path().contains(event->pos()))
        {
            QToolTip::showText(event->globalPos(), tr(p->to_string().c_str()));
        }
    }

}


}
}