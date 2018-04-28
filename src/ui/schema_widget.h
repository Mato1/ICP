#ifndef ICP_BLOCKBUILDER_SRC_UI_SCHEMA_WIDGET_H
#define ICP_BLOCKBUILDER_SRC_UI_SCHEMA_WIDGET_H

#include "model/schema.h"
#include <QWidget>

namespace icp
{
    namespace ui 
    {
        class SchemaWidget : public QWidget {
            
            Q_OBJECT

            private slots:
                void show_context_menu(const QPoint &pos);
                void new_block();                
            private:
                void init();

            public:
                SchemaWidget();

        };
    }
}

#endif //ICP_BLOCKBUILDER_SRC_UI_SCHEMA_WIDGET_H