#ifndef ICP_BLOCKBUILDER_SRC_UI_RESIZABLE_FRAME_H
#define ICP_BLOCKBUILDER_SRC_UI_RESIZABLE_FRAME_H

#include <QFrame>
#include <QMouseEvent>
#include <QWidget>

namespace icp
{
    namespace ui
    {
        class ResizableFrame : public QFrame
        {
            Q_OBJECT

            public:
                explicit ResizableFrame(QWidget *parent = 0);
                ~ResizableFrame();

                enum StartPosition {topleft, left, bottomleft, bottom, bottomright, right, topright, top, move_mouse};

            protected:
                void mousePressEvent(QMouseEvent * event) override;
                void mouseMoveEvent(QMouseEvent * event) override;

            private:
                QPoint drag_start_pos;
                QRect drag_start_geom;
                enum StartPosition start_pos;
        };
    }
}

#endif //ICP_BLOCKBUILDER_SRC_UI_RESIZABLE_FRAME_H