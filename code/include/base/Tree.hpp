#ifndef _BASE_TREE_H_
#define _BASE_TREE_H_

#include <memory>
#include <functional>

using std::shared_ptr;
using std::make_shared;


enum walk_order{
    PREORDER,
    INORDER,
    POSTORDER
};


template<typename ty>
class TreeNode : public std::enable_shared_from_this<TreeNode<ty>>{

public:
    using node = TreeNode<ty>;
    using pointer = TreeNode<ty>*;
    using nodeptr = shared_ptr<TreeNode<ty>>;

public:
    ty data;
    pointer left;
    pointer right;
    pointer parent;

    TreeNode(): data(), left(nullptr), right(nullptr), parent(nullptr){};
    TreeNode(ty tk): data(tk), left(nullptr), right(nullptr), parent(nullptr){};

private:
    inline void InOrder(void visit(const ty& data_), pointer root){
        if(root->left != nullptr){
            InOrder(visit, root->left);
        }
        visit(root->data);
        if(root->right != nullptr){
            InOrder(visit, root->right);
        }
    }
    inline void PreOrder(void visit(const ty& data_), pointer root){
        visit(root->data);
        if(root->left != nullptr){            
            PreOrder(visit, root->left);
        }
        if(root->right != nullptr){
            PreOrder(visit, root->right);
        }
    }
    inline void PostOrder(void visit(const ty& data_), pointer root){
        if(root->left != nullptr){
            PostOrder(visit, root->left);
        }
        if(root->right != nullptr){
            PostOrder(visit, root->right);
        }
        visit(root->data);
    }

public:
//just walk from itself
    inline void Walk(walk_order order, void visit(const ty& data_)){
        switch (order){
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

    inline void InsertLeft(pointer left){
        this->left = left;
        left->parent = this;
    }

    inline void InsertRight(pointer right){
        this->right = right;
        right->parent = this;
    }

//This function can cause crash if the judge function is not correct
    inline pointer FindChildR(std::function<bool(const pointer _node)> judge){
        auto root = this;
        while(!judge(root)) {
            root = root->right;
        }   
        return root;
    }

    inline void ReplaceByL(pointer target){
        if(this->parent == nullptr){
            this->parent = target;
            target->left = this;
            return;
        }
        if(this->parent->left == this)
            this->parent->left = target;
        else
            this->parent->right = target;
        target->parent = this->parent;
        this->parent = target;
        target->left = this;
    }
    inline void ReplaceByR(pointer target){
        if(this->parent == nullptr){
            this->parent = target;
            target->left = this;
            return;
        }
        if(this->parent->left == this)
            this->parent->left = target;
        else
            this->parent->right = target;
        target->parent = this->parent;
        this->parent = target;
        target->right = this;
    }
};

template <typename ty>
class Tree{


};

#endif