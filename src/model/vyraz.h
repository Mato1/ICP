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
         * Trieda vyraz reprezentuje 1 vyraz/vypocet.
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
                std::vector<Port *> input_ports; //Where to take data from.
                std::vector<Port *> output_ports; //Where to output result to.
                DataType expected_type; //Ocakavany datovy typ na zaklade vstupneho vyrazu.
                std::string premenna; //Do akeho pismenka ulozim vysledok.
                double vysledok; //Vysledok celeho vyrazu.

            public:
                /**
                 * @brief Vytvori novy objekt Vyraz
                 * na zaklade expression, ktory neskor
                 * sluzi na vypocitanie vysledku.
                 * 
                 * @param expression Dany vyraz v infix napr. (a+b+c+d)
                 */
                Vyraz(std::string expression);

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
                 * @brief Priradi vstupny port vyrazu.
                 * Zo vstupnych portov sa zozbieraju data pre
                 * vyhodnotenie vyrazu.
                 * 
                 * @param port Vstupny port na priradenie tomuto vyrazu.
                 */
                void assign_input_port(Port * port);

                /**
                 * @brief Priradi vystupny port vyrazu.
                 * Vyraz priradi vysledok vystupnym portom
                 * Je mozno pouzit vysledok 1 vyrazu pre viacej 
                 * vystupnych portov.
                 * 
                 * @param port Vystupny port na priradenie tomuto vyrazu.
                 */
                void assign_output_port(Port * port);
               
                /**
                 * @brief Vezme data zo vstupnych portov
                 * vyhodnoti vyraz a data ulozi do vystupnych portov.
                 * 
                 * @return double Vypocitany vysledok.
                 */
                double eval();

                inline std::vector<Port *> get_output_ports() const 
                {
                    return this->output_ports;
                }

                inline std::vector<Port *> get_input_ports() const 
                {
                    return this->input_ports;
                }
                
                /**
                 * @brief Nastavi nazov premennej do ktorej 
                 * chceme ulozit vysledok.
                 * 
                 * @param premenna 
                 */
                inline void set_premenna(std::string premenna)
                {
                    this->premenna = premenna;
                }

                inline std::string get_premenna() const
                {
                    return this->premenna;
                }

                inline double get_vysledok() const
                {
                    return this->vysledok;
                }
 
            private:
            
                /**
                 * @brief Pomocna metoda ktora 
                 * vyplnu pomocnu premennu expected_type 
                 * konkretnymi hodnotami jednotlivych premennych.
                 * Hodnoty su zozbierane zo vstupnych portov. 
                 */
                void fill_expected_type();
                
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