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
    std::string nazov_bloku;
    std::string nazov;
    int cislo = 0;
    PortType port_type;
    DataType * data_type;
    bool connected = false;
    bool ready = false;

public:
    /**
     * @brief Vytvori novy port.
     *
     * @param nazov_bloku Nazov bloku ktoremu bude patrit port.
     * @param cislo Cislo portu.Urcuje vacsinou poradie portu.
     * @param port_type Typ portu napriklad INPUT(vstupny).
     */
    Port(std::string nazov_bloku, int cislo, PortType port_type);

    void set_data_type(std::string retazec);

    inline DataType * get_data_type() const
    {
        return this->data_type;
    }

    inline PortType get_port_type() const
    {
        return this->port_type;
    }

    inline const std::string  &get_nazov() const
    {
        return this->nazov;
    }

    inline bool is_active()
    {
        return this->data_type->is_filled() && ready;
    }

    inline const std::string  &get_nazov_bloku() const
    {
        return this->nazov_bloku;
    }

    inline std::string to_string()
    {
        return get_nazov() + ": " + get_data_type()->to_string();
    }

    inline int get_cislo() const
    {
        return cislo;
    }

    inline void set_connected(bool b)
    {
        this->connected = b;
    }

    inline bool is_connected() const
    {
        return this->connected;
    }

    inline void set_ready(bool b)
    {
        this->ready = b;
    }


};
}
}


#endif //ICP_BLOCKBUILDER_SRC_MODEL_PORT_H