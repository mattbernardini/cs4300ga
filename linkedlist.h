#include <string.h>
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
static int llid = 0;




#endif /* LINKEDLIST_H */
template <class T>
inline LinkedList<T>::LinkedList(T * data)
{
    this->id = llid++;
    this->data = (T *)malloc(sizeof(T));
    memcpy(this->data, data, sizeof(T));
    this->next = NULL;
}
template <class T>
inline LinkedList<T>::LinkedList()
{
    this->id = llid++;
    this->data = NULL;
    this->next = NULL;
}

template <class T>
inline void LinkedList<T>::appendNode(T * data)
{
    if (this->data == NULL)
    {
        this->data = (T *)malloc(sizeof(T));
        memcpy(this->data, data, sizeof(T));
        return;
    }
    if (this->next == NULL)
        this->next = new LinkedList<T>(data);
    else
        this->next->appendNode(data);
}

template <class T>
inline void LinkedList<T>::appendNode(LinkedList * node)
{
    if (this->next == NULL)
        this->next = node;
    else
        this->next->appendNode(node);
}

template <class T>
inline LinkedList<T> * LinkedList<T>::deleteNode(T * data)
{
    if (this->data == data)
    {
        free(this->data);
        return this->next;
    }
    else
        this->next = this->next->deleteNode(data);

}

template <class T>
inline LinkedList<T> * LinkedList<T>::deleteNode(int id)
{
    if (this->id == id)
    {
        free(this->data);
        return this->next;
    }
    else
        this->next = this->next->deleteNode(id);

}
template <class T>
inline size_t LinkedList<T>::size()
{
    size_t counter = 0;
    LinkedList<T> * traveler = this;
    while (traveler != NULL)
    {
        counter++;
        traveler = traveler->next;
    }
    return counter;
}

template <class T>
inline LinkedList<T>& LinkedList<T>::operator[] (size_t idx)
{
    LinkedList<T> * traveler = this;
    for (size_t i = 0; i < idx; i++)
        traveler = traveler->next;
    return traveler;
}
template <class T>
inline LinkedList<T>* LinkedList<T>::operator+= (LinkedList<T> * lhs)
{
    for (size_t i = 0; i < lhs->size(); i++)
        this->appendNode(&lhs[i]);
    return this;
}
template <class T>
inline LinkedList<T>& LinkedList<T>::operator+= (T * data)
{
    this->appendNode(data);
}
template <class T>
inline T * LinkedList<T>::at(size_t idx)
{
    LinkedList<T> * traveler = this;
    for (size_t i = 0; i < idx; i++)
        traveler = traveler->next;
    return traveler->data;
}
