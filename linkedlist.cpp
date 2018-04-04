#include "linkedlist.h"
#include <cstring>
#include <cstdlib>
static int llid = 0;

template <class T>
LinkedList<T>::LinkedList(T * data)
{
    this->id = llid++;
    this->data = (T *)malloc(sizeof(T));
    memcpy(this->data, data, sizeof(T));
    this->next = NULL;
}
template <class T>
LinkedList<T>::LinkedList()
{
    this->id = llid++;
    this->data = NULL;
    this->next = NULL;
}

template <class T>
void LinkedList<T>::appendNode(T * data)
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
void LinkedList<T>::appendNode(LinkedList * node)
{
    if (this->next == NULL)
        this->next = node;
    else
        this->next->appendNode(node);
}

template <class T>
LinkedList<T> * LinkedList<T>::deleteNode(T * data)
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
LinkedList<T> * LinkedList<T>::deleteNode(int id)
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
size_t LinkedList<T>::size()
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
LinkedList<T>& LinkedList<T>::operator[] (size_t idx)
{
    LinkedList<T> * traveler = this;
    for (size_t i = 0; i < idx; i++)
        traveler = traveler->next;
    return traveler;
}
template <class T>
LinkedList<T>* LinkedList<T>::operator+= (LinkedList<T> * lhs)
{
    for (size_t i = 0; i < lhs->size(); i++)
        this->appendNode(lhs[i]);
    return this;
}
template <class T>
LinkedList<T>& LinkedList<T>::operator+= (T * data)
{
    this->appendNode(data);
}
template <class T>
T * LinkedList<T>::at(size_t idx)
{
    LinkedList<T> * traveler = this;
    for (size_t i = 0; i < idx; i++)
        traveler = traveler->next;
    return traveler;
}

