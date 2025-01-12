#include <memory>
#include <string>
#include <utility>
#include <vector>

struct RopeNode;
using RopeNodePtr = std::shared_ptr<RopeNode>;

struct RopeNode
{
    int weight;
    RopeNodePtr lChild;
    RopeNodePtr rChild;
    std::string content;

    bool isLeaf() { return lChild == nullptr && rChild == nullptr; }
};

class Rope
{
    static const int MAX_WEIGHT = 5;
    RopeNodePtr root;

public:
    Rope() = default;
    Rope(const std::string& str);
    Rope(const char* str);
    Rope(char c);

    std::pair<Rope, Rope> split(int index) const;
    void concat(const Rope& other);
    void insert(const Rope& other, int index);
    char at(int index) const;
    void subString(int start, int end, Rope& result) const;
    void erase(int start, int end);
    void rebalance();
    int length() const;

    RopeNodePtr rootNode() const { return root; }
    std::string asString() const;
    void print() const;

private:
    RopeNodePtr buildTree(std::vector<RopeNodePtr>& leaves);
    RopeNodePtr copySubtree(RopeNodePtr node);
    void copyOnWrite();

    std::string nodeAsString(RopeNodePtr node) const;
    int nodeDepth(const RopeNodePtr node) const;
    int nodeLength(const RopeNodePtr node) const;

    void printBranches(const RopeNodePtr node, const std::string& prefix = "", bool isLeft = false) const;
};
