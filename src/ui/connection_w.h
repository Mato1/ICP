#ifndef ICP_BLOCKBUILDER_SRC_UI_CONNECTION_H
#define ICP_BLOCKBUILDER_SRC_UI_CONNECTION_H

#include <QWidget>
#include "model/prepojenie.h"

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
            ConnectionW(Block * const output_block, Port * const output_port, Block * const input_block, Port * const input_port);

    
    };
}
}
#endif