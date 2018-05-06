#include "model/data_type.h"

namespace icp
{
namespace model
{

DataType::~DataType()
{
    clear();
}

void DataType::add(std::string nazov)
{
    if (!has_name(nazov))
    {
        data.insert(std::pair<std::string, double *>(nazov, nullptr));
    }
}

void DataType::add(std::string nazov, double hodnota)
{
    if (!has_name(nazov))
    {
        double * d = new double(hodnota);
        data.insert(std::pair<std::string, double *>(nazov, d));
    }
}

void DataType::set(std::string nazov, double hodnota)
{
    if (has_name(nazov))
    {
        if (data[nazov] == nullptr)
        {
            data[nazov] = new double(hodnota);
        } else
        {
            (*data[nazov]) = hodnota;
        }
    }
}

void DataType::set(DataType other)
{
    for (auto it = other.data.begin(); it != other.data.end(); ++it)
    {
        this->set(it->first, *(it->second));
    }
}

double * DataType::get(std::string nazov)
{
    if (!has_name(nazov))
    {
        return nullptr;
    }

    return data[nazov];
}

bool DataType::has_name(std::string nazov)
{
    if (data.find(nazov) == data.end())
    {
        return false;
    }

    return true;
}

bool DataType::is_filled()
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

void DataType::clear()
{
    for (auto it = data.begin(); it != data.end(); it++)
    {
        delete it->second;
    }

    data.clear();
}

std::string DataType::to_string()
{
    std::string s = "";

    for (auto it = data.begin(); it != data.end(); it++)
    {
        if (it->second == nullptr)
        {
            s += it->first + "=" + "null";
        }
        else
        {
            s += it->first + "=" + std::to_string(*it->second);
        }

        s += ",";
    }

    s = s.substr(0, s.length() - 1);

    return s;
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
bool DataType::equals(DataType * other)
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



}
}