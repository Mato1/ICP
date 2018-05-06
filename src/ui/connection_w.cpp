/**
 * @brief 
 * 
 * @file connection_w.cpp
 * @author Libor Moravcik, Matej Mrazik
 * @date 2018-05-06
 */
#include "ui/connection_w.h"
#include <QPalette>
#include <QColor>
#include <QPainter>

namespace icp
{
namespace ui
{

ConnectionW::ConnectionW(BlockW * const output_block, PortW * const output_port,
                         BlockW * const input_block, PortW * const input_port)
    : model::Prepojenie (output_block, output_port, input_block, input_port)
{

}


}
}