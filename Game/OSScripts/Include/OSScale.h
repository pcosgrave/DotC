#pragma once

//============================================================================================================
//  Defense of the Citadel, Copyright (c) 2010 Michael Lyashenko and Philip Cosgrave. All rights reserved.
//											www.nextrevision.com
//============================================================================================================
// Scales the owner of the script to the specified size over specified amount of time
//============================================================================================================

class OSScale : public Script
{
	Vector3f	mStartScale;
	Vector3f	mTargetScale;
	ulong		mStartTime;
	float		mDuration;

public:

	R5_DECLARE_INHERITED_CLASS("OSScale", OSScale, Script, Script);

	OSScale() : mStartScale(0.0f), mTargetScale(1.0f), mStartTime(0), mDuration(0.0f) {}

	// Starts the scaling process
	void Start (Vector3f& targetScale, float duration);

	// Perform the per-frame scaling
	virtual void OnPreUpdate();
};