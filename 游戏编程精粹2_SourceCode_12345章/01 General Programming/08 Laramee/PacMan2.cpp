/* Copyright (C) Fran�ois Dominic Laram�e, 2001. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Fran�ois Dominic Laram�e, 2001"
 */
void PacMan::HandleInteractions( Entity * target )
{
  if ( target->GetClass() == GHOST && 
       ( target->GetState() == AFRAID || 
         target->GetState() == GETTINGSCARED ) ) 
  {
    ...
  }
}
