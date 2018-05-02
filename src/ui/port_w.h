#ifndef ICP_BLOCKBUILDER_SRC_UI_PORTW_H
#define ICP_BLOCKBUILDER_SRC_UI_PORTW_H

#include <QWidget>
#include "model/port.h"

namespace icp 
{
namespace ui
{
class PortW : public QWidget
{
    Q_OBJECT

    public slots:
        // void show_context_menu(const QPoint &pos);


    private:
        std::string style_sheet  = "background-color:rgb(64, 140, 20);";

    private:
        model::Port * port;

    public:
        PortW(std::string nazov_bloku, int cislo, model::PortType type);
        
        inline model::Port * get_port()
        {
            return port;
        }
};
 
}
}

#endif //ICP_BLOCKBUILDER_SRC_UI_PORT_H