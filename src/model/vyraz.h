/**
 * @brief
 * Hlavickovy subor pre triedu Vyraz.
 *
 * @file vyraz.h
 * @author Libor Moravcik, Matej Mrazik
 * @date 2018-04-28
 */
#ifndef ICP_BLOCKBUILDER_SRC_MODEL_VYRAZ_H
#define ICP_BLOCKBUILDER_SRC_MODEL_VYRAZ_H

#include <string>
#include <vector>
#include "model/data_type.h"
#include "model/port.h"

namespace icp
{
namespace model
{

/**
 * @brief
 * Trieda vyraz reprezentuje 1 vyraz/vyraz.
 * Vyrazom sa rozumie taky matematicky vyraz
 * zlozeny z operacii scitanie, odcitanie, nasobenie,
 * delenie, (), umocnenie , z cislic a z malych a velkych pismen
 * anglickej abecedy. napr. (a+b*c/d)^3.
 */
class Vyraz
{
private:
    std::string infix; //Vyraz v Infix.
    std::string postfix; //Vyraz v Postfix.
    Port * input_port; //Where to take data from.
    Port * output_port; //Where to output result to.
    std::string premenna; //Do akeho pismenka ulozim vysledok.

public:

    /**
     * @brief Vytvori novy objekt Vyraz
     * na zaklade expression, ktory neskor
     * sluzi na vypocitanie vysledku.
     *
     * @param expression Dany vyraz v infix napr. (a+b+c+d)
     * @param premenna Do akej premennej chceme ulozit vysledok.
     */

    Vyraz(std::string expression, std::string premenna);

    /**
     * @brief Vezme data zo vstupnych portov
     * vyhodnoti vyraz a data ulozi do vystupnych portov.
     *
     * @return int ERROR CODE.
     */
    int eval();

    /**
     * @brief Priradi vstupny port vyrazu.
     * Zo vstupnych portov sa zozbieraju data pre
     * vyhodnotenie vyrazu.
     *
     * @param port Vstupny port na priradenie tomuto vyrazu.
     */
    inline void set_input_port(Port * port)
    {
        this->input_port = port;
    }

    /**
     * @brief Priradi vystupny port vyrazu.
     * Vyraz priradi vysledok vystupnym portom
     * Je mozno pouzit vysledok 1 vyrazu pre viacej
     * vystupnych portov.
     *
     * @param port Vystupny port na priradenie tomuto vyrazu.
     */
    inline void set_output_port(Port * port)
    {
        this->output_port = port;
    }

    inline const std::string&  get_infix() const
    {
        return this->infix;
    }

    inline Port * get_input_port() const
    {
        return this->input_port;
    }

    inline Port * get_output_port() const
    {
        return this->output_port;
    }

    inline const std::string&  get_premenna() const
    {
        return this->premenna;
    }
    
private:

    /**
     * @brief Pomocna metoda pre prevedenie
     * vyrazu z infix do postfix a zaroven
     * zostavenie expected_type.
     *
     * @return std::string Postfix vyraz
     */
    std::string parse();

    /**
     * @brief Pomocna funkcia pre zistenie
     * precedencie operatora.
     * Nahradza precedencnu tabulku.
     *
     * @param c Znak ako operator.
     * @return int Precedencna hodnota operatora.
     */
    int precedence(char c);

};
}
}

#endif //ICP_BLOCKBUILDER_SRC_MODEL_VYRAZ_H