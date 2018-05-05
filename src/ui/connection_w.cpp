#include "ui/connection_w.h"
#include <QPalette>
#include <QColor>

namespace icp
{
namespace ui
{
ConnectionW::ConnectionW(BlockW * const output_block, PortW * const output_port,
                         BlockW * const input_block, PortW * const input_port)
    : model::Prepojenie (output_block, output_port, input_block, input_port)
{
    QPalette pal = palette();
    setMinimumHeight(10);
    pal.setColor(QPalette::Background, QColor(200, 200, 200));
    setAutoFillBackground(true);
    setPalette(pal);

    show();
}
}
}