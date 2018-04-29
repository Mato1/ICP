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
               outfile << "\t" "\t" "block "<< b->get_nazov() << std::endl;
               outfile << "\t" "\t" "\t" "inputs" << std::endl;
               for (Port *p : b->get_input_ports())
               {
                    outfile << "\t" "\t" "\t" "\t" "port "<< p->get_nazov() << std::endl;
                    outfile << "\t" "\t" "\t" "\t" "\t" "type" << std::endl;
                    std::map<std::string, double*> hodnoty = p->get_data_type().data;

                    for (auto it = hodnoty.begin(); it != hodnoty.end(); ++it)
                    {
                        std::cout << "\t" "\t" "\t" "\t" "\t" "\t" << it->first << " " << it->second << '\n';
                    }

                    outfile << "\t" "\t" "\t" "\t" "\t" "end type" << std::endl;
                    outfile << "\t" "\t" "\t" "\t""end port" << std::endl;
               }
               outfile << "\t" "\t" "\t" "end inputs" << std::endl;
            
                for (Port *p : b->get_output_ports())
                {
                    outfile << "\t" "\t" "\t" "\t" "port "<< p->get_nazov() << std::endl;
                    outfile << "\t" "\t" "\t" "\t" "\t" "type" << std::endl;
                    std::map<std::string, double*> hodnoty = p->get_data_type().data;

                    for (auto it = hodnoty.begin(); it != hodnoty.end(); ++it)
                    {
                        std::cout << "\t" "\t" "\t" "\t" "\t" "\t" << it->first << " " << it->second << '\n';
                    }

                    outfile << "\t" "\t" "\t" "\t" "\t" "end type" << std::endl;
                    outfile << "\t" "\t" "\t" "\t" "end port" << std::endl;
                }
                outfile << "\t" "\t" "\t" "end outputs" << std::endl;
                
                outfile << "\t" "\t" "\t" "\t" "vypocty" << std::endl;
                for (auto v: b->get_vypocty())
                {
                    outfile << "\t" "\t" "\t" "\t" "\t" << v->get_infix() << std::endl;

                    outfile << "\t" "\t" "\t" "\t" "\t" "\t" "inputs" << std::endl;
                    for (Port *vp : v->get_input_ports())
                    {
                        outfile << "\t" "\t" "\t" "\t" "\t" "\t" "\t" << vp->get_nazov() << std::endl;
                    }
                    outfile << "\t" "\t" "\t" "\t" "\t" "\t" "end inputs" << std::endl;

                    outfile << "\t" "\t" "\t" "\t" "\t" "\t" "outputs" << std::endl;
                    for (Port *vp : v->get_output_ports())
                    {
                        outfile << "\t" "\t" "\t" "\t" "\t" "\t" "\t" << vp->get_nazov() << std::endl;
                    }
                    outfile << "\t" "\t" "\t" "\t" "\t" "\t" "end inputs" << std::endl;

                    outfile << "\t" "\t" "\t" "\t" "\t" "\t" "premenna " << v->get_premenna() << std::endl;
                    outfile << "\t" "\t" "\t" "\t" "\t" "\t" "end premenna " << v->get_premenna() << std::endl;

                    outfile << "\t" "\t" "\t" "\t" "\t" "end " << v->get_infix() << std::endl;
                }

                outfile << "\t" "\t" "\t" "\t" "end vypocty" << std::endl;
                outfile << "\t" "\t" "block "<< b->get_nazov() << std::endl;
            }
            outfile << "\t" "\t" "end blocks " << std::endl << std::endl;

            outfile << "\t" "\t" "prepojenia" << std::endl;
            for(Prepojenie * prepoj : prepojenia)
            {
                outfile << "\t" "\t" "\t" << prepoj->get_output_block()<<" "<< prepoj->get_output_port()<< " " << prepoj->get_input_block()<< " "<< prepoj->get_input_port() << std::endl; 
            }
            outfile << "\t" "\t" "end prepojenia" << std::endl;

            outfile << "end schema" << std::endl;
        }

    }
}