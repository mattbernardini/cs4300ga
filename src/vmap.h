/*
 * vmap.h
 *
 *  Created on: Jun 22, 2018
 *      Author: matt
 */
#include "vertex.h"
#ifndef SRC_VMAP_H_
#define SRC_VMAP_H_

namespace AI {
  template<typename T>
  class Vmap {
    public:
      T arrayLocation;
      T * vertexIndex;
      Vmap();
      virtual ~Vmap();
      /*
       * Operator overloads
       */
      bool operator==(Vertex * vertex)
      { return vertex->getIndex() == *vertexIndex; }
  };

} /* namespace AI */

#endif /* SRC_VMAP_H_ */
