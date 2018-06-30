/*
 * Network.cpp
 *
 *  Created on: Mar 27, 2018
 *      Author: matt
 */

#include <cstdio>
#include <vector>
#include <queue>
#include <math.h>
#include <unistd.h>
#include <algorithm>

#include "network.h"
#include "gmlreader.h"
namespace AI {
static const char className[] = "Network";

Network::Network(char * fileName, BaseLogger * logger)
{
	// TODO Auto-generated constructor stub
	this->logger = logger;
	const char r[] = {'r'};
    this->fileName = fileName;
	this->verticies = new std::vector<Vertex *>();
	this->edges = new std::vector<Edge *> ();
	this->vertMatrix = NULL;
	this->clusters = new std::vector<AI::Cluster *>();
	this->inputFile = fopen(fileName, r);
	this->directed = false;
	this->rowSize = NULL;
	this->density = NULL;
	this->gmlWriter = NULL;
	this->readGmlFile();
	this->bfs();
	this->findClusters();
	this->printFinalClusters();
}

Network::~Network() {
	// TODO Auto-generated destructor stub
}
void Network::findClusters()
{	/* Queue all clusters that we found */
	int lastClusterCount = 0;
	std::queue<Cluster *> q;
	do {
	  lastClusterCount = this->clusters->size();
    for (auto cluster : this->clusters)
      q.push(cluster);
    /* Keep queueing each cluster until they cannot be split */
    while (!q.empty())
    { /* Pull the first cluster from the queue */
      Cluster * tmp = q.front();
      q.pop();
      /* See if we can split it */
      if (tmp->shouldClusterBeSplit())
        this->clusters->push_back(tmp->splitCluster());
    }
	} while (lastClusterCount != this->clusters->size());
	for (auto cluster : this->clusters)
		cluster->setVertexClusterId();
	fprintf(stderr, "Done making clusters\n");
}

void Network::bfs(AI::Cluster * clusters)
{ std::vector<Vertex *> visitied;
	std::queue<Vertex *> q;
	for (auto vertex : clusters->getVerticies())
	{ /* If we have already visited this point ignore it */
		if (std::find(visitied.begin(), visitied.end(), vertex) == visitied.back() && vertex != visitied.back())
		{ q.push(vertex);
			/* Else we loop through the edges and make the graph */
			AI::Cluster * cluster = new AI::Cluster(this->logger);
			while (!q.empty())
			{ Vertex * tmp = q.front();
				q.pop();
				if (std::find(visitied.begin(), visitied.end(), tmp) == visitied.back() && tmp != visitied.back())
				{ visitied.push_back(tmp);
					cluster->addVertex(tmp);
					for (auto edge : tmp->getEdges())
					{ cluster->addEdge(edge);
						q.push(edge->getOther(tmp));
					}
				}
			}
			this->clusters->push_back(cluster);
		}
	}
  for (auto cluster : this->clusters)
  { fprintf(stderr, "New cluster %i\n", cluster->id);
    for (auto vertex : cluster->getVerticies())
      fprintf(stderr, "%u\n", static_cast<unsigned int>(vertex->getIndex()));
  }
}
void Network::bfs()
{ std::vector<Vertex *> visitied;
	std::queue<Vertex *> q;
	for (std::vector<Vertex *>::iterator it = this->verticies->begin();
			it != this->verticies->end(); ++it)
	{ /* If we have already visited this point ignore it */
		if (this->findVertexPtr(&visitied, (*it)->getIndex()) == NULL)
		{	q.push((*it));
			/* Else we loop through the edges and make the graph */
			AI::Cluster * cluster = new AI::Cluster(this->logger);
			while (!q.empty())
			{	Vertex * tmp = q.front();
				q.pop();
				if (this->findVertexPtr(&visitied, tmp->getIndex()) == NULL)
				{	visitied.push_back(tmp);
					cluster->addVertex(tmp);
					for (std::vector<Edge *>::iterator eit = tmp->getEdges()->begin();
							eit != tmp->getEdges()->end(); ++eit)
					{	cluster->addEdge((*eit));
						Vertex * o = (*eit)->getSource()->getIndex() == tmp->getIndex() ?
								(*eit)->getTarget() :
								(*eit)->getSource();
						q.push(o);
					}
				}
			}
			this->clusters->push_back(cluster);
		}
	}
	for (unsigned int i = 0; i < this->clusters->size(); i++)
	{ fprintf(stderr, "New cluster %i\n", i);
		for (std::vector<Vertex *>::iterator it = (this->clusters->at(i))->getVerticies()->begin();
					it !=  (this->clusters->at(i))->getVerticies()->end(); ++it)
		{ fprintf(stderr, "%u\n", static_cast<unsigned int>((*it)->getIndex()));
		}
	}
}
void Network::clearOutClusters()
{ std::queue<Cluster *> q;
	for (unsigned int i = 0; i < this->clusters->size(); i++)
	{ if (this->clusters->at(i)->getVerticies()->size() > 0 ||
				!this->clusters->at(i)->getVerticies()->empty())
		{
		}
		else
			q.push(this->clusters->at(i));
	}
	while (!q.empty())
	{ Cluster * tmp = q.front();
		this->removeCluster(tmp);
		free(tmp);
	}
}
void Network::removeCluster(Cluster * cluster)
{ int counter = 0;
	for (std::vector<Cluster *>::iterator it = this->clusters->begin();
			it != this->clusters->end(); it++, counter++)
	{ if (cluster->getId() == (*it)->getId())
		{ this->clusters->erase(this->clusters->begin() + counter);
			return;
		}
	}
}
void Network::printFinalClusters()
{	for (unsigned int i = 0; i < this->clusters->size(); i++)
	{
		if (this->clusters->at(i)->getVerticies()->size() > 0 ||
				!this->clusters->at(i)->getVerticies()->empty())
		{
			this->clusters->at(i)->logClusterState();
		}
	}
	fprintf(this->logger->logFile, "\n\n");
	for (int i = 0; i < this->verticies->size(); i++)
		fprintf(this->logger->logFile, "%d", this->verticies->at(i)->getClusterId());
}
} /* namespace AI */
