/* ============================================================================
 *  _LEdgeInfo.h
 * ============================================================================

 *  Author:         (C) 2003 Stefano Emiliozzi, Camil Demetrescu
 *  License:        See the end of this file for license information
 *  Created:        January 12, 2003
 *  Module:         LL

 *  Last changed:   $Date: 2003/01/31 15:54:44 $
 *  Changed by:     $Author: stefano $   
 *  Revision:       $Revision: 1.6 $    
*/


#ifndef ___LEdgeInfo__
#define ___LEdgeInfo__

#include "LEdgeInfo.h"

void _LEdgeInfo_DeleteItemAt(LEdgeInfo* This, LGraph_TEdge* inEdge);
void _LEdgeInfo_AddInfo(LEdgeInfo* This, LGraph_TEdge* inEdge);
void _LEdgeInfo_CallItemsDestructor(LEdgeInfo* This);

#endif

/* Copyright (C) 2003 Stefano Emiliozzi, Camil Demetrescu

 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.

 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

