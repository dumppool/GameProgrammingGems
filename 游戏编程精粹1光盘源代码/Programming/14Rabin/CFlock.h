/* Copyright (C) Steve Rabin, 2000. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Steve Rabin, 2000"
 */
//*********************************************************************
// Name:     CFlock.h
// Purpose:  Class definitions and method prototypes for the flocks
//           themselves.
//*********************************************************************

#ifndef _CFLOCK_H
#define _CFLOCK_H

//
// includes
//

#include "defaults.h"
#include "util.h"

#include "CBoid.h"

//
// class definition
//

class CFlock 
{

   public:

      ///////////////////
      // static variables
      ///////////////////

      // number of flocks

      static int FlockCount;

      // list of flocks

      static CFlock * ListOfFlocks[MaxFlocks];

      ///////////////////////////////
      // constructors and destructors
      ///////////////////////////////

      // Constructor.
      // Creates a new flock.

      CFlock (void);

      // Destructor.

      ~CFlock (void);

      /////////////////////
      // flocking functions
      /////////////////////

      // Update.
      // Updates all members of a flock.

      void Update (float deltaTime);

      //////////////////////
      // rendering functions
      //////////////////////

      void Draw (void);

      //////////////////////////
      // miscellaneous functions
      //////////////////////////

      // AddTo.
      // Adds the indicated boid to the flock.

      void AddTo (CBoid * boid);

      // GetCount.
      // Returns the # of boids in a given flock.

      int GetCount (void);

      // GetFirstMember.
      // Returns a pointer to the first boid 
	  // in a given flock (if any).

      CBoid * GetFirstMember (void);

      // PrintData.
      // Dumps all data describing a given flock.

      void PrintData (void);

      // RemoveFrom.
      // Removes the indicated boid from the flock.

      void RemoveFrom (CBoid * boid);

   private:

      int     m_id;                    // id of this flock

      int     m_num_members;           // number of boids in this flock

      CBoid   *m_first_member;         // pointer to first member

};

#endif