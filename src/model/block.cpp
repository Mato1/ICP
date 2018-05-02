#include "model/block.h"

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
            for(std::vector<Vyraz*>::iterator it = vypocty.begin(); it != vypocty.end(); ++it)
            {
                delete (*it);
            }

            vypocty.clear();

            for(std::vector<Port*>::iterator it = input_ports.begin(); it != input_ports.end(); ++it)
            {
                delete (*it);
            }

            input_ports.clear();
            
            for(std::vector<Port*>::iterator it = output_ports.begin(); it != output_ports.end(); ++it)
            {
                delete (*it);
            }

            output_ports.clear();
            
        }

        void Block::add_vypocet(Vyraz * v)
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

        Vyraz * Block::add_vypocet(std::string vyraz, std::string premenna)
        {
            Vyraz * vypocet = new Vyraz(vyraz);
            vypocet->set_premenna(premenna);
            vypocty.push_back(vypocet);
            

            return vypocet;
        }

        Port * Block::add_port(DataType type, PortType port_type)
        {   
            Port * port = nullptr;
            if (port_type == PortType::input)
            {
                port = new Port(nazov, input_ports.size(), port_type);
                port->set_data_type(type);
                input_ports.push_back(port);       
            }
            else if (port_type == PortType::output)
            {
                port = new Port(nazov, output_ports.size(), port_type);
                port->set_data_type(type);
                input_ports.push_back(port);       
            }
            return port;
        }

        void Block::remove_port(std::string nazov)
        {
            Port * p = port_exists(nazov);
            remove_port(p);
        }

        void Block::remove_port(Port *p)
        {
             if (p != nullptr)
            {
                if (p->get_port_type() == PortType::input)
                {
                    input_ports.erase(std::remove(input_ports.begin(), input_ports.end(), p), input_ports.end());
                }
                else if (p->get_port_type() == PortType::output)
                {
                    output_ports.erase(std::remove(output_ports.begin(), output_ports.end(), p), output_ports.end());
                }

                delete p;
            }
        }

        Port * Block::port_exists(std::string nazov)
        {
            if (nazov.find(INPUT_PORT_PREFIX) != std::string::npos)
            {
                for(Port * p : input_ports)
                {
                    if (p->get_nazov().compare(nazov) == 0)
                    {
                        return p;
                    }
                }
            }
            else if (nazov.find(OUTPUT_PORT_PREFIX) != std::string::npos)
            {
                 for(Port * p : output_ports)
                {
                    if (p->get_nazov().compare(nazov) == 0)
                    {
                        return p;
                    }
                }
            }

            return nullptr;
        }
    
        void Block::add_port_to_vypocet(Port * port, Vyraz * v)
        {
            if (port->get_port_type() == PortType::input)
            {
                v->assign_input_port(port);
            }
            else if (port->get_port_type() == PortType::output)
            {
                v->assign_output_port(port);
            }
        }

        void Block::eval()
        {
            for(auto v : vypocty)
            {
                v->eval();
            }
        }
    }
}