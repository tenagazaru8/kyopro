#include <iostream>
#include <vector>

struct grid
{
    
};

void f(auto a)
{
    std::cout << a << std::endl;
}

int main()
{
    std::string s = "hello";
    char c = '#';
    int a = 100;

    f(s);
    f(c);
    f(a);
}