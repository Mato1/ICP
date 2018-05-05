#include "ui/resizable_frame.h"


namespace icp
{
namespace ui
{
ResizableFrame::ResizableFrame(QWidget * parent)
    : QFrame(parent)
{
    setMouseTracking(true);
}

ResizableFrame::~ResizableFrame()
{

}

void ResizableFrame::mousePressEvent(QMouseEvent * event)
{
    if (event->button() == Qt::LeftButton)
    {
        drag_start_pos = event->pos();
        drag_start_geom = geometry();
    }
}

void ResizableFrame::mouseMoveEvent(QMouseEvent * event)
{
    if (!(event->buttons() & Qt::LeftButton))
    {
        // No drag, just change the cursor and return
        if (event->x() <= 3 && event->y() <= 3)
        {
            start_pos = topleft;
            setCursor(Qt::SizeFDiagCursor);
        }
        else if (event->x() <= 3 && event->y() >= height() - 3)
        {
            start_pos = bottomleft;
            setCursor(Qt::SizeBDiagCursor);
        }
        else if (event->x() >= width() - 3 && event->y() <= 3)
        {
            start_pos = topright;
            setCursor(Qt::SizeBDiagCursor);
        }
        else if (event->x() >= width() - 3 && event->y() >= height() - 3)
        {
            start_pos = bottomright;
            setCursor(Qt::SizeFDiagCursor);
        }
        else if (event->x() <= 3)
        {
            start_pos = left;
            setCursor(Qt::SizeHorCursor);
        }
        else if (event->x() >= width() - 3)
        {
            start_pos = right;
            setCursor(Qt::SizeHorCursor);
        }
        else if (event->y() <= 3)
        {
            start_pos = top;
            setCursor(Qt::SizeVerCursor);
        }
        else if (event->y() >= height() - 3)
        {
            start_pos = bottom;
            setCursor(Qt::SizeVerCursor);
        }
        else
        {
            start_pos = move_mouse;
            setCursor(Qt::SizeAllCursor);
        }

        return;
    }

    switch (start_pos)
    {
    case topleft:
        setGeometry(drag_start_geom.left()  - (drag_start_pos.x() - event->x()),
                    drag_start_geom.top()   - (drag_start_pos.y() - event->y()),
                    drag_start_geom.width() + (drag_start_pos.x() - event->x()),
                    height()                + (drag_start_pos.y() - event->y()));
        drag_start_geom = geometry();
        break;

    case bottomleft:
        setGeometry(drag_start_geom.left() - (drag_start_pos.x() - event->x()),
                    drag_start_geom.top(),
                    drag_start_geom.width() + (drag_start_pos.x() - event->x()),
                    event->y());
        drag_start_geom = geometry();
        break;

    case topright:
        setGeometry(drag_start_geom.left(),
                    drag_start_geom.top() - (drag_start_pos.y() - event->y()),
                    event->x(),
                    height() + (drag_start_pos.y() - event->y()));
        drag_start_geom = geometry();
        break;

    case bottomright:
        setGeometry(drag_start_geom.left(),
                    drag_start_geom.top(),
                    event->x(),
                    event->y());
        break;

    case left:
        setGeometry(drag_start_geom.left() - (drag_start_pos.x() - event->x()),
                    drag_start_geom.top(),
                    drag_start_geom.width() + (drag_start_pos.x() - event->x()),
                    height());
        drag_start_geom = geometry();
        break;

    case right:
        setGeometry(drag_start_geom.left(),
                    drag_start_geom.top(),
                    event->x(),
                    height());
        break;

    case top:
        setGeometry(drag_start_geom.left(),
                    drag_start_geom.top() - (drag_start_pos.y() - event->y()),
                    drag_start_geom.width(),
                    height() + (drag_start_pos.y() - event->y()));
        drag_start_geom = geometry();
        break;

    case bottom:
        setGeometry(drag_start_geom.left(),
                    drag_start_geom.top(),
                    width(),
                    event->y());
        break;

    case move_mouse:
        setGeometry(drag_start_geom.left() - (drag_start_pos.x() - event->x()),
                    drag_start_geom.top() - (drag_start_pos.y() - event->y()),
                    width(),
                    height());
        drag_start_geom = geometry();
        break;

    default:
        break;
    }
}
}
}