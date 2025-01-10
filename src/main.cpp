#include "rope.hpp"

#include <cmath>
#include <iostream>

int main()
{
    std::string str = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Praesent mollis arcu ut ligula accumsan, sit amet vulputate nunc efficitur. Phasellus eget ex a nisi hendrerit egestas. Sed tristique ultricies dui eu bibendum. Nullam felis odio, pretium quis scelerisque at, interdum quis velit. Sed iaculis mattis nulla, sed malesuada justo viverra.";
    Rope rope(str);
    rope.print();

    // for (int i = 0; i <= str.length(); i++)
    // {
    //     Rope rope(str);
    //
    //     auto [left, right] = rope.split(i);
    //
    //     right.print();
    //     std::cout << std::endl;
    // }

    return 0;
}
