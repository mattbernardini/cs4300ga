#include "ga_driver.h"
namespace AI
{

GaDriver::GaDriver(int intitialPopSize, int stringSize)
{
    this->popSize = intitialPopSize;
    this->stringSize = stringSize;
    this->population = new LinkedList<Person>();
    this->generateInitialChildren();
}
void GaDriver::generateInitialChildren()
{
    for (size_t i = 0; i < this->popSize; i++)
        this->population->appendNode(new Person(this->stringSize));
}
Person * GaDriver::getParent()
{
    int p1 = rand() % this->population->size();
    int p2 = rand() % this->population->size();
    return this->population->at(p1)->getFitness() >= this->population->at(p2)->getFitness() ?
        this->population->at(p1) : this->population->at(p2);
}
LinkedList<Person> * GaDriver::recombine(Person * p1, Person * p2)
{
    int c = (rand() % 10 - 1) + 1;
    if (c > 6)
    {
        LinkedList<Person> * ll = new LinkedList<Person>(new Person(p1)); 
        *ll += new LinkedList<Person>(new Person(p1)); 
        return ll;
    }
    else
    {
        // We are recombining the parents
        Person * c1 = new Person(this->stringSize);
        Person * c2 = new Person(this->stringSize);
        for (size_t i = 0; i < this->stringSize; i++)
        {
            if (rand() % 2 == 0)
            {
                c2->bitString[i] = p1->bitString[i];
                c1->bitString[i] = p2->bitString[i];
            }
            else
            {
                c2->bitString[i] = p2->bitString[i];
                c1->bitString[i] = p1->bitString[i];
            }
        }
        LinkedList<Person> * ll = new LinkedList<Person>(c1);
        ll->appendNode(c2);
        free(c1);
        free(c2);
        return ll;
    }
}
void GaDriver::generateNextGen()
{
    LinkedList<Person> * nextGen = new LinkedList<Person>();
    for (size_t i = 0; i < (this->popSize - 1) / 2; i++)
    {
        *nextGen += this->recombine(this->getParent(), this->getParent());
    }
    
}
}
