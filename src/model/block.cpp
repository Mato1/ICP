/**
 * @brief 
 * 
 * @file block.cpp
 * @author Libor Moravcik, Matej Mrazik
 * @date 2018-05-06
 */
#include "model/block.h"
#include <chrono>
#include <thread>
#include "ui/block_w.h"

namespace icp
{
namespace model
{
Block::Block(std::string nazov)
    : nazov(nazov)
{

}

Block::~Block()
{
    // for(std::vector<Vyraz*>::iterator it = vypocty.begin(); it != vypocty.end(); ++it)
    // {
    //     delete (*it);
    // }

    // vypocty.clear();

    // for(std::vector<Port*>::iterator it = input_ports.begin(); it != input_ports.end(); ++it)
    // {
    //     delete (*it);
    // }

    // input_ports.clear();

    // for(std::vector<Port*>::iterator it = output_ports.begin(); it != output_ports.end(); ++it)
    // {
    //     delete (*it);
    // }

    // output_ports.clear();

}

void Block::add_vyraz(Vyraz * v)
{
    if (v != nullptr)
    {
        vypocty.push_back(v);
    }
}

void Block::add_port(Port * p)
{
    if (p != nullptr)
    {
        if (p->get_port_type() == PortType::input)
        {
            input_ports.push_back(p);
        }
        else if (p->get_port_type() == PortType::output)
        {
            output_ports.push_back(p);
        }
    }
}

void Block::remove_port(std::string nazov)
{
    Port * p = port_exists(nazov);
    remove_port(p);
}

void Block::remove_port(Port * p)
{
    if (p != nullptr)
    {
        if (p->get_port_type() == PortType::input)
        {
            input_ports.erase(std::remove(input_ports.begin(), input_ports.end(), p),
                              input_ports.end());
        }
        else if (p->get_port_type() == PortType::output)
        {
            output_ports.erase(std::remove(output_ports.begin(), output_ports.end(), p),
                               output_ports.end());
        }

        // delete p;
    }
}

Port * Block::get_port(std::string nazov)
{
    return port_exists(nazov);
}

Port * Block::port_exists(std::string nazov)
{
    if (nazov.find(INPUT_PORT_PREFIX) != std::string::npos)
    {
        for (Port * p : input_ports)
        {
            if (p->get_nazov().compare(nazov) == 0)
            {
                return p;
            }
        }
    }
    else if (nazov.find(OUTPUT_PORT_PREFIX) != std::string::npos)
    {
        for (Port * p : output_ports)
        {
            if (p->get_nazov().compare(nazov) == 0)
            {
                return p;
            }
        }
    }

    return nullptr;
}

void Block::eval()
{
    while (!all_input_ports_active())
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }

    for (auto v : vypocty)
    {
        v->eval();
    }

    ui::BlockW * b = ( ui::BlockW *) this;
    b->repaint();
    
    this->evaluated = true;
}

bool Block::all_input_ports_active()
{
    for (auto var : input_ports)
    {
        if (!var->is_active())
        {
            return false;
        }
    }

    return true;
}

bool Block::all_input_ports_filled()
{
    for (auto var : input_ports)
    {
        if (!var->is_active())
        {
            return false;
        }
    }

    return true;
}

}
}