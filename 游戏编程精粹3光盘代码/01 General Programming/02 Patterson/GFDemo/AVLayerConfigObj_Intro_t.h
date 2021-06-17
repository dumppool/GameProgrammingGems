/* Copyright (C) Scott Patterson, 2002. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Scott Patterson, 2002"
 */

// AVLayerConfigObj_Intro_t.h: interface for the AVLayerConfigObj_Intro_t class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AVLAYERCONFIGOBJ_INTRO_T_H__3660693D_EC9E_41EB_BE7E_4B6CD0234430__INCLUDED_)
#define AFX_AVLAYERCONFIGOBJ_INTRO_T_H__3660693D_EC9E_41EB_BE7E_4B6CD0234430__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GFInclude/Framework/AVLayerConfig_t.h"

class AVLayerConfigObj_Intro_t : public AVLayerConfig_t  
{
public:
	AVLayerConfigObj_Intro_t();
	virtual ~AVLayerConfigObj_Intro_t();

	virtual void Configure( AVLayer_t *pAVLayer );
};

#endif // !defined(AFX_AVLAYERCONFIGOBJ_INTRO_T_H__3660693D_EC9E_41EB_BE7E_4B6CD0234430__INCLUDED_)
