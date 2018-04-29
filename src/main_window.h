#ifndef ICP_BLOCKBUILDER_SRC_MAIN_WINDOW_H
#define ICP_BLOCKBUILDER_SRC_MAIN_WINDOW_H

#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QLabel>
#include <QVBoxLayout>
#include <QPalette>
#include "ui/schema_widget.h"


namespace icp {
    class MainWindow : public QMainWindow {
        private:
            const std::string DEFAULT_WINDOW_TITLE = "BlockBuilder";
            const int DEFAULT_PREFFERED_WINDOW_WIDTH  = 1280;
            const int DEFAULT_PREFFERED_WINDOW_HEIGHT = 960;
            const int DEFAULT_MIN_WINDOW_WIDTH = 640;
            const int DEFAULT_MIN_WINDOW_HEIGHT = 480;

        private:
            QMenu * menu_insert;
            QMenu * menu_subor;
            QAction * action_new_schema;
            QAction * action_new_block;
            QAction * action_new_connection;
            QAction * action_new_port;
            QAction * action_new_expression;
            QAction * action_save_schema;
            QAction * action_load_schema;
            QAction * action_close_schema; 
            ui::SchemaWidget * schema_widget;
            
        private:
            void set_up_menu();

        public:
            MainWindow();

        private slots:
            void new_schema();
    };
}

#endif //ICP_BLOCKBUILDER_SRC_MAIN_WINDOW_H