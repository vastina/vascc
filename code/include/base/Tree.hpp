#ifndef _BASE_TREE_H_
#define _BASE_TREE_H_

#include <memory>

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
    using nodeptr = shared_ptr<TreeNode<ty>>;

public:
    ty data;
    nodeptr left;
    nodeptr right;
    nodeptr parent;

    TreeNode(): data(), left(nullptr), right(nullptr), parent(nullptr){};
    TreeNode(ty tk): data(tk), left(nullptr), right(nullptr), parent(nullptr){};

private:
    inline void InOrder(void show(const ty& data_), nodeptr root){
        if(root->left != nullptr){
            InOrder(show, root->left);
        }
        show(root->data);
        if(root->right != nullptr){
            InOrder(show, root->right);
        }
    }
    inline void PreOrder(void show(const ty& data_), nodeptr root){
        show(root->data);
        if(root->left != nullptr){            PreOrder(show, root->left);
        }
        if(root->right != nullptr){
            PreOrder(show, root->right);
        }
    }
    inline void PostOrder(void show(const ty& data_), nodeptr root){
        if(root->left != nullptr){
            PostOrder(show, root->left);
        }
        if(root->right != nullptr){
            PostOrder(show, root->right);
        }
        show(root->data);
    }

public:
    inline void Walk(walk_order order, void show(const ty& data_)){
        auto root = make_shared<node>(*this);
        switch (order){
            case PREORDER:
                PreOrder(show, root);
                break;
            case INORDER:
                InOrder(show, root);
                break;
            case POSTORDER:
                PostOrder(show, root);
                break;
            default:
                break;
        }
    }

    inline void InsertLeft(nodeptr left){
        this->left = left;
        left->parent = make_shared<node>(*this);
    }

    inline void InsertRight(nodeptr right){
        this->right = right;
        right->parent = make_shared<node>(*this);
    }

//This function can cause crash if the judge function is not correct
    inline nodeptr FindChildR(bool judge(nodeptr _node)){
        auto root = make_shared<node>(*this);
        while(!judge(root)) {
            root = root->right;
        }   
        return root;
    }

    inline void ReplaceByLL(nodeptr target){
        if(this->parent == nullptr){
            this->parent = target;
            target->left = make_shared<node>(*this);
            return;
        }
        this->parent->left = target;
        target->parent = this->parent;
        this->parent = target;
        target->left = make_shared<node>(*this);
    }
    inline void ReplaceByLR(nodeptr target){
        if(this->parent == nullptr){
            this->parent = target;
            target->left = make_shared<node>(*this);
            return;
        }
        this->parent->left = target;
        target->parent = this->parent;
        this->parent = target;
        target->right = make_shared<node>(*this);
    }
    inline void ReplaceByRL(nodeptr target){
        if(this->parent == nullptr){
            this->parent = target;
            target->left = make_shared<node>(*this);
            return;
        }
        this->parent->right = target;
        target->parent = this->parent;
        this->parent = target;
        target->left = make_shared<node>(*this);
    }
    inline void ReplaceByRR(nodeptr target){
        if(this->parent == nullptr){
            this->parent = target;
            target->right = make_shared<node>(*this);
            return;
        }
        this->parent->right = target;
        target->parent = this->parent;
        this->parent = target;
        target->right = make_shared<node>(*this);
    }
};

template <typename ty>
class Tree{


};

#endif