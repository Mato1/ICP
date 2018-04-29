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
        menu_subor = menuBar()->addMenu(tr("&File"));
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
        action_close_schema->setStatusTip(tr("CLose a schema"));
        menu_subor->addAction(action_close_schema);


        menu_insert = menuBar()->addMenu(tr("&Insert"));
        action_new_block = new QAction(tr("&New Block"), this);
        action_new_block->setStatusTip(tr("Create a new block"));
        menu_insert->addAction(action_new_block);

        action_new_connection = new QAction(tr("&New Connection"), this);
        action_new_connection->setStatusTip(tr("Create a new connection."));
        menu_insert->addAction(action_new_connection);

        action_new_port = new QAction(tr("&New Port"), this);
        action_new_port->setStatusTip(tr("Create a new port."));
        menu_insert->addAction(action_new_port);

        action_new_expression = new QAction(tr("&New expression"), this);
        action_new_expression->setStatusTip(tr("Create a new expression."));
        menu_insert->addAction(action_new_expression);

    }

    void MainWindow::new_schema()
    {
        schema_widget = new ui::SchemaWidget("Untitled Schema");
        setCentralWidget(schema_widget);
        setWindowTitle(tr((DEFAULT_WINDOW_TITLE + " - " + schema_widget->get_schema()->get_nazov()).c_str()));
        schema_widget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        QPalette palette = schema_widget->palette();
        palette.setColor(QPalette::Background, QColor(40,44,52));
        schema_widget->setAutoFillBackground(true);
        schema_widget->setPalette(palette);
        schema_widget->show();
        
        connect(action_save_schema, &QAction::triggered, schema_widget, &ui::SchemaWidget::save_schema);
        connect(action_load_schema, &QAction::triggered, schema_widget, &ui::SchemaWidget::load_schema);
        connect(action_close_schema, &QAction::triggered, schema_widget, &ui::SchemaWidget::close_schema);
        connect(action_new_block, &QAction::triggered, schema_widget, &ui::SchemaWidget::new_block);
        connect(action_new_connection, &QAction::triggered, schema_widget, &ui::SchemaWidget::new_connection);
        connect(action_new_port, &QAction::triggered, schema_widget, &ui::SchemaWidget::new_port);
        connect(action_new_expression, &QAction::triggered, schema_widget, &ui::SchemaWidget::new_expression);
    }
}