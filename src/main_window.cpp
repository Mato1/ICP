#include "main_window.h"
#include <QWidget>
#include <iostream>


namespace icp {

    MainWindow::MainWindow()
    {
        setWindowTitle(tr(DEFAULT_WINDOW_TITLE.c_str()));
        setMinimumSize(DEFAULT_MIN_WINDOW_WIDTH, DEFAULT_MIN_WINDOW_HEIGHT);
        resize(DEFAULT_PREFFERED_WINDOW_WIDTH, DEFAULT_PREFFERED_WINDOW_HEIGHT);
        set_up_menu();
    }
    
    void MainWindow::set_up_menu()
    {          
        menu_subor = menuBar()->addMenu(tr("&Schema"));
        action_new_schema = new QAction(tr("&New"), this);
        action_new_schema->setShortcuts(QKeySequence::New);
        action_new_schema->setStatusTip(tr("Create a new schema"));
        connect(action_new_schema, &QAction::triggered, this, &MainWindow::new_schema);
        menu_subor->addAction(action_new_schema);

        action_save_schema = new QAction(tr("&Save"), this);
        action_save_schema->setStatusTip(tr("Save a schema"));
        menu_subor->addAction(action_save_schema);

        action_load_schema = new QAction(tr("&Load"), this);
        action_load_schema->setStatusTip(tr("Load a schema"));
        menu_subor->addAction(action_load_schema);

        action_close_schema = new QAction(tr("&Close"), this);
        action_close_schema->setStatusTip(tr("Close a schema"));
        menu_subor->addAction(action_close_schema);

        menu_insert = menuBar()->addMenu(tr("&Insert"));
        action_new_block = new QAction(tr("&Block"), this);
        action_new_block->setStatusTip(tr("Create a new block"));
        menu_insert->addAction(action_new_block);

        action_new_connection = new QAction(tr("&Connection"), this);
        action_new_connection->setStatusTip(tr("Create a new connection."));
        menu_insert->addAction(action_new_connection);


        menu_insert = menuBar()->addMenu(tr("&Play"));
        action_play_schema = new QAction(tr("&Start play"), this);
        action_play_schema->setStatusTip(tr("Start play"));
        menu_insert->addAction(action_play_schema);

        action_next_step = new QAction(tr("&Next step"), this);
        action_next_step->setStatusTip(tr("Do next step"));
        menu_insert->addAction(action_next_step);

    }

    void MainWindow::new_schema()
    {
        schema_widget = new ui::SchemaW("Untitled Schema");
        setCentralWidget(schema_widget);
        setWindowTitle(tr((DEFAULT_WINDOW_TITLE + " - " + schema_widget->get_schema()->get_nazov()).c_str()));
        schema_widget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        QPalette palette = schema_widget->palette();
        palette.setColor(QPalette::Background, QColor(40,44,52));
        schema_widget->setAutoFillBackground(true);
        schema_widget->setPalette(palette);
        schema_widget->show();
        
        connect(action_save_schema, &QAction::triggered, schema_widget, &ui::SchemaW::save_schema);
        connect(action_load_schema, &QAction::triggered, schema_widget, &ui::SchemaW::load_schema);
        connect(action_close_schema, &QAction::triggered, schema_widget, &ui::SchemaW::close_schema);
        connect(action_new_block, &QAction::triggered, schema_widget, &ui::SchemaW::new_block);
        connect(action_new_connection, &QAction::triggered, schema_widget, &ui::SchemaW::new_connection);
        connect(action_play_schema, &QAction::triggered, schema_widget, &ui::SchemaW::play_schema);
        connect(action_next_step, &QAction::triggered, schema_widget, &ui::SchemaW::next_step);
    }
}