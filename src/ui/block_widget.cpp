#include "ui/block_widget.h"
#include <iostream>
#include <QInputDialog>

namespace icp
{
    namespace ui 
    {
        BlockWidget::BlockWidget(std::string nazov)
        {
            block = new model::Block(nazov);
            setStyleSheet(style_sheet.c_str());
            setLayout(&layout_base);
            setLineWidth(LINE_WIDTH);
            setFrameShape(Shape::Box);
            label_nazov.setText(tr(nazov.c_str()));        
            setMinimumWidth(MINIMUM_WIDTH);
            setMinimumHeight(MINIMUM_HEIGHT);
            layout_base.addWidget(&label_nazov);
            label_nazov.setAlignment(Qt::AlignCenter);

            this->setContextMenuPolicy(Qt::CustomContextMenu);
            connect(this, &BlockWidget::customContextMenuRequested, this, &BlockWidget::show_context_menu);
        }


        void BlockWidget::show_context_menu(const QPoint &pos)
        {
            QMenu contextMenu(tr("Context menu"), this);
            QAction action_edit_block("Rename Block", this);
            contextMenu.addAction(&action_edit_block);
            connect(&action_edit_block, &QAction::triggered, this, &BlockWidget::rename_block);

            // podla mna by to mohlo byt aj tu :D 
            QAction action_add_expression("Add expression", this);
            contextMenu.addAction(&action_add_expression);
            connect(&action_add_expression, &QAction::triggered, this, &BlockWidget::add_expression);

            QAction action_add_input_port("Add input port", this);
            contextMenu.addAction(&action_add_input_port);
            connect(&action_add_input_port, &QAction::triggered, this, &BlockWidget::add_input_port);

            QAction action_add_output_port("Add output port", this);
            contextMenu.addAction(&action_add_output_port);
            connect(&action_add_output_port, &QAction::triggered, this, &BlockWidget::add_output_port);
            contextMenu.exec(mapToGlobal(pos));
        }

        void BlockWidget::rename_block()
        {
           bool ok;
           QString text = QInputDialog::getText(this, tr(block->get_nazov().c_str()),
                                         tr("Novy nazov:"), QLineEdit::Normal,
                                         block->get_nazov().c_str(), &ok);
        }

        void BlockWidget::add_expression()
        {
            std::cout << "Add expression to block" << std::endl;
        }

        void BlockWidget::add_input_port()
        {
            std::cout << "Add input port to block" << std::endl;
        }

        void BlockWidget::add_output_port()
        {
            std::cout << "Add output port to block" << std::endl;
        }

    }
}