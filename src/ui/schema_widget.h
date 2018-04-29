#ifndef ICP_BLOCKBUILDER_SRC_UI_SCHEMA_WIDGET_H
#define ICP_BLOCKBUILDER_SRC_UI_SCHEMA_WIDGET_H

#include "model/schema.h"
#include <QWidget>

namespace icp
{
    namespace ui 
    {
        class SchemaWidget : public QWidget, public model::Schema {
            
            Q_OBJECT

            public slots:
                void show_context_menu(const QPoint &pos);
                void new_block();
                void new_connection();                
            private:
                void init();

            public:
                SchemaWidget(std::string nazov);

        };
    }
}

#endif //ICP_BLOCKBUILDER_SRC_UI_SCHEMA_WIDGET_H