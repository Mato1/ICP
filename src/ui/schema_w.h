#ifndef ICP_BLOCKBUILDER_SRC_UI_SCHEMAW_H
#define ICP_BLOCKBUILDER_SRC_UI_SCHEMAW_H

#include "model/schema.h"
#include "ui/block_w.h"
#include <QWidget>
#include <QPainter>

namespace icp
{
    namespace ui 
    {
        class SchemaW : public QWidget, public model::Schema {
            
            Q_OBJECT

            private:
                // model::Schema * schema = nullptr;
                

            public slots:
                void show_context_menu(const QPoint &pos);
                void new_block();
                void new_connection(); 
                void save_schema(); 
                void load_schema();
                void close_schema();  
                void play_schema();
                void next_step();  
                void new_schema();    
                   
            private:    
                void init();

            public:
                SchemaW(std::string nazov);
                ~SchemaW();

        };
    }
}

#endif //ICP_BLOCKBUILDER_SRC_UI_SCHEMAW_H