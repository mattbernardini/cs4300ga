/*
 * cluster.h
 *
 *  Created on: Mar 31, 2018
 *      Author: matt
 */
#include "Vertex.h"
#include <map>
#include <set>

#include "edge.h"
#include "vmap.h"
#include "densitymap.h"
#ifndef BFS_CLUSTER_H_
#define BFS_CLUSTER_H_
namespace AI {
/*
 *
 */
template<typename intType, typename floatType>
class Cluster {
private:
	/*
	 * Private variables
	 */
	std::set<AI::Vertex *> * verticies;
	std::set<AI::Edge *> * edges;
	std::vector<Vmap<intType>> * vmap;
	std::vector<floatType> * density;
  std::vector<DensityMap<floatType, intType>> * densities;
	std::vector<floatType> * vertMatrix;
	floatType baseDensity;
	int mapNum;
	int id;
	bool directed;
	/*
	 * Private functions
	 */
	floatType findBaseDensity();
	floatType findDensities();
	void buildArray();
	AI::Vertex * findNextVertex(std::vector<AI::Vertex *> * currentState,
			std::vector<AI::Vertex *> * currentList);
	Vertex * findVertex(int index);
	void orderVertexs();
	void removeEdge(Edge * edge);
	void removeVertex(Vertex * vert);
	void removeVertex(std::vector<Vertex * > * v, Vertex * vert);
	void redoVmap ();
	void checkEdges();
	bool doesVertexExist(Vertex * vert);
	bool doesVertexExist(std::vector<AI::Vertex *> * v, Vertex * vert);
	void cleanEdges();
	bool doesEdgeExist(Edge * edge);
	bool doesEdgeExist(std::vector<Edge *> * v, Edge * edge);
	void removeDuplicateEdges();
public:
	/*
	 * Getters
	 * All values are const so that they do not allow changes
	 * All changes must happen through setters/adders
	 */
	std::vector<AI::Edge *> * getEdges(){return this->edges;};
	std::vector<AI::Vertex *> * getVerticies(){return this->verticies;};
	const int getId() {return this->id;};
	/*
	 * Setters/adders
	 */
	void addVertex(AI::Vertex * vert);
	void addEdge(AI::Edge * edge);

	Cluster();
	bool shouldClusterBeSplit();
	AI::Cluster * splitCluster();
	virtual ~Cluster();
	void logClusterState();
	void setVertexClusterId();
};

} /* namespace AI */

#endif /* BFS_CLUSTER_H_ */
