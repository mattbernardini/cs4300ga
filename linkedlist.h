#include <stdlib.h>
#ifndef LINKEDLIST_H
#define LINKEDLIST_H 
template <class T>
class LinkedList
{
private:
    int id;
    T * data;
    LinkedList<T> * next;
public:
    LinkedList (T * data);
    LinkedList ();
    ~LinkedList();
    LinkedList<T>* operator+= (LinkedList<T> * lhs);
    LinkedList<T>& operator+= (T * data);
    LinkedList<T>& operator[] (size_t idx);
    void appendNode(T * data);
    void appendNode(LinkedList * list);
    LinkedList<T> * deleteNode (T * data);
    LinkedList<T> * deleteNode (int id);
    size_t size();
    T * at(size_t idx);
};

#endif /* LINKEDLIST_H */
