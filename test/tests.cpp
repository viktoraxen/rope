#include <gtest/gtest.h>
#include <rope.hpp>
#include <fstream>

const std::string STR = "Lorem ipsum odor amet, consectetuer adipiscing elit. Ultrices nostra curae mi dui litora lacinia egestas hac. Pharetra tristique arcu blandit montes rhoncus. Mi venenatis blandit dignissim; gravida non amet tempor curabitur. Pellentesque natoque sapien posuere imperdiet praesent cursus lacinia. Sit rhoncus fusce rhoncus hendrerit scelerisque etiam. Ad curabitur litora taciti, rhoncus natoque eros quis. Cras morbi class pretium congue mollis purus blandit gravida volutpat. \
Rutrum dolor mollis nascetur elit ac molestie ullamcorper rutrum vulputate. Ut volutpat senectus neque cubilia turpis vulputate. Massa purus euismod elementum at et nunc eget. Rutrum finibus penatibus himenaeos lacinia litora et. Pellentesque cubilia aenean diam etiam habitasse justo mollis. Lobortis adipiscing taciti faucibus ex primis lectus lectus. Cursus sociosqu malesuada vivamus lobortis eget curabitur. \
Ultricies condimentum aliquet potenti fames viverra. Scelerisque porttitor bibendum suspendisse; nunc duis eget. Eleifend suspendisse curabitur metus natoque inceptos viverra rutrum aliquam. Orci neque venenatis feugiat malesuada pellentesque tincidunt. Litora euismod dui dui maximus etiam semper erat magnis inceptos. Hendrerit diam accumsan tempus dapibus; cras mollis. Quisque ut vestibulum dictum risus ridiculus vehicula natoque sociosqu hendrerit. \
Donec bibendum at viverra vehicula ultrices? Senectus facilisi senectus; morbi nisl suspendisse mattis diam. Gravida leo arcu magnis mus venenatis litora auctor. Nisi primis odio est id erat pellentesque taciti. Dictum interdum dictumst posuere blandit pulvinar semper a nisi blandit. Donec vulputate congue purus; felis ex gravida. Congue laoreet integer etiam nascetur phasellus netus ante aliquam. Sodales himenaeos vitae curae neque dui arcu suscipit. Senectus tincidunt per faucibus risus malesuada conubia penatibus. Sollicitudin condimentum interdum nostra purus morbi arcu aliquet. \
Massa dapibus pellentesque arcu id convallis a a. Mus ut mus dolor habitant laoreet ex ex potenti! In non pretium varius nec, mattis venenatis. Per donec ut platea dictum sagittis inceptos sociosqu nunc nibh. Pulvinar non parturient nibh donec at convallis molestie. Primis sem ligula sit eros nulla lacus maximus viverra. ";

struct RopeTestData
{
    std::string readFile(const char* filename)
    {
        std::ifstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error(std::string("File not found: ") + filename);
        }
        std::ostringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }

    RopeTestData()
        : rope(STR)
    {
        for (int i = 0; i <= STR.length(); i++)
        {
            splits.push_back(rope.split(i));
            ats.push_back(rope.at(i));
        }

        negativeSplit = rope.split(-1);
        negativeAt = rope.at(-1);
        oobSplit = rope.split(STR.length() + 1);
        oobAt = rope.at(STR.length() + 1);
    }

    Rope rope;
    std::vector<char> ats;
    char negativeAt;
    char oobAt;
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

    // EXPECT_THROW(ropeTestData.negativeSplit, )

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

TEST(RopeAt, CorrectChar)
{
    for (int i = 0; i < STR.length(); i++)
        ASSERT_EQ(ropeTestData.ats.at(i), STR.at(i));
}

TEST(RopeAt, NegativeIndex)
{
    ASSERT_EQ(ropeTestData.negativeAt, '\0');
}

TEST(RopeAt, OutOfBoundsIndex)
{
    ASSERT_EQ(ropeTestData.oobAt, '\0');
}
