#ifndef ICP_BLOCKBUILDER_SRC_UI_BLOCKBODY_H
#define ICP_BLOCKBUILDER_SRC_UI_BLOCKBODY_H

#include <QFrame>
#include <QLabel>
#include <QListView>
#include <QVBoxLayout>
#include <QColor>

namespace icp 
{
namespace ui 
{

class BlockBody : public QFrame
{
    Q_OBJECT

    public slots:
        void show_context_menu(const QPoint &pos);


    private: 
        const int LINE_WIDTH     = 3;

    private:
        QPoint drag_start_pos;
        QRect  drag_start_geom;
        QPoint offset;
        QLabel label_nazov;
        QVBoxLayout layout_block;

    public:
        BlockBody(std::string nazov);
        
        inline void set_nazov(std::string nazov)
        {
            this->label_nazov.setText(nazov.c_str());
        }


        inline void add_expression(QLabel * label)
        {
            layout_block.addWidget(label);
        }
};

}
}

#endif // !ICP_BLOCKBUILDER_SRC_UI_BLOCK_BLOCKBODY_H