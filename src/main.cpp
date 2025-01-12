#include "rope.hpp"

#include <cmath>
#include <iostream>

int main()
{
    std::string str = "hejhopp";
    Rope rope(str);

    Rope r(rope);

    rope.rootNode()->rChild->content = "dkfsd";
    rope.print();
    r.print();

    return 0;
}
