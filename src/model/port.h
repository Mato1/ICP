/**
 * @brief Hlavickovy subor pre triedu Port.
 * 
 * @file port.h
 * @author Libor Moravcik, Matej Mrazik
 * @date 2018-04-28
 */
#ifndef ICP_BLOCKBUILDER_SRC_MODEL_PORT_H
#define ICP_BLOCKBUILDER_SRC_MODEL_PORT_H

#include "model/data_type.h"
#include <string>
#include <map>

namespace icp 
{
    namespace model 
    {
        enum PortType { output, input }; //Typ portu
        static std::string const INPUT_PORT_PREFIX  = "pin";
        static std::string const OUTPUT_PORT_PREFIX = "pout";
        
        /**
         * @brief Jednotlive instancie su odlisene na zaklade typu portu. 
         * Porty su v podstate aktivovane vtedy ked obsahuju validne data kde ani
         * jeden nieje nullptr. 
         * Ked sa port aktivuje je pripraveny na prenos dat cez prepojenie.
         * Obsahuje urcity Datovy Typ ktory prijma.
         * Nazov portu je tvoreny z prefixu (podla typu portu), nazvu bloku ktoremu
         * port patri a z cisla toho typu portu v ramci daneho bloku.
         * Cisla portu su indexovane od 0.
         * 
         * napr. pin_bll_3 : znaci 3. vstupny port bloku bll  
         */
        class Port
        {
            
            private:
                std::string nazov;
                DataType type;
                bool active = false;
                PortType port_type; 

            public:
                /**
                 * @brief Vytvori novy port.
                 * 
                 * @param nazov_bloku Nazov bloku ktoremu bude patrit port.
                 * @param cislo Cislo portu.Urcuje vacsinou poradie portu.
                 * @param port_type Typ portu napriklad INPUT(vstupny).
                 */
                Port(std::string nazov_bloku, int cislo, PortType port_type)
                    : port_type(port_type)
                {
                    if (port_type == PortType::input)
                    {
                        nazov = INPUT_PORT_PREFIX + "_" + nazov_bloku + "_" + std::to_string(cislo);
                    } 
                    else if (port_type == PortType::output)
                    {
                        nazov = OUTPUT_PORT_PREFIX + "_" + nazov_bloku + "_" + std::to_string(cislo);
                    }
                }

                inline PortType get_port_type()
                {
                    return this->port_type;
                }

                inline void set_port_type(PortType port_type)
                {
                    this->port_type = port_type;
                }

                inline void set_type(DataType type)
                {
                    this->type = type;
                }

                inline DataType get_type()
                {
                    return this->type;
                }
                
                inline const std::string&  get_nazov()
                {
                    return this->nazov;
                }

                inline void set_active(bool active)
                {
                    this->active = active;
                }

                inline const bool& is_active() const 
                {
                    return this->active;
                }

    
        };
    }
}


#endif //ICP_BLOCKBUILDER_SRC_MODEL_PORT_H