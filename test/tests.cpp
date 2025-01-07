#include <gtest/gtest.h>
#include <rope.hpp>

TEST(RopeTest, Constructor)
{
    std::string str = "lorem ipsum";
    Rope rope(str);
    
    ASSERT_EQ(str, rope.asString());
}
