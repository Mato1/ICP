#ifndef ICP_BLOCKBUILDER_SRC_UI_BLOCK_H
#define ICP_BLOCKBUILDER_SRC_UI_BLOCK_H

#include "ui/resizable_frame.h"
#include "ui/block_body_w.h"
#include "ui/ports_w.h"
#include "ui/port_w.h"
#include "ui/vyraz_w.h"
#include "model/block.h"
#include <string>
#include <QVBoxLayout>
#include <QWidget>

namespace icp
{
namespace ui
{

class BlockW : public ResizableFrame, public model::Block
{
    Q_OBJECT

    public slots:
        void rename_block();
        void add_expression();
        void add_input_port();
        void add_output_port();

    private:
        const int MINIMUM_WIDTH  = 100;
        const int MINIMUM_HEIGHT = 100;
        // model::Block * block       = nullptr;
        QVBoxLayout * layout_base  = nullptr;
        PortsW * w_input_ports     = nullptr;
        BlockBody  * block_body    = nullptr;
        PortsW * w_output_ports    = nullptr;
    public:
        BlockW(std::string nazov);

        // inline model::Block* get_block() const
        // {
        //     return block;
        // }
        
};

}
}

#endif //ICP_BLOCKBUILDER_SRC_UI_BLOCK_H