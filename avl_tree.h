//
// Created by teich on 27/11/2022.
//

#ifndef MAIN23A1_CPP_AVL_TREE_H
#define MAIN23A1_CPP_AVL_TREE_H
#include <iostream>
#include "list.h"

    template <class T>
    class AVLNode {
    private:
        AVLNode();

        T     m_data;
        AVLNode* m_left;
        AVLNode* m_right;
        AVLNode* m_parent;

    public:
        AVLNode(const T& value) : m_data(value), m_left(NULL), m_right(NULL), m_parent(NULL) {}
        ~AVLNode() {}

        const T&  getValue() const { return m_data; }
        void      setLeft (AVLNode* left) { m_left = left; }
        AVLNode*  getLeft() const { return m_left; }
        void      setRight (AVLNode* right) { m_right = right; }
        AVLNode*  getRight() const { return m_right; }
        void      setParent (AVLNode* parent) { m_parent = parent; }
        AVLNode*  getParent() const { return m_parent; }

        void      print() const { std::cout << m_data << std::endl; }

    };

    template <class T>
    class AVLTree {
    public:
        AVLTree() : m_root(NULL) {}
        ~AVLTree();

        bool insert(const T& value);

        AVLNode<T>* getRoot() const { return m_root; }

        AVLNode<T>* find(AVLNode<T>* root, const T& value) const;
        void remove(AVLNode<T>* root, const T& value) const;


        int  height(AVLNode<T>* root) const;
        int  balanceFactor(AVLNode<T>* root) const;

        void rotateLeft (AVLNode<T>* root);
        void rotateRight(AVLNode<T>* root);

        void printPreOrder (AVLNode<T>* root) const; // Parent, Left, Right
        void printInOrder  (AVLNode<T>* root) const; // Left, Parent, Right
        void printPostOrder(AVLNode<T>* root) const; // Left, Right, Parent

        void PrintBreadthSearchFirst() const;

    private:
        void insertAvlNode(AVLNode<T>* root, AVLNode<T>* ins);
        void deleteAvlNode(AVLNode<T>* node);

        AVLNode<T>* m_root;
    };

    template <class T>
    AVLTree<T>::~AVLTree() {
        if( m_root ) {
            deleteAvlNode(m_root);
        }
    }

    template <class T>
    void AVLTree<T>::deleteAvlNode(AVLNode<T>* node) {
        if( node ) {
            deleteAvlNode(node->getLeft());
            deleteAvlNode(node->getRight());
            delete node; // Post Order Deletion
        }
    }

    template <class T>
    bool AVLTree<T>::insert(const T& value) {
        AVLNode<T>* new_node = new (std::nothrow) AVLNode<T>(value);

        if( !new_node )
            return true; // Out of memory

        if( !m_root ) // Special case
            m_root = new_node;
        else
            insertAvlNode(m_root, new_node);

        return false;
    }

    template <class T>
    void AVLTree<T>::insertAvlNode(AVLNode<T>* root, AVLNode<T>* ins) {
        // Binary Search Tree insertion algorithm
        if(ins->getValue() < root->getValue() ) {
            if(root->getLeft() ) // If there is a left child, keep searching
                insertAvlNode(root->getLeft(), ins);
            else { // Found the right spot
                root->setLeft(ins);
                ins->setParent(root);
            }
        }
        else {
            if(root->getRight() ) // If there is a right child, keep searching
                insertAvlNode(root->getRight(), ins);
            else {// Found the right spot
                root->setRight(ins);
                ins->setParent(root);
            }
        }

        // AVL balancing algorithm
        int balance = balanceFactor(root);
        if( balance > 1 ) { // left tree unbalanced
            if(balanceFactor(root->getLeft()) < 0 ) // right child of left tree is the cause
                rotateLeft(root->getLeft()); // double rotation required
            rotateRight(root);
        }
        else if( balance < -1 ) { // right tree unbalanced
            if(balanceFactor(root->getRight()) > 0 ) // left child of right tree is the cause
                rotateRight(root->getRight());
            rotateLeft(root);
        }
    }

    template <class T>
    void AVLTree<T>::printPreOrder(AVLNode<T>* root) const {
        if( root ) {
            root->print();                   // Parent
            printPreOrder(root->getLeft());  // Left
            printPreOrder(root->getRight()); // Right
        }
    }

    template <class T>
    void AVLTree<T>::printInOrder(AVLNode<T>* root) const {
        if( root ) {
            printInOrder(root->getLeft());  // Left
            root->print();                  // Parent
            printInOrder(root->getRight()); // Right
        }
    }
    template <class T>
    void AVLTree<T>::printPostOrder(AVLNode<T>* root) const {
        if( root ) {
            printPostOrder(root->getLeft());  // Left
            printPostOrder(root->getRight()); // Right
            root->print();                    // Parent
        }
    }

// Depth-First Search
    template <class T>
    AVLNode<T>* AVLTree<T>::find(AVLNode<T>* root, const T& value) const {
        if( root ) {
            //std::cout << root->getValue() << std::endl;
            if(root->getValue() == value )
                return root; // Found
            else if( value < root->getValue() )
                return find(root->getLeft(), value);
            else
                return find(root->getRight(), value);
        }

        return NULL;
    }

    template<class T>
    void AVLTree<T>::remove(AVLNode<T> *root, const T &value) const {

        AVLNode<T> willDeleted = find(this, value);
        AVLNode<T> parent = willDeleted.getParent();

        // 1: if is a leaf
        if(!height(willDeleted)){
            if(willDeleted.getValue() < parent.getValue()){
                parent.setLeft(NULL);
            }
            else{
                parent.setRight(NULL);
            }
        }
        else{
            // 2: if he has a single son
            if(willDeleted.getLeft()==NULL){
                parent.setRight(willDeleted.getRight());
                willDeleted.setRight(NULL);
            }
            else if (willDeleted.getRight()==NULL){
                parent.setLeft(willDeleted.getLeft());
                willDeleted.setLeft(NULL);
            }
        }


    }


template <class T>
    int AVLTree<T>::height(AVLNode<T>* root) const {
        int height = -1;
        if( root ) {
            int left  = height(root->getLeft());
            int right = height(root->getRight());
            height = 1 + ((left > right) ? left : right) ;
            // height of leaf is 0
        }
        return height;
    }

    template <class T>
    int  AVLTree<T>::balanceFactor(AVLNode<T>* root) const {
        int balance = 0;
        if( root ) {
            balance = height(root->getLeft()) - height(root->getRight());
        }
        return balance;
    }

    template <class T>
    void AVLTree<T>::rotateLeft (AVLNode<T>* root) {
        AVLNode<T>* newroot = root->getRight();
        root->setRight(newroot->getLeft());
        newroot->setLeft(root);

        if(root->getParent() == NULL ) {
            m_root = newroot;
            newroot->setParent(NULL);
        }
        else {
            if(root->getParent()->getLeft() == root ) {
                root->getParent()->setLeft(newroot);
            }
            else {
                root->getParent()->setRight(newroot);
            }
            newroot->setParent(root->getParent());
        }
        root->setParent(newroot);
    }

    template <class T>
    void AVLTree<T>::rotateRight(AVLNode<T>* root) {
        // Rotate node
        AVLNode<T>* newroot = root->getLeft();
        root->setLeft(newroot->getRight());
        newroot->setRight(root);

        // Adjust tree
        if(root->getParent() == NULL ) {
            m_root = newroot;
            newroot->setParent(NULL);
        }
        else {
            if(root->getParent()->getLeft() == root ) {
                root->getParent()->setLeft(newroot);
            }
            else {
                root->getParent()->setRight(newroot);
            }
            newroot->setParent(root->getParent());
        }

        root->setParent(newroot);
    }

    template <class T>
    void AVLTree<T>::PrintBreadthSearchFirst() const {
        List< AVLNode<T>* > node_list;

        node_list.PushFront(m_root);
        while( node_list.Length() > 0 ) {
            AVLNode<T>* n;
            node_list.PopFront(&n);
            n->print();
            if(n->getLeft() )  node_list.PushBack(n->getLeft());
            if(n->getRight() ) node_list.PushBack(n->getRight());
        }
    }


#endif //MAIN23A1_CPP_AVL_TREE_H
