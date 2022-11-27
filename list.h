//
// Created by teich on 27/11/2022.
//

#ifndef MAIN23A1_CPP_LIST_H
#define MAIN23A1_CPP_LIST_H


#include <iostream>

    template<class T>
    class Element {
    public:
        Element(const T& value) : data_(value), next_(NULL) {}
        ~Element() {}

        void        SetValue(const T& value) { data_ = value; }
        const T&    GetValue() const { return data_; }
        void        SetNext(Element<T>* next) { next_ = next; }
        Element<T>* GetNext() const { return next_; }

        void        Print() const { std::cout << data_ << std::endl; }

    private:
        Element();  // Prevent calling the default ctor

        T           data_;
        Element<T>* next_;
    };

    template <class T>
    class List {
    public:
        List(): head_(NULL), tail_(NULL) {}
        ~List();

        bool PushFront(const T& value);
        bool PushBack (const T& value);
        bool PopFront(T* value);
        bool PopBack (T* value);

        int  Length() const;
        void Print() const;

        bool Insert(const T& value, const int& position);
        bool Delete(const int& position);

        int  Find(const int& value) const;
        bool Get (const int& position, T* value);

    private:
        Element<T>* head_;
        Element<T>* tail_;
    };

    template <class T>
    List<T>::~List() {
        Element<T>* current = head_;

        while ( current ) {
            Element<T>* next = current->GetNext();
            delete(current);
            current = next;
        }
    }

// O(1)
    template <class T>
    bool List<T>::PushFront(const T& value) {
        Element<T>* element = new( std::nothrow ) Element<T>(value);

        if( element ) {
            if( !head_ ) {
                // Special case, list is empty
                head_ = element;
                tail_ = element;
            }
            else {
                element->SetNext(head_);
                head_ = element;
            }
            return false;
        }
        else {
            return true; // Error allocating memory
        }
    }

// O(1)
    template <class T>
    bool List<T>::PushBack(const T& value) {
        Element<T>* element = new (std::nothrow) Element<T>(value);

        if( element ) {
            if( !head_ ) {
                // Special case, list is empty
                head_ = element;
                tail_ = element;
            }
            else {
                tail_->SetNext(element);
                tail_ = element;
            }
            return false;
        }
        else {
            return true;
        }
    }

// O(1)
    template <class T>
    bool List<T>::PopFront(T* value) {
        if( value && head_ ) {
            *value = head_->GetValue();

            Element<T>* new_head = head_->GetNext();
            delete head_;
            head_ = new_head;

            return false;
        }
        else
            return true; // Error: nullptr passed as parameter or list empty
    }

// O(n)
    template <class T>
    bool List<T>::PopBack(T* value) {
        if( value && tail_ ) {
            *value = tail_->GetValue();

            // Special case: one element list
            if( head_ == tail_ ) {
                delete head_;
                head_ = NULL;
                tail_ = NULL;
            }
            else {
                Element<T>* new_tail;

                new_tail = head_;

                while( new_tail->GetNext() != tail_ ) // Skip elements till the element before tail
                    new_tail = new_tail->GetNext();

                new_tail->SetNext(NULL);
                delete tail_;
                tail_ = new_tail;
            }
            return false;
        }
        else
            return true; // Error: nullptr passed as parameter or list empty
    }

// O(n)
    template <class T>
    void List<T>::Print() const {
        Element<T>* e;
        for(e = head_; e; e = e->GetNext() )
            e->Print();
    }

// O(n)
    template <class T>
    int List<T>::Length() const {
        Element<T>* e;
        int count = 0;

        for(e = head_; e; e = e->GetNext() ) ++count;

        return count;
    }

// best O(1), avg O(n), wst O(n)
    template <class T>
    bool List<T>::Insert(const T& value, const int& position) {
        if( position < 0 )
            return true;

        if( !position ) // Special case: position = 0, insert into head
            return PushFront(value);

        Element<T>* element_before = head_;

        for(int count = 0; count < position - 1; ++count ) {
            element_before = element_before->GetNext();

            if( !element_before )
                return true; // Out of bounds
        }

        Element<T>* new_element = new (std::nothrow) Element<T>(value);
        if( !new_element )
            return true;

        new_element->SetNext(element_before->GetNext());
        element_before->SetNext(new_element);

        if( element_before == tail_ ) // Special case: insert at the end
            tail_ = new_element;

        return false;
    }

    template <class T>
    bool List<T>::Delete(const int& position) {
        if( position < 0 )
            return true;

        if( !head_ )
            return true; // List empty

        Element<T>* target = head_;

        if( !position ) {// Special case: position = 0, delete head
            head_ = head_->GetNext();
            delete target;

            return false;
        }

        Element<T>* element_before = head_;
        for(int count = 0; count < position - 1; ++count ) {
            element_before = element_before->GetNext();

            if( !element_before )
                return true; // Out of bounds
        }

        target = element_before->GetNext();

        if( !target )
            return true; // Out of bounds

        element_before->SetNext(target->GetNext());

        if( target == tail_ )
            tail_ = element_before;

        delete target;

        return false;
    }

    template <class T>
    int List<T>::Find(const int& value) const {
        int position = -1;

        Element<T>* e = head_;

        for(int count = 0; e; e = e->GetNext(), ++count ) {
            if( e->GetValue() == value ) {
                position = count;
                break;
            }
        }

        return position;
    }

    template <class T>
    bool List<T>::Get(const int& position, T* value) {
        if( position < 0 || !value )
            return true; // invalid input

        Element<T>* e = head_;

        for(int count = 0; e && count < position; e = e->GetNext(), ++count ) ;

        if( !e )
            return true;

        *value = e->GetValue();

        return false;
    }


#endif //MAIN23A1_CPP_LIST_H
