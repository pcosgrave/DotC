#pragma once

//============================================================================================================
//  Defense of the Citadel, Copyright (c) 2010 Michael Lyashenko and Philip Cosgrave. All rights reserved.
//											www.nextrevision.com
//============================================================================================================
// Attaching this script to an object will smoothly destroy it in the scene
//============================================================================================================

class OSDestroy : public Script
{
	ulong		mStartTime;
	Vector3f	mStartPos;
	float		mDuration;
	float		mStartRadius;
	Vector3f	mStartScale;

public:

	R5_DECLARE_INHERITED_CLASS("OSDestroy", OSDestroy, Script, Script);

	OSDestroy() : mStartTime(0), mDuration(1.0f), mStartRadius(0.0f), mStartScale(1.0f) {}

	// Callback triggered when the death animation finishes playing
	void OnAnimationFinished (Model* model, const Animation* anim, float timeToEnd);

	// Plays the death animation
	virtual void OnInit();

	// Should start the fading-out process after a certain amount of time has passed
	virtual void OnPreUpdate();
};