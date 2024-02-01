//============================================================================================================
//  Defense of the Citadel, Copyright (c) 2010 Michael Lyashenko and Philip Cosgrave. All rights reserved.
//											www.nextrevision.com
//============================================================================================================

#include "../../Other/Include/_All.h"
using namespace R5;

//============================================================================================================
// See if this object is a ModelInstance
//============================================================================================================

void OSFace::OnInit()
{
	ModelInstance* inst = R5_CAST(ModelInstance, mObject);
	if (inst != 0) mModel = inst->GetModel();
}

//============================================================================================================
// Start the facing process
//============================================================================================================

void OSFace::Face (float rot, float delay)
{
	mStart	= mObject->GetRelativeRotation().GetFlatYaw();
	mEnd	= rot;
	mDelay	= delay;
	mTime	= Time::GetSeconds();
}

//============================================================================================================
// Rotate the object so it's facing the direction it's traveling in
//============================================================================================================

void OSFace::OnPreUpdate()
{
	const Quaternion& currentRot (mObject->GetRelativeRotation());
	float curr = currentRot.GetFlatYaw();
	float rot (mEnd);

	if (mDelay > 0.0f)
	{
		double delta = Time::GetSeconds() - mTime;
		float factor = (float)delta / mDelay;

		if (factor >= 1.0f)
		{
			factor = 1.0f;
			mDelay = 0.0f;
		}

		// This is where we should be
		rot = WrapAngle(mStart + WrapAngle(mEnd - mStart) * Interpolation::Cosine(factor));
	}

	// Update the object's rotation
	Quaternion targetRot (Vector3f(Float::Sin(rot), Float::Cos(rot), 0.0f));
	mObject->SetRelativeRotation(targetRot);

	// Whether we're still rotating -- 1 degree margin of error
	if (Float::Abs(WrapAngle(mEnd - rot)) < 0.01745329252f) DestroySelf();
}