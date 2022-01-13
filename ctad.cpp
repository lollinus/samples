#include <iostream>
#include <iterator>
#include <type_traits>

template <typename T>
struct MyVec
{
    template <typename Iter>
    MyVec(Iter, Iter)
    {
    }
};
template <typename Iter>
MyVec(Iter, Iter) -> MyVec<typename std::iterator_traits<Iter>::value_type>;

template <typename A, typename B>
struct MyAdvancedPair
{
    template <typename T, typename U>
    MyAdvancedPair(T &&, U &&)
    {
    }
};
template <typename X, typename Y>
MyAdvancedPair(X, Y) -> MyAdvancedPair<X, Y>;

int main()
{
    int *ptr = nullptr;
    MyVec v(ptr, ptr);
    static_assert(std::is_same_v<decltype(v), MyVec<int>>);
    MyAdvancedPair adv(1729, "taxicab");
    static_assert(std::is_same_v<decltype(adv), MyAdvancedPair<int, const char *>>);

    char buba[3]{"bu"};

    std::decay<char[3]>::type b = buba;
    using bt = std::decay<char[3]>::type;
    std::cout << b << std::endl;

    // std::is_null_pointer<bt>::value;

    // auto ev = std::is_empty<MyVec<int>>::value;
    // auto ie = std::__is_empty(int);


}
