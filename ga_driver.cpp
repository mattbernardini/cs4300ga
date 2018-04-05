#include "ga_driver.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
namespace AI
{

GaDriver::GaDriver(int intitialPopSize, int stringSize, BaseLogger * master, BaseLogger * pop, BaseLogger * mut)
{
    this->popSize = intitialPopSize;
    this->stringSize = stringSize;
    this->itteration = 0;
    this->avgFitness = 0;
    this->timesAvgFitHasntChanged = 0;
    this->foundGlobalOptimum = false;
    this->population = new std::vector<Person *>();
    this->master = master;
    this->pop = pop;
    this->mutation = mut;
    this->generateInitialChildren();
    this->printCurrentGeneration();
}
GaDriver::~GaDriver()
{
    for (int i = 0; i < this->population->size(); i++)
    {
    	Person * tmp = this->population->at(i);
		free(tmp);
    }
    free(this->population);
}
void GaDriver::printCurrentGeneration()
{
	double avgFit = this->getAvgFitness();
	Person * low = this->getLowestFitness(), * high = this->getMaxFitness();
	if (high->getFitness() == this->stringSize)
	{
		this->foundGlobalOptimum = true;
		return;
	}
	if (avgFit <= this->avgFitness)
		this->timesAvgFitHasntChanged++;
	else
		this->timesAvgFitHasntChanged = 0, this->avgFitness = avgFit;
	if (this->timesAvgFitHasntChanged == 3)
		return;

	this->master->writeToLogFile(INFO, "Generation %i\n", this->itteration);
	this->pop->writeToLogFile(INFO, "Generation %i\n", this->itteration);
	this->mutation->writeToLogFile(INFO, "Generation %i\n", this->itteration);
	fprintf(stderr, "Generation %i\n", this->itteration);
	fprintf(stderr, "Average fitness: %e\nMax fitness: %i\nLow fitness: %i\n",
			avgFit, high->getFitness(), low->getFitness());
	this->master->writeToLogFile(INFO, "Average fitness: %e\nMax fitness: %i\nLow fitness: %i\n",
			avgFit, high->getFitness(), low->getFitness());
	for (int i = 0; i < this->population->size(); i++)
	{
		char buffer[4096];
		memset(buffer, '\0', sizeof(char) * 4096);
		for (int j = 0; j < this->stringSize; j++)
		{
			char tmp[3];
			sprintf(tmp, "%i", this->population->at(i)->bitString[j]);
			strcat(buffer, tmp);
		}
		this->pop->writeToLogFile(INFO, "Parent %i, ID: %i\n\t\t\t%s", i, this->population->at(i)->id, buffer);
	}
}
bool GaDriver::shouldContinue()
{
	if (this->timesAvgFitHasntChanged == 3 || this->foundGlobalOptimum)
		return false;
	return true;
}
bool GaDriver::foundOptimum()
{
	return this->foundGlobalOptimum;
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
		if (this->population->at(i)->getFitness() > num)
		{
			num = this->population->at(i)->getFitness();
			p = this->population->at(i);
		}
	}
	return p;
}
Person * GaDriver::getLowestFitness()
{
	int num = this->stringSize;
	Person * p;
	for (int i = 0; i < this->population->size(); i++)
	{
		if (this->population->at(i)->getFitness() < num)
		{
			num = this->population->at(i)->getFitness();
			p = this->population->at(i);
		}
	}
	return p;
}
void GaDriver::generateInitialChildren()
{
    for (size_t i = 0; i < this->popSize; i++)
        this->population->push_back(new Person(this->stringSize, this->mutation));
}
Person * GaDriver::getParent()
{
    int p1 = rand() % this->population->size();
    int p2 = rand() % this->population->size();
    return this->population->at(p1)->getFitness() > this->population->at(p2)->getFitness() ?
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
    for (int i = 0; i < (this->popSize - 1) / 2; i++)
    {
    	tmp = this->recombine(this->getParent(), this->getParent());
    	nextGen->push_back(tmp->at(0));
    	nextGen->push_back(tmp->at(1));
    	free(tmp);
    }
    std::vector<Person *> * parents = this->findBestParents();
    nextGen->push_back(parents->front());
    nextGen->push_back(parents->back());
    for (int i = 0; i < nextGen->size(); i++)
    	nextGen->at(i)->mutate();
    for (int i = 0; i < this->population->size(); i++)
    {
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
