#include "ui/block_w.h"
#include "ui/schema_w.h"
#include <QInputDialog>
#include <QLineEdit>
#include <iostream>
#include <QRegExpValidator>
#include "main_window.h"
#include "ui/valid_dialog.h"
#include "ui/add_vyraz_dialog.h"
#include "ui/schema_w.h"
namespace icp
{
namespace ui
{

BlockW::BlockW(std::string nazov, QWidget * parent)
    : model::Block(nazov)
{
    setContentsMargins(0, 0, 0, 0);
    setLayout(&layout_block);
    setLineWidth(LINE_WIDTH);
    setFrameShape(Shape::Box);
    setMinimumSize(MINIMUM_WIDTH, MINIMUM_HEIGHT);
    label_nazov.setText(tr(nazov.c_str()));
    layout_block.setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    label_nazov.setAlignment(Qt::AlignHCenter);
    layout_block.addWidget(&label_nazov);
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &BlockW::customContextMenuRequested, this, &BlockW::s_show_context_menu);

    QPalette pal = palette();
    pal.setColor(QPalette::Background, QColor(18, 84, 104));
    pal.setColor(QPalette::Foreground, Qt::white);
    setAutoFillBackground(true);
    setPalette(pal);
    setGeometry(0, 0, BASE_WIDTH, BASE_HEIGHT);
    this->setParent(parent);
    show();

}

BlockW::~BlockW()
{
    for (model::Port * p : input_ports)
    {
        PortW * pp =  static_cast<PortW *>(p);
        pp->deleteLater();
    }

    for (model::Port * p : output_ports)
    {
        PortW * pp =  static_cast<PortW *>(p);
        pp->deleteLater();
    }
}

void BlockW::s_show_context_menu(const QPoint &pos)
{
    QMenu contextMenu(tr("Context menu"), this);

    QAction action_eval_block("Evaluate Block", this);
    contextMenu.addAction(&action_eval_block);
    connect(&action_eval_block, &QAction::triggered, this, &BlockW::eval_block);

    QAction action_edit_block("Rename Block", this);
    contextMenu.addAction(&action_edit_block);
    connect(&action_edit_block, &QAction::triggered, this, &BlockW::s_rename_block);

    QAction action_add_expression("Add expression", this);
    contextMenu.addAction(&action_add_expression);
    connect(&action_add_expression, &QAction::triggered, this, &BlockW::s_add_expression);

    QAction action_add_input_port("Add input port", this);
    contextMenu.addAction(&action_add_input_port);
    connect(&action_add_input_port, &QAction::triggered, this, &BlockW::s_add_input_port);

    QAction action_add_output_port("Add output port", this);
    contextMenu.addAction(&action_add_output_port);
    connect(&action_add_output_port, &QAction::triggered, this, &BlockW::s_add_output_port);
    contextMenu.exec(mapToGlobal(pos));

    QAction action_delete_block("Delete Block", this);
    contextMenu.addAction(&action_delete_block);
    connect(&action_delete_block, &QAction::triggered, this, &BlockW::s_delete_block);

}

void BlockW::eval_block()
{
    std::cout << "Evaluating block" << std::endl;

    for (PortW * p : input_ports)
    {
        if (!p->is_connected())
        {
            for (auto it = p->get_data_type().data.begin(); it != p->get_data_type().data.end(); ++it)
            {
                bool ok;
                double n = QInputDialog::getDouble(this, tr(p->get_nazov().c_str()), tr(it->first.c_str()),
                                                   0, -2147483647, 2147483647, 4, &ok);

                if (ok)
                {
                    p->get_data_type().set(it->first, n);
                }

            }

        }
    }

    // this->eval();


}

void BlockW::s_delete_block()
{
    this->deleteLater();
}

void BlockW::s_rename_block()
{
    SchemaW * schema = (SchemaW *) parent();
    QString text = tr(get_nazov().c_str());
    int ok;

    do
    {

        ValidDialog dialog("^\\w*$", "Novy Nazov", "Nazov:", text.toStdString());
        ok = dialog.exec();

        if (ok == QDialog::Accepted)
        {
            text = dialog.get_value();
        }

    }
    while (schema->get_block(text.toStdString()) != nullptr && ok == QDialog::Accepted);


    set_nazov(text.toStdString());
}

void BlockW::s_add_expression()
{
    QString expr = "";
    QString premenna = "";
    PortW * input_port  = nullptr;
    PortW * output_port = nullptr;
    AddVyrazDialog dialog(get_input_ports(), get_output_ports());


    if (dialog.exec() == QDialog::Accepted)
    {
        expr = dialog.get_value();
        premenna = dialog.get_premenna();
        QString qs_input_port = dialog.get_selected_input_port();
        QString qs_output_port = dialog.get_selected_output_port();

        for (auto p : input_ports)
        {
            if (p->get_nazov().compare(qs_input_port.toStdString()) == 0)
            {
                input_port = p;
                break;
            }
        }

        for (auto p : output_ports)
        {
            if (p->get_nazov().compare(qs_output_port.toStdString()) == 0)
            {
                output_port = p;
                break;
            }
        }

        VyrazW * vyraz_w = new VyrazW(expr.toStdString(), premenna.toStdString(), this);
        vyraz_w->set_input_port(input_port);
        vyraz_w->set_output_port(output_port);
        add_expression(vyraz_w);
    }
}

void BlockW::add_expression(VyrazW * v)
{
    this->add_vyraz(v);
    layout_block.addWidget(v);
}

void BlockW::add_port(model::PortType type)
{
    QString text = "";

    ValidDialog dialog("^(?:[a-zA-Z]\\,)+[a-zA-Z]", "New Port", "Premenne:", "", "a,b,c,d");

    if (dialog.exec() == QDialog::Accepted)
    {
        text = dialog.get_value();

        if (text.toStdString().back() == ',')
        {
            std::string s =  text.toStdString();
            s.pop_back();
            text = tr(s.c_str());
        }

        PortW * port_w = nullptr;


        if (type == model::PortType::input)
        {
            port_w = new PortW(get_nazov(), get_input_ports().size(), type, (QWidget *)this->parent());
        }
        else if (type == model::PortType::output)
        {
            port_w = new PortW(get_nazov(), get_output_ports().size(), type, (QWidget *)this->parent());
        }

        port_w->set_data_type(text.toStdString());
        
        add_port(port_w);
      
    }
}

void BlockW::add_port(PortW *p)
{
    p->setParent((QWidget *)this->parent());

    if (p->get_port_type() == model::PortType::input)
    {
        input_ports.push_back(p);
        p->setGeometry(geometry().left() - MainWindow::GRID_SQUARE_SIZE,
                                geometry().top() + (MainWindow::GRID_SQUARE_SIZE * input_ports.size()), p->width(),
                                p->height());

    } else if (p->get_port_type() == model::PortType::output)
    {
        output_ports.push_back(p);
        p->setGeometry(geometry().left() + geometry().width(),
                                geometry().top() + (MainWindow::GRID_SQUARE_SIZE * output_ports.size()), p->width(),
                                p->height());
    }

    Block::add_port(p);

    MINIMUM_HEIGHT = (input_ports.size() < output_ports.size() ? (output_ports.size()) *
                      MainWindow::GRID_SQUARE_SIZE : (input_ports.size()) * MainWindow::GRID_SQUARE_SIZE);
    
    setMinimumHeight(MINIMUM_HEIGHT);
    BASE_HEIGHT = (input_ports.size() < output_ports.size() ? (output_ports.size() + 1) *
                   MainWindow::GRID_SQUARE_SIZE : (input_ports.size() + 1) * MainWindow::GRID_SQUARE_SIZE);

    if (height() < BASE_HEIGHT)
    {
        resize(width(), BASE_HEIGHT);
    }

}

void BlockW::s_add_input_port()
{
    this->add_port(model::PortType::input);
}

void BlockW::s_add_output_port()
{
    this->add_port(model::PortType::output);
}

void BlockW::mouseMoveEvent(QMouseEvent * e)
{

    ResizableFrame::mouseMoveEvent(e);


    for (unsigned int i = 0; i < input_ports.size(); i++)
    {
        input_ports[i]->setGeometry(geometry().left() - MainWindow::GRID_SQUARE_SIZE,
                                    geometry().top() + (MainWindow::GRID_SQUARE_SIZE * (i+1)), input_ports[i]->width(),
                                    input_ports[i]->height());
    }

    for (unsigned int i = 0; i < output_ports.size(); i++)
    {
        output_ports[i]->setGeometry(geometry().left() + geometry().width(),
                                     geometry().top() + (MainWindow::GRID_SQUARE_SIZE * (i+1)), output_ports[i]->width(),
                                     output_ports[i]->height());
    }

    SchemaW * p = static_cast<SchemaW *>(parent());
    p->update();
}

void BlockW::mouseReleaseEvent(QMouseEvent * e)
{

    if (e->button() == Qt::LeftButton)
    {
        int x = geometry().left();
        int y = geometry().top();
        int w = this->width();
        int h = this->height();

        if (start_pos == StartPosition::move_mouse)
        {
            x = (geometry().left() / MainWindow::GRID_SQUARE_SIZE) * MainWindow::GRID_SQUARE_SIZE ;
            y = (geometry().top() / MainWindow::GRID_SQUARE_SIZE) * MainWindow::GRID_SQUARE_SIZE ;

            w = (((x + width()) / MainWindow::GRID_SQUARE_SIZE) ) * MainWindow::GRID_SQUARE_SIZE;
            h = (((y + height()) / MainWindow::GRID_SQUARE_SIZE)) * MainWindow::GRID_SQUARE_SIZE;

        }
        else if (start_pos == StartPosition::left || start_pos == StartPosition::top
                 || start_pos == StartPosition::topleft)
        {
            w = (((geometry().left() + width()) / MainWindow::GRID_SQUARE_SIZE)) *
                MainWindow::GRID_SQUARE_SIZE;
            h = (((geometry().top() + height()) / MainWindow::GRID_SQUARE_SIZE)) *
                MainWindow::GRID_SQUARE_SIZE;

            x = (geometry().left() / MainWindow::GRID_SQUARE_SIZE) * MainWindow::GRID_SQUARE_SIZE ;
            y = (geometry().top() / MainWindow::GRID_SQUARE_SIZE) * MainWindow::GRID_SQUARE_SIZE ;

        }
        else if (start_pos == StartPosition::right || start_pos == StartPosition::topright)
        {
            w = (((geometry().left() + width()) / MainWindow::GRID_SQUARE_SIZE) + 1) *
                MainWindow::GRID_SQUARE_SIZE;
            h = (((geometry().top() + height()) / MainWindow::GRID_SQUARE_SIZE)) *
                MainWindow::GRID_SQUARE_SIZE;

            x = (geometry().left() / MainWindow::GRID_SQUARE_SIZE) * MainWindow::GRID_SQUARE_SIZE ;
            y = (geometry().top() / MainWindow::GRID_SQUARE_SIZE) * MainWindow::GRID_SQUARE_SIZE ;

        }
        else if (start_pos == StartPosition::bottomright)
        {
            w = (((geometry().left() + width()) / MainWindow::GRID_SQUARE_SIZE) + 1) *
                MainWindow::GRID_SQUARE_SIZE;
            h = (((geometry().top() + height()) / MainWindow::GRID_SQUARE_SIZE) + 1) *
                MainWindow::GRID_SQUARE_SIZE;

            x = (geometry().left() / MainWindow::GRID_SQUARE_SIZE) * MainWindow::GRID_SQUARE_SIZE ;
            y = (geometry().top() / MainWindow::GRID_SQUARE_SIZE) * MainWindow::GRID_SQUARE_SIZE ;


        }
        else if (start_pos == StartPosition::bottom || start_pos == StartPosition::bottomleft)
        {
            w = (((geometry().left() + width()) / MainWindow::GRID_SQUARE_SIZE)) *
                MainWindow::GRID_SQUARE_SIZE;
            h = (((geometry().top() + height()) / MainWindow::GRID_SQUARE_SIZE) + 1) *
                MainWindow::GRID_SQUARE_SIZE;

            x = (geometry().left() / MainWindow::GRID_SQUARE_SIZE) * MainWindow::GRID_SQUARE_SIZE ;
            y = (geometry().top() / MainWindow::GRID_SQUARE_SIZE) * MainWindow::GRID_SQUARE_SIZE ;
        }

        w -= x;
        h -= y;

        if (w < BASE_WIDTH)
        {
            w = BASE_WIDTH;
        }

        if (h < BASE_HEIGHT)
        {
            h = BASE_HEIGHT;
        }

        setGeometry(x, y, w, h);
        start_pos = StartPosition::move_mouse;
    }

    for (unsigned int i = 0; i < input_ports.size(); i++)
    {
        input_ports[i]->setGeometry(geometry().left() - MainWindow::GRID_SQUARE_SIZE,
                                    geometry().top() + (MainWindow::GRID_SQUARE_SIZE * (i+1)), input_ports[i]->width(),
                                    input_ports[i]->height());
    }

    for (unsigned int i = 0; i < output_ports.size(); i++)
    {
        output_ports[i]->setGeometry(geometry().left() + geometry().width(),
                                     geometry().top() + (MainWindow::GRID_SQUARE_SIZE * (i+1)), output_ports[i]->width(),
                                     output_ports[i]->height());
    }

    SchemaW * p = static_cast<SchemaW *>(parent());
    p->update();
}

void BlockW::paintEvent(QPaintEvent * event)
{
    ResizableFrame::paintEvent(event);

    for (model::Vyraz * vv : get_vypocty())
    {
        VyrazW * v = static_cast<VyrazW *>(vv);

        if (v->get_input_port() != nullptr && v->get_output_port() != nullptr)
        {
            PortW * pin   = static_cast<PortW *>(v->get_input_port());
            PortW * pout  = static_cast<PortW *>(v->get_output_port());

            if (pin == nullptr || pout == nullptr )
            {
                return;
            }

            QPainter painter(this);
            QPen pen;
            pen.setStyle(Qt::DotLine);
            pen.setWidth(1);
            pen.setDashOffset(4);

            pen.setBrush(QColor(50, 240, 220));
            painter.setPen(pen);



            painter.drawLine(0, 42 * (pin->get_cislo() + 1),
                             v->geometry().topLeft().x(), v->geometry().topLeft().y() + (v->height() / 2));

            pen.setBrush(QColor(227, 140, 255));
            painter.setPen(pen);
            painter.drawLine(v->geometry().topRight().x(), v->geometry().topRight().y() + (v->height() / 2),
                             this->width(), 42 * (pout->get_cislo() + 1));

        }


    }
}



}
}
