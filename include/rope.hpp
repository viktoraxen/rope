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
    const int MAX_WEIGHT = 5;

    RopeNodePtr root;

public:
    Rope() = default;
    Rope(const std::string& str);
    ~Rope() = default;

    std::string asString() const;
    void print() const;

    std::pair<Rope, Rope> split(int index);

private:

    RopeNodePtr buildTree(std::vector<RopeNodePtr>& leaves);

    std::string nodeAsString(RopeNodePtr node) const;
    // TODO: Implement totalWeight
    int totalWeight(const RopeNodePtr node) const { return nodeAsString(node).length(); };
    int nodeDepth(const RopeNodePtr node) const;
    void printBranches(const RopeNodePtr node, const std::string& prefix = "", bool isLeft = false) const;
};
