#include <vector>
#include <iostream>
#include <algorithm>

void print_container(const std::vector<int>& c)
{
    for (auto &i : c) {
        std::cout << i << " ";
    }
    std::cout << std::endl;
}

int main( )
{
    std::vector<int> c{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    print_container(c);

    c.erase(c.begin());
    print_container(c);

    c.erase(c.begin()+2, c.begin()+5);
    print_container(c);

    auto it = std::find(c.cbegin(), c.cend(), 8);
    it = c.erase(it);
    c.insert(it, 11);

    print_container(c);

    it = std::find(c.cbegin(), c.cend(), 9);
    it = c.erase(it);
    c.insert(it, 13);

    print_container(c);

    c.push_back(33);
    print_container(c);
    auto ite = c.crbegin();
    auto it1 = ite.base();

    std::cout << "size:" << c.size() << ", dist:" << std::distance(c.begin(), c.end())
              << ", rdist:" << std::distance(c.rbegin(), c.rend()) << ", ite:" << *ite
              << ", it1:" << *it1 << std::endl;


    c.erase(it);

    print_container(c);
}
