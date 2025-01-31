#include "rope.hpp"

#include <algorithm>
#include <cmath>
#include <cstring>
#include <functional>
#include <iostream>
#include <memory>
#include <utility>

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

    root = buildTree(leaves);
}

Rope::Rope(const char* str)
    : Rope(std::string(str))
{}

Rope::Rope(char c)
    : Rope(std::string(1, c))
{}

bool Rope::operator==(const Rope& other) const
{
    std::function<bool(RopeNodePtr, RopeNodePtr)> cmpNode = [&] (RopeNodePtr node, RopeNodePtr other) -> bool
    {
        if (node == nullptr || other == nullptr)
            return node == nullptr && other == nullptr;

        if (node->isLeaf())
        {
            if (!other->isLeaf())
                return false;

            return node->content == other->content
                && node->weight == other->weight;
        }

        return cmpNode(node->lChild, other->lChild)
            && cmpNode(node->rChild, other->rChild)
            && node->weight == other->weight;
    };

    return cmpNode(root, other.root);
}

std::string Rope::asString() const
{
    return nodeAsString(root);
}

void Rope::print() const
{
    std::cout << "Rope Tree" << std::endl;

    printBranches(root);
}

std::pair<Rope, Rope> Rope::split(int index) const
{
    if (root == nullptr)
        return {Rope(), Rope()};

    if (index < 0 || index > nodeLength(root))
        return {Rope(), Rope()};

    std::function<std::pair<RopeNodePtr, RopeNodePtr>(RopeNodePtr, int)> splitNode = [&](RopeNodePtr node, int index) -> std::pair<RopeNodePtr, RopeNodePtr>
    {
        if (node->isLeaf())
        {
            if (index == 0)
                return {nullptr, node};

            if (index == node->content.length())
                return {node, nullptr};

            std::string leftString = node->content.substr(0, index);
            std::string rightString = node->content.substr(index);

            auto left = std::make_shared<RopeNode>();
            left->content = leftString;
            left->weight = leftString.length();

            auto right = std::make_shared<RopeNode>();
            right->content = rightString;
            right->weight = rightString.length();

            return {left, right};
        }

        if (index < node->weight)
        {
            auto [left, right] = splitNode(node->lChild, index);

            auto newRight = std::make_shared<RopeNode>();
            newRight->lChild = right;
            newRight->rChild = node->rChild;
            newRight->weight = node->weight - index;

            return {left, newRight};
        }
        else if (index == node->weight)
        {
            return {node->lChild, node->rChild};
        }
        else
        {
            auto [left, right] = splitNode(node->rChild, index - node->weight);

            auto newLeft = std::make_shared<RopeNode>();
            newLeft->lChild = node->lChild;
            newLeft->rChild = left;
            newLeft->weight = node->weight;

            return {newLeft, right};
        }
    };

    auto [left, right] = splitNode(root, index);

    Rope leftRope, rightRope;

    leftRope.root = left;
    rightRope.root = right;

    return {leftRope, rightRope};
}

void Rope::concat(const Rope& other)
{
    copyOnWrite();

    RopeNodePtr newRoot = std::make_shared<RopeNode>();

    newRoot->lChild = root;
    newRoot->rChild = copySubtree(other.root);
    newRoot->weight = length();

    root = newRoot;

    rebalance();
}

void Rope::insert(const Rope& other, int index)
{
    if (index < 0 || index > nodeLength(root))
        throw std::out_of_range("Index out of range");

    copyOnWrite();

    auto [left, right] = split(index);

    left.concat(other);
    left.concat(right);

    root = left.root;

    rebalance();
}

char Rope::at(int index) const
{
    if (index < 0 || index >= length())
        return '\0';

    std::function<char(RopeNodePtr, int)> findChar = [&](RopeNodePtr node, int index) -> char
    {
        if (node == nullptr)
            return '\0';

        if (node->isLeaf())
        {
            if (index >= node->weight)
                return '\0';

            return node->content.at(index);
        }

        if (index < node->weight)
            return findChar(node->lChild, index);

        return findChar(node->rChild, index - node->weight);
    };

    return findChar(root, index);
}

Rope Rope::subString(int start, int end) const
{
    if (start < 0 || start > length() || end < 0 || end > length())
        throw std::out_of_range("Index out of range");

    if (start >= end)
        return Rope();

    auto [left, right] = split(end);
    auto [_, mid] = left.split(start);

    return mid;
}

void Rope::erase(int start, int end)
{
    if (start < 0 || start > length() || end < 0 || end > length())
        throw std::out_of_range("Index out of range");

    if (start >= end)
        return;

    auto [left, last] = split(end);
    auto [first, mid] = left.split(start);

    first.concat(last);
    root = first.root;

    rebalance();
}


void Rope::rebalance()
{
    copyOnWrite();

    auto leaves = collectLeaves();
    root = buildTree(leaves);
}

int Rope::length() const
{
    return nodeLength(root);
}

RopeNodePtr Rope::buildTree(std::vector<RopeNodePtr>& leaves)
{
    if (leaves.empty())
        return nullptr;

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

    return leaves.front();
}

RopeNodePtr Rope::copySubtree(RopeNodePtr node)
{
    if (node == nullptr)
        return nullptr;

    RopeNodePtr newNode = std::make_shared<RopeNode>();

    newNode->lChild = copySubtree(node->lChild);
    newNode->rChild = copySubtree(node->rChild);
    newNode->content = node->content;
    newNode->weight = node->weight;

    return newNode; 
}

std::vector<RopeNodePtr> Rope::collectLeaves() const
{
    std::function<void(RopeNodePtr, std::vector<RopeNodePtr>&)> collect = [&](RopeNodePtr node, std::vector<RopeNodePtr>& leaves)
    {
        if (node == nullptr)
            return;

        if (node->isLeaf())
        {
            leaves.push_back(node);
            return;
        }

        collect(node->lChild, leaves);
        collect(node->rChild, leaves);
    };

    std::vector<RopeNodePtr> leaves;
    collect(root, leaves);
    return leaves;
}

void Rope::copyOnWrite()
{
    if (!root.unique())
        root = copySubtree(root);
}

std::string Rope::nodeAsString(RopeNodePtr node) const
{
    if (node == nullptr)
        return "";

    if (node->isLeaf())
        return node->content;

    return nodeAsString(node->lChild) + nodeAsString(node->rChild);
}

int Rope::nodeDepth(const RopeNodePtr node) const
{
    if (node == nullptr)
        return 0;

    if (node->isLeaf())
        return 1;

    return 1 + std::max(nodeDepth(node->lChild), nodeDepth(node->rChild));
}

int Rope::nodeLength(const RopeNodePtr node) const
{
    if (node == nullptr)
        return 0;

    if (node->isLeaf())
        return node->content.length();

    return nodeLength(node->lChild) + nodeLength(node->rChild);
}

void Rope::printBranches(const RopeNodePtr node, const std::string& prefix, bool isLeft) const
{
    if (node == nullptr)
    {
        std::cout << "└─ " << "<empty>" << std::endl;
        return;
    }

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
