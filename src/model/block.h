/**
 * @brief Deklaruje triedu Block spolu s jej metodami.
 * Reprezentuje jeden blok schemy.
 * 
 * @file block.h
 * @author Libor Moravcik, Matej Mrazik
 * @date 2018-04-28
 */
#ifndef ICP_BLOCKBUILDER_SRC_MODEL_BLOCK_H
#define ICP_BLOCKBUILDER_SRC_MODEL_BLOCK_H

#include "model/port.h"
#include "model/vyraz.h"
#include <vector>
#include <map>

namespace icp
{
    namespace model
    {
        /**
         * @brief 
         * Trieda Block si uchovava vsetky vypocty ktore obsahuje,
         * vstupne a vystupne porty a priamych nasledovnikov a
         * priamych predchodcov bloku. Obsahuje metody
         * na vytvorenie a zmazanie vypoctov a vytvorenie
         * vstupnych a vystupnych portov.
         */
        class Block 
        {
            
            private:
               const std::string nazov;
               std::vector<Vyraz *> vypocty;
               std::vector<Port *> input_ports;
               std::vector<Port *> output_ports;
               std::vector<Block *> previous_blocks;
               std::vector<Block *> next_blocks;

            public:
                /**
                 * @brief Vytvori novy objekt block.
                 * 
                 * @param nazov Nazov blocku.
                 */
                Block(std::string nazov);

                /**
                 * @brief Destroy the Block object.
                 * 
                 */
                ~Block();

                /**
                 * @brief Prida novy vypocet na zaklade vyrazu.
                 * 
                 * @param vyraz Vyraz v infix zapise. napr. (a+b+c+d)
                 * @param premenna Do akej premennej sa ma ulozit vysledna hodnota.
                 * @return Vyraz* Ukazatel na instanciu triedy vypocet.
                 */
                Vyraz * add_vypocet(std::string vyraz, std::string premenna);

                /**
                 * @brief Prida vstupny port blocku.
                 * 
                 * @param type Typ ktory bude prijmat dany port.
                 * @return Port* Ukazatel na objekt Port.
                 */
                Port * add_port(DataType type, PortType port_type);

                /**
                 * @brief Odstrani port podla nazvu a uvolni pamat.
                 * 
                 * @param nazov 
                 */
                void remove_port(std::string nazov);
                
                /**
                 * @brief Odstrani zadany port a uvolni pamet.
                 * 
                 * @param p 
                 */
                void remove_port(Port *p);

                /**
                 * @brief Zisti ci port s danym nazvom existuje.
                 * Ak ano vrati ukazatel na dany port.
                 * Inak vrati nullptr.
                 * 
                 * @param nazov Nazov portu.
                 * @return Port* Ukazaten na port.
                 */
                Port * port_exists(std::string nazov);
                
                /**
                 * @brief Priradi vypocet danemu portu.
                 * V pripade ze je port vstupny tak priradi vypoctu
                 * ako vstupny port.
                 * V pripade ze je port vystupny tak prirady vypoctu
                 * ako vystupny port.
                 * 
                 * @param port Port ktory chceme priradit.
                 * @param v Vyraz ktoremu chceme port priradit.
                 */
                void add_port_to_vypocet(Port * port, Vyraz * v);

                /**
                 * @brief Vyhodnoti dany block.
                 * Prejde jednotlivymi vyrazmi ktore si vezmu
                 * data zo vstupnych portov a vysledky priradia
                 * vystupnym portom. Vystupne porty sa tak stavaju
                 * aktivne.
                 */
                void eval();

                /**
                 * @brief Vracia ci su vsetky vstupne porty aktivne.
                 * Len vtedy je mozne vyhodnotit block.
                 * Aktivny port znamena ze vsetky potrebne 
                 * data v porte maju priradene hodnoty ine
                 * nez nullptr.
                 * 
                 * @return true Ak su vsetky aktivne. 
                 * @return false Ak je aspon 1 port neaktivny.
                 */
                inline bool all_input_ports_active()
                {
                    for(auto var : input_ports)
                    {
                        if (!var->is_active())
                        {
                            return false;
                        }
                    }

                    return true;
                }
                
                /**
                 * @brief Pomocna funkcia ktora
                 * prida priameho nasledovnika tomuto
                 * blocku. Vola sa pri vytvarani
                 * prepojenia.
                 * 
                 * @param b Block ktory bude jednym z priamych nasledovnikov tohto blocku.
                 */
                inline void add_next_block(Block * b)
                {
                    this->next_blocks.push_back(b);
                }

                /**
                 * @brief Pomocna funkcia ktora
                 * prida priameho predchodcu tomuto
                 * blocku. Vola sa pri vytvarani
                 * prepojenia.
                 * 
                 * @param b  Block ktory bude jednym z priamych predchodcov tohto blocku.
                 */
                inline void add_previous_block(Block * b)
                {
                    this->previous_blocks.push_back(b);
                }

                /**
                 * @brief Vrati zoznam vsetkych priamych predchodcov tohto blocku.
                 * 
                 * @return std::vector<Block *> Vsetci priamy prechodcovia blocku.
                 */
                inline std::vector<Block *> get_previous_blocks()
                {
                    return previous_blocks;
                }

                /**
                 * @brief Vrati zoznam vsetkych priamych nasledovnikov tohto blocku.
                 * 
                 * @return std::vector<Block *> Vsetci priamy nasledovnici blocku.
                 */
                inline std::vector<Block *> get_next_blocks()
                {
                    return next_blocks;
                }

                /**
                 * @brief Zisti ci je tento block korenovym blockom (root).
                 * Korenovy block je v priade ze nema ani jedneho predchodcu.
                 * 
                 * @return true Je root.
                 * @return false Nie je root.
                 */
                inline bool is_root() const 
                {
                    return previous_blocks.empty();
                }

                /**
                 * @brief Zisti ci je tento block koncovym blockom (end).
                 * Koncovy block je v pripade ze nema ani jedneho nasledovnika.
                 * 
                 * @return true Je koncovy block.
                 * @return false Nie je koncovy block.
                 */
                inline bool is_end() const 
                {
                    return next_blocks.empty();
                }

                /**
                 * @brief Vrati zoznam vsetkych vstupnych portov.
                 * 
                 * @return const std::vector<Port*> Vsetky vstupne porty
                 */
                inline const std::vector<Port*> get_input_ports()
                {
                    return input_ports;
                }

                /**
                 * @brief Vrati zoznam vsetkych vystupnych portov.
                 * 
                 * @return const std::vector<Port*> Vsetky vystupne porty.
                 */
                inline const std::vector<Port*> get_output_ports()
                {
                    return output_ports;
                }
              
                inline const std::string& get_nazov()
                {
                    return nazov;
                }

                inline const std::vector<Vyraz*> get_vypocty()
                {
                    return vypocty;
                }
        };        
    }
}


#endif //ICP_BLOCKBUILDER_SRC_MODEL_BLOCK_H