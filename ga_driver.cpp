#include "ga_driver.h"
#include <stdlib.h>
#include <stdio.h>
namespace AI
{

GaDriver::GaDriver(int intitialPopSize, int stringSize)
{
    this->popSize = intitialPopSize;
    this->stringSize = stringSize;
    this->itteration = 0;
    this->avgFitness = 0;
    this->population = new std::vector<Person *>();
    this->generateInitialChildren();
    this->printCurrentGeneration();
}
void GaDriver::printCurrentGeneration()
{
	fprintf(stderr, "Generation %i\n", this->itteration);
	for (int i = 0; i < this->population->size(); i++)
	{
		fprintf(stderr, "Parent %i, ID: %i\n", i, this->population->at(i)->id);
		for (int j = 0; j < this->stringSize; j++)
		{
			fprintf(stderr, "%i", this->population->at(i)->bitString[j]);
		}
		fprintf(stderr, "\n");
	}
}
double GaDriver::getAvgFitness()
{
	double num = 0;
	for (int i = 0; i < this->population->size(); i++)
	{
		num += this->population->at(i)->getFitness();
	}
	return num / (double) this->population->size();
}
Person * GaDriver::getMaxFitness()
{
	int num = 0;
	Person * p;
	for (int i = 0; i < this->population->size(); i++)
	{
		num = this->population->at(i)->getFitness() > num ?
				this->population->at(i)->getFitness(), p = this->population->at(i) : num;
	}
	return p;
}
Person * GaDriver::getLowestFitness()
{
	int num = this->stringSize;
	Person * p;
	for (int i = 0; i < this->population->size(); i++)
	{
		num = this->population->at(i)->getFitness() < num ?
				this->population->at(i)->getFitness(), p = this->population->at(i) : num;
	}
	return p;
}
void GaDriver::generateInitialChildren()
{
    for (size_t i = 0; i < this->popSize; i++)
        this->population->push_back(new Person(this->stringSize));
}
Person * GaDriver::getParent()
{
    int p1 = rand() % this->population->size();
    int p2 = rand() % this->population->size();
    return this->population->at(p1)->getFitness() >= this->population->at(p2)->getFitness() ?
        this->population->at(p1) : this->population->at(p2);
}
std::vector<Person *> * GaDriver::recombine(Person * p1, Person * p2)
{
    int c = (rand() % 10 - 1) + 1;
    Person * c1 = new Person(p1), * c2 = new Person(p2);
    std::vector<Person *> * ll = new std::vector<Person *> ();
    if (c > 6)
    {
    	ll->push_back(c1);
        ll->push_back(c2);
        return ll;
    }
    else
    {
        // We are recombining the parents
#pragma omp parallel for
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
        ll->push_back(c1);
        ll->push_back(c2);
        return ll;
    }
}
std::vector<Person *> * GaDriver::findBestParents()
{
	std::vector<Person *> * bestParents = new std::vector<Person *>();
	Person * p1 = this->population->at(0), * p2 = this->population->at(1);
	for (int i = 0; i < this->population->size(); i++)
	{
		if (this->population->at(i)->getFitness() > p1->getFitness() &&
				(this->population->at(i)->id != p1->id && this->population->at(i)->id != p2->id))
		{
			p1 = this->population->at(i);
		}
		else
		{
			if (this->population->at(i)->getFitness() > p2->getFitness() &&
					(this->population->at(i)->id != p1->id && this->population->at(i)->id != p2->id))
			{
				p2 = this->population->at(i);
			}
		}
	}
	bestParents->push_back(p1);
	bestParents->push_back(p2);
	return bestParents;
}
void GaDriver::generateNextGen()
{
	std::vector<Person *> * nextGen = new std::vector<Person *>();
	std::vector<Person *> * tmp;
    for (size_t i = 0; i < (this->popSize - 1) / 2; i++)
    {
    	tmp = this->recombine(this->getParent(), this->getParent());
    	nextGen->push_back(tmp->at(0));
    	nextGen->push_back(tmp->at(1));
    	free(tmp);
    }
    std::vector<Person *> * parents = this->findBestParents();
    nextGen->push_back(parents->front());
    nextGen->push_back(parents->back());
    for (int i = 0; i < this->population->size(); i++)
    {
    	nextGen->at(i)->mutate();
    	Person * tmp = this->population->at(i);
    	if (!(tmp->id == parents->front()->id || tmp->id == parents->back()->id))
    		free(tmp);
    }
    this->population = nextGen;
    this->itteration++;
    this->printCurrentGeneration();
    free(parents);
}
}
