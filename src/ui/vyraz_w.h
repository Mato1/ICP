#ifndef ICP_BLOCKBUILDER_SRC_UI_VYRAZ_H
#define ICP_BLOCKBUILDER_SRC_UI_VYRAZ_H

#include <QLabel>
#include "model/vyraz.h"

namespace icp 
{
namespace ui 
{
    class VyrazW : public QLabel
    {
        Q_OBJECT

        public slots:


    private:
        model:: Vyraz * vyraz;

    public:
        VyrazW(std::string expression);

        inline model::Vyraz * get_infix_w()
        {
            return vyraz;
        }

    };
}
}
#endif