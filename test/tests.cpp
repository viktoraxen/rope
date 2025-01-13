#include <gtest/gtest.h>
#include <rope.hpp>

const std::string LOREM = "Lorem ipsum odor amet, consectetuer adipiscing elit. Ultrices nostra curae mi dui litora lacinia egestas hac. Pharetra tristique arcu blandit montes rhoncus. Mi venenatis blandit dignissim; gravida non amet tempor curabitur. Pellentesque natoque sapien posuere imperdiet praesent cursus lacinia. Sit rhoncus fusce rhoncus hendrerit scelerisque etiam. Ad curabitur litora taciti, rhoncus natoque eros quis. Cras morbi class pretium congue mollis purus blandit gravida volutpat. \
    Rutrum dolor mollis nascetur elit ac molestie ullamcorper rutrum vulputate. Ut volutpat senectus neque cubilia turpis vulputate. Massa purus euismod elementum at et nunc eget. Rutrum finibus penatibus himenaeos lacinia litora et. Pellentesque cubilia aenean diam etiam habitasse justo mollis. Lobortis adipiscing taciti faucibus ex primis lectus lectus. Cursus sociosqu malesuada vivamus lobortis eget curabitur. \
    Ultricies condimentum aliquet potenti fames viverra. Scelerisque porttitor bibendum suspendisse; nunc duis eget. Eleifend suspendisse curabitur metus natoque inceptos viverra rutrum aliquam. Orci neque venenatis feugiat malesuada pellentesque tincidunt. Litora euismod dui dui maximus etiam semper erat magnis inceptos. Hendrerit diam accumsan tempus dapibus; cras mollis. Quisque ut vestibulum dictum risus ridiculus vehicula natoque sociosqu hendrerit. \
    Donec bibendum at viverra vehicula ultrices? Senectus facilisi senectus; morbi nisl suspendisse mattis diam. Gravida leo arcu magnis mus venenatis litora auctor. Nisi primis odio est id erat pellentesque taciti. Dictum interdum dictumst posuere blandit pulvinar semper a nisi blandit. Donec vulputate congue purus; felis ex gravida. Congue laoreet integer etiam nascetur phasellus netus ante aliquam. Sodales himenaeos vitae curae neque dui arcu suscipit. Senectus tincidunt per faucibus risus malesuada conubia penatibus. Sollicitudin condimentum interdum nostra purus morbi arcu aliquet. \
    Massa dapibus pellentesque arcu id convallis a a. Mus ut mus dolor habitant laoreet ex ex potenti! In non pretium varius nec, mattis venenatis. Per donec ut platea dictum sagittis inceptos sociosqu nunc nibh. Pulvinar non parturient nibh donec at convallis molestie. Primis sem ligula sit eros nulla lacus maximus viverra. ";

const std::string SHORT_STR_1 = "Lorem ipsum odor amet, consectetuer adipiscing elit.";
const std::string SHORT_STR_2 = "Porttitor dictum consectetur dolor lacinia mattis netus duis non!";

TEST(RopeConstruct, Content)
{
    Rope rope(LOREM);

    ASSERT_EQ(rope.asString(), LOREM);
}

TEST(RopeCopyConstructor, SameContent)
{
    Rope rope("123");
    Rope copy(rope);

    ASSERT_EQ(rope.asString(), copy.asString());
}

TEST(RopeCopyConstructor, SeparatelyModifiable)
{
    Rope rope("123");
    Rope copy(rope);

    rope.concat("789");
    copy.concat("456");

    ASSERT_EQ(rope.asString(), "123789");
    ASSERT_EQ(copy.asString(), "123456");
}

TEST(RopeBasics, Length)
{
    Rope rope(LOREM);

    ASSERT_EQ(rope.length(), LOREM.length());
}

TEST(RopeRebalance, NewlyCreatedUnchanged)
{
    Rope rope(LOREM);
    Rope newRope(rope);
    rope.rebalance();

    ASSERT_EQ(rope, newRope);
}

static bool splitInit = false;
static std::vector<std::pair<Rope, Rope>> splits;

void initSplits()
{
    if (splitInit)
        return;

    splitInit = true;

    Rope rope(LOREM);

    for (int i = 0; i <= LOREM.length(); i++)
        splits.push_back(rope.split(i));
}

TEST(RopeSplit, NoLettersLost)
{
    initSplits();

    for (int i = 0; i <= LOREM.length(); i++)
    {
        auto [left, right] = splits[i];

        ASSERT_EQ(left.asString() + right.asString(), LOREM);
    }
}

TEST(RopeSplit, LeftEqual)
{
    initSplits();

    for (int i = 0; i <= LOREM.length(); i++)
    {
        auto [left, right] = splits[i];

        ASSERT_EQ(left.asString(), LOREM.substr(0, i));
    }
}

TEST(RopeSplit, RightEqual)
{
    initSplits();

    for (int i = 0; i <= LOREM.length(); i++)
    {
        auto [left, right] = splits[i];

        ASSERT_EQ(right.asString(), LOREM.substr(i));
    }
}

TEST(RopeSplit, NegativeIndex)
{
    Rope rope(LOREM);

    auto [left, right] = rope.split(-1);

    ASSERT_EQ(left.asString(), "");
    ASSERT_EQ(right.asString(), "");

    ASSERT_EQ(left.rootNode(), nullptr);
    ASSERT_EQ(right.rootNode(), nullptr);
}

TEST(RopeSplit, OutOfBoundIndex)
{
    Rope rope(LOREM);

    auto [left, right] = rope.split(LOREM.length() + 1);

    ASSERT_EQ(left.asString(), "");
    ASSERT_EQ(right.asString(), "");

    ASSERT_EQ(left.rootNode(), nullptr);
    ASSERT_EQ(right.rootNode(), nullptr);
}

TEST(RopeSplit, EmptyRope) {
    Rope emptyRope("");
    auto [left, right] = emptyRope.split(0);

    ASSERT_EQ(left.asString(), "");
    ASSERT_EQ(right.asString(), "");
    ASSERT_EQ(left.rootNode(), nullptr);
    ASSERT_EQ(right.rootNode(), nullptr);
}

TEST(RopeSplit, SingleCharacter) {
    Rope single("A");
    
    auto [left0, right0] = single.split(0);
    ASSERT_EQ(left0.asString(), "");
    ASSERT_EQ(right0.asString(), "A");

    auto [left1, right1] = single.split(1);
    ASSERT_EQ(left1.asString(), "A");
    ASSERT_EQ(right1.asString(), "");
}

TEST(RopeSplit, MultipleSplitsOnSameRope) {
    Rope rope("ABCDEF");
    auto [left1, right1] = rope.split(3);  // "ABC" + "DEF"
    auto [left2, right2] = rope.split(2);  // Should still split the original rope at 2 => "AB" + "CDEF"

    // The original rope's asString() might still be "ABCDEF" if split doesn't modify it
    ASSERT_EQ(rope.asString(), "ABCDEF");
    ASSERT_EQ(left1.asString(), "ABC");
    ASSERT_EQ(right1.asString(), "DEF");
    ASSERT_EQ(left2.asString(), "AB");
    ASSERT_EQ(right2.asString(), "CDEF");
}

TEST(RopeSplit, LargeString) {
    std::string large(100000, 'A');
    Rope rope(large);

    auto [left, right] = rope.split(50000);

    ASSERT_EQ(left.asString().size(), 50000);
    ASSERT_EQ(right.asString().size(), 50000);
    ASSERT_EQ(left.asString(), std::string(50000, 'A'));
    ASSERT_EQ(right.asString(), std::string(50000, 'A'));
}

TEST(RopeSplit, LeafBoundarySplit) {
    // Suppose MAX_WEIGHT = 5; let’s build a rope manually or by passing a string of length 10:
    Rope rope("ABCDEFGHIJ"); // 2 leaves if your build logic is strict about MAX_WEIGHT=5

    int boundaryIndex = 5; // Split between 'ABCDE' and 'FGHIJ'
    auto [left, right] = rope.split(boundaryIndex);

    ASSERT_EQ(left.asString(), "ABCDE");
    ASSERT_EQ(right.asString(), "FGHIJ");
}

TEST(RopeSplit, OriginalRopeStillValid) {
    Rope rope("TestingRope");
    auto [left, right] = rope.split(7);

    ASSERT_EQ(rope.asString(), "TestingRope");
}

TEST(RopeAt, CorrectChar)
{
    Rope rope(LOREM);

    for (int i = 0; i < LOREM.length(); i++)
        ASSERT_EQ(rope.at(i), LOREM.at(i));
}

TEST(RopeAt, NegativeIndex)
{
    Rope rope(LOREM);

    ASSERT_EQ(rope.at(-1), '\0');
}

TEST(RopeAt, OutOfBoundsIndex)
{
    Rope rope(LOREM);

    ASSERT_EQ(rope.at(LOREM.length() + 1), '\0');
}

TEST(RopeAt, EmptyRope) {
    Rope empty("");

    ASSERT_EQ(empty.at(0), '\0');     // Index 0 is out of bounds
    ASSERT_EQ(empty.at(-1), '\0');    // Negative index
    ASSERT_EQ(empty.at(1), '\0');     // Positive index
}

TEST(RopeAt, SingleCharacter) {
    Rope single("A");

    ASSERT_EQ(single.at(0), 'A');

    ASSERT_EQ(single.at(-1), '\0');
    ASSERT_EQ(single.at(1), '\0');
}


TEST(RopeConcat, NonEmptyStrings)
{
    Rope rope = Rope(SHORT_STR_1);
    rope.concat(Rope(SHORT_STR_2));

    ASSERT_EQ(rope.asString(), SHORT_STR_1 + SHORT_STR_2);
}

TEST(RopeConcat, OneEmtpyString)
{
    Rope rope = Rope(SHORT_STR_1);
    rope.concat(Rope(""));

    ASSERT_EQ(rope.asString(), SHORT_STR_1);
}

TEST(RopeConcat, TwoEmtpyStrings)
{
    Rope rope = Rope("");
    rope.concat(Rope(""));

    ASSERT_EQ(rope.asString(), "");
}

TEST(RopeConcat, SelfConcat)
{
    Rope rope = Rope(SHORT_STR_1);
    rope.concat(rope);

    ASSERT_EQ(rope.asString(), SHORT_STR_1 + SHORT_STR_1);
}

TEST(RopeConcat, SelfConcatMultiple)
{
    Rope rope = Rope(SHORT_STR_1);
    rope.concat(rope);
    rope.concat(rope);

    ASSERT_EQ(rope.asString(), SHORT_STR_1 + SHORT_STR_1 + SHORT_STR_1 + SHORT_STR_1);
}

TEST(RopeConcat, Repeat)
{
    Rope rope = Rope("");

    for (const char& c : SHORT_STR_1)
        rope.concat(Rope(c));

    ASSERT_EQ(rope.asString(), SHORT_STR_1);
}

TEST(RopeInsert, NonEmptyStrings)
{
    Rope rope = Rope(SHORT_STR_1);
    rope.insert(Rope(SHORT_STR_2), 10);

    ASSERT_EQ(rope.asString(), SHORT_STR_1.substr(0, 10) + SHORT_STR_2 + SHORT_STR_1.substr(10));
}

TEST(RopeInsert, OneEmtpyString)
{
    Rope rope = Rope(SHORT_STR_1);
    rope.insert(Rope(""), 10);

    ASSERT_EQ(rope.asString(), SHORT_STR_1);
}

TEST(RopeInsert, TwoEmtpyStrings)
{
    Rope rope = Rope("");
    rope.insert(Rope(""), 0);

    ASSERT_EQ(rope.asString(), "");
}

TEST(RopeInsert, SelfInsert)
{
    Rope rope = Rope(SHORT_STR_1);
    rope.insert(rope, 10);

    ASSERT_EQ(rope.asString(), SHORT_STR_1.substr(0, 10) + SHORT_STR_1 + SHORT_STR_1.substr(10));
}

TEST(RopeInsert, SelfInsertMultiple)
{
    Rope rope = Rope(SHORT_STR_1);
    rope.insert(rope, 10);
    rope.insert(rope, 10);

    std::string oneInsert = SHORT_STR_1.substr(0, 10) + SHORT_STR_1 + SHORT_STR_1.substr(10);

    ASSERT_EQ(rope.asString(),oneInsert.substr(0, 10) + oneInsert + oneInsert.substr(10)); 
}

TEST(RopeInsert, Beginning)
{
    Rope rope = Rope(SHORT_STR_1);
    rope.insert(Rope(SHORT_STR_2), 0);

    ASSERT_EQ(rope.asString(), SHORT_STR_2 + SHORT_STR_1);
}

TEST(RopeInsert, End)
{
    Rope rope = Rope(SHORT_STR_1);
    rope.insert(Rope(SHORT_STR_2), SHORT_STR_1.length());

    ASSERT_EQ(rope.asString(), SHORT_STR_1 + SHORT_STR_2);
}

TEST(RopeInsert, OutOfBounds)
{
    Rope rope = Rope(SHORT_STR_1);
    ASSERT_THROW(rope.insert(Rope(""), SHORT_STR_1.length() + 1), std::out_of_range);
}

TEST(RopeInsert, NegativeIndex)
{
    Rope rope = Rope(SHORT_STR_1);
    ASSERT_THROW(rope.insert(Rope(""), -1), std::out_of_range);
}
