#ifndef ICP_BLOCKBUILDER_SRC_UI_SCHEMAW_H
#define ICP_BLOCKBUILDER_SRC_UI_SCHEMAW_H

#include "model/schema.h"
#include <QWidget>
#include <QPainter>
#include <QMenu>


namespace icp
{
namespace ui
{

class SchemaW : public QWidget, public model::Schema
{

    Q_OBJECT

private:
    QMenu  *  context_menu        = nullptr;
    QAction * action_insert_block = nullptr;
    QAction * action_insert_con   = nullptr;


public slots:
    void show_context_menu(const QPoint &pos);
    void new_block();
    void new_connection();
    void play_schema();
    void next_step();

private:
    void init();

public:
    SchemaW(std::string nazov);
    ~SchemaW();

    void paintEvent(QPaintEvent * event) override;

    std::string get_nazov()
    {
        return this->nazov;
    }

    void mouseMoveEvent(QMouseEvent * event) override;

    std::vector<model::Block*> get_blocks()
    {
        return blocks;
    }

    std::vector<model::Prepojenie*> get_prepojenia()
    {
        return prepojenia;
    }
};
}
}

#endif //ICP_BLOCKBUILDER_SRC_UI_SCHEMAW_H