#ifndef ICP_BLOCKBUILDER_SRC_UI_VYRAZ_H
#define ICP_BLOCKBUILDER_SRC_UI_VYRAZ_H

#include <QLabel>
#include "model/vyraz.h"

namespace icp 
{
namespace ui 
{
    class VyrazW : public QLabel, public model::Vyraz
    {
        Q_OBJECT

        public slots:

    public:
        VyrazW(std::string expression, std::string premenna);

    };
}
}
#endif