#include <vector>
#include "person.h"
#ifndef GA_DRIVER_H
#define GA_DRIVER_H
namespace AI
{
class GaDriver
{
private:
    std::vector<Person *> * population;
    int popSize;
    int stringSize;
    int itteration;
    double avgFitness;
    std::vector<Person *> * recombine(Person * p1, Person * p2);
    Person * getParent();
    void generateInitialChildren();
    void printCurrentGeneration();
    double getAvgFitness();
    Person * getMaxFitness();
    Person * getLowestFitness();
    std::vector<Person *> * findBestParents();

public:
    GaDriver(int intitialPopSize, int stringSize);
    ~GaDriver();
    void generateNextGen();
};
}

#endif /* end of include guard: GA_DRIVER_H */
