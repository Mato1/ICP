#ifndef ICP_BLOCKBUILDER_SRC_UI_BLOCK_WIDGET_H
#define ICP_BLOCKBUILDER_SRC_UI_BLOCK_WIDGET_H

#include <QFrame>
#include <QLabel>
#include <QVBoxLayout>
#include <QListView>
#include <QMouseEvent>
#include "model/block.h"
#include "ui/resizable_frame.h"

namespace icp
{
    namespace ui
    {
        class BlockWidget : public ResizableFrame
        {
            Q_OBJECT
            private:
                const int MINIMUM_WIDTH  = 100;
                const int MINIMUM_HEIGHT = 100;
                const int LINE_WIDTH     = 3;
                model::Block * block = nullptr;
                std::string style_sheet = "background-color:rgb(64, 92, 104);";

            private:
                QPoint drag_start_pos;
                QRect  drag_start_geom;
                QPoint offset;
                QLabel label_nazov;
                QVBoxLayout layout_base;
                QListView list_view;

            public:
                BlockWidget(std::string nazov);

                inline model::Block * get_block() const
                {
                    return block;
                }
        };
    }
}

#endif //ICP_BLOCKBUILDER_SRC_UI_BLOCK_WIDGET_H