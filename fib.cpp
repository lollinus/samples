
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <limits>
#include <ratio>
#include <cassert>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>

template<int N>
constexpr uint64_t fibT()
{
    return fibT<N-1>() + fibT<N-2>();
}

template<>
constexpr uint64_t fibT<0>()
{
    return 0;
}

template<>
constexpr uint64_t fibT<1>()
{
    return 1;
}

template<>
constexpr uint64_t fibT<2>()
{
    return 1;
}

template <int N>
struct fibT_s {
    constexpr static uint64_t val = fibT_s<N-1>::val + fibT_s<N-2>::val;
};

template<>
struct fibT_s<0> {
    constexpr static uint64_t val = 0;
};

template<>
struct fibT_s<1> {
    constexpr static uint64_t val = 1;
};

/**
 * Calculate \p n'th element of fibonacci
 */
uint64_t fib(unsigned int n)
{
    if (n == 0)
    {
        return 0;
    }

    if (n == 1)
    {
        return 1;
    }

    uint64_t prev = 0;
    uint64_t nth = 1;
    while (n > 1)
    {
        size_t curr = nth + prev;
        prev = nth;
        nth = curr;
        n--;
    }

    return nth;
}

uint64_t f2n_impl(unsigned int n2, std::vector<uint64_t>& vals)
{
    std::cout << "entry: f2n_impl(" << n2 << ")" << std::endl;
    // std::string out{"["};
    // out.append(std::to_string(vals.front()));

    // out = std::accumulate(std::next(vals.cbegin()), vals.cend(), std::move(out),
    //                       [](std::string o, uint64_t v)
    //                       {
    //                           return std::move(o) + "," + std::to_string(v);
    //                       });
    // out.append("]");
    // std::cout << "f2n_impl vals:" << out << std::endl;

    if (n2 == 0)
    {
        // std::cout << "exit: f2n_impl(" << n2 << ") -> " << 0 << std::endl;
        return 0;
    }

    if (vals.size() > n2 && vals[n2] > 0)
    {
        // std::cout << "exit: c: f2n_impl(" << n2 << ") -> " << vals[n2] << std::endl;
        return vals[n2];
    }

    uint64_t res = 0;
    /** for even numbers */
    if (!(n2 % 2))
    {
        uint64_t n = n2 / 2;
        uint64_t n1 = (n2 / 2) - 1;
        uint64_t fn = f2n_impl(n, vals);
        uint64_t fn1 = f2n_impl(n1, vals);

        std::cout << "fn(" << n << ") -> " << fn << std::endl;
        std::cout << "fn(" << n1 << ") -> " << fn1 << std::endl;

        res = ((2 * fn1) + fn) * fn;
    }
    else
    {
        uint64_t fv2n2 = f2n_impl(n2 - 2, vals);
        uint64_t fv2n1 = f2n_impl(n2 - 1, vals);
        res = fv2n2 + fv2n1;
    }

    if (vals.size() <= n2)
    {
        // std::cout << "f2n_impl: vals.size() " << vals.size() << " resize:  " << n2 + 1 << std::endl;
        vals.resize(n2 + 1);
    }

    vals[n2] = res;

    // std::cout << "exit: f2n_impl(" << n2 << ") -> " << vals[n2] << std::endl;
    return res;
}

static std::vector<uint64_t> vals{0, 1, 1};

uint64_t f2n(unsigned int n2)
{
    std::cout << "entry: f2n(" << n2 << ")" << std::endl;

    if (n2 == 0)
    {
        return 0;
    }

    uint64_t res = 0;
    if (vals.size() > n2 && vals[n2] > 0)
    {
        res = vals[n2];
    }
    else
    {
        res = f2n_impl(n2, vals);
    }

    std::cout << "exit: f2n(" << n2 << ") -> " << res << std::endl;
    return res;
}

int main()
{

    std::cout << "Max possible value size_t: " << std::numeric_limits<size_t>::max() << std::endl;
    std::cout << "next: " << std::numeric_limits<size_t>::max() + 1 << std::endl;
    std::cout << "Max possible value uint64_t: " << std::numeric_limits<uint64_t>::max()
              << std::endl;

    // max size_t:    18446744073709551615
    // 93: 12200160415121876738

    for (int n = 93; n >= 0; n--)
    {
        std::cout << "---- " << n << " ----" << std::endl;
        auto fv = fib(n);
        auto f2v = f2n(n);
        std::cout << "fib(" << n << ") -> " << fv << ", f2n -> " << f2v << std::endl;
        assert(fv == f2v);
    }

    // std::string out{"["};
    // out.append(std::to_string(vals.front()));
    // out = std::accumulate(std::next(vals.cbegin()), vals.cend(), std::move(out),
    //                       [](std::string o, uint64_t v)
    //                       {
    //                           return std::move(o) + "," + std::to_string(v);
    //                       });
    // out.append("]");
    // std::cout << "Accumulated vals: " << out << std::endl;

    std::cout << " " << 0 << ": " << fibT<0>() << " fibT_s<" << 0 << ">::val " << fibT_s<0>::val
              << std::endl;
    std::cout << " " << 1 << ": " << fibT<1>() << " fibT_s<" << 1 << ">::val " << fibT_s<1>::val
              << std::endl;
    std::cout << " " << 2 << ": " << fibT<2>() << " fibT_s<" << 2 << ">::val " << fibT_s<2>::val
              << std::endl;
    std::cout << " " << 3 << ": " << fibT<3>() << " fibT_s<" << 3 << ">::val " << fibT_s<3>::val
              << std::endl;
    std::cout << " " << 4 << ": " << fibT<4>() << " fibT_s<" << 4 << ">::val " << fibT_s<4>::val
              << std::endl;
    std::cout << " " << 5 << ": " << fibT<5>() << " fibT_s<" << 5 << ">::val " << fibT_s<5>::val
              << std::endl;
    std::cout << " " << 6 << ": " << fibT<6>() << " fibT_s<" << 6 << ">::val " << fibT_s<6>::val
              << std::endl;
    std::cout << " " << 7 << ": " << fibT<7>() << " fibT_s<" << 7 << ">::val " << fibT_s<7>::val
              << std::endl;
    std::cout << " " << 8 << ": " << fibT<8>() << " fibT_s<" << 8 << ">::val " << fibT_s<8>::val
              << std::endl;
    std::cout << " " << 9 << ": " << fibT<9>() << " fibT_s<" << 9 << ">::val " << fibT_s<9>::val
              << std::endl;
    std::cout << " " << 10 << ": " << fibT<10>() << " fibT_s<" << 10 << ">::val " << fibT_s<10>::val
              << std::endl;
    std::cout << " " << 11 << ": " << fibT<11>() << " fibT_s<" << 11 << ">::val " << fibT_s<11>::val
              << std::endl;
}
