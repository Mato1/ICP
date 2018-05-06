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

private slots:
    void s_show_context_menu(const QPoint &pos);
    void s_rename_block();
    void s_add_expression();
    void s_add_input_port();
    void s_add_output_port();
    void s_delete_block();

public slots:
    void eval_block();

private:
    int MINIMUM_WIDTH  = icp::MainWindow::GRID_SQUARE_SIZE * 1;
    int MINIMUM_HEIGHT = icp::MainWindow::GRID_SQUARE_SIZE * 1;
    int BASE_WIDTH     = icp::MainWindow::GRID_SQUARE_SIZE * 2;
    int BASE_HEIGHT    = icp::MainWindow::GRID_SQUARE_SIZE * 2;
    const int LINE_WIDTH = 3;

    QLabel label_nazov;
    QVBoxLayout layout_block;

    std::vector<PortW *> input_ports;
    std::vector<PortW *> output_ports;
    void add_port(model::PortType type);



public:
    BlockW(std::string nazov, QWidget * parent = 0);
    ~BlockW();

    void add_expression(VyrazW * v);
    void add_port(PortW * p);

    void mouseReleaseEvent(QMouseEvent * e) override;
    void mouseMoveEvent(QMouseEvent * event) override;
    void paintEvent(QPaintEvent * event) override;

    inline void set_nazov(std::string nazov)
    {
        Block::set_nazov(nazov);
        this->label_nazov.setText(nazov.c_str());
    }
};

}
}

#endif //ICP_BLOCKBUILDER_SRC_UI_BLOCK_H