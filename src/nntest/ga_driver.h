#include <vector>
#include "person.h"
#include "base_logger.h"
#ifndef GA_DRIVER_H
#define GA_DRIVER_H
namespace AI
{
class GaDriver
{
private:
    std::vector<Person *> * population;
	BaseLogger * master, *pop, *mutation;
    int popSize;
    int stringSize;
    int itteration;
    double avgFitness;
    int timesAvgFitHasntChanged;
    bool foundGlobalOptimum;
    std::vector<Person *> * recombine(Person * p1, Person * p2);
    Person * getParent();
    void generateInitialChildren();
    void printCurrentGeneration();
    double getAvgFitness();
    Person * getMaxFitness();
    Person * getLowestFitness();
    std::vector<Person *> * findBestParents();

public:
    GaDriver(int intitialPopSize, int stringSize, BaseLogger * master, BaseLogger * pop, BaseLogger * mut);
    ~GaDriver();
    void generateNextGen();
    bool shouldContinue();
    bool foundOptimum();
};
}

#endif /* end of include guard: GA_DRIVER_H */
