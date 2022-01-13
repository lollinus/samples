#include <ios>
#include <iostream>
#include <ostream>
#include <string>
#include <vector>
#include <map>

int main()
{
    std::vector<std::pair<std::string, std::string>> data1{
        {"p1/v1", "v11"},
        {"p1/v2", "v12"},
        {"p1/v3", "v13"},
    };

    std::vector<std::pair<std::string, std::string>> data2{
        {"p1/v1", "v211"},
        {"p2/v1", "v21"},
        {"p2/v2", "v22"},
        {"p2/v3", "v23"},
    };
    std::map<std::string, std::string> dataMap;

    std::move(data1.begin(), data1.end(), std::inserter(dataMap, dataMap.begin()));

    std::cout << "data1: " << std::endl;
    for (auto& [key, val] : data1)
    {
        std::cout << key << ":" << val << std::endl;
    }
    std::cout << "data2: " << std::endl;
    for (auto& [key, val] : data2)
    {
        std::cout << key << ":" << val << std::endl;
    }
    std::cout << "dataMap: " << std::endl;
    for (auto& [key, val] : dataMap)
    {
        std::cout << key << ":" << val << std::endl;
    }

    auto d = data2.begin();
    while (d != data2.end())
    {
        std::cout << "d{" << d->first << ":" << d->second << "} - ";
        auto r = dataMap.try_emplace(std::move(d->first), std::move(d->second));
        std::cout << "r:" << std::boolalpha << r.second << " <> d{" << d->first << ":" << d->second
                  << "}" << std::endl;
        if (r.second)
        {
            d = data2.erase(d);
        }
        else
        {
            d = std::next(d);
        }
    }

    std::cout << "dataMap <- data2: " << std::endl;
    for (auto& [key, val] : dataMap)
    {
        std::cout << key << ":" << val << std::endl;
    }
    std::cout << "data2: " << std::endl;
    for (auto& [key, val] : data2)
    {
        std::cout << key << ":" << val << std::endl;
    }

    return 0;
}
