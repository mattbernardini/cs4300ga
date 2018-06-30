/*
 * network_inbetween.h
 *
 *  Created on: Jun 22, 2018
 *      Author: matt
 */
#include <forward_list>

#include "edge.h"
#include "vertex.h"

#ifndef SRC_NETWORK_INBETWEEN_H_
#define SRC_NETWORK_INBETWEEN_H_

struct _inbetween {
	std::forward_list<AI::Edge *> edges;
	std::forward_list<AI::Vertex *> verticies;
};

typedef struct _inbetween * Inbetween;

#endif /* SRC_NETWORK_INBETWEEN_H_ */
