#ifndef _BASE_TREE_H_
#define _BASE_TREE_H_

#include <memory>

#include <folly/Function.h>

using std::make_shared;
using std::shared_ptr;

enum walk_order {
    PREORDER,
    INORDER,
    POSTORDER
};

template <typename ty>
// it's actually just a binary tree
class TreeNode : public std::enable_shared_from_this<TreeNode<ty>> {

  public:
    using node = TreeNode<ty>;
    using pointer = TreeNode<ty> *;
    using nodeptr = shared_ptr<TreeNode<ty>>;

  public:
    ty data;
    pointer left;
    pointer right;
    pointer parent;

    TreeNode() : data(), left(nullptr), right(nullptr), parent(nullptr){};
    TreeNode(const ty &_data)
        : data(_data), left(nullptr), right(nullptr), parent(nullptr){};
    TreeNode(ty &&_data)
        : data(std::move(_data)), left(nullptr), right(nullptr),
          parent(nullptr){};

  private:
    inline void
    InOrder(void visit(const ty &data_), pointer root) {
        if (root->left != nullptr) {
            InOrder(visit, root->left);
        }
        visit(root->data);
        if (root->right != nullptr) {
            InOrder(visit, root->right);
        }
    }
    inline void
    PreOrder(void visit(const ty &data_), pointer root) {
        visit(root->data);
        if (root->left != nullptr) {
            PreOrder(visit, root->left);
        }
        if (root->right != nullptr) {
            PreOrder(visit, root->right);
        }
    }
    inline void
    PostOrder(void visit(const ty &data_), pointer root) {
        if (root->left != nullptr) {
            PostOrder(visit, root->left);
        }
        if (root->right != nullptr) {
            PostOrder(visit, root->right);
        }
        visit(root->data);
    }

  public:
    // just walk from itself
    inline void
    Walk(walk_order order, void visit(const ty &data_)) {
        switch (order) {
        case PREORDER:
            PreOrder(visit, this);
            break;
        case INORDER:
            InOrder(visit, this);
            break;
        case POSTORDER:
            PostOrder(visit, this);
            break;
        default:
            break;
        }
    }

    inline void
    InsertLeft(pointer left) {
        this->left = left;
        left->parent = this;
    }

    inline void
    InsertRight(pointer right) {
        this->right = right;
        right->parent = this;
    }

    inline void
    InsertByCompare(
        pointer new_node, folly::Function<bool(const ty &a, const ty &b)> compare = [](const ty &a, const ty &b) { return a < b; }) {
        auto root = this;
        while (true) {
            if (compare(new_node->data, root->data)) {
                if (root->left == nullptr) {
                    root->InsertLeft(new_node);
                    return;
                }
                root = root->left;
            } else {
                if (root->right == nullptr) {
                    root->InsertRight(new_node);
                    return;
                }
                root = root->right;
            }
        }
    }
    inline void
    InsertByCompare(
        const ty &data_, folly::Function<bool(const ty &a, const ty &b)> compare = [](const ty &a, const ty &b) { return a < b; }) {
        auto new_node = new node(data_);
        InsertByCompare(new_node, compare);
    }
    // This function can cause crash if the judge function is not correct
    inline pointer
    FindChildR(std::function<bool(const pointer _node)> judge) {
        auto root = this;
        while (!judge(root)) {
            root = root->right;
        }
        return root;
    }
    inline pointer
    FindChildL(std::function<bool(const pointer _node)> judge) {
        auto root = this;
        while (!judge(root)) {
            root = root->left;
        }
        return root;
    }
    inline void
    ReplaceByL(pointer target) {
        if (this->parent == nullptr) {
            this->parent = target;
            target->left = this;
            return;
        }
        if (this->parent->left == this)
            this->parent->left = target;
        else
            this->parent->right = target;
        target->parent = this->parent;
        this->parent = target;
        target->left = this;
    }
    inline void
    ReplaceByR(pointer target) {
        if (this->parent == nullptr) {
            this->parent = target;
            target->left = this;
            return;
        }
        if (this->parent->left == this)
            this->parent->left = target;
        else
            this->parent->right = target;
        target->parent = this->parent;
        this->parent = target;
        target->right = this;
    }
}; // This one should be modified after the completion of <ty, lty, rty>

// template <typename ty, //root type
//         typename lty,  //left type
//         typename rty>  //righttype
// class TreeNode{
// public:
//     using node = TreeNode<ty, lty, rty>;
//     using Lpointer = ;

// public:

// };

#endif