#ifndef ICP_BLOCKBUILDER_SRC_UI_SCHEMA_WIDGET_H
#define ICP_BLOCKBUILDER_SRC_UI_SCHEMA_WIDGET_H

#include "model/schema.h"
#include "ui/block_widget.h"
#include <QWidget>

namespace icp
{
    namespace ui 
    {
        class SchemaWidget : public QWidget {
            
            Q_OBJECT

            private:
                model::Schema * schema = nullptr;

            public slots:
                void show_context_menu(const QPoint &pos);
                void new_block();
                void new_connection(); 
                void new_port();
                void new_expression();
                void save_schema(); 
                void load_schema();
                void close_schema();           
            private:    
                void init();

            public:
                SchemaWidget(std::string nazov);
                ~SchemaWidget();
                
                inline model::Schema * get_schema() const
                {
                    return schema;
                }
      

        };
    }
}

#endif //ICP_BLOCKBUILDER_SRC_UI_SCHEMA_WIDGET_H