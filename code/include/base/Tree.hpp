#ifndef _BASE_TREE_H_
#define _BASE_TREE_H_

#include <iostream>
#include <memory>

#include <folly/Function.h>

enum class walk_order
{
  PREORDER,
  INORDER,
  POSTORDER
};

namespace vastina {

template<typename ty>
// it's actually just a binary tree
class TreeNode : public std::enable_shared_from_this<TreeNode<ty>>
{

public:
  using node = TreeNode<ty>;
  using pointer = TreeNode<ty>*;
  using nodeptr = std::shared_ptr<TreeNode<ty>>;

public:
  ty data;
  pointer left;
  pointer right;
  pointer parent;

  TreeNode() : data(), left( nullptr ), right( nullptr ), parent( nullptr ) {}
  TreeNode( const ty& _data ) : data( _data ), left( nullptr ), right( nullptr ), parent( nullptr ) {}
  TreeNode( ty&& _data ) : data( std::move( _data ) ), left( nullptr ), right( nullptr ), parent( nullptr ) {}

private:
  inline void InOrder(
    void visit( const ty& data_ ),
    pointer root,
    void leftmark() = [] { std::cout << "go left\n"; },
    void rightmark() = [] { std::cout << "go right\n"; } )
  {
    if ( root->left != nullptr ) {
      leftmark();
      InOrder( visit, root->left );
    }
    visit( root->data );
    if ( root->right != nullptr ) {
      rightmark();
      InOrder( visit, root->right );
    }
  }
  inline void PreOrder(
    void visit( const ty& data_ ),
    pointer root,
    void leftmark() = [] { std::cout << "go left\n"; },
    void rightmark() = [] { std::cout << "go right\n"; } )
  {
    visit( root->data );
    if ( root->left != nullptr ) {
      leftmark();
      PreOrder( visit, root->left );
      // for test
      std::cout << "back to father\n";
    }
    if ( root->right != nullptr ) {
      rightmark();
      PreOrder( visit, root->right );
      // for test
      std::cout << "back to father\n";
    }
  }
  inline void PostOrder(
    void visit( const ty& data_ ),
    pointer root,
    void leftmark() = [] { std::cout << "go left\n"; },
    void rightmark() = [] { std::cout << "go right\n"; } )
  {
    if ( root->left != nullptr ) {
      leftmark();
      PostOrder( visit, root->left );
    }
    if ( root->right != nullptr ) {
      rightmark();
      PostOrder( visit, root->right );
    }
    visit( root->data );
  }

public:
  // just walk from itself
  inline void Walk( walk_order order, void visit( const ty& data_ ) )
  {
    switch ( order ) {
      case walk_order::PREORDER:
        PreOrder( visit, this );
        break;
      case walk_order::INORDER:
        InOrder( visit, this );
        break;
      case walk_order::POSTORDER:
        PostOrder( visit, this );
        break;
      default:
        break;
    }
  }

  inline void InsertLeft( pointer left )
  {
    this->left = left;
    left->parent = this;
  }

  inline void InsertRight( pointer right )
  {
    this->right = right;
    right->parent = this;
  }

  inline void InsertByCompare(
    pointer new_node,
    const folly::Function<bool( const ty& a, const ty& b )>& compare
    = []( const ty& a, const ty& b ) { return a < b; } )
  {
    auto root = this;
    while ( true ) {
      if ( const_cast<folly::Function<bool( const pointer _node )>&>( compare )( new_node->data, root->data ) ) {
        if ( root->left == nullptr ) {
          root->InsertLeft( new_node );
          return;
        }
        root = root->left;
      } else {
        if ( root->right == nullptr ) {
          root->InsertRight( new_node );
          return;
        }
        root = root->right;
      }
    }
  }
  inline void InsertByCompare(
    const ty& data_,
    const folly::Function<bool( const ty& a, const ty& b )>& compare
    = []( const ty& a, const ty& b ) { return a < b; } )
  {
    auto new_node = new node( data_ );
    InsertByCompare( new_node, compare );
  }
  // This function can cause crash if the judge function is not correct
  inline pointer FindChildR( const folly::Function<bool( const pointer _node )>& judge )
  {
    auto root = this;
    while ( !const_cast<folly::Function<bool( const pointer _node )>&>( judge )( root ) ) {
      root = root->right;
    }
    return root;
  }
  inline pointer FindChildL( const std::function<bool( const pointer _node )>& judge )
  {
    auto root = this;
    while ( !const_cast<std::function<bool( const pointer _node )>&>( judge )( root ) ) {
      root = root->left;
    }
    return root;
  }
  inline void ReplaceByL( pointer target )
  {
    if ( this->parent == nullptr ) {
      this->parent = target;
      target->left = this;
      return;
    }
    if ( this->parent->left == this )
      this->parent->left = target;
    else
      this->parent->right = target;
    target->parent = this->parent;
    this->parent = target;
    target->left = this;
  }
  inline void ReplaceByR( pointer target )
  {
    if ( this->parent == nullptr ) {
      this->parent = target;
      target->left = this;
      return;
    }
    if ( this->parent->left == this )
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

}; // namespace vastina

#endif