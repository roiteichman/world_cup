//
// Created by teich on 27/11/2022.
//

#ifndef MAIN23A1_CPP_AVL_TREE_H
#define MAIN23A1_CPP_AVL_TREE_H
#include <iostream>

    template <class T>
    class AVLNode {
    private:
        AVLNode();

        T     m_data;
        AVLNode* m_left;
        AVLNode* m_right;
        AVLNode* m_parent;
        int m_height;

    public:
        AVLNode(const T& value) : m_data(value), m_left(nullptr), m_right(nullptr), m_parent(nullptr), m_height(0) {}
        ~AVLNode() {}

        const T&  getValue () const { return m_data; }
        void      setValue(T data) {m_data=data; }
        void      setLeft (AVLNode* left) { m_left = left; }
        AVLNode*  getLeft () const { return m_left; }
        void      setRight (AVLNode* right) { m_right = right; }
        AVLNode*  getRight () const { return m_right; }
        void      setParent (AVLNode* parent) { m_parent = parent; }
        AVLNode*  getParent () const { return m_parent; }
        void      removeData () { m_data = nullptr; }
        void      setHeight (int maxHeight) { m_height = maxHeight;}
        int       getHeight () const {return m_height;}

    };

    template <class T>
    class AVLTree {
    public:
        AVLTree(bool orderBy=true, AVLNode<T>* root = nullptr) : m_orderBy(orderBy), m_root(root) {}
        ~AVLTree();

        bool insert(const T& value);

        AVLNode<T>* getRoot() const { return m_root; }

        AVLNode<T>* find(AVLNode<T>* root, const T& value);
        AVLNode<T> *findInt(AVLNode<T> *root, int value) const;
        void remove(AVLNode<T>* root, const T& value);

        void findNext(AVLNode<T>* leaf);
        void findPrevious(AVLNode<T>* leaf);


        int  height(const AVLNode<T>* root) const;
        int maxHeight (const AVLNode<T>* root) const;
        int  balanceFactor(AVLNode<T>* root) const;
        void balanceTheTree(AVLNode<T>* root);

        void rotateLeft (AVLNode<T>* B);
        void rotateRight(AVLNode<T>* C);

        void printInOrder  (AVLNode<T>* root, int *const output, int& i) ; // Left, Parent, Right
        void printInOrderT  (AVLNode<T>* root, T *const output, int& i) ; // Left, Parent, Right
        AVLNode<T> *sortedArrayToAVL(T* arr, int start, int end);

    private:
        void insertAvlNodeByStats(AVLNode<T>* root, AVLNode<T>* newNode);
        void insertAvlNodeByIds(AVLNode<T>* root, AVLNode<T>* newNode);
        void deleteAvlNode(AVLNode<T>* node);
        bool m_orderBy;

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
            node->removeData();
            delete node; // Post Order Deletion
        }
    }

    template <class T>
    bool AVLTree<T>::insert(const T& value) {
        AVLNode<T> *newNode;
        try {
            newNode = new AVLNode<T>(value);
        } catch (const bad_alloc &e) {
            return true; // Out of memory
        }

        if (!m_root) { // Special case the tree is empty
            m_root = newNode;
        }
        else {
            if (m_orderBy){
                insertAvlNodeByStats(m_root, newNode);

            }
            else{
                insertAvlNodeByIds(m_root, newNode);
            }
        }
        return false;
    }

    template <class T>
    void AVLTree<T>::insertAvlNodeByStats(AVLNode<T>* root, AVLNode<T>* newNode) {
        // Binary Search Tree insertion algorithm

        // comparing by the value of the pointer
        if(*(newNode->getValue()) < *(root->getValue()) ) {
            if(root->getLeft() ) // If there is a left child, keep searching
                insertAvlNodeByStats(root->getLeft(), newNode);
            else { // Found the right spot
                root->setLeft(newNode);
                newNode->setParent(root);
            }
        }
        else {
            if(root->getRight() ) // If there is a right child, keep searching
                insertAvlNodeByStats(root->getRight(), newNode);
            else {// Found the right spot
                root->setRight(newNode);
                newNode->setParent(root);
            }
        }
        // if the node exist we catch it from outside

        root->setHeight(maxHeight(root));

        balanceTheTree(root);
        //root->getLeft()->getValue().setClosest() = findNext(root->getLeft());
    }

template <class T>
void AVLTree<T>::insertAvlNodeByIds(AVLNode<T> *root, AVLNode<T> *newNode) {
    // Binary Search Tree insertion algorithm

    // comparing by the value of the pointer
    if(*(newNode->getValue()) > *(root->getValue()) ) {
        if(root->getRight() ) // If there is a left child, keep searching
            insertAvlNodeByIds(root->getRight(), newNode);
        else { // Found the right spot
            root->setRight(newNode);
            newNode->setParent(root);
        }
    }
    else {
        if(root->getLeft() ) // If there is a right child, keep searching
            insertAvlNodeByIds(root->getLeft(), newNode);
        else {// Found the right spot
            root->setLeft(newNode);
            newNode->setParent(root);
        }
    }
    // if the node exist we catch it from outside

    root->setHeight(maxHeight(root));

    balanceTheTree(root);
    //root->getLeft()->getValue().setClosest() = findNext(root->getLeft());
}

template<class T>
int AVLTree<T>::maxHeight(const AVLNode<T> *root) const {
    return (height(root->getLeft()) > height(root->getRight())) ? height(root->getLeft())+1 : height(root->getRight()) +1;
}


template <class T>
int AVLTree<T>::height(const AVLNode<T>* root) const {
    if (root){
        return root->getHeight();
    }
    return -1;
}

template<class T>
void AVLTree<T>::balanceTheTree(AVLNode<T> *root) {
    // AVL balancing algorithm - Based on slide 14 on tutorial 5 in data structures
    int balance = balanceFactor(root);
    if( balance > 1 ) { // left tree unbalanced
        if(balanceFactor(root->getLeft()) < 0 ) { // right child of left tree is the cause
            rotateLeft(root->getLeft()); // double rotation required - means LR rotate
            root->getLeft()->getLeft()->setHeight(maxHeight(root->getLeft()->getLeft())); // update the height of B after changing pointers
            root->getLeft()->setHeight(maxHeight(root->getLeft())); // update the height of A
        }
        rotateRight(root); // LL rotate
        root->setHeight(maxHeight(root)); // update height of C
        root->getParent()->setHeight(maxHeight(root->getParent())); // update height of A
    }
    else if( balance < -1 ) { // right tree unbalanced
        if(balanceFactor(root->getRight()) > 0 ) { // left child of right tree is the cause
            rotateRight(root->getRight()); // RL rotate
            root->getRight()->getRight()->setHeight(maxHeight(root->getRight()->getRight())); // update the height of B after changing pointers
            root->getRight()->setHeight(maxHeight(root->getRight())); // update the height of A
        }
        rotateLeft(root); // RR rotate
        root->setHeight(maxHeight(root)); // update height of C
        root->getParent()->setHeight(maxHeight(root->getParent())); // update height of A
    }
}

template <class T>
void AVLTree<T>::rotateLeft (AVLNode<T>* B) {
    AVLNode<T>* A = B->getRight();
    B->setRight(A->getLeft());
    if (B->getRight())
        B->getRight()->setParent(B);
    A->setLeft(B);
    A->setParent(B->getParent());


    if(B->getParent() == nullptr ) {
        m_root = A;
    }
    else {
        if(B->getParent()->getLeft() == B ) {
            B->getParent()->setLeft(A);
        }
        else {
            B->getParent()->setRight(A);
        }
    }
    B->setParent(A);
}

    template <class T>
    void AVLTree<T>::rotateRight(AVLNode<T>* C) {
        // Rotate node
        AVLNode<T>* A = C->getLeft();
        C->setLeft(A->getRight());
        if (C->getLeft())
            C->getLeft()->setParent(C);
        A->setRight(C);

        // Adjust tree
        if(C->getParent() == nullptr ) {
            m_root = A;
            A->setParent(nullptr);
        }
        else {
            if(C->getParent()->getLeft() == C ) {
                C->getParent()->setLeft(A);
            }
            else {
                C->getParent()->setRight(A);
            }
            A->setParent(C->getParent());
        }

        C->setParent(A);
    }

    template <class T>
    int  AVLTree<T>::balanceFactor(AVLNode<T>* root) const {
    int balance = 0;
    if( root ) {
        balance = height(root->getLeft()) - height(root->getRight());
    }
    return balance;
}

    template<class T>
    void AVLTree<T>::findNext(AVLNode<T> *leaf) {
        AVLNode<T> *next = leaf;
        if (!(next->getParent())){
            return;
        }
        while (next->getParent()->getRight() == next) {
            next = next->getParent();
            if (!(next->getParent())){
                // means that we got to the root
                break;
            }
        }
        if(next->getParent()) {
            leaf->getValue()->setClosestRight(&(*next->getParent()->getValue()));
            next->getParent()->getValue()->setClosestLeft(&(*leaf->getValue()));
        }
    }



    template<class T>
    void AVLTree<T>::findPrevious(AVLNode<T> *leaf) {
        AVLNode<T> *next = leaf;
        if (!(next->getParent())){
            return;
        }
        while (next->getParent()->getLeft() == next) {
            next = next->getParent();
            if (!(next->getParent())){
                // means that we got to the root
                break;
            }
        }
        if(next->getParent()) {
            leaf->getValue()->setClosestLeft(&(*next->getParent()->getValue()));
            next->getParent()->getValue()->setClosestRight(&(*leaf->getValue()));
        }
    }

    template <class T>
    void AVLTree<T>::printInOrder(AVLNode<T>* root, int *const output, int& i) {
        if( root ) {
            printInOrder(root->getLeft(), output, i);  // Left
            output[i]=root->getValue()->getID(); // Parent
            i++;
            printInOrder(root->getRight(), output, i); // Right
        }
    }

template <class T>
void AVLTree<T>::printInOrderT(AVLNode<T>* root, T* const output, int& i) {
    if( root ) {
        printInOrderT(root->getLeft(), output, i);  // Left
        output[i] = root->getValue(); // Parent
        i++;
        printInOrderT(root->getRight(), output, i); // Right
    }
}


// Depth-First Search
    template <class T>
    AVLNode<T>* AVLTree<T>::find(AVLNode<T>* root, const T& value) {
    bool byIDs = false;
        if (m_orderBy == byIDs){
            return findInt(root, value->getID());
        }
        if( root ) {
            //std::cout << root->getValue() << std::endl;
            if(root->getValue() == value )
                return root; // Found
            else if( *value < *(root->getValue()) )
                return find(root->getLeft(), value);
            else
                return find(root->getRight(), value);
        }

        return nullptr;
    }
    template <class T>
    AVLNode<T>* AVLTree<T>::findInt(AVLNode<T>* root, int value) const{
    if( root ) {
        //std::cout << root->getValue() << std::endl;
        if(root->getValue()->getID() == value )
            return root; // Found
        else if( value < root->getValue()->getID() )
            return findInt(root->getLeft(), value);
        else
            return findInt(root->getRight(), value);
    }

    return nullptr;
}


    template<class T>
    void AVLTree<T>::remove(AVLNode<T>* root, const T &value){

        AVLNode<T>* willDeleted = find(root, value);
        if(!willDeleted)
            return;
        AVLNode<T>* parent = willDeleted->getParent();

        bool removed = false;

        // 1: if is a leaf - height==0
        if (!height(willDeleted)) {
            // if is not a root
            if (parent){
                if (willDeleted == parent->getLeft()) {
                    // after recursion the leaf is smaller than the parent but he is a right son of him
                    parent->setLeft(nullptr);
                    // if parent now is leaf
                    if (!parent->getRight()){
                        parent->setHeight(0);
                    }
                } else {
                    parent->setRight(nullptr);
                    // if parent now is leaf
                    if (!parent->getLeft()){
                        parent->setHeight(0);
                    }
                }
            }
            // if is the last junction in tree reset the tree, pointer to nullptr
            else
                m_root= nullptr;
            removed = true;
        }
        else if(willDeleted->getLeft() == nullptr || willDeleted->getRight() == nullptr) {
            AVLNode<T> *singleSon;

            // 2: if it has a single son
            if (willDeleted->getLeft() == nullptr) {
                singleSon = willDeleted->getRight();
            } else if (willDeleted->getRight() == nullptr) {
                singleSon = willDeleted->getLeft();
            }
            if (parent) {
                if (willDeleted == parent->getLeft()) {
                    // if willDeleted is a left son
                    parent->setLeft(singleSon);
                    singleSon->setParent(parent);
                } else {
                    parent->setRight(singleSon);
                    singleSon->setParent(parent);
                }
            }
            else {
                singleSon->setParent(parent);
                m_root = singleSon;
                removed = true;
            }
            willDeleted->setRight(nullptr);
            willDeleted->setLeft(nullptr);
            removed = true;
        }

        if (removed) {
            willDeleted->setParent(nullptr);
            deleteAvlNode(willDeleted);
            while (parent) {
                balanceTheTree(parent);
                parent = parent->getParent();
            }
        }

        // 3: if it is an internal junction
        if (!removed) {
            // searching for the next junction one step right and all the way down to the left
            AVLNode<T>* nextJunction = willDeleted->getRight();
            //bool hasLeftSon = false;
            while (nextJunction->getLeft()) {
                nextJunction = nextJunction->getLeft();
                //hasLeftSon = true;
            }

            T temp = willDeleted->getValue();
            //nextJunction->setValue(temp);
            willDeleted->setValue(nextJunction->getValue());
            /*
            // this node could have one son from right or none
            AVLNode<T>* rightSonOfNextJunction = nextJunction->getRight();

            // checking if willDeleted is the root
            if (parent){
                // in case willDeleted is left son
                if (willDeleted == parent->getLeft()) {
                    // update the left son
                    parent->setLeft(nextJunction);
                    nextJunction->setParent(parent);
                } else {
                    // update the right son
                    parent->setRight(nextJunction);
                    nextJunction->setParent(parent);
                }
            }

            if(hasLeftSon){
                nextJunction->getParent()->setLeft(willDeleted);
                nextJunction->setRight(willDeleted->getRight());
                willDeleted->setParent(nextJunction->getParent());
                nextJunction->getRight()->setParent(nextJunction);
                willDeleted->getParent()->setLeft(willDeleted);

            }
            else{
                nextJunction->setRight(willDeleted);
                willDeleted->setParent(nextJunction);
            }

            nextJunction->setLeft(willDeleted->getLeft());
            nextJunction->getLeft()->setParent(nextJunction);
            nextJunction->setParent(parent);


            willDeleted->setRight(rightSonOfNextJunction);
            willDeleted->setLeft(nullptr);

            if(!parent){
                this->m_root=nextJunction;
                // if it was the root update the pointer of the root in the tree
            }

            int tempHeight = willDeleted->getHeight();
            willDeleted->setHeight(nextJunction->getHeight());
            nextJunction->setHeight(tempHeight);
            */
            // now we can delete it by steps 1 or 2
            remove(nextJunction, value);
        }
    }


template <class T>
AVLNode<T>* AVLTree<T>::sortedArrayToAVL(T* arr, int start, int end)
{
    /* Base Case */
    if (start > end)
        return NULL;

    /* Get the middle element and make it root */
    int mid = (start + end)/2;
    AVLNode<T> *root;
    try {
        root = new AVLNode<T>(arr[mid]);
    }
    catch (const bad_alloc& e){
        return nullptr;
    }


    int height=0;
    /* Recursively construct the left subtree and make it
       left child of root */
    root->setLeft(sortedArrayToAVL(arr, start, mid - 1));
    if(root->getLeft()) {
        root->getLeft()->setParent(root);
        height = root->getLeft()->getHeight()+1;
    }
    /* Recursively construct the right subtree and make it
       right child of root */
    root->setRight(sortedArrayToAVL(arr, mid + 1, end));
    if(root->getRight()) {
        root->getRight()->setParent(root);
        height = height>root->getRight()->getHeight()+1 ? height : root->getRight()->getHeight()+1;
    }
    root->setHeight(height);
    return root;
}

#endif //MAIN23A1_CPP_AVL_TREE_H

