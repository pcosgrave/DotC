//============================================================================================================
//  Defense of the Citadel, Copyright (c) 2010 Michael Lyashenko and Philip Cosgrave. All rights reserved.
//											www.nextrevision.com
//============================================================================================================

#include "../../Other/Include/_All.h"
using namespace R5;

//============================================================================================================
// See if this object is a ModelInstance
//============================================================================================================

void OSMove::OnInit()
{
	mInst = R5_CAST(ModelInstance, mObject);
	if (mInst == 0) DestroySelf();
}

//============================================================================================================
// Deactivate run / walk animations
//============================================================================================================

void OSMove::OnDestroy()
{
	Model* model = mInst->GetModel();

	if (model != 0)
	{
		model->StopAnimation("Walk", 0.8f);
		model->StopAnimation("Run", 0.8f);
	}
}

//============================================================================================================
// Move the object to the specified position over time
//============================================================================================================

void OSMove::Move (const Vector3f& pos, float delay, float haste)
{
	// Remove conflicting scripts
	mObject->RemoveScript<OSCombatAnimations>();
	mObject->RemoveScript<OSMelee>();
	mObject->RemoveScript<OSFace>();

	// Set up all the states
	mStart		= mIsActive ? mEnd : mObject->GetRelativePosition();
	mEnd		= pos;
	mDelay		= delay;
	mTime		= Time::GetSeconds();
	mIsActive	= true;
	mRotation	= true;

	// Play a proper animation
	Model* model = mInst->GetModel();
	if (model != 0) model->PlayAnimation(haste < 0.75f ? "Walk" : "Run");
}

//============================================================================================================
// Updates the object's position
//============================================================================================================

void OSMove::OnPreUpdate()
{
	if (mIsActive)
	{
		const Vector3f& curr = mObject->GetRelativePosition();
		Vector3f pos (mEnd);

		// If we're currently walking to the destination, interpolate along the path
		if (mDelay > 0.0f)
		{
			double delta = Time::GetSeconds() - mTime;
			float factor = (float)delta / mDelay;

			if (factor >= 1.0f)
			{
				factor = 1.0f;
				mDelay = 0.0f;
				OnDestroy();
			}

			// This is where we should be
			pos = Interpolation::Linear(mStart, mEnd, factor);
		}

		// Make the final position smoothly interpolated
		pos = Interpolation::Linear(curr, pos, Time::GetDelta() * 5.0f);

		// Update the object's position
		mObject->SetRelativePosition(pos);

		// Whether we're still moving
		mIsActive = ((mEnd - pos).Sum() > 0.01f);
	}
}

//============================================================================================================
// Rotate the object so it's facing the direction it's traveling in
//============================================================================================================

void OSMove::OnUpdate()
{
	if (mRotation)
	{
		// If OSFace gets added at some point, we don't need to rotate anymore
		if (!mIsActive || mObject->GetScript<OSFace>())
		{
			mRotation = false;
		}
		else
		{
			Vector3f vel (mObject->GetRelativeVelocity());
			vel.z = 0.0f;

			if (vel.Sum() > 0.001f)
			{
				float speed = vel.Magnitude();

				// Smoothly interpolate the current rotation to the desired one
				float delta = Time::GetDelta() * 5.0f;
				Quaternion rot (Interpolation::Slerp(mObject->GetRelativeRotation(), Quaternion(vel), delta));

				// Use the flat forward vector, eliminating the 'roll' component in addition to the Z axis
				mObject->SetRelativeRotation(Vector3f(rot.GetFlatForward()));
			}
		}
	}
}