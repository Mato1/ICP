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

}
}