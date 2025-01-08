#include <gtest/gtest.h>
#include <rope.hpp>

std::string STR = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Praesent mollis arcu ut ligula accumsan, sit amet vulputate nunc efficitur. Phasellus eget ex a nisi hendrerit egestas. Sed tristique ultricies dui eu bibendum. Nullam felis odio, pretium quis scelerisque at, interdum quis velit. Sed iaculis mattis nulla, sed malesuada justo viverra.";

struct RopeTestData
{
    RopeTestData()
        : rope(STR)
    {
        for (int i = 0; i <= STR.length(); i++)
            splits.push_back(rope.split(i));

        negativeSplit = rope.split(-1);
        oobSplit = rope.split(STR.length() + 1);
    }

    Rope rope;
    std::vector<std::pair<Rope, Rope>> splits;
    std::pair<Rope, Rope> negativeSplit;
    std::pair<Rope, Rope> oobSplit;
};

static RopeTestData ropeTestData;

void checkTreeWeights(RopeNodePtr node)
{
    if (!node)
        return;

    if (node->isLeaf())
    {
        EXPECT_EQ(node->weight, node->content.length());
        return;
    }

    EXPECT_EQ(node->weight, node->lChild->subtreeWeight());

    checkTreeWeights(node->lChild);
    checkTreeWeights(node->rChild);
}

TEST(RopeConstruct, Content)
{
    ASSERT_EQ(STR, ropeTestData.rope.asString());
}

TEST(RopeConstruct, CorrectWeights)
{
    checkTreeWeights(ropeTestData.rope.rootNode());
}

TEST(RopeSplit, NoLettersLost)
{
    for (int i = 0; i <= STR.length(); i++)
    {
        auto [left, right] = ropeTestData.splits[i];

        ASSERT_EQ(STR, left.asString() + right.asString());
    }
}

TEST(RopeSplit, LeftEqual)
{
    for (int i = 0; i <= STR.length(); i++)
    {
        auto [left, right] = ropeTestData.splits[i];

        ASSERT_EQ(left.asString(), STR.substr(0, i));
    }
}

TEST(RopeSplit, RightEqual)
{
    for (int i = 0; i <= STR.length(); i++)
    {
        auto [left, right] = ropeTestData.splits[i];

        ASSERT_EQ(right.asString(), STR.substr(i));
    }
}

TEST(RopeSplit, CorrectWeights)
{
    for (const auto [left, right] : ropeTestData.splits)
    {
        checkTreeWeights(left.rootNode());
        checkTreeWeights(right.rootNode());
    }
}

TEST(RopeSplit, NegativeIndex)
{
    auto [left, right] = ropeTestData.negativeSplit;

    ASSERT_EQ(left.asString(), "");
    ASSERT_EQ(right.asString(), "");

    ASSERT_EQ(left.rootNode(), nullptr);
    ASSERT_EQ(right.rootNode(), nullptr);
}

TEST(RopeSplit, OutOfBoundIndex)
{
    auto [left, right] = ropeTestData.negativeSplit;

    ASSERT_EQ(left.asString(), "");
    ASSERT_EQ(right.asString(), "");

    ASSERT_EQ(left.rootNode(), nullptr);
    ASSERT_EQ(right.rootNode(), nullptr);
}
