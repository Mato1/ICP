#include "model/prepojenie.h"

namespace icp
{
namespace model
{

Prepojenie::Prepojenie(Block * const output_block, Port * const output_port,
                       Block * const input_block,  Port * const input_port)
    : output_block(output_block), output_port(output_port),
      input_block(input_block), input_port(input_port)
{
    output_block->add_next_block(input_block);
    input_block->add_previous_block(output_block);
    output_port->set_connected(true);
    input_port->set_connected(true);
}

Prepojenie::~Prepojenie()
{
    output_block->remove_next_block(input_block);
    input_block->remove_previous_block(output_block);
    output_port->set_connected(false);
    input_port->set_connected(false);
}

void Prepojenie::transfer() const
{
    input_port->get_data_type().set(output_port->get_data_type());
}

std::string Prepojenie::to_string()
{
    std::string s =  "Out: " + output_port->get_nazov() + "\n";
    s += "In: " + input_port->get_nazov();

    return s;
}

}
}
