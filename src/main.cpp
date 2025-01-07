#include "rope.hpp"

#include <cmath>
#include <iostream>

int main()
{
    std::string str = "lorem ipsum dolor sit amet";

    for (int i = 0; i <= str.length(); i++)
    {
        Rope rope(str);

        auto [left, right] = rope.split(i);

        right.print();
        std::cout << std::endl;
    }

    return 0;
}
