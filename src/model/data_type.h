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
                clear();
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

            void add(std::string nazov)
            {
                if (!has_name(nazov))
                {
                    data.insert(std::pair<std::string, double *>(nazov, nullptr));
                }
            }

            void add(std::string nazov, double hodnota)
            {
                if (!has_name(nazov))
                {
                    double * d = new double(hodnota);
                    data.insert(std::pair<std::string, double *>(nazov, d));
                }
            }

            void set(std::string nazov, double hodnota)
            {
                if (has_name(nazov))
                {
                    double * d = new double(hodnota);
                    data[nazov] = d;
                }
            }

            double * get(std::string nazov)
            {
                if (!has_name(nazov)){
                    return nullptr;
                }
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

            void clear()
            {
                for (auto it = data.begin(); it != data.end(); it++)
                {
                    delete it->second;
                }
                data.clear();
            }

            std::string to_string()
            {
                std::string s = "";

                for (auto it = data.begin(); it != data.end(); it++)
                {
                    if (it->second == nullptr)
                    {
                        s += it->first + "=" + "null\n";
                    } 
                    else
                    {
                        s += it->first + "=" + std::to_string(*it->second) + "\n";
                    }
                }

                return s;
            }


        };

    }
}



#endif //ICP_BLOCKBUILDER_SRC_MODEL_DATA_TYPE_H