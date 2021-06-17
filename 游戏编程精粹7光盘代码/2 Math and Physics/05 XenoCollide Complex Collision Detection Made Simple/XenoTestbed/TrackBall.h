#pragma once

#include "Math\math.h"

//////////////////////////////////////////////////////////////////////////////
// TrackBall is part of the testbed framework.  It keeps track of the
// camera's orientation in the scene.

class TrackBall
{

public:

	TrackBall(void);
	~TrackBall(void);

	void Roll(float x, float y);
	void SetMagnitudeScale(float scale);
	float GetMagnitudeScale(void);

	// Quaternion-style accessors
	Quat GetRotation(void);
	void SetRotation(const Quat& rot);

	// Euler-style accessors
	void GetYawPitchRoll(float32* yaw, float32* pitch, float* roll);
	void SetYawPitchRoll(float32 yaw, float32 pitch, float32 roll);

	float m_magnitudeScale;
	Quat m_rotation;

};
