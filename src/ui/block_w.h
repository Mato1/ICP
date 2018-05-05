#ifndef ICP_BLOCKBUILDER_SRC_UI_BLOCK_H
#define ICP_BLOCKBUILDER_SRC_UI_BLOCK_H

#include "main_window.h"
#include "ui/resizable_frame.h"
#include "ui/port_w.h"
#include "ui/vyraz_w.h"
#include "model/block.h"
#include <string>
#include <QHBoxLayout>
#include <QWidget>
#include <QMouseEvent>
#include <vector>

namespace icp
{
namespace ui
{

class BlockW : public ResizableFrame, public model::Block
{
    Q_OBJECT

public slots:
    void show_context_menu(const QPoint &pos);
    void eval_block();
    void rename_block();
    void add_expression();
    void add_input_port();
    void add_output_port();
    void delete_block();

private:
    void add_port(model::PortType type);
    int MINIMUM_WIDTH  = icp::MainWindow::GRID_SQUARE_SIZE * 1;
    int MINIMUM_HEIGHT = icp::MainWindow::GRID_SQUARE_SIZE * 1;
    int BASE_WIDTH     = icp::MainWindow::GRID_SQUARE_SIZE * 2;
    int BASE_HEIGHT    = icp::MainWindow::GRID_SQUARE_SIZE * 2;
    const int LINE_WIDTH = 3;

    QLabel label_nazov;
    QVBoxLayout layout_block;

    std::vector<PortW *> input_ports;
    std::vector<PortW *> output_ports;



public:
    BlockW(std::string nazov);
    ~BlockW();

    void mouseReleaseEvent(QMouseEvent * e) override;
    void mouseMoveEvent(QMouseEvent * event) override;

    inline void set_nazov(std::string nazov)
    {
        Block::set_nazov(nazov);
        this->label_nazov.setText(nazov.c_str());
    }

    void paintEvent(QPaintEvent * event) override;

};

}
}

#endif //ICP_BLOCKBUILDER_SRC_UI_BLOCK_H