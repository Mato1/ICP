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

    ~DataType();

    void add(std::string nazov);

    void add(std::string nazov, double hodnota);

    void set(std::string nazov, double hodnota);

    void set(DataType other);

    double * get(std::string nazov);

    bool has_name(std::string nazov);

    bool is_filled();

    void clear();

    std::string to_string();

       /**
     * @brief Dva datove typy sa rovnaju ak maju rovnaky pocet
     * premennych a zaroven sa nazvy premennych rovnaju.
     * Na poradi nezalezi.
     *
     * @param other
     * @return true
     * @return false
     */
    bool equals(DataType * other);

};

}
}



#endif //ICP_BLOCKBUILDER_SRC_MODEL_DATA_TYPE_H