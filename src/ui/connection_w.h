#ifndef ICP_BLOCKBUILDER_SRC_UI_CONNECTION_H
#define ICP_BLOCKBUILDER_SRC_UI_CONNECTION_H


#include "model/prepojenie.h"
#include "ui/block_w.h"
#include "ui/port_w.h"
#include <QPainterPath>

namespace icp
{
namespace ui
{
class ConnectionW : public model::Prepojenie
{

private:
    QPainterPath path;

public:
    ConnectionW(BlockW * const output_block, PortW * const output_port,
                BlockW * const input_block, PortW * const input_port);

    inline void set_painter_path(QPainterPath p)
    {
        this->path = p;
    }
    
     inline QPainterPath get_painter_path()
    {
        return this->path;
    }

};
}
}
#endif