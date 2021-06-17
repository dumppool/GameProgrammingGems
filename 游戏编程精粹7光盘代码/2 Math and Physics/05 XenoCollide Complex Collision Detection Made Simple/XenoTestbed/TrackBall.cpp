#include "StdAfx.h"
#include ".\trackball.h"

TrackBall::TrackBall(void)
: m_magnitudeScale( 1.0f )
{
	m_rotation = Quat(0, 0, 0, 1);
}

TrackBall::~TrackBall(void)
{
}

void TrackBall::Roll(float x, float y)
{
	// Build a Vector that is rotated 90 degrees relative to the direction of trackball roll
	// This is the axis of rotation of the ball
	Vector v ( -y, x, 0 );

	// Get the magnitude of the ball movement
	float magnitude = v.Len3() * m_magnitudeScale;

	// If the Vector has no magnitude, the ball hasn't moved -- so we're done
	if (magnitude == 0) return;

	// Normalize the rotation axis
	v = v / magnitude;

	// Build a quaternion from the rotation axis and rotation magnitude
	Quat q ( v, magnitude );

	// Rotate the trackball orientation by the quaternion that represents the new motion
	m_rotation = q * m_rotation;
	m_rotation.Normalize4();
}

void TrackBall::SetMagnitudeScale(float scale)
{
	m_magnitudeScale = scale;
}

float TrackBall::GetMagnitudeScale(void)
{
	return m_magnitudeScale;
}

Quat TrackBall::GetRotation(void)
{
	return m_rotation;
}

void TrackBall::SetRotation(const Quat& rot)
{
	m_rotation = rot;
}

void TrackBall::GetYawPitchRoll(float32* yaw, float32* pitch, float* roll)
{
	Matrix m(m_rotation);
	Euler e(m);
	if (yaw) *yaw = e.Z();
	if (pitch) *pitch = e.Y();
	if (roll) *roll = e.X();
}

void TrackBall::SetYawPitchRoll(float32 yaw, float32 pitch, float32 roll)
{
	Euler e(roll, pitch, yaw);
	Matrix m(e);
	m_rotation = Quat(m);
	m_rotation.Normalize4();
}
