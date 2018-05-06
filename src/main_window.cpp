#include "main_window.h"
#include <QWidget>
#include <iostream>
#include <QFont>
#include <QFileDialog>
#include <fstream>
#include "model/block.h"
#include "ui/block_w.h"
#include "model/prepojenie.h"
#include "ui/connection_w.h"
#include "model/data_type.h"
#include "ui/vyraz_w.h"

namespace icp
{

MainWindow::MainWindow()
{
    setWindowTitle(tr(DEFAULT_WINDOW_TITLE.c_str()));
    setMinimumSize(DEFAULT_MIN_WINDOW_WIDTH, DEFAULT_MIN_WINDOW_HEIGHT);
    resize(DEFAULT_PREFFERED_WINDOW_WIDTH, DEFAULT_PREFFERED_WINDOW_HEIGHT);

    default_text = new QLabel(DEFAULT_WELCOME_MSG.c_str());
    QFont  font = default_text->font();
    font.setPixelSize(22);
    default_text->setFont(font);
    default_text->setAlignment(Qt::AlignCenter);

    setCentralWidget(default_text);
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

    menu_insert = menuBar()->addMenu(tr("&Action"));
    action_play_all_schema = new QAction(tr("&Eval All"), this);
    action_play_all_schema->setStatusTip(tr("Evaluate schema"));
    menu_insert->addAction(action_play_all_schema);

    
    action_play_schema = new QAction(tr("&Eval Step"), this);
    action_play_schema->setStatusTip(tr("Evaluate scheme step by step"));
    menu_insert->addAction(action_play_schema);

    action_next_step = new QAction(tr("&Step"), this);
    action_next_step->setStatusTip(tr("Step"));
    menu_insert->addAction(action_next_step);

    connect(action_save_schema,    &QAction::triggered, this, &MainWindow::save_schema);
    connect(action_load_schema,    &QAction::triggered, this, &MainWindow::load_schema);
    connect(action_close_schema,   &QAction::triggered, this, &MainWindow::close_schema);
}

void MainWindow::new_schema()
{
    create_schema(DEFAULT_SCHEMA_TITLE);
}

void MainWindow::create_schema(std::string nazov)
{
    schema_widget = new ui::SchemaW(nazov);

    setCentralWidget(schema_widget);
    setWindowTitle(tr((DEFAULT_WINDOW_TITLE + " - " + schema_widget->get_nazov()).c_str()));

    schema_widget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QPalette palette = schema_widget->palette();
    palette.setColor(QPalette::Background, QColor(40, 44, 52));
    schema_widget->setAutoFillBackground(true);
    schema_widget->setPalette(palette);
    schema_widget->show();

    connect(action_new_block,      &QAction::triggered, schema_widget, &ui::SchemaW::new_block);
    connect(action_new_connection, &QAction::triggered, schema_widget, &ui::SchemaW::new_connection);
    connect(action_play_all_schema,    &QAction::triggered, schema_widget, &ui::SchemaW::play_all_schema);
    connect(action_play_schema,    &QAction::triggered, schema_widget, &ui::SchemaW::play_schema);
    connect(action_next_step,      &QAction::triggered, schema_widget, &ui::SchemaW::next_step);
}

void MainWindow::close_schema()
{
    disconnect(action_new_block,      &QAction::triggered, schema_widget, &ui::SchemaW::new_block);
    disconnect(action_new_connection, &QAction::triggered, schema_widget, &ui::SchemaW::new_connection);
    disconnect(action_play_all_schema,    &QAction::triggered, schema_widget, &ui::SchemaW::play_all_schema);
    disconnect(action_play_schema,    &QAction::triggered, schema_widget, &ui::SchemaW::play_schema);
    disconnect(action_next_step,      &QAction::triggered, schema_widget, &ui::SchemaW::next_step);

    if (schema_widget != nullptr)
    {
        schema_widget->deleteLater();
    }

    default_text = new QLabel(DEFAULT_WELCOME_MSG.c_str());
    QFont  font = default_text->font();
    font.setPixelSize(22);
    default_text->setFont(font);
    default_text->setAlignment(Qt::AlignCenter);

    setCentralWidget(default_text);
}

void MainWindow::save_schema()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                       "/home/untitled.txt",
                       tr("Untitled (*.txt)"));
    std::ofstream outfile(fileName.toStdString());
    outfile << "schema " << schema_widget->get_nazov() <<std::endl;

    outfile << "blocks" << std::endl;

    for (model::Block * b : schema_widget->get_blocks())
    {
        outfile << "block "<< b->get_nazov() << std::endl;
        outfile << "inputs" << std::endl;

        // tu sa este bude ukladat velkost bloku a jeho rozmiestnenie
        // BlockW * blockW = static_cast<BlockW*>(b);
        // tu sa doplna sirka, vyska, x,y

        for (auto p : b->get_input_ports())
        {
            outfile << "port "<< p->get_nazov() << std::endl;
            outfile << "type" << std::endl;
            std::map<std::string, double *> hodnoty_in = p->get_data_type()->data;

            for (auto it_in = hodnoty_in.begin(); it_in != hodnoty_in.end(); ++it_in)
            {
                outfile << it_in->first << std::endl;
            }

            outfile << "end type" << std::endl;
            outfile << "end port" << std::endl;
        }

        outfile << "end inputs" << std::endl;
        outfile << "outputs" << std::endl;

        for (auto p : b->get_output_ports())
        {
            outfile << "port "<< p->get_nazov() << std::endl;
            outfile << "type" << std::endl;
            std::map<std::string, double *> hodnoty_out = p->get_data_type()->data;

            for (auto it = hodnoty_out.begin(); it != hodnoty_out.end(); ++it)
            {
                outfile << it->first << std::endl;
                // std::cout <<"tu som este bol" << std::endl;
            }

            outfile << "end type" << std::endl;
            outfile << "end port" << std::endl;
        }

        outfile <<"end outputs" << std::endl;

        outfile <<"vypocty" << std::endl;

        for (auto v: b->get_vypocty())
        {
            outfile << v->get_infix() << std::endl;

            outfile << "inputs_vyp" << std::endl;
            outfile <<  v->get_input_port()->get_nazov() << std::endl;

            outfile << "end inputs_vyp" << std::endl;

            outfile << "outputs_vyp" << std::endl;

            outfile << v->get_output_port()->get_nazov() << std::endl;

            outfile << "end outputs_vyp" << std::endl;

            outfile << "premenna " << v->get_premenna() << std::endl;
            outfile << "end premenna " << v->get_premenna() << std::endl;

            outfile << "end " << v->get_infix() << std::endl;
        }

        outfile << "end vypocty" << std::endl;
        outfile << "end block "<< b->get_nazov() << std::endl;
    }

    outfile << "end blocks " << std::endl << std::endl;

    outfile << "prepojenia" << std::endl;

    for (model::Prepojenie * prepoj : schema_widget->get_prepojenia())
    {
        outfile <<  prepoj->get_output_block()->get_nazov()<<" "<<
                prepoj->get_output_port()->get_nazov()<< " " << prepoj->get_input_block()->get_nazov()<<
                " "<< prepoj->get_input_port()->get_nazov() << " " << std::endl;
    }

    outfile << "end prepojenia" << std::endl;

    outfile << "end schema" << std::endl;
}

