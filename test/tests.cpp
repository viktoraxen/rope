#include <gtest/gtest.h>
#include <rope.hpp>

std::string STR = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Praesent mollis arcu ut ligula accumsan, sit amet vulputate nunc efficitur. Phasellus eget ex a nisi hendrerit egestas. Sed tristique ultricies dui eu bibendum. Nullam felis odio, pretium quis scelerisque at, interdum quis velit. Sed iaculis mattis nulla, sed malesuada justo viverra.";

TEST(RopeBasic, Constructor)
{
    Rope rope(STR);

    ASSERT_EQ(STR, rope.asString());
}

TEST(RopeSplit, NoLettersLost)
{
    for (int i = 0; i <= STR.length(); i++)
    {
        Rope rope(STR);

        auto [left, right] = rope.split(i);

        ASSERT_EQ(rope.asString(), left.asString() + right.asString());
    }
}

TEST(RopeSplit, LeftEqual)
{
    for (int i = 0; i <= STR.length(); i++)
    {
        Rope rope(STR);

        auto [left, right] = rope.split(i);

        ASSERT_EQ(left.asString(), STR.substr(0, i));
    }
}

TEST(RopeSplit, RightEqual)
{
    for (int i = 0; i <= STR.length(); i++)
    {
        Rope rope(STR);

        auto [left, right] = rope.split(i);

        ASSERT_EQ(right.asString(), STR.substr(i));
    }
}
