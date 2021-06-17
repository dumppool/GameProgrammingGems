/* Copyright (C) William van der Sterren, 2002. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) William van der Sterren, 2002"
 */

/***************************************************************************  
 *                                                                            
 *     purpose: class to represent ray (part of the 'pie' scan template)
*/


#ifndef _ASE_RAY_H_
#define _ASE_RAY_H_

#if _MSC_VER > 1000
#pragma once
#endif


#include "ase_coordpair.h"


class ASE_Ray 
{ 
  public:
    ASE_Ray();

    void SetRay(unsigned int aSector, const CoordPairList& aTrace); 

  public: // queries
    void GetRayForLocation(const ASE_CoordPair aPosition, CoordPairList& theRayTrace) const;

    bool IsPartOfSector(unsigned int aSector) const;

    bool HasSameRay(const CoordPairList& anotherTrace) const;
  
  private:
    unsigned int  m_Sector;
    CoordPairList m_Trace;
};



#endif // _ASE_RAY_H_