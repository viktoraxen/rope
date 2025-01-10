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
    int subtreeWeight() 
    {
        if (isLeaf())
            return content.length();

        return lChild->subtreeWeight() + rChild->subtreeWeight();
    }
};

class Rope
{
    const int MAX_WEIGHT = 5;
    RopeNodePtr root;

public:
    Rope() = default;
    Rope(const std::string& str);
    Rope(const Rope& other) = default;
    Rope& operator=(const Rope& other);
    Rope(Rope&& other) = default;
    Rope& operator=(Rope&& other);

    std::pair<Rope, Rope> split(int index);
    void concat(const Rope& other);
    void insert(const Rope& other, int index);
    char at(int index) const;
    void subString(int start, int end, Rope& result) const;
    void erase(int start, int end);
    void rebalance();

    RopeNodePtr rootNode() const { return root; }
    std::string asString() const;
    void print() const;

private:
    RopeNodePtr buildTree(std::vector<RopeNodePtr>& leaves);

    std::string nodeAsString(RopeNodePtr node) const;
    int nodeDepth(const RopeNodePtr node) const;
    void printBranches(const RopeNodePtr node, const std::string& prefix = "", bool isLeft = false) const;
};
