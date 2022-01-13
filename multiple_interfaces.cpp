#include <iostream>
#include <memory>

class IBaseA
{
public:
    virtual ~IBaseA() = default;
};

class IBaseB
{
public:
    virtual ~IBaseB() = default;

    virtual void my_name() const = 0;
};

class IBaseC : public IBaseA, public IBaseB
{
public:
    ~IBaseC() = default;

};

class Implementation : public IBaseC
{
    std::string _name;

public:
    Implementation(std::string name) : _name{name}
    {
    }

    ~Implementation()
    {
        std::cout << "~Implementation" << std::endl;
    }

    void my_name() const override
    {
        std::cout << "Implementation::my_name: " << _name << "\n";
    }
};

class IBaseD
{
public:
    virtual ~IBaseD() = default;

    virtual void my_name() const = 0;
};

class D : public IBaseD {
    std::string _name;

public:
    D(std::string name) : _name{name}
    {
    }

    ~D()
    {
        std::cout << "~D" << std::endl;
    }

    void my_name() const override
    {
        std::cout << "D::my_name: " << _name << "\n";
    }
};

int main(int argc, char **argv)
{
    std::cout << "Hello\n";
    {
        std::unique_ptr<IBaseC> tst = std::make_unique<Implementation>("Stefan");

        tst->my_name();
    }
    {
        std::unique_ptr<IBaseD> tst = std::make_unique<D>("Zenon");

        tst->my_name();
    }

    std::cout << "Bye\n";
}
