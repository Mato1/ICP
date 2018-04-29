#include "ui/block_widget.h"

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
        }


    }
}