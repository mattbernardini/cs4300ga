#include "ga_driver.h"
#include "linkedlist.h"
using namespace AI;

int main (int argc, char ** argv)
{
	GaDriver * ga = new GaDriver(6, 5);
	ga->generateNextGen();
    return 0;
}
