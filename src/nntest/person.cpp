#include "person.h"
#include <stdlib.h>
#include <stdio.h>
namespace AI
{
static int pid = 0;
Person::Person(int stringSize, BaseLogger * mutation)
{
    this->id = pid++;
    this->stringSize = stringSize;
    this->bitString = (int*)malloc(sizeof(int) * stringSize);
    this->mutation = mutation;
    this->generateBitString();
}
Person::Person(Person * p)
{
    this->id = pid++;
    this->stringSize = p->stringSize;
    this->bitString = (int*)malloc(sizeof(int) * stringSize);
    this->mutation = p->mutation;
    for (size_t i = 0; i < this->stringSize; i++)
        this->bitString[i] = p->bitString[i];
}
Person::~Person()
{
	free(this->bitString);
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
void Person::mutate()
{
	for (size_t i = 0; i < this->stringSize; i++)
	{
		if (rand() % this->stringSize == 0)
		{
			this->bitString[i] = this->bitString[i] == 0 ? 1 : 0;
			this->mutation->writeToLogFile(INFO, "Child %i mutated bit location: %i",this->id, i);
		}

	}
}
}
