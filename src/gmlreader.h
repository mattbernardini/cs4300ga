/*
 * gmlreader.h
 *
 *  Created on: Jun 22, 2018
 *      Author: matt
 */
#include <string>
#include <cstdio>
#include <algorithm>

#include "network_inbetween.h"

#ifndef SRC_GMLREADER_H_
#define SRC_GMLREADER_H_

namespace AI {

class GmlReader {
public:
	GmlReader(std::string fileName);
	virtual ~GmlReader();
	static Inbetween readGmlFile (std::string fileName)
	{	char string[50];
		FILE * gmlFile = fopen(fileName.c_str(), "r");
		Inbetween rtValue;
		Vertex * source, * target;
		float weight;
		/* Clear out beginning of file */
		while (1)
		{	/* If we hit the end of the file log and exit */
			if(feof(gmlFile))
				exit(-1);
			fscanf(gmlFile, "%s", string);
			/* Stop when we get to graph declaration */
			if(strncmp(string, "graph", 5) == 0)
				break;
		}
		/* Read in vertexes */
		while(1)
		{ /* If we hit the end of the file no edges */
			if (feof(gmlFile))
				break;
			/* Scan in next string */
			fscanf(gmlFile, "%s", string);
			/* Check to see if we have a node id */
			if (strncmp(string, "id", 2) == 0)
			{ /* If we did create new vertex and add to vector */
				fscanf(gmlFile, "%s", string);
				rtValue->verticies.push_front(new Vertex(static_cast<unsigned int>(atoi(string))));
			}
			/* Check to see if we made it to edge declarations */
			if(strncmp(string, "edge", 4) == 0)
				break; // stop when read in first "edge"
		}
		while (!feof(gmlFile))
		{	fscanf(gmlFile, "%s", string); // read in string from file
			if(!strncmp(string, "[", 1) == 0) // check for correct character
				break;
			fscanf(gmlFile, "%s", string); // read in string from file
			if(!strncmp(string, "source", 6) == 0) // check for correct character
				exit(-1);
			fscanf(gmlFile, "%s", string); // read in start node from file
			source = std::find(rtValue->verticies.begin(), rtValue->verticies.end(), atoi(string));
			fscanf(gmlFile, "%s", string); // read in string from file
			if(!strncmp(string, "target", 6) == 0) // check for correct character
			  exit(-1);
			fscanf(gmlFile, "%s", string); // read in target node from file
			target = std::find(rtValue->verticies.begin(), rtValue->verticies.end(), atoi(string));
			fscanf(gmlFile, "%s", string); // read in string from file
			weight = 1.0;
			if(strncmp(string, "weight", 6) == 0)
			{ // check to see if weight specified
			  fscanf(gmlFile, "%s", string); // read in string from file
			  weight = atof(string);
			  fscanf(gmlFile, "%s", string); // read in string from file
			  fscanf(gmlFile, "%s", string); // read in string from file
			}
			else
			  fscanf(gmlFile, "%s", string);
		  rtValue->edges.push_front(new Edge(source, target, weight));
		}
		return rtValue;
	}
};

} /* namespace AI */

#endif /* SRC_GMLREADER_H_ */
