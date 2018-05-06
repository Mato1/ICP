/**
 * @brief 
 * 
 * @file main_window.h
 * @author Libor Moravcik, Matej Mrazik
 * @date 2018-05-06
 */
#ifndef ICP_BLOCKBUILDER_SRC_MAIN_WINDOW_H
#define ICP_BLOCKBUILDER_SRC_MAIN_WINDOW_H

#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QLabel>
#include <QVBoxLayout>
#include <QPalette>
#include "ui/schema_w.h"


namespace icp
{

class MainWindow : public QMainWindow
{

public:

    static const int DEFAULT_PREFFERED_WINDOW_WIDTH  = 1280;
    static const int DEFAULT_PREFFERED_WINDOW_HEIGHT = 960;
    static const int DEFAULT_MIN_WINDOW_WIDTH        = 640;
    static const int DEFAULT_MIN_WINDOW_HEIGHT       = 480;
    static const int GRID_SQUARE_SIZE                = 40;

private:

    const std::string DEFAULT_WINDOW_TITLE = "BlockBuilder";
    const std::string DEFAULT_SCHEMA_TITLE = "Untitled_Schema";
    const std::string DEFAULT_WELCOME_MSG  = "Press CTRL+N to create a New schema";

private:

    QMenu * menu_insert;
    QMenu * menu_subor;
    QAction * action_new_schema;
    QAction * action_new_block;
    QAction * action_new_connection;
    QAction * action_save_schema;
    QAction * action_load_schema;
    QAction * action_close_schema;
    QAction * action_play_schema;
    QAction * action_next_step;
    ui::SchemaW * schema_widget;
    QLabel * default_text;

private:
    void set_up_menu();
    void create_schema(std::string nazov);

public:
    MainWindow();

private slots:
    void new_schema();
    void close_schema();
    void save_schema();
    void load_schema();
};

}

#endif //ICP_BLOCKBUILDER_SRC_MAIN_WINDOW_H