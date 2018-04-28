/**
 * @brief 
 * Hlavickovy subor pre triedu Prepojenie.
 * 
 * @file prepojenie.h
 * @author Libor Moravcik, Matej Mrazik
 * @date 2018-04-28
 */
#ifndef ICP_BLOCKBUILDER_SRC_MODEL_PREPOJ_H
#define ICP_BLOCKBUILDER_SRC_MODEL_PREPOJ_H

#include "model/port.h"

namespace icp 
{
    namespace model 
    {   
        /**
         * @brief 
         * Trieda Prepojenie sluzi na prepajanie 2 portov 2 blokov.
         * Spaja vystupny port 1 bloku so vstupnym portom 2.bloku.
         * Sluzi na prenos dat medzi tymito portami.
         * Data musia byt na oboch stranach rovnakeho Datoveho Typu. 
         */
        class Prepojenie
        {
            private:
                Block * const output_block; //Blok kde zacina prepojenie
                Port  * const output_port; //Konkretny port na ktory sa prepojenie napaja v ramci output_block.
                Block * const input_block; //Blok kde konci prepojenie
                Port  * const input_port; //Konkretny port na ktory sa prepojenie napaja v ramci input_block.

            public:

                /**
                 * @brief Vytvori nove prepojenie.
                 * 
                 * @param output_block 
                 * @param output_port 
                 * @param input_block 
                 * @param input_port 
                 */
                Prepojenie(Block * const output_block, Port * const output_port, Block * const input_block, Port * const input_port)
                    : output_block(output_block), output_port(output_port), input_block(input_block), input_port(input_port)
                {
                    output_block->add_next_block(input_block);
                    input_block->add_previous_block(output_block);
                }

                inline Block * get_input_block() const
                {
                    return this->input_block;
                }

                inline Block * get_output_block() const
                {
                    return this->output_block;
                }
                           
                inline Port * get_input_port() const
                {
                    return this->input_port;
                }

                inline Port *  get_output_port() const
                {
                    return this->output_port;
                }

                /**
                 * @brief Prenesie/nastavi data zo output_portu
                 * do input_port. V pripade ze sa datove typy na oboch
                 * stranach nezhoduju nerobi nic.
                 */
                inline void transfer() const
                {
                    input_port->set_type(output_port->get_type());
                }
                
        };
    }
}


#endif //ICP_BLOCKBUILDER_SRC_MODEL_PREPOJ_H