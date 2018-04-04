#include <vector>
#include "person.h"
#include "linkedlist.h"
#ifndef GA_DRIVER_H
#define GA_DRIVER_H
namespace AI
{
class GaDriver
{
private:
    LinkedList<Person> * population;
    int popSize;
    int stringSize;
    LinkedList<Person> * recombine(Person * p1, Person * p2);
    Person * getParent();
    void generateInitialChildren();
public:
    GaDriver(int intitialPopSize, int stringSize);
    ~GaDriver();
    void generateNextGen();
};
}

#endif /* end of include guard: GA_DRIVER_H */