void MainWindow::load_schema()
{

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                       "/home",
                       tr("Untitled (*.txt)"));
    int schema = 0;
    int blocks = 0;
    int block = 0;
    int inputs = 0;
    int outputs = 0;
    int prepojenie = 0;
    int port = 0;
    int vypocty = 0;
    int infix = 0;
    int inputs_vyp = 0;
    int outputs_vyp = 0;
    int type = 0;
    int premenna = 0;
    int length_str = 0;
    int input_cislo_portu = 0;
    int output_cislo_portu = 0;
    int ui = 0;
    std::string infix_str;
    std::ifstream f(fileName.toStdString());
    std:: string str;
    std::string port_str;
    std::string input_vyraz;
    std:: string output_vyraz;
    model::DataType data_type;
    ui::BlockW * b = nullptr;
    ui::PortW * p = nullptr;
    ui::VyrazW * v = nullptr;
    ui::ConnectionW * prepoj = nullptr;

    if (! f.is_open())
    {
        // error neslo otvorit subor
    }

    while (std::getline(f, str))
    {
        std::string end_str = str.substr(0, 3);

        if (end_str == "end")
        {
            length_str = str.length() - 4;

            if (str.substr(4, length_str) == "type")
            {
                type = 2;
            }

            else if (str.substr(4, length_str) == "inputs")
            {
                inputs = 2;
            }
            else if (str.substr(4, length_str) == "outputs")
            {
                outputs = 2;
            }
            else if (str.substr(4, length_str) == "port")
            {
                b->add_port(p);
                port = 0;
                type = 0;
            }
            else if (str.substr(4, length_str) == "inputs_vyp")
            {
                inputs_vyp = 2;
            }
            else if (str.substr(4, length_str) == "outputs_vyp")
            {
                outputs_vyp = 2;
            }
            else if (str.substr(4, length_str) == "vypocty")
            {
                vypocty = 2;
            }
            else if (str.substr(4, 8) == "premenna")
            {
                premenna = 0;
            }
            else if (str.substr(4, length_str) == infix_str)
            {
                infix = 0;
                inputs_vyp = 0;
                outputs_vyp = 0;
            }
            else if (str.substr(4, length_str) == "prepojenia")
            {
                prepojenie = 0;
            }
            else if (str.substr(4, 6) == "blocks")
            {
                blocks = 2;
            }
            else if (str.substr(4, 5) == "block")
            {
                block = 0;
                inputs = 0;
                outputs = 0;
                inputs_vyp = 0;
                outputs_vyp = 0;
                premenna = 0;
                input_cislo_portu = 0;
                output_cislo_portu = 0;
                vypocty = 0;
                infix = 0;
                ui = 0;
            }

            // break;
        }

        else if (schema == 0)
        {
            std::string schema_str = str.substr(0, 6);

            if (schema_str != "schema")
            {
                //error lebo je to zle ulozene
            }
            else
            {
                length_str = str.length() - 7;
                create_schema(str.substr(7, length_str));
            }

            schema = 1;
        }
        else if (blocks == 0 and schema == 1 and blocks !=2)
        {
            if (str == "blocks")
            {
                blocks = 1;
            }
            else
            {
                //mam tu error lebo som cakal blocks ale dostal som daco ine
            }

        }
        else if (block == 0 and blocks !=2)
        {
            std::string block_str = str.substr(0, 5);
            // std::cout << block_str << std::endl;

            if (block_str != "block")
            {
                //error malo tam byt block
            }
            else
            {
                // std::cout << "som tu" << std::endl;
                length_str = str.length() - 6;
                b = new ui::BlockW(str.substr(6, length_str), schema_widget);
                schema_widget->add_block(b);
            }

            block = 1;
            // break;
        }
        // else if (ui == 0 and block == 1 and blocks !=2)
        // {
        //
        //     // tu bude nacitanie velkosti a pozicie bloku
        //     ui = 1;
        //     break;
        // }
        else if (inputs == 0 and blocks !=2)
        {
            // std::cout<<"inputs som tam" << std::endl;
            if (str == "inputs")
            {
                inputs = 1;
            }

            // break;
        }
        else if (inputs != 2 and port == 0 and blocks !=2)
        {
            port_str = str.substr(0, 4);

            if (port_str != "port")
            {
                // error malo tu byt port
            }
            else
            {
                length_str = str.length() - 5;
                port_str = str.substr(5, length_str);
                p = new ui::PortW(b->get_nazov(), input_cislo_portu, model::PortType::input, schema_widget);
                // std::cout << "port nacitavanie" << std::endl;
                input_cislo_portu +=1;
                //vyrobim port a este ho nebudem pridavat bloku lebo nepoznam jeho type-hotovo
            }

            port = 1;
            // break;
        }
        else if (inputs == 1 and port == 1 and type == 0 and blocks !=2)
        {
            if (str != "type")
            {
                // mam tu erro
            }
            else
            {
                type = 1;
                // break;
            }
        }
        else if (type == 1)
        {
            // std::cout << "type nacitavanie" << std::endl;
            length_str = str.length() - 1;
            data_type.add(str.substr(0, 1));
            p->set_data_type(str.substr(0, 1));
            // b->add_port(p);
            // budem tu pridavat do mapy typi ktore budem ziskavat
            // tu neviem ako rozdelim to ze jedno je hodnota a jedno nazov a neviem akej to je dlzka
            // musim pridaj port do bloku -hotovo
            // break;
        }
        else if (outputs == 0 and blocks !=2)
        {
            if (str == "outputs")
            {
                outputs = 1;
            }

            // break;
        }
        else if (outputs != 2 and port == 0 and blocks !=2)
        {
            port_str = str.substr(0, 4);

            if (port_str != "port")
            {
                // error malo tu byt port
            }
            else
            {
                // std::cout << "nacitavam output" << std::endl;
                length_str = str.length() - 5;
                port_str = str.substr(5, length_str);
                p = new ui::PortW(b->get_nazov(), output_cislo_portu, model::PortType::output, schema_widget);
                output_cislo_portu +=1;
                //vyrobim port a este ho nebudem pridavat bloku lebo nepoznam jeho type -hotovo
            }

            port = 1;
            // break;
        }
        else if (outputs == 1 and port == 1 and type == 0 and blocks !=2)
        {
            if (str != "type")
            {
                // mam tu erro
            }
            else
            {
                type = 1;
                // vytvorim si tu mapu na ulozenie potom typov
                // break;
            }
        }
        else if (type == 1 and blocks !=2)
        {
            length_str = str.length() - 1;
            data_type.add(str.substr(0, 1));
            p->set_data_type(str.substr(0, 1));
            // b->add_port(p);
            // budem tu pridavat do mapy typi ktore budem ziskavat
            // tu neviem ako rozdelim to ze jedno je hodnota a jedno nazov a neviem akej to je dlzky
            // musim pridaj port do bloku - hotovo
            // break;
        }
        else if (vypocty == 0 and blocks !=2)
        {
            if (str == "vypocty")
            {
                vypocty = 1;
            }
            else
            {
                // error
            }

            // break;
        }
        else if (vypocty == 1 and infix == 0 and blocks !=2)
        {
            // nastavim vyrazu infix
            infix_str = str;
            infix = 1;
        }
        else if (infix == 1 and inputs_vyp == 0 and blocks !=2)
        {
            if (str == "inputs_vyp")
            {
                inputs_vyp = 1;
            }
            else
            {
                // dalsia chyba
            }

            // break;
        }
        else if (inputs_vyp == 1 and blocks !=2)
        {
            input_vyraz = str;
        }
        else if (infix == 1 and inputs_vyp == 2 and outputs_vyp == 0 and blocks !=2)
        {
            if (str == "outputs_vyp")
            {
                outputs_vyp = 1;
            }
            else
            {
                // dalsia chyba
            }

            // break;
        }
        else if (infix == 1 and outputs_vyp == 1 and blocks !=2)
        {
            output_vyraz = str;
            // break;
        }
        else if (infix == 1 and premenna == 0 and blocks !=2)
        {
            std::string premenna_str = str.substr(0, 8);

            if (premenna_str != "premenna")
            {
                // error mala tu byt premenna
            }
            else
            {
                length_str = str.length() - 9;
                premenna_str = str.substr(9, length_str);
                v = new ui::VyrazW(infix_str, premenna_str, b);
                v->set_input_port(b->get_port(input_vyraz));
                v->set_output_port(b->get_port(output_vyraz));
                b->add_expression(v);
                // dostal som nazov premennej kde sa ma ulozit vysledok
            }

            premenna = 1;
            // break;
        }
        else if (blocks == 2 and prepojenie == 0)
        {
            if (str != "prepojenia")
            {
                // error malo tam byt prepojenie
            }
            else
            {
                prepojenie = 1;
            }

            // break;
        }
        else if (prepojenie == 1)
        {
            // nacitam informacie pre prepojenie
            size_t pos = 0;
            // std::cout<<str<< std::endl;
            std::string token;
            std::string prepojenie_str[4];
            std::string delimiter = " ";
            int i = 0;

            while ((pos = str.find(delimiter)) != std::string::npos)
            {
                token = str.substr(0, pos);
                prepojenie_str[i] = token;
                str.erase(0, pos + delimiter.length());
                i++;
            }


            ui::BlockW * out_block = static_cast<ui::BlockW *>(schema_widget->get_block(prepojenie_str[0]));
            ui::PortW * out_port = static_cast<ui::PortW *>(schema_widget->get_block(prepojenie_str[0])->get_port(prepojenie_str[1]));
            ui::BlockW * in_block = static_cast<ui::BlockW *>(schema_widget->get_block(prepojenie_str[2]));
            ui::PortW * in_port = static_cast<ui::PortW *>(schema_widget->get_block(prepojenie_str[2])->get_port(prepojenie_str[3]));

            prepoj = new ui::ConnectionW(out_block, out_port, in_block, in_port);
            schema_widget->add_prepoj(prepoj);
            // break;
        }

        // else
        // {
        //     // chyba nieco ine tam bolo
        // }

        schema_widget->repaint();
    }
}


}