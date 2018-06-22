#include "ga_driver.h"
#include "linkedlist.h"

using namespace AI;

int main (int argc, char ** argv)
{
	int popSize = 10, timesSucceeded = 0, lastPopSize = 0, high = 0, low = 0, stringSize;
	fprintf(stdout, "Please enter string size:");
	fscanf(stdin, "%i", &stringSize);
	BaseLogger * m = new BaseLogger("master.log");
	BaseLogger * population = new BaseLogger("population.log");
	BaseLogger * mutation = new BaseLogger("mutation.log");
	bool haveSucceeded = false, haveFailed = false;
	while(1)
	{
		timesSucceeded = 0;
		if (popSize >= 81920)
			break;
		fprintf(stderr, "Generating next population with size %i, string size %i\n",
				popSize, stringSize);
		m->writeToLogFile(INFO, "Generating next population with size %i, string size %i",
				popSize, stringSize);
#pragma omp parallel for
		for (int i = 0; i < 5; i++)
		{
			GaDriver * ga = new GaDriver(popSize, stringSize, m, population, mutation);
			while (ga->shouldContinue())
				ga->generateNextGen();
			if (ga->foundOptimum())
			{
				m->writeToLogFile(INFO, "Found global optimum on itteration %i", i);
				fprintf(stderr, "max\n");
				timesSucceeded++;
			}
			delete ga;
		}
		if (timesSucceeded == 5)
		{
			haveSucceeded = true;
			if (!haveFailed)
			{
				m->writeToLogFile(INFO, "Increasing population size from %i to %i",
						popSize, popSize *2);
				high = popSize;
				lastPopSize = popSize;
				popSize = popSize * 2;
			}
			else
			{
				m->writeToLogFile(INFO, "Succeeded last time, Increasing population size from %i to %i",
						popSize, (popSize + low) / 2);
				high = popSize;
				lastPopSize = popSize;
				popSize = (high + low) / 2;
			}
		}
		else
		{
			haveFailed = true;
			if (haveSucceeded)
			{
				m->writeToLogFile(INFO, "Increasing population size from %i to %i",
										popSize, (high + low) / 2);
				popSize = (high + low) / 2;
				lastPopSize = popSize;
				low = popSize;
			}
			else
			{
				m->writeToLogFile(INFO, "Increasing population size from %i to %i",
						popSize, popSize *2);
				low = popSize;
				lastPopSize = popSize;
				popSize = popSize * 2;
			}
		}
		if (popSize == lastPopSize)
		{
			m->writeToLogFile(INFO, "Exiting after finding population size %i for string size %i",
					popSize, stringSize);
			fprintf(stderr, "Exiting after finding population size %i for string size %i\n",
					popSize, stringSize);
			exit(1);
		}
	}
	if (timesSucceeded == 5)
	{

	}
	else
		m->writeToLogFile(INFO, "Exiting after reaching population limit");
    return 0;
}
