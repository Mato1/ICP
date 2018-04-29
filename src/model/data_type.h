#ifndef ICP_BLOCKBUILDER_SRC_MODEL_DATA_TYPE_H
#define ICP_BLOCKBUILDER_SRC_MODEL_DATA_TYPE_H

#include <vector>
#include <map>
#include <string>
#include <algorithm>

namespace icp
{
    namespace model 
    {
        struct DataType
        {
            std::map<std::string, double *> data;

            ~DataType()
            {
                for (auto it = data.begin(); it != data.end(); it++)
                {
                    delete it->second;
                }
            }

            /**
             * @brief Dva datove typy sa rovnaju ak maju rovnaky pocet 
             * premennych a zaroven sa nazvy premennych rovnaju.
             * Na poradi nezalezi.
             * 
             * @param other 
             * @return true 
             * @return false 
             */
            bool equals(DataType * other)
            {   
                if (other->data.size() != data.size())
                {
                    return false;
                }

                for (auto it = data.begin(); it != data.end(); ++it)
                {
                    if (other->data.find(it->first) != other->data.end())
                    {
                        return true;
                    }
                }
                
                return false;
            }

            void add(std::string nazov, double hodnota)
            {
                double * d = new double(hodnota);
                data.insert(std::pair<std::string, double *>(nazov, d));
            }

            double * get(std::string nazov)
            {
                return data[nazov];
            }

            bool has_name(std::string nazov)
            {
                if (data.find(nazov) == data.end())
                {
                    return false;
                } 

                return true;
            }

            bool is_filled()
            {
                for (auto it = data.begin(); it != data.end(); it++)
                {
                    if (it->second == nullptr)
                    {
                        return false;
                    }
                }

                return true;
            }


        };

    }
}



#endif //ICP_BLOCKBUILDER_SRC_MODEL_DATA_TYPE_H