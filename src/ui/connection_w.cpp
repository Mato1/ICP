#include "ui/connection_w.h"
#include <QPalette>
#include <QColor>
#include <QPainter>

namespace icp
{
namespace ui
{

ConnectionW::ConnectionW(BlockW * const output_block, PortW * const output_port,
                         BlockW * const input_block, PortW * const input_port)
    : model::Prepojenie (output_block, output_port, input_block, input_port)
{
    show();
}

void ConnectionW::paintEvent(QPaintEvent * event)
{
  
    QPainter painter(this);
    QPen pen;

    pen.setStyle(Qt::SolidLine);
    pen.setWidth(8);
    pen.setBrush(QColor(255, 175, 0));
    painter.setPen(pen);

    PortW * out_p = static_cast<PortW*>(get_output_port());
    PortW * in_p = static_cast<PortW*>(get_input_port());

    QRect out_geom = out_p->geometry();
    QRect in_geom = in_p->geometry();
    
    QPoint b_point = out_geom.topRight();
    QPoint e_point = in_geom.topLeft();

    QPoint middle_point = (b_point + e_point) / 2;

    painter.drawLine(b_point.x(), b_point.y(), e_point.x(), e_point.y());
    // painter.drawLine(middle_point.x(), b_point.y(), middle_point.x(), e_point.y());
    // painter.drawLine(middle_point.x(), e_point.y(), e_point.x(), e_point.y());
    
    
}

}
}