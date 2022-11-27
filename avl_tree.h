//
// Created by teich on 27/11/2022.
//

#ifndef MAIN23A1_CPP_AVL_TREE_H
#define MAIN23A1_CPP_AVL_TREE_H
#include <iostream>
#include "list.h"

    template <class T>
    class AVLNode {
    public:
        AVLNode(const T& value) : m_data(value), m_left(NULL), m_right(NULL), m_parent(NULL) {}
        ~AVLNode() {}

        const T&  GetValue() const { return m_data; }
        void      SetLeft(AVLNode* left) { m_left = left; }
        AVLNode*  GetLeft() const { return m_left; }
        void      GetRight(AVLNode* right) { m_right = right; }
        AVLNode*  GetRight() const { return m_right; }
        void      SetParent(AVLNode* parent) { m_parent = parent; }
        AVLNode*  GetParent() const { return m_parent; }

        void      Print() const { std::cout << m_data << std::endl; }

    private:
        AVLNode();

        T     m_data;
        AVLNode* m_left;
        AVLNode* m_right;
        AVLNode* m_parent;
    };

    template <class T>
    class AVLTree {
    public:
        AVLTree() : m_root(NULL) {}
        ~AVLTree();

        bool Insert(const T& value);

        AVLNode<T>* GetRoot() const { return m_root; }

        AVLNode<T>* Find(AVLNode<T>* root, const T& value) const;

        int  Height(AVLNode<T>* root) const;
        int  BalanceFactor(AVLNode<T>* root) const;

        void RotateLeft (AVLNode<T>* root);
        void RotateRight(AVLNode<T>* root);

        void PrintPreOrder (AVLNode<T>* root) const; // Parent, Left, Right
        void PrintInOrder  (AVLNode<T>* root) const; // Left, Parent, Right
        void PrintPostOrder(AVLNode<T>* root) const; // Left, Right, Parent

        void PrintBreadthSearchFirst() const;

    private:
        void InsertAVLNode(AVLNode<T>* root, AVLNode<T>* ins);
        void DeleteAVLNode(AVLNode<T>* node);

        AVLNode<T>* m_root;
    };

    template <class T>
    AVLTree<T>::~AVLTree() {
        if( m_root ) {
            DeleteAVLNode(m_root);
        }
    }

    template <class T>
    void AVLTree<T>::DeleteAVLNode(AVLNode<T>* node) {
        if( node ) {
            DeleteAVLNode( node->GetLeft() );
            DeleteAVLNode( node->GetRight() );
            delete node; // Post Order Deletion
        }
    }

    template <class T>
    bool AVLTree<T>::Insert(const T& value) {
        AVLNode<T>* new_node = new (std::nothrow) AVLNode<T>(value);

        if( !new_node )
            return true; // Out of memory

        if( !m_root ) // Special case
            m_root = new_node;
        else
            InsertAVLNode(m_root, new_node);

        return false;
    }

    template <class T>
    void AVLTree<T>::InsertAVLNode(AVLNode<T>* root, AVLNode<T>* ins) {
        // Binary Search Tree insertion algorithm
        if( ins->GetValue() < root->GetValue() ) {
            if( root->GetLeft() ) // If there is a left child, keep searching
                InsertAVLNode(root->GetLeft(), ins);
            else { // Found the right spot
                root->SetLeft(ins);
                ins->SetParent(root);
            }
        }
        else {
            if( root->GetRight() ) // If there is a right child, keep searching
                InsertAVLNode(root->GetRight(), ins);
            else {// Found the right spot
                root->SetRight(ins);
                ins->SetParent(root);
            }
        }

        // AVL balancing algorithm
        int balance = BalanceFactor(root);
        if( balance > 1 ) { // left tree unbalanced
            if( BalanceFactor( root->GetLeft() ) < 0 ) // right child of left tree is the cause
                RotateLeft( root->GetLeft() ); // double rotation required
            RotateRight(root);
        }
        else if( balance < -1 ) { // right tree unbalanced
            if( BalanceFactor( root->GetRight() ) > 0 ) // left child of right tree is the cause
                RotateRight( root->GetRight() );
            RotateLeft(root);
        }
    }

    template <class T>
    void AVLTree<T>::PrintPreOrder(AVLNode<T>* root) const {
        if( root ) {
            root->Print();                   // Parent
            PrintPreOrder(root->GetLeft());  // Left
            PrintPreOrder(root->GetRight()); // Right
        }
    }

    template <class T>
    void AVLTree<T>::PrintInOrder(AVLNode<T>* root) const {
        if( root ) {
            PrintInOrder(root->GetLeft());  // Left
            root->Print();                  // Parent
            PrintInOrder(root->GetRight()); // Right
        }
    }
    template <class T>
    void AVLTree<T>::PrintPostOrder(AVLNode<T>* root) const {
        if( root ) {
            PrintPostOrder(root->GetLeft());  // Left
            PrintPostOrder(root->GetRight()); // Right
            root->Print();                    // Parent
        }
    }

// Depth-First Search
    template <class T>
    AVLNode<T>* AVLTree<T>::Find(AVLNode<T>* root, const T& value) const {
        if( root ) {
            //std::cout << root->GetValue() << std::endl;
            if( root->GetValue() == value )
                return root; // Found
            else if( value < root->GetValue() )
                return Find( root->GetLeft(), value );
            else
                return Find( root->GetRight(), value );
        }

        return NULL;
    }

    template <class T>
    int AVLTree<T>::Height(AVLNode<T>* root) const {
        int height = 0;
        if( root ) {
            int left  = Height(root->GetLeft());
            int right = Height(root->GetRight());
            height = 1 + ((left > right) ? left : right) ;
        }
        return height;
    }

    template <class T>
    int  AVLTree<T>::BalanceFactor(AVLNode<T>* root) const {
        int balance = 0;
        if( root ) {
            balance = Height(root->GetLeft()) - Height(root->GetRight());
        }
        return balance;
    }

    template <class T>
    void AVLTree<T>::RotateLeft (AVLNode<T>* root) {
        AVLNode<T>* newroot = root->GetRight();
        root->SetRight(newroot->GetLeft());
        newroot->SetLeft(root);

        if( root->GetParent() == NULL ) {
            m_root = newroot;
            newroot->SetParent(NULL);
        }
        else {
            if( root->GetParent()->GetLeft() == root ) {
                root->GetParent()->SetLeft(newroot);
            }
            else {
                root->GetParent()->SetRight(newroot);
            }
            newroot->SetParent(root->GetParent());
        }
        root->SetParent(newroot);
    }

    template <class T>
    void AVLTree<T>::RotateRight(AVLNode<T>* root) {
        // Rotate node
        AVLNode<T>* newroot = root->GetLeft();
        root->SetLeft(newroot->GetRight());
        newroot->SetRight(root);

        // Adjust tree
        if( root->GetParent() == NULL ) {
            m_root = newroot;
            newroot->SetParent(NULL);
        }
        else {
            if( root->GetParent()->GetLeft() == root ) {
                root->GetParent()->SetLeft(newroot);
            }
            else {
                root->GetParent()->SetRight(newroot);
            }
            newroot->SetParent(root->GetParent());
        }

        root->SetParent(newroot);
    }

    template <class T>
    void AVLTree<T>::PrintBreadthSearchFirst() const {
        List< AVLNode<T>* > node_list;

        node_list.PushFront(m_root);
        while( node_list.Length() > 0 ) {
            AVLNode<T>* n;
            node_list.PopFront(&n);
            n->Print();
            if( n->GetLeft() )  node_list.PushBack(n->GetLeft());
            if( n->GetRight() ) node_list.PushBack(n->GetRight());
        }
    }

#endif //MAIN23A1_CPP_AVL_TREE_H
