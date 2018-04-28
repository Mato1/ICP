#include "main_window.h"
#include <QWidget>
#include <iostream>


namespace icp {

    MainWindow::MainWindow()
    {
        // QWidget *widget = new QWidget;
        // setCentralWidget(widget);

        // QWidget *topFiller = new QWidget;
        // topFiller->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        // QWidget *bottomFiller = new QWidget;
        // bottomFiller->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        // QVBoxLayout *layout = new QVBoxLayout;
        // layout->setMargin(5);
        // layout->addWidget(topFiller);
        // // layout->addWidget(infoLabel);
        // layout->addWidget(bottomFiller);
        // widget->setLayout(layout);

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
    
    }

    void MainWindow::new_schema()
    {
        schema_widget = new ui::SchemaWidget();
        setCentralWidget(schema_widget);
        schema_widget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        QPalette palette = schema_widget->palette();
        palette.setColor(QPalette::Background, QColor(40,44,52));
        schema_widget->setAutoFillBackground(true);
        schema_widget->setPalette(palette);
        schema_widget->show();
    }
}