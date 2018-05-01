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
{
    layout_base = new QVBoxLayout();
    block = new model::Block(nazov);
    block_body = new BlockBody(nazov);
    input_ports = new PortsW();
    output_ports = new PortsW();

    setLayout(layout_base);
    layout_base->addWidget(input_ports);
    layout_base->addWidget(block_body);
    layout_base->addWidget(output_ports);
    
    setLineWidth(1);
    setFrameShape(Shape::Box);
    
  

    QSizePolicy input_ports_sp(QSizePolicy::Preferred, QSizePolicy::Preferred);
    input_ports_sp.setVerticalStretch(1);
    input_ports->setSizePolicy(input_ports_sp);

    QSizePolicy body_block_sp(QSizePolicy::Preferred, QSizePolicy::Preferred);
    body_block_sp.setVerticalStretch(5);
    block_body->setSizePolicy(body_block_sp);

    QSizePolicy output_ports_sp(QSizePolicy::Preferred, QSizePolicy::Preferred);
    output_ports_sp.setVerticalStretch(1);
    output_ports->setSizePolicy(output_ports_sp);
    
    block_body->setParent(this);
    input_ports->setParent(this);
    output_ports->setParent(this);

}

void BlockW::rename_block()
{
    SchemaW * schema = (SchemaW*) parent();
    bool ok;
    QString text = tr(block->get_nazov().c_str());

    do 
    {
        text = QInputDialog::getText(this,tr("Nazov bloku"),
                                     tr("Novy nazov:"), QLineEdit::Normal,
                                     text, &ok);
                
        if (ok == true && text.toStdString().compare(block->get_nazov()) == 0)
        {
            return;
        }

    } while (schema->get_schema()->get_block(text.toStdString()) != nullptr && ok == true);

        block->set_nazov(text.toStdString());
        block_body->set_nazov(block->get_nazov());
           
}

void BlockW::add_expression()
{
    std::cout << "Add expression to block" << std::endl;
}

void BlockW::add_input_port()
{
    std::cout << "Add input port to block" << std::endl;
}

void BlockW::add_output_port()
{
    std::cout << "Add output port to block" << std::endl;
}

}
}
