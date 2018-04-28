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

            void equals(DataType * /*other*/)
            {
                //TO-DO dorobit ci sa 2 datove typy rovnaju
                //Rovnaju sa v pripade ze hash mapy obsahuju rovnake znaky ako kluce.
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