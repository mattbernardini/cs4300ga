/*
 * densitymap.h
 *
 *  Created on: Jun 22, 2018
 *      Author: matt
 */

#ifndef SRC_DENSITYMAP_H_
#define SRC_DENSITYMAP_H_

namespace AI {
  template<typename intType, typename floatType>
  class DensityMap {
    public:
      floatType density;
      intType startIndex;
      intType endIndex;
      DensityMap();
      DensityMap(floatType density, intType startIndex, intType endIndex)
      { this->density = density;
        this->startIndex = startIndex;
        this->endIndex = endIndex;
      }
      virtual ~DensityMap();
  };

} /* namespace AI */

#endif /* SRC_DENSITYMAP_H_ */
