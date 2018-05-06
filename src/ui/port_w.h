/**
 * @brief 
 * 
 * @file port_w.h
 * @author Libor Moravcik, Matej Mrazik
 * @date 2018-05-06
 */
#ifndef ICP_BLOCKBUILDER_SRC_UI_PORTW_H
#define ICP_BLOCKBUILDER_SRC_UI_PORTW_H

#include <QWidget>
#include "model/port.h"
#include <QMouseEvent>
#include <QPaintEvent>
#include <QRgb>
#include <QColor>

namespace icp
{
namespace ui
{

class PortW : public QWidget, public model::Port
{
    Q_OBJECT

public slots:
    // void show_context_menu(const QPoint &pos);

private:
    const int MAX_HEIGHT = 8;
public:
    PortW(std::string nazov_bloku, int cislo, model::PortType type, QWidget * parent = 0);
    void mouseMoveEvent(QMouseEvent * event) override;
    void paintEvent(QPaintEvent * event) override;
};

}
}

#endif //ICP_BLOCKBUILDER_SRC_UI_PORT_H