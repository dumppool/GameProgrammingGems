#ifndef _fsm_fsm_h
#define _fsm_fsm_h

// Local Includes
#include "State.h"

// FSM Class
class CFSM
{
protected:
	CState *m_pCurrentState;						// Current State
	CState *m_pNewState;							// New State
	CStateTemplate<CFSM> m_StateInitial;			// Initial State

public:
	// Constructor
	CFSM();

	// Destructor
	virtual ~CFSM() {}

	// Global Functions
	virtual void Update();

	// State Functions
	bool IsState(CState &State);
	bool GotoState(CState &NewState);

	virtual void BeginStateInitial() {}
	virtual void StateInitial() {}
	virtual void EndStateInitial() {}
};

#endif
