/* Copyright (C) Scott Patterson, 2002. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Scott Patterson, 2002"
 */

// TaskConfigObj_Intro_t.cpp: implementation of the TaskConfigObj_Intro_t class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TaskConfigObj_Intro_t.h"
#include "GFInclude/Systems/Systems_t.h"
#include "GFInclude/Systems/ResourceSys_t.h"
#include "GFInclude/Framework/FramePlayer_t.h"
#include "GameInstanceIDs.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

TaskConfigObj_Intro_t::TaskConfigObj_Intro_t()
{

}

TaskConfigObj_Intro_t::~TaskConfigObj_Intro_t()
{

}

void TaskConfigObj_Intro_t::Configure( Task_t *pTask, Systems_t *pSystems )
{
	// we know it is a frame player
	FramePlayer_t *pFramePlayer = (FramePlayer_t *)pTask;

	// get the resource sys
	ResourceSys_t *pResourceSys = pSystems->GetResourceSys();

	// pushing layers
	AVLayer_t *pAVLayer;
	LogicLayer_t *pLogicLayer;

	pAVLayer = pResourceSys->GetAVLayer( INSTANCE_ID_AVLAYER_INTRO );
	pFramePlayer->Post_AVLayerCommand( AVLayerCommand_t::PUSH_BACK, pAVLayer );

	pLogicLayer = pResourceSys->GetLogicLayer( INSTANCE_ID_LOGICLAYER_INTRO );
	pFramePlayer->Post_LogicLayerCommand( LogicLayerCommand_t::PUSH_BACK, pLogicLayer );
}
