/**
 * @brief 
 * 
 * @file port.cpp
 * @author Libor Moravcik, Matej Mrazik
 * @date 2018-05-06
 */
#include "model/port.h"

namespace icp
{
namespace model
{

Port::Port(std::string nazov_bloku, int cislo, PortType port_type)
    : nazov_bloku(nazov_bloku), cislo(cislo), port_type(port_type)
{
    if (port_type == PortType::input)
    {
        nazov = nazov_bloku + "_" + INPUT_PORT_PREFIX + std::to_string(cislo);
    }
    else if (port_type == PortType::output)
    {
        nazov = nazov_bloku + "_" + OUTPUT_PORT_PREFIX + std::to_string(cislo);
    }
}

void Port::set_data_type(std::string retazec)
{
    data_type = new DataType();
    size_t pos = 0;
    std::string token;
    std::string delimiter = ",";

    while ((pos = retazec.find(delimiter)) != std::string::npos)
    {
        token = retazec.substr(0, pos);
        data_type->add(token);
        retazec.erase(0, pos + delimiter.length());
    }

    data_type->add(retazec);
}

}
}