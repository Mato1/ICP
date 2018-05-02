/**
 * @brief Obsahuje definicie pre triedu Schema.
 * 
 * @file schema.cpp
 * @author Libor Moravcik, Matej Mrazik
 * @date 2018-04-28
 */
#include "model/schema.h"
#include "model/port.h"
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
            for(auto it = blocks.begin(); it != blocks.end(); ++it)
            {
                delete (*it);
            }

            blocks.clear();
        }

        void Schema::add_block(Block * block)
        {
            if (!block_exists(block->get_nazov()))
            {
                blocks.push_back(block);
            }
        }

        Block * Schema::add_block(std::string nazov)
        {
            Block * b = nullptr;

            if (!block_exists(nazov))
            {
                b = new Block(nazov);
                blocks.push_back(b);
            }

            return b;
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
                delete block;
            }
        }

        Prepojenie * Schema::add_prepoj(Block * bout, Port * const pout, Block * bin, Port * const pin)
        {
            if (prepoj_exists_on_ports(pout, pin))
            {
                return nullptr;
            }

            Prepojenie * p = new Prepojenie(bout, pout, bin, pin);
            prepojenia.push_back(p);
            
            return p;
        }

        void  Schema::remove_prepoj(Prepojenie * p)
        {
            auto it = std::find(prepojenia.begin(), prepojenia.end(), p);
            if (it != prepojenia.end())
            {
                delete (*it);
                prepojenia.erase(it);
            }
        }

        void Schema::eval()
        {
            for (auto p : prepojenia)
            {
                fut_vec_prepoj.push_back(std::async(std::launch::async, &Schema::prepojenie_transfer, this, p) );
            }

            for(auto block : blocks)
            {
                fut_vec_eval.push_back(std::async(std::launch::async, &Schema::eval_block, this, block) );
            }
        }

        void Schema::eval_block(Block * block)
        { 
            while (!block->all_input_ports_active())
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(200));
            }
            
            block->eval();
        }
        

        bool Schema::detect_cycles()
        {
            std::vector<Block *> root_blocks = get_root_blocks();
            std::vector<Port *> visited_input_ports;
            
            for(auto block : root_blocks)
            {
               fut_vec_cycles.push_back( std::async(std::launch::async, &Schema::detect_cycle, this, block, visited_input_ports) );
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
            for (auto b : blocks)
            {
                if (b->get_nazov().compare(nazov) == 0)
                {
                    return b;
                }
            }

            return nullptr;
        }

        std::vector<Block *> Schema::get_root_blocks()
        {
            std::vector<Block *> root_blocks;
            
            for(auto b : blocks)
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
            for(auto p : block->get_input_ports())
            {
                if ( (std::find(visited_input_ports.begin(), visited_input_ports.end(), p)) == visited_input_ports.end() )
                {
                    visited_input_ports.push_back(p);
                    for(auto b : block->get_next_blocks())
                    {
                        detect_cycle(b, visited_input_ports);
                    }
                } else
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
            for(Prepojenie * p : prepojenia)
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

            outfile << "\t" "blocks" << std::endl;

            for(Block * b : blocks)
            {
               outfile << "\t\t" "block "<< b->get_nazov() << std::endl;
               outfile << "\t\t\t" "inputs" << std::endl;
               // tu sa este bude ukladat velkost bloku a jeho rozmiestnenie
               for (Port *p : b->get_input_ports())
               {
                    outfile << "\t\t\t\t" "port "<< p->get_nazov() << std::endl;
                    outfile << "\t\t\t\t\t" "type" << std::endl;
                    std::map<std::string, double*> hodnoty = p->get_data_type().data;

                    for (auto it = hodnoty.begin(); it != hodnoty.end(); ++it)
                    {
                        std::cout << "\t\t\t\t\t\t" << it->first << " " << it->second << '\n';
                    }

                    outfile << "\t\t\t\t\t" "end type" << std::endl;
                    outfile << "\t\t\t\t" "end port" << std::endl;
               }
               outfile << "\t\t\t" "end inputs" << std::endl;
               outfile << "\t\t\t" "outputs" << std::endl;
            
                for (Port *p : b->get_output_ports())
                {
                    outfile << "\t\t\t\t" "port "<< p->get_nazov() << std::endl;
                    outfile << "\t\t\t\t\t" "type" << std::endl;
                    std::map<std::string, double*> hodnoty = p->get_data_type().data;

                    for (auto it = hodnoty.begin(); it != hodnoty.end(); ++it)
                    {
                        std::cout << "\t\t\t\t\t\t" << it->first << std::endl;
                    }

                    outfile << "\t\t\t\t\t" "end type" << std::endl;
                    outfile << "\t\t\t\t" "end port" << std::endl;
                }
                outfile << "\t\t\t" "end outputs" << std::endl;
                
                outfile << "\t\t\t" "vypocty" << std::endl;
                for (auto v: b->get_vypocty())
                {
                    outfile << "\t\t\t\t\t" << v->get_infix() << std::endl;

                    outfile << "\t\t\t\t\t\t" "inputs_vyp" << std::endl;
                    for (Port *vp : v->get_input_ports())
                    {
                        outfile << "\t\t\t\t\t\t\t" << vp->get_nazov() << std::endl;
                    }
                    outfile << "\t\t\t\t\t\t" "end inputs_vyp" << std::endl;

                    outfile << "\t\t\t\t\t\t" "outputs_vyp" << std::endl;
                    for (Port *vp : v->get_output_ports())
                    {
                        outfile << "\t\t\t\t\t\t\t" << vp->get_nazov() << std::endl;
                    }
                    outfile << "\t\t\t\t\t\t" "end outputs_vyp" << std::endl;

                    outfile << "\t\t\t\t\t\t" "premenna " << v->get_premenna() << std::endl;
                    outfile << "\t\t\t\t\t\t" "end premenna " << v->get_premenna() << std::endl;

                    outfile << "\t\t\t\t\t" "end " << v->get_infix() << std::endl;
                }

                outfile << "\t\t\t" "end vypocty" << std::endl;
                outfile << "\t\t" "end block "<< b->get_nazov() << std::endl;
            }
            outfile << "\t" "end blocks " << std::endl << std::endl;

            outfile << "\t\t" "prepojenia" << std::endl;
            for(Prepojenie * prepoj : prepojenia)
            {
                outfile << "\t\t\t" << prepoj->get_output_block()->get_nazov()<<" "<< prepoj->get_output_port()->get_nazov()<< " " << prepoj->get_input_block()->get_nazov()<< " "<< prepoj->get_input_port()->get_nazov() << std::endl; 
            }
            outfile << "\t\t" "end prepojenia" << std::endl;

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
            int ui  = 0;
            int premenna = 0;
            int length_str = 0;
            int input_cislo_portu = 1;
            int output_cislo_portu = 1;
            std::ifstream f(file);
            std:: string str;
            std::string port_str;
            DataType data_type;
            if (! f.is_open())
            {
                // error neslo otvorit subor
            }
            while (std::getline(f, str))
            {
                std::string end_str = str.substr(0,3);
                if (end_str == "end")
                {
                    length_str = str.length() - 4;
                    if(str.substr(4,length_str) == "type")
                    {
                        type = 0;
                        data_type.clear();
                        if (inputs == 1)
                        {

                            // tu pridam blok do inputs port aj s jeho typom
                        }
                        else
                        {
                            // je to outputs port takze ho pridam do outputs port
                        }
                        break;
                    }
                    else if (str.substr(4,length_str) == "inputs")
                    {
                        inputs = 2;
                        break;
                    }
                    else if (str.substr(4,length_str) == "outputs")
                    {
                        outputs = 2;
                        break;
                    }
                    else if (str.substr(4,length_str) == "port")
                    {
                        port = 0;
                        break;
                    }
                    else if (str.substr(4,length_str) == "inputs_vyp")
                    {
                        inputs_vyp = 2;
                        break;
                    }
                    else if (str.substr(4,length_str) == "outputs_vyp")
                    {
                        outputs_vyp = 2;
                        break;
                    }
                    else if (str.substr(4,length_str) == "premenna")
                    {
                        premenna = 0;
                        break;
                    }
                    else if (str.substr(4,length_str) == "prepojenia")
                    {
                        prepojenie = 0;
                        break;
                    }
                    else if (str.substr(4,length_str) == "block")
                    {
                        block = 0;
                        inputs = 0;
                        outputs = 0;
                        inputs_vyp = 0;
                        outputs_vyp = 0;
                        break;
                    }
                    else if (str.substr(4,length_str) == "blocks")
                    {
                        blocks =2;
                        break;
                    }
                }
                if (schema == 0)
                {
                    std::string schema_str = str.substr(0,6);
                    if (schema_str != "schema")
                    {
                        //error lebo je to zle ulozene
                    }
                    else
                    {
                        length_str = str.length() - 7;
                        Schema(str.substr(7,length_str));
                        // vytvorim a nastavim meno schema cez str.substr(7,length_str)
                    }
                    schema = 1;
                    break;
                }
                else if (blocks == 0)
                {
                    if(str == "blocks")
                    {
                        blocks = 1;
                    }
                    else
                    {
                        //mam tu error lebo som cakal blocks ale dostal som daco ine
                    }
                    break;
                }
                else if (block == 0)
                {
                    std::string block_str = str.substr(0,5);
                    if (block_str != "block")
                    {
                        //error malo tam byt block
                    }
                    else
                    {
                        length_str = str.length() - 6;
                        Block * b = new Block(str.substr(6, length_str));
                        add_block(b);
                        // vytvorim a nastavim meno bloku  pomocou str.substr(6,length_str)
                    }
                    block = 1;
                    break;
                }
                else if (ui == 0)
                {
                    // tu bude nacitanie velkosti a pozicie bloku
                    ui = 1;
                    break;
                }
                else if (inputs == 0)
                {
                    if (str == "inputs")
                    {
                        inputs = 1;
                    }
                    break;
                }
                else if (inputs != 2 and port == 0)
                {
                    port_str = str.substr(0,4);
                    if (port_str != "port")
                    {
                        // error malo tu byt port
                    }
                    else
                    {
                        input_cislo_portu +=1;
                        length_str = str.length() - 5;
                        port_str = str.substr(5,length_str);
                        //vyrobim port a este ho nebudem pridavat bloku lebo nepoznam jeho type
                    }
                    port = 1;
                    break;
                }
                else if (inputs == 1 and port == 1 and type == 0)
                {
                    if (str != "type")
                    {
                        // mam tu erro
                    }
                    else
                    {
                        type = 1;
                        // vytvorim si tu mapu na ulozenie potom typov
                        break;
                    }
                }
                else if(type == 1)
                {
                    // budem tu pridavat do mapy typi ktore budem ziskavat
                    // tu neviem ako rozdelim to ze jedno je hodnota a jedno nazov a neviem akej to je dlzky
                    // Port * Block::add_port(port_str, "INPUT");
                    // musim pridaj port do bloku 
                    break;
                }
                else if (outputs == 0)
                {
                    if (str == "inputs")
                    {
                        outputs = 1;
                    }
                    break;
                }
                else if (outputs != 2 and port == 0)
                {
                    port_str = str.substr(0,4);
                    if (port_str != "port")
                    {
                        // error malo tu byt port
                    }
                    else
                    {
                        output_cislo_portu +=1;
                        length_str = str.length() - 5;
                        port_str = str.substr(5,length_str);
                        //vyrobim port a este ho nebudem pridavat bloku lebo nepoznam jeho type
                    }
                    port = 1;
                    break;
                }
                else if (outputs == 1 and port == 1 and type == 0)
                {
                    if (str != "type")
                    {
                        // mam tu erro
                    }
                    else
                    {
                        type = 1;
                        // vytvorim si tu mapu na ulozenie potom typov
                        break;
                    }
                }
                else if(type == 1)
                {
                    // budem tu pridavat do mapy typi ktore budem ziskavat
                    // tu neviem ako rozdelim to ze jedno je hodnota a jedno nazov a neviem akej to je dlzky
                    // Port * Block::add_port(port_str, "INPUT");
                    // musim pridaj port do bloku 
                    break;
                }
                else if (vypocty == 0)
                {
                    if (str == "vypocty")
                    {
                        vypocty = 1;
                    }
                    else
                    {
                        // error
                    }
                    break;
                }
                else if (vypocty == 1 and infix == 0)
                {
                    // nastavim vyrazu infix
                    infix = 1;
                    break;
                }
                else if (infix == 1 and inputs_vyp == 0)
                {
                    if(str == "inputs_vyp")
                    {
                        inputs_vyp = 1;
                    }
                    else 
                    {
                        // dalsia chyba
                    }
                    break;
                }
                else if (inputs_vyp == 1)
                {
                    // budem tu nacitavav nazov vstupnych portov pre vypocet
                    break;
                }
                else if (infix == 1 and inputs_vyp == 2 and outputs_vyp == 0)
                {
                    if(str == "outputs_vyp")
                    {
                        outputs_vyp = 1;
                    }
                    else 
                    {
                        // dalsia chyba
                    }
                    break;
                }
                else if (infix == 1 and outputs_vyp == 1)
                {
                    // budem tu nacitavav nazov vstupnych portov pre vypocet
                    break;
                }
                else if (infix == 1 and premenna == 0)
                {
                    std::string premenna_str = str.substr(0,8);
                    if (premenna_str != "premenna")
                    {
                        // error mala tu byt premenna
                    }
                    else
                    {
                        length_str = str.length() - 9;
                        premenna_str = str.substr(9,length_str);
                        // dostal som nazov premennej kde sa ma ulozit vysledok
                    }
                    premenna = 1;
                    break;
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
                    break;
                }
                else if (prepojenie == 1)
                {
                    // nacitam informacie pre prepojenie
                    break;
                }
                else
                {
                    // chyba nieco ine tam bolor
                }
            }
        }
    }
}