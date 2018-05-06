/**
 * @brief Obsahuje definicie pre triedu Schema.
 *
 * @file schema.cpp
 * @author Libor Moravcik, Matej Mrazik
 * @date 2018-04-28
 */
#include "model/schema.h"
#include "model/port.h"
#include "model/block.h"
#include "model/prepojenie.h"
#include <iostream>
#include <fstream>

namespace icp
{
namespace model
{
Schema::Schema(std::string nazov)
    : nazov(nazov)
{

}

Schema::~Schema()
{
    // for(auto it = blocks.begin(); it != blocks.end(); ++it)
    // {
    //     delete (*it);
    // }

    // blocks.clear();
}

void Schema::add_block(Block * block)
{
    if (!block_exists(block->get_nazov()))
    {
        blocks.push_back(block);
    }
}

Block * Schema::get_block(std::string nazov)
{
    return block_exists(nazov);
}

void Schema::remove_block(std::string nazov)
{
    remove_block(block_exists(nazov));
}

void Schema::remove_block(Block * block)
{
    if (block != nullptr)
    {
        blocks.erase(std::remove(blocks.begin(), blocks.end(), block), blocks.end());
        // delete block;
    }
}

void Schema::add_prepoj(Prepojenie * prepoj)
{
    if (prepoj != nullptr)
    {
        prepojenia.push_back(prepoj);
    }
}

void  Schema::remove_prepoj(Prepojenie * p)
{
    if (p != nullptr)
    {
        prepojenia.erase(std::remove(prepojenia.begin(), prepojenia.end(), p), prepojenia.end());
    }
}

void Schema::eval()
{
    for (auto p : prepojenia)
    {
        fut_vec_prepoj.push_back(std::async(std::launch::async, &Schema::prepojenie_transfer, this, p) );
    }

    for (auto block : blocks)
    {
        fut_vec_eval.push_back(std::async(std::launch::async, &Block::eval, block) );
    }
}


bool Schema::detect_cycles()
{
    std::vector<Block *> root_blocks = get_root_blocks();
    std::vector<Port *> visited_input_ports;

    for (auto block : root_blocks)
    {
        fut_vec_cycles.push_back( std::async(std::launch::async, &Schema::detect_cycle, this,
                                             block, visited_input_ports) );
    }

    bool result = false;

    for (size_t i = 0; i < fut_vec_cycles.size(); ++i)
    {
        result += fut_vec_cycles[i].get();
    }

    return result;
}

Block * Schema::block_exists(std::string nazov)
{
    for (auto it = blocks.begin(); it != blocks.end(); it++)
    {
        if ((*it)->get_nazov().compare(nazov) == 0)
        {
            return (*it);
        }
    }

    return nullptr;
}

std::vector<Block *> Schema::get_root_blocks()
{
    std::vector<Block *> root_blocks;

    for (auto b : blocks)
    {
        if (b->is_root())
        {
            root_blocks.push_back(b);
        }
    }

    return root_blocks;
}

bool Schema::detect_cycle(Block * block, std::vector<Port *> visited_input_ports)
{
    for (auto p : block->get_input_ports())
    {
        if ( (std::find(visited_input_ports.begin(), visited_input_ports.end(),
                        p)) == visited_input_ports.end() )
        {
            visited_input_ports.push_back(p);

            for (auto b : block->get_next_blocks())
            {
                detect_cycle(b, visited_input_ports);
            }
        }
        else
        {
            return true;
        }
    }

    return false;
}


void Schema::prepojenie_transfer(Prepojenie * p)
{
    while (!p->get_output_port()->is_active())
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }

    p->transfer();
}

bool Schema::prepoj_exists_on_ports(Port * const pout, Port * const pin)
{
    for (Prepojenie * p : prepojenia)
    {
        if (p->get_output_port() == pout || p->get_input_port() == pin)
        {
            return true;
        }
    }

    return false;
}


void Schema::save(std::string file)
{
    std::ofstream outfile(file);

    outfile << "schema " << get_nazov() <<std::endl;

    outfile << "blocks" << std::endl;

    for (Block * b : blocks)
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
            std::map<std::string, double *> hodnoty_in = p->get_data_type().data;

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
            std::map<std::string, double *> hodnoty_out = p->get_data_type().data;

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

    for (Prepojenie * prepoj : prepojenia)
    {
        outfile <<  prepoj->get_output_block()->get_nazov()<<" "<<
                prepoj->get_output_port()->get_nazov()<< " " << prepoj->get_input_block()->get_nazov()<<
                " "<< prepoj->get_input_port()->get_nazov() << " " << std::endl;
    }

    outfile << "end prepojenia" << std::endl;

    outfile << "end schema" << std::endl;
}

void Schema::load(std::string file)
{
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
    std::ifstream f(file);
    std:: string str;
    std::string port_str;
    std::string input_vyraz;
    std:: string output_vyraz;
    DataType data_type;
    Block * b = nullptr;
    Port * p = nullptr;
    Vyraz * v = nullptr;  
    Prepojenie * prepoj = nullptr;

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

            // if (str.substr(4, length_str) == "type")
            // {
            //     type = 0;
            //     data_type.clear();

            //     if (inputs == 1)
            //     {

            //         // tu pridam blok do inputs port aj s jeho typom
            //     }
            //     else
            //     {
            //         // je to outputs port takze ho pridam do outputs port
            //     }

            //     break;
            // }
            if (str.substr(4, length_str) == "type")
            {
                type = 2;
            }

            else if (str.substr(4, length_str) == "inputs")
            {
                // std::cout << "end inputs" << std::endl;
                inputs = 2;
                // break;
            }
            else if (str.substr(4, length_str) == "outputs")
            {
                // std::cout << "end outputs" << std::endl;
                outputs = 2;
                // break;
            }
            else if (str.substr(4, length_str) == "port")
            {
                b->add_port(p);
                port = 0;
                // std::cout << "end port" << std::endl;
                type = 0;
                // break;
            }
            else if (str.substr(4, length_str) == "inputs_vyp")
            {
                inputs_vyp = 2;
                // break;
            }
            else if (str.substr(4, length_str) == "outputs_vyp")
            {
                outputs_vyp = 2;
                // break;
            }
            else if (str.substr(4, length_str) == "premenna")
            {
                premenna = 0;
                // break;
            }
            else if (str.substr(4, length_str) == "prepojenia")
            {
                prepojenie = 0;
                // break;
            }
            else if (str.substr(4, 6) == "blocks")
            {
                // std::cout << "end blocks" << std::endl;
                blocks = 2;
                // break;
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
                // break;
                // std::cout<<"end block" <<std::endl;
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
                // std::cout << "som tu schema" << std::endl;
                length_str = str.length() - 7;
                // std::cout << str.substr(7, length_str) << std::endl;
                Schema(str.substr(7, length_str));
            }

            schema = 1;
            // break;
        }
        else if (blocks == 0 and schema == 1 and blocks !=2)
        {
            // std::cout << "som tu blocks" << std::endl;
            // std::cout << str << std::endl;
            if (str == "blocks")
            {
                // std::cout << "aj tu som bol" << std::endl;
                blocks = 1;
            }
            else
            {
                //mam tu error lebo som cakal blocks ale dostal som daco ine
            }

            // break;
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
                b = new Block(str.substr(6, length_str));
                add_block(b);
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
                p = new Port(b->get_nazov(), input_cislo_portu, PortType::input);
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
                p = new Port(b->get_nazov(), output_cislo_portu,PortType::output);
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
            // std::cout << "infix" << std::endl;
            // nastavim vyrazu infix
            infix_str = str;
            infix = 1;
            // break;
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
            std::cout << "input vyraz" << std::endl;
            input_vyraz = str;

            // budem tu nacitavav nazov vstupnych portov pre vyraz
            // break;
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
            // budem tu nacitavav nazov vstupnych portov pre vyraz
            // std::cout << "output vyraz" << std::endl;
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
                v = new Vyraz(infix_str, premenna_str);
                b->add_vyraz(v);
                v->set_input_port(b->get_port(input_vyraz));
                v->set_output_port(b->get_port(output_vyraz));
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
            prepoj = new Prepojenie(get_block(prepojenie_str[0]),get_block(prepojenie_str[0])->get_port(prepojenie_str[1]), get_block(prepojenie_str[2]),get_block(prepojenie_str[2])->get_port(prepojenie_str[3]));
            add_prepoj(prepoj);
            // break;
        }
        // else
        // {
        //     // chyba nieco ine tam bolo
        // }
    }
}

}
}