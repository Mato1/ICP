#ifndef ICP_BLOCKBUILDER_SRC_UI_BLOCKBODY_H
#define ICP_BLOCKBUILDER_SRC_UI_BLOCKBODY_H

#include <QFrame>
#include <QLabel>
#include <QListView>
#include <QVBoxLayout>

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
        const int MINIMUM_WIDTH  = 100;
        const int MINIMUM_HEIGHT = 100;
        const int LINE_WIDTH     = 3;
        std::string style_sheet  = "background-color:rgb(64, 92, 104); selection-background-color: rgb(22, 22, 22)";

    private:
     
        QPoint drag_start_pos;
        QRect  drag_start_geom;
        QPoint offset;
        QLabel label_nazov;
        QVBoxLayout layout_block;
        QListView list_view;

    public:
        BlockBody(std::string nazov);
        
        inline void set_nazov(std::string nazov)
        {
            this->label_nazov.setText(nazov.c_str());
        }
};

}
}

#endif // !ICP_BLOCKBUILDER_SRC_UI_BLOCK_BLOCKBODY_H