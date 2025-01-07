#include <memory>
#include <string>
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
    const int MAX_WEIGHT = 4;

    RopeNodePtr root;

public:
    Rope(const std::string& str);
    ~Rope() = default;

    std::string asString() const;
    void print() const;

private:
    void rebuildTree(std::vector<RopeNodePtr>& leaves);

    std::string nodeAsString(RopeNodePtr node) const;
    int nodeDepth(RopeNodePtr node) const;
    void printBranches(RopeNodePtr node, const std::string& prefix = "", bool isLeft = false) const;
};
