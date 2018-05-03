/**
 * @brief 
 * Hlavickovy subor pre triedu Schema.
 *  
 * @file schema.h
 * @author Libor Moravcik, Matej Mrazik
 * @date 2018-04-28
 */
#ifndef ICP_BLOCKBUILDER_SRC_MODEL_SCHEMA_H
#define ICP_BLOCKBUILDER_SRC_MODEL_SCHEMA_H

#include <string>
#include "model/block.h"
#include "model/prepojenie.h"
#include "model/port.h"
#include <future>
#include <chrono>
#include <thread>
#include <vector>

namespace icp
{
    namespace model
    {   
        /**
         * @brief 
         * Trieda Schema je backend pre triedu 
         * SchemaWidget a obsahuje metody pre vytvorenie, nacitanie schemy,
         * manazovanie jednotlivych blokov a pripojeni.
         */
        class Schema 
        {
             private:

                std::string nazov;
                
                //Pre ulozenie vysledkov z asynchronne zavolanych funkcii.
                std::vector<std::future<void>> fut_vec_prepoj;
                std::vector<std::future<void>> fut_vec_eval;
                std::vector<std::future<bool>> fut_vec_cycles;
                /////////////////////////////////////////////////////////

                std::vector<Block*> blocks;
                std::vector<Prepojenie*> prepojenia;

            public:
                /**
                 * @brief Vytvori novu schemu s danym nazvom.
                 * 
                 * @param nazov 
                 */
                Schema(std::string nazov);

                /**
                 * @brief Odstrani schemu.
                 * 
                 */
                ~Schema(); 

            public:

                void add_block(Block * block);


                /**
                 * @deprecated
                 * @brief Vytvori novy block so zadanym nazvom.
                 * V pripade ze sa uz block s danym nazvom v scheme 
                 * nachadza vracia nullptr.
                 * 
                 * @param nazov Nazov noveho bloku.
                 * @return Block* Vytvoreny blok.
                 */
                Block * add_block(std::string nazov);

                /**
                 * @brief Vyhlada block podla nazvu.
                 * V pripade ze sa block s nazvom nenasiel vracia nullptr.
                 * 
                 * @param nazov Nazov bloku.
                 * @return Block* Najdeny blok.
                 */
                Block * get_block(std::string nazov);


                /**
                 * @brief Odstrani blok a uvolni pamat. V pripade ze blok
                 * s danym nazvom neexistuje nerobi nic.
                 * 
                 * @param nazov Nazov bloku.
                 */
                void remove_block(std::string nazov);

                /**
                 * @brief Odstrani dany blok, uvolni pamat.
                 * 
                 * @param block 
                 */
                void remove_block(Block * block);

                /**
                 * @brief Vytvori nove prepojenie medzi 2 blokmi.
                 * V pripade ze sa zavola prepojenie na porty ktore uz 
                 * prepojenie maju vracia nullptr. V pripade ze chceme
                 * z nejakeho podivneho dovodu nahradit prepojenie na portoch
                 * inym tak ho musime najprv vymazat.
                 * 
                 * @param bout Vystupny blok. Zaciatok prepojenia.
                 * @param pout Vystupny port daneho vystupneho bloku.
                 * @param bin Vstupny blok. Koniec prepojenia.
                 * @param pin Vstupny port daneho vstupneho bloku. 
                 * 
                 * @return Prepojenie* Vytvorene prepojenie.
                 */
                Prepojenie * add_prepoj(Block * bout, Port * const pout, 
                                        Block * bin, Port * const pin);

                /**
                 * @brief Vymaze dane prepojenie a uvolni pamat.
                 * 
                 * @param p Prepojenie ktore sa ma vymazat.
                 */
                void remove_prepoj(Prepojenie * p);

                /**
                 * @brief Vyhodnoti danu schemu.
                 */
                void eval();
                
                /**
                 * @brief Vyhodnoti dany blok.
                 * 
                 * @param block Blok ktory chceme vyhodnotit.
                 */
                void eval_block(Block * block);

                /**
                 * @brief Detekuje ci sa v scheme nenachadzaju cykly.
                 * V takomto pripade nemoze nastat vypocet.
                 * Cyklus nastava vtedy ak nejaky vstupny port bloku
                 * sme navstivili viac ako raz. 
                 * Metoda zavola pre vsetky root bloky rekurzivnu funkciu
                 * detect_cycle(Block * block, std::vector<Port *> visited_input_ports).
                 * 
                 * @return true Ak sa cykly nachadzaju.
                 * @return false  Ak sa cykly nenachadzaju.
                 */
                bool detect_cycles();

                //TO-DO pre Mata aj to okomentuj.
                void save(std::string file);
                //TO-DO pre Mata aj to okomentuj.
                void load(std::string file); 

                inline std::string const& get_nazov() const
                {
                    return nazov;
                }   

                inline const std::vector<Block*> get_all_blocks()
                {
                    return blocks;
                }
                          
            private:
                /**
                 * @brief Zisti ci existuje block s danym nazvom.
                 * Ak ano vrati ho inak vrati nullptr.
                 * V podstate je to rovnake ako get_block.
                 * 
                 * @param nazov Nazov bloku.
                 * @return Block* Najdeny blok.
                 */
                Block * block_exists(std::string nazov);

                /**
                 * @brief Vrati vsetky bloky ktore su v scheme
                 * tkzv. root alebo pociatocne.
                 * Root bloky su tie ktore nemaju ziadne predchadzajuce
                 * bloky na svoje vstupne porty napojene.
                 * 
                 * @return std::vector<Block *> Vysledny vektor root blokov. 
                 */
                std::vector<Block *> get_root_blocks();

                /**
                 * @brief Pomocna funkcia na presunutie dat z 
                 * vystupnych portov na vstupne porty medzi blokmi.
                 * Funkcia sa pouziva v asynchronnych vlaknach.
                 * Vola sa pre vsetky prepojenia a prepojenia ako samostatne
                 * asynchronne vlakna cakaju kym budu moct preniest
                 * data. Data sa prenasaju v pripade ze su vystupne porty aktivne.
                 * 
                 * @param p Prepojenie ktory ma spustit transfer dat.
                 */
                void prepojenie_transfer(Prepojenie * p);

                /**
                 * @brief Pomocna metoda ktora zisti ci sa na danych portoch uz
                 * nachadzaju nejake prepojenia. Prehladava
                 * zoznam vsetkych prepojeni s danymi vstupnymi a vystupnymi portmi.
                 * 
                 * @param pout Vystupny port.
                 * @param pin Vstupny port.
                 * @return true Ak uz existuje prepojenie na danych portoch.
                 * @return false Ak prepojenie na danych portoch neexistuje.
                 */
                bool prepoj_exists_on_ports(Port * const pout, Port * const pin);

                 /**
                 * @brief Pomocna funkcia ktora sluzi na detekovanie cyklov.
                 * @see detect_cycles(). Rekurzivne vola samu seba zacinajuc od 
                 * root blockov a potom znova pre vsetkych dalsich priamych nasledovnikov.
                 * Atd.....
                 * 
                 * @param block Aktualny navstiveny block.
                 * @param visited_input_ports Zoznam navstivenych portov.
                 * @return true Je cyklus.
                 * @return false Nie je cyklus.
                 */
                bool detect_cycle(Block * block, std::vector<Port *> visited_input_ports);           

        };
    }
}

#endif //ICP_BLOCKBUILDER_SRC_MODEL_SCHEMA_H