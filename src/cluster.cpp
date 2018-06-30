/*
 * cluster.cpp
 *
 *  Created on: Mar 31, 2018
 *      Author: matt
 */

#include "cluster.h"
#include <math.h>
#include <queue>
#include <stdlib.h>
#include <set>
namespace AI {
static int clusterId = 0;
static const char className[] = "Cluster";
template<typename intType, typename floatType>
Cluster<typename intType, typename floatType>::Cluster()
{ this->verticies = new std::set<AI::Vertex *>();
	this->edges = new std::set<AI::Edge *>();
	this->vmap = new std::vector<Vmap<intType>>();
	this->directed = false;
	this->density = NULL;
	this->vertMatrix = NULL;
	this->baseDensity = 0;
	this->mapNum = 0;
	this->id = clusterId++;
}
template <typename intType, typename floatType>
Cluster<typename intType, typename floatType>::~Cluster() {
    this->edges->clear();
    this->verticies->clear();
    delete this->verticies;
    delete this->edges;
}

template <typename intType, typename floatType>
void Cluster<typename intType, typename floatType>::logClusterState()
{
	this->cleanEdges();
	char buffer[4096], tmp[16];
	sprintf(tmp, "%i", this->id);
	strcat(tmp, ".gml");
	this->gmlWriter = new GmlWriter(tmp);
	fprintf(stderr, "Opened gml file %s\n", tmp);
	memset(buffer, '\0', sizeof(char) * 512);
	memset(tmp, '\0', sizeof(char) * 16);
	strcat(buffer, "Vertex: ");
	for (std::vector<Vertex *>::iterator it = this->verticies->begin();
			it != this->verticies->end(); ++it)
	{
		this->gmlWriter->writeNode(static_cast<int>((*it)->getIndex()));
		//sprintf(tmp, "%4i ", static_cast<int>((*it)->getIndex()));
		//strcat(buffer, tmp);
		//memset(tmp, '\0', sizeof(char) * 16);
	}
	strcat(buffer, "\n\t\t\t\t\t\tEdges: ");
	for (std::vector<Edge *>::iterator it = this->edges->begin();
			it != this->edges->end(); ++it)
	{
		this->gmlWriter->writeEdge(static_cast<int>((*it)->getSource()->getIndex()),
				static_cast<int>((*it)->getTarget()->getIndex()), (*it)->getWeight());
		//sprintf(tmp, "%u-%u\t", (*it)->getSource()->getIndex(), (*it)->getTarget()->getIndex());
		//strcat(buffer, tmp);
	}
	this->gmlWriter->writeEnd();
    this->gmlWriter->~GmlWriter();
	//this->logger->writeToLogFile(INFO, "%s %i: %s",
	//		className, this->id, buffer);
}

template <typename intType, typename floatType>
bool Cluster<typename intType, typename floatType>::doesVertexExist(Vertex * vert)
{ for (auto vertex : this->verticies)
		if (vert == vertex)
			return true;
	return false;
}

template <typename intType, typename floatType>
bool Cluster<typename intType, typename floatType>::doesVertexExist(std::vector<AI::Vertex *> * v, Vertex * vert)
{ for (auto vertex : v)
		if (vert == vertex)
			return true;
	return false;
}

template <typename intType, typename floatType>
bool Cluster<typename intType, typename floatType>::doesEdgeExist(Edge * edge)
{ for (auto _edge : this->edges)
		if (edge == _edge)
			return true;
	return false;
}

template <typename intType, typename floatType>
bool Cluster<typename intType, typename floatType>::doesEdgeExist(std::vector<Edge *> * edges, Edge * edge)
{ for (auto _edge : edges)
		if (edge == _edge)
			return true;
	return false;
}

template <typename intType, typename floatType>
void Cluster<typename intType, typename floatType>::removeDuplicateEdges()
{ std::queue<Edge *> p;
	std::vector<Edge *> seen;
	for (auto edge : this->edges)
		if (!doesEdgeExist(&seen, edge))
			seen.push_back(edge);
		else
			p.push(edge);
	while (!p.empty())
	{
		Edge * tmp = p.front();
		p.pop();
		this->removeEdge(tmp);
	}
}

template <typename intType, typename floatType>
void Cluster<typename intType, typename floatType>::checkEdges()
{ std::queue<Edge *> p;
	std::vector<Edge *> t;
	for (std::vector<Edge *>::iterator it = this->edges->begin();
			it != this->edges->end(); it++)
	{
		if (!this->doesVertexExist((*it)->getTarget()) ||
				!this->doesVertexExist((*it)->getSource()))
		{
			p.push((*it));
			t.push_back((*it));
		}
	}
	while (!p.empty())
	{
		Edge * tmp = p.front();
		p.pop();
		this->removeEdge(tmp);
	}
	this->removeDuplicateEdges();
}

template <typename intType, typename floatType>
void Cluster<typename intType, typename floatType>::redoVmap ()
{ this->mapNum = 0;
	for (auto v : this->vmap)
	  v.arrayLocation = mapNum++;
}

template <typename intType, typename floatType>
void Cluster<typename intType, typename floatType>::addVertex(AI::Vertex * vert)
{ this->verticies->insert(vert);
  Vmap<intType> t = { .arrayLocation = 0, .vertexIndex = &vert->index };
  this->vmap->push_back(t);
}

template <typename intType, typename floatType>
void Cluster<typename intType, typename floatType>::addEdge(AI::Edge * edge)
{ this->edges->insert(edge);
}

template <typename intType, typename floatType>
void Cluster<typename intType, typename floatType>::buildArray()
{ int s = pow(this->verticies->size(), 2);
	if (!this->vertMatrix)
	  delete this->vertMatrix;
	this->vertMatrix = new std::vector<floatType>(s);
	for (Edge * edge : this->edges)
	{ int sourceIndex = std::find(this->vmap->begin(), this->vmap->end(), edge->getSource())->arrayLocation;
		int targetIndex = std::find(this->vmap->begin(), this->vmap->end(), edge->getTarget())->arrayLocation;
		this->vertMatrix[(sourceIndex * this->verticies->size()) + targetIndex] = edge->getWeight();
		if (!directed)
		  this->vertMatrix[(targetIndex * this->verticies->size()) + sourceIndex] = edge->getWeight();
	}
}

template <typename intType, typename floatType>
floatType Cluster<typename intType, typename floatType>::findBaseDensity()
{ this->buildArray();
	int s = this->verticies->size();
	floatType numerator = 0,
			denominator = pow(this->verticies->size(), 2);
	// Loop controls finding numerator
	for (int j = 0; j < s; j++)
		for (int k = 0; k < s; k++)
			numerator += this->vertMatrix[(s * j) + k];
	return numerator / denominator;
}

template <typename intType, typename floatType>
void Cluster<typename intType, typename floatType>::setVertexClusterId()
{ for (auto vertex : this->verticies)
		vertex->setClusterId(this->id);
}

template <typename intType, typename floatType>
bool Cluster<typename intType, typename floatType>::shouldClusterBeSplit()
{ /* Make sure we are ready to calculate this */
	this->density = this->findDensities();
	for (unsigned int i = 0; i < this->verticies->size() * 2; i++)
		if (this->density[i] > this->baseDensity)
			return true;
	return false;
}
Vertex * Cluster::findVertex(int index)
{
	for (std::vector<Vertex *>::iterator it = this->verticies->begin();
			it != this->verticies->end(); ++it)
	{
		if ((*it)->getIndex() == static_cast<unsigned int>(index))
		{
			return (*it);
		}
	}
	return NULL;
}
void Cluster::removeEdge(Edge * edge)
{
	int counter = 0;
	for (std::vector<Edge *>::iterator it = this->edges->begin();
			it != this->edges->end(); it++, counter++)
	{
		if (edge->getId() == (*it)->getId())
		{
			this->edges->erase(this->edges->begin() + counter);
			return;
		}
	}
}
void Cluster::removeVertex(Vertex * vert)
{
	int counter = 0;
	for (std::vector<Vertex *>::iterator it = this->verticies->begin();
			it != this->verticies->end(); it++, counter++)
	{
		if (vert->getIndex() == (*it)->getIndex())
		{
			this->verticies->erase(this->verticies->begin() + counter);
			return;
		}
	}
}
void Cluster::removeVertex(std::vector<Vertex * > * v, Vertex * vert)
{
	int counter = 0;
	for (std::vector<Vertex *>::iterator it = v->begin();
			it != v->end(); it++, counter++)
	{
		if (vert->getIndex() == (*it)->getIndex())
		{
			v->erase(v->begin() + counter);
			return;
		}
	}
}
void Cluster::cleanEdges()
{
	std::queue<Edge *> q;
	for (std::vector<Edge *>::iterator it = this->edges->begin();
			it != this->edges->end(); it++)
	{
		q.push((*it));
	}
	while (!q.empty())
	{
		Edge * tmp = q.front();
		q.pop();
		if ((this->findVertex(static_cast<int>(tmp->getSource()->getIndex())) == NULL) ||
				this->findVertex(static_cast<int>(tmp->getSource()->getIndex())) == NULL)
		{
			this->removeEdge(tmp);
		}
	}
}
template <typename intType, typename floatType>
AI::Cluster * Cluster<typename intType, typename floatType>::splitCluster()
{ floatType maxDensity = std::max_element(this->density->begin(), this->density->end());
  intType location = 0;
  AI::Cluster * newCluster = new Cluster<intType, floatType>();
  for (auto d : this->density)
    if (d == maxDensity)
      break;
    else
      location++;
  std::move ()
}

template <typename intType, typename floatType>
floatType Cluster<typename intType, typename floatType>::findDensities()
{ /* Make sure we are setup properly */
	this->baseDensity = this->findBaseDensity();
	int s = this->verticies->size(), total;
	if (!this->density)
	  delete this->density;
	for (int i = 1; i <= s; i++)
	  total += i;
	this->density = new std::vector<floatType>(total);
	for (int i = 0; i < s; i++)
	{ /* Local variables to each iteration */
	  floatType numerator = 0, numerator2 = 0,
				denominator = (i +1) * (i + 1), denominator2 = (i +1) * (i + 1);
		// Loop controls finding numerator
		for (int j = 0; j <= i; j++)
		{ for (int k = j; k <= i; k++)
			{ numerator += this->vertMatrix[(s * j) + k];
				if (!directed)
				  if ((s * (s - j) + (s - k)) < s &&
                (s * (s-j)) + (s - k) >= 0)
				    numerator2 += this->vertMatrix[(s * (s - j)) + (s - k)];
			}
		  for (int k = i + 1; k < s; k++)
      { if (this->vertMatrix[(s * j) + k] != 0)
          denominator += this->vertMatrix[(s * j) + k];
        if (!directed)
          if ((s * (s - j) + (s - k)) < s &&
              (s * (s-j)) + (s - k) >= 0 &&
              this->vertMatrix[(s * (s - j)) + (s - k)] != 0 )
            denominator2 += this->vertMatrix[(s * (s - j)) + (s - k)];
      }
		}
		// Loop through and build denominator
		for (int j = 0; j <= i; j++)
		{ for (int k = i + 1; k < s; k++)
			{ if (this->vertMatrix[(s * j) + k] != 0)
					denominator += this->vertMatrix[(s * j) + k];
				if (!directed)
				  if ((s * (s - j) + (s - k)) < s &&
              (s * (s-j)) + (s - k) >= 0 &&
              this->vertMatrix[(s * (s - j)) + (s - k)] != 0 )
				    denominator2 += this->vertMatrix[(s * (s - j)) + (s - k)];
			}
		}
		this->densities->push_back(new DensityMap())
		density[i] = numerator / denominator;
		if (!directed)
		  density[s + i] = numerator2 / denominator2;
	}
}
} /* namespace AI */
