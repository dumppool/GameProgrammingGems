/* Copyright (C) Scott Patterson, 2002. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Scott Patterson, 2002"
 */

// LogicLayerObj_GameWin_t.cpp: implementation of the LogicLayerObj_GameWin_t class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LogicLayerObj_GameWin_t.h"
#include "GFInclude/Systems/Systems_t.h"
#include "GFInclude/Systems/ResourceSys_t.h"
#include "GFInclude/Systems/InputSys_t.h"
#include "AVLayerObj_GameWin_t.h"
#include "GameInstanceIDs.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

LogicLayerObj_GameWin_t::LogicLayerObj_GameWin_t()
{
	m_pSystems = 0;
	m_bExclusive = true;
}

LogicLayerObj_GameWin_t::~LogicLayerObj_GameWin_t()
{

}

bool LogicLayerObj_GameWin_t::Connect( Systems_t *pSystems )
{
	m_pSystems = pSystems;

	ResourceSys_t *pResourceSys = m_pSystems->GetResourceSys();
	m_pAVLayer = (AVLayerObj_GameWin_t *)pResourceSys->GetAVLayer( INSTANCE_ID_AVLAYER_GAMEWIN );

	return true;
}

void LogicLayerObj_GameWin_t::Disconnect()
{
}

void LogicLayerObj_GameWin_t::Update()
{
	InputSys_t *pInputSys = m_pSystems->GetInputSys();

	KeyEvent_t KeyEvent;
	while( pInputSys->ReadKey( KeyEvent ) )
	{
		if( KeyEvent.m_nType == KeyEvent_t::EVENT_KEY_DOWN )
		{
			switch( KeyEvent.m_nKeyCode )
			{
			case GF_KEYCODE_UP:
				break;
			case GF_KEYCODE_DOWN:
				break;
			case GF_KEYCODE_LEFT:
				break;
			case GF_KEYCODE_RIGHT:
				break;
			case GF_KEYCODE_ENTER:
				break;
			case GF_KEYCODE_ESCAPE:
				break;
			}
		}
	}
}

void LogicLayerObj_GameWin_t::SetExclusive( bool bEnable )
{
	m_bExclusive = bEnable;
}

bool LogicLayerObj_GameWin_t::IsExclusive()
{
	return m_bExclusive;
}

