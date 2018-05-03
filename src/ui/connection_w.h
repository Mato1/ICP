#ifndef ICP_BLOCKBUILDER_SRC_UI_CONNECTION_H
#define ICP_BLOCKBUILDER_SRC_UI_CONNECTION_H

#include <QWidget>
#include "model/prepojenie.h"
#include "ui/block_w.h"
#include "ui/port_w.h"

namespace icp 
{
namespace ui 
{
    class ConnectionW : public QWidget, public model::Prepojenie
    {
        Q_OBJECT

        public slots:

        private:

        public:
            ConnectionW(BlockW * const output_block, PortW * const output_port, BlockW * const input_block, PortW * const input_port);

    
    };
}
}
#endif