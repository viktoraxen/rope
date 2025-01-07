#include "rope.hpp"

#include <algorithm>
#include <cmath>
#include <cstring>
#include <iostream>
#include <memory>

Rope::Rope(const std::string& str)
{
    int leafCount = std::ceil(str.length() / float(MAX_WEIGHT));

    auto leaves = std::vector<RopeNodePtr>(leafCount);

    for (int i = 0; i < leafCount; i++)
    {
        int startIndex = i * MAX_WEIGHT;

        leaves[i] = std::make_shared<RopeNode>();
        leaves[i]->content = str.substr(startIndex, MAX_WEIGHT);
        leaves[i]->weight = leaves[i]->content.length();
    }

    rebuildTree(leaves);
}

void Rope::rebuildTree(std::vector<RopeNodePtr>& leaves)
{
    if (leaves.empty())
    {
        root = nullptr;
        return;
    }

    while (leaves.size() > 1)
    {
        int newSize = std::ceil(leaves.size() / 2.0);

        for (int i = 0; i < newSize; i++)
        {
            if (i * 2 == leaves.size() - 1)
            {
                leaves[i] = leaves.back();
                break;
            }

            auto node = std::make_shared<RopeNode>();
            node->lChild = leaves[i * 2];
            node->rChild = leaves[i * 2 + 1];
            node->weight = nodeAsString(node->lChild).length();

            leaves[i] = node;
        }

        leaves.resize(newSize);
    }

    root = leaves[0];
}

void Rope::print() const
{
    std::cout << "Rope Tree" << std::endl;

    printBranches(root);
}

void Rope::printBranches(RopeNodePtr node, const std::string& prefix, bool isLeft) const
{
    std::cout << prefix << (isLeft ? "├─ " : "└─ ");

    if (node->isLeaf())
        std::cout << "\033[32m" << "\"" << node->content << "\" (weight=" << node->weight << ")\033[0m\n";
    else {
        if (isLeft)
            std::cout << "\033[35m";
        else 
            std::cout << "\033[36m";

        std::cout << "[Node: weight=" << node->weight << "]\033[m\n";
    }


   std::string childPrefix = prefix + (isLeft ? "│   " : "    ");

    if (node->lChild)
        printBranches(node->lChild, childPrefix, true);

    if (node->rChild)
        printBranches(node->rChild, childPrefix, false);
}

std::string Rope::asString() const
{
    return nodeAsString(root);
}

std::string Rope::nodeAsString(RopeNodePtr node) const
{
    if (node->lChild == nullptr)
        return node->content;

    if (node->rChild == nullptr)
        return nodeAsString(node->lChild);

    return nodeAsString(node->lChild) + nodeAsString(node->rChild);
}

int Rope::nodeDepth(RopeNodePtr node) const
{
    if (node->isLeaf())
        return 1;

    return std::max(nodeDepth(node->lChild), nodeDepth(node->rChild));
}
