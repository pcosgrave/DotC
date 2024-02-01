#pragma once

//============================================================================================================
//  Defense of the Citadel, Copyright (c) 2010 Michael Lyashenko and Philip Cosgrave. All rights reserved.
//											www.nextrevision.com
//============================================================================================================
// Moves the model to the specified position over time, calling proper animations in the process
//============================================================================================================

class OSMove : public Script
{
	ModelInstance*	mInst;
	Vector3f		mStart;
	Vector3f		mEnd;
	double			mTime;
	float			mDelay;
	bool			mIsActive;
	bool			mRotation;

	OSMove() : mInst(0), mTime(0.0), mDelay(0.0f), mIsActive(false), mRotation(true) {}

public:

	R5_DECLARE_INHERITED_CLASS("OSMove", OSMove, Script, Script);

	// Whether the object is GetRelativeRotation
	bool IsMoving() const { return mIsActive; }

	// Move the object to the specified position over time
	void Move (const Vector3f& pos, float delay, float haste);

	// See if this object is a ModelInstance
	virtual void OnInit();

	// Deactivate run / walk animations
	virtual void OnDestroy();

	// Updates the object's position
	virtual void OnPreUpdate();

	// Rotate the object so it's facing the direction it's traveling in
	virtual void OnUpdate();
};