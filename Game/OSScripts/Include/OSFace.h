#pragma once

//============================================================================================================
//  Defense of the Citadel, Copyright (c) 2010 Michael Lyashenko and Philip Cosgrave. All rights reserved.
//											www.nextrevision.com
//============================================================================================================
// Rotates the model to the specified rotation over time, calling proper animations in the process
//============================================================================================================

class OSFace : public Script
{
	Model*	mModel;
	float	mStart;
	float	mEnd;
	double	mTime;
	float	mDelay;
	bool	mIsActive;

	OSFace() : mModel(0), mTime(0.0), mDelay(0.0f), mIsActive(false) {}

public:

	R5_DECLARE_INHERITED_CLASS("OSFace", OSFace, Script, Script);

	// Start the rotation process
	void Face (float rot, float delay);

	virtual void OnInit();
	virtual void OnPreUpdate();
};