#include "person.h"
#include <stdlib.h>
namespace AI
{
static int pid = 0;
Person::Person(int stringSize)
{
    this->id = pid++;
    this->stringSize = stringSize;
    this->bitString = (int*)malloc(sizeof(int) * stringSize);
    this->generateBitString();
}
Person::Person(Person * p)
{
    this->id = pid++;
    this->stringSize = p->stringSize;
    this->bitString = (int*)malloc(sizeof(int) * stringSize);
    for (size_t i = 0; i < this->stringSize; i++)
        this->bitString[i] = p->bitString[i];
}
bool Person::operator== (Person * lhs)
{
    return this->id == lhs->id;
}
void Person::generateBitString()
{
    for (size_t i = 0; i < this->stringSize; i++)
        this->bitString[i] = rand() % 2;
}
int Person::getFitness()
{
    int rt = 0;
    for (size_t i = 0; i < this->stringSize; i++)
        rt += this->bitString[i];
    return rt;
}
}
