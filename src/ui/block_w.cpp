#include "ui/block_w.h"
#include "ui/schema_w.h"
#include <QInputDialog>
#include <QLineEdit>
#include <iostream>

namespace icp 
{
namespace ui 
{

BlockW::BlockW(std::string nazov)
    : model::Block(nazov)
{
    setMinimumWidth(MINIMUM_WIDTH);
    setMinimumHeight(MINIMUM_HEIGHT);
    
    layout_base = new QVBoxLayout();
    // block = new model::Block(nazov);
    block_body = new BlockBody(nazov);
    w_input_ports = new PortsW();
    w_output_ports = new PortsW();

    setLayout(layout_base);
    layout_base->addWidget(w_input_ports);
    layout_base->addWidget(block_body);
    layout_base->addWidget(w_output_ports);
    
    setLineWidth(1);
    setFrameShape(Shape::Box);
    
    QSizePolicy input_ports_sp(QSizePolicy::Preferred, QSizePolicy::Preferred);
    input_ports_sp.setVerticalStretch(1);
    w_input_ports->setSizePolicy(input_ports_sp);

    QSizePolicy body_block_sp(QSizePolicy::Preferred, QSizePolicy::Preferred);
    body_block_sp.setVerticalStretch(5);
    block_body->setSizePolicy(body_block_sp);

    QSizePolicy output_ports_sp(QSizePolicy::Preferred, QSizePolicy::Preferred);
    output_ports_sp.setVerticalStretch(1);
    w_output_ports->setSizePolicy(output_ports_sp);
    
    block_body->setParent(this);
    w_input_ports->setParent(this);
    w_output_ports->setParent(this);

}

void BlockW::rename_block()
{
    SchemaW * schema = (SchemaW*) parent();
    bool ok;
    QString text = tr(get_nazov().c_str());

    do 
    {
        text = QInputDialog::getText(this,tr("Nazov bloku"),
                                     tr("Novy nazov:"), QLineEdit::Normal,
                                     text, &ok);
                
        if (ok == true && text.toStdString().compare(get_nazov()) == 0)
        {
            return;
        }

    } while (schema->get_block(text.toStdString()) != nullptr && ok == true);

    if (ok == false)
    {
        return;
    }

    set_nazov(text.toStdString());
    set_nazov(get_nazov());
           
}

void BlockW::add_expression()
{
    // std::cout << "Add expression to block" << std::endl;
    QString text = "";
    QString text2 = "";
    bool ok;
    QStringList items_input;
    QStringList items_output;

    for (unsigned i=0; i < get_input_ports().size(); i++) 
    {
        items_input << tr (get_input_ports()[i]->get_nazov().c_str());
    }

    for (unsigned i=0; i < get_output_ports().size(); i++) 
    {
        items_output << tr (get_output_ports()[i]->get_nazov().c_str());
    }

    QString item_input = QInputDialog::getItem(this, tr("Input port"),
                                         tr("Name port"), items_input, 0, false, &ok);

    QString item_output = QInputDialog::getItem(this, tr("Output port"),
                                         tr("Name port"), items_output, 0, false, &ok);

    text = QInputDialog::getText(this,tr("Vyraz:"),
                                 tr("Vzorec:"), QLineEdit::Normal,
                                 text, &ok);
    text2 = QInputDialog::getText(this,tr("Vyraz:"),
                                 tr("Premenna:"), QLineEdit::Normal,
                                 text2, &ok);

    if (ok == false)
    {
        return;
    }

    VyrazW * vyraz_w = new VyrazW(text.toStdString(), text2.toStdString());
    this->add_vypocet(vyraz_w);

    for (unsigned i=0; i < get_input_ports().size(); i++) 
    {
        if (item_input.toStdString() == get_input_ports()[i]->get_nazov().c_str())
        {
            vyraz_w->assign_input_port(get_input_ports()[i]);
            std::cout << vyraz_w->get_input_ports()[0]->get_nazov() << std::endl;
        }
    }

    for (unsigned i=0; i < get_output_ports().size(); i++) 
    {
        if (item_output.toStdString() == get_output_ports()[i]->get_nazov().c_str())
        {
            vyraz_w->assign_output_port(get_output_ports()[i]);
            std::cout << vyraz_w->get_output_ports()[0]->get_nazov() << std::endl;
        }
    }

    block_body->add_expression(vyraz_w);
}

void BlockW::add_input_port()
{

    QString text = "";
    bool ok;
    text = QInputDialog::getText(this,tr("Datovy Typ:"),
                                 tr("Premenne:"), QLineEdit::Normal,
                                 text, &ok);

    if (ok == false)
    {
        return;
    }

    PortW * port_w = new PortW(get_nazov(), get_input_ports().size(), model::PortType::input);
    w_input_ports->add_port(port_w);
    this->add_port(port_w);
    port_w->set_data_type(text.toStdString());
    std::cout << port_w->get_data_type().to_string() << std::endl;

}

void BlockW::add_output_port()
{
    QString text = "";
    bool ok;
    text = QInputDialog::getText(this,tr("Datovy Typ:"),
                                 tr("Premenne:"), QLineEdit::Normal,
                                 text, &ok);

    if (ok == false)
    {
        return;
    }

    PortW * port_w = new PortW(get_nazov(), get_output_ports().size(), model::PortType::output);
    w_output_ports->add_port(port_w);
    add_port(port_w);
    port_w->set_data_type(text.toStdString());
    std::cout << port_w->get_data_type().to_string() << std::endl;
}

}
}
