#ifndef ICP_BLOCKBUILDER_SRC_UI_PORTSW_H
#define ICP_BLOCKBUILDER_SRC_UI_PORTSW_H

#include <QWidget>
#include "ui/port_w.h"
#include <QHBoxLayout>

namespace icp 
{
namespace ui
{

class PortsW : public QWidget
{
    QHBoxLayout * layout = nullptr;

    public:
        PortsW()
        {
            layout = new QHBoxLayout();
            setLayout(layout);
        }

        void add_port(PortW * port)
        {
            layout->addWidget(port);
            port->setParent(this);
            show();
        }

};

}
}

#endif //ICP_BLOCKBUILDER_SRC_UI_PORTSW_H