#include "ui/block_body_w.h"

#include <QMenu>
#include "ui/block_w.h"
#include <QPalette>

namespace icp 
{
namespace ui 
{

BlockBody::BlockBody(std::string nazov)
{
    setLayout(&layout_block);
    setLineWidth(LINE_WIDTH);
    setFrameShape(Shape::Box);
    label_nazov.setText(tr(nazov.c_str()));
    layout_block.addWidget(&label_nazov);
    label_nazov.setAlignment(Qt::AlignCenter);
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &BlockBody::customContextMenuRequested, this, &BlockBody::show_context_menu);
    
    QPalette pal = palette();
    pal.setColor(QPalette::Background, QColor(64,92,104));
    pal.setColor(QPalette::Foreground, Qt::white);
    setAutoFillBackground(true);
    setPalette(pal);

}

void BlockBody::show_context_menu(const QPoint &pos)
{
    BlockW * block = (BlockW *) parent();

    QMenu contextMenu(tr("Context menu"), this);

    QAction action_edit_block("Rename Block", block);
    contextMenu.addAction(&action_edit_block);
    connect(&action_edit_block, &QAction::triggered, block, &BlockW::rename_block);

    // podla mna by to mohlo byt aj tu :D 
    QAction action_add_expression("Add expression", block);
    contextMenu.addAction(&action_add_expression);
    connect(&action_add_expression, &QAction::triggered, block, &BlockW::add_expression);

    QAction action_add_input_port("Add input port", block);
    contextMenu.addAction(&action_add_input_port);
    connect(&action_add_input_port, &QAction::triggered, block, &BlockW::add_input_port);

    QAction action_add_output_port("Add output port", block);
    contextMenu.addAction(&action_add_output_port);
    connect(&action_add_output_port, &QAction::triggered, block, &BlockW::add_output_port);
    contextMenu.exec(mapToGlobal(pos));
}


}
}
