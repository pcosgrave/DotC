//============================================================================================================
//  Defense of the Citadel, Copyright (c) 2010 Michael Lyashenko and Philip Cosgrave. All rights reserved.
//											www.nextrevision.com
//============================================================================================================

#include "../../Other/Include/_All.h"
using namespace R5;

//============================================================================================================
// Starts the scaling process
//============================================================================================================

void OSScale::Start (Vector3f& targetScale, float duration)
{
	mStartScale	 = mObject->GetRelativeScale();
	mTargetScale = targetScale;
	mDuration	 = (duration > 0.0f) ? duration : 0.25f;
	mStartTime	 = Time::GetMilliseconds();
}

//============================================================================================================
// Perform the per-frame scaling
//============================================================================================================

void OSScale::OnPreUpdate()
{
	if (mDuration > 0.0f)
	{
		float factor = 0.001f * (Time::GetMilliseconds() - mStartTime) / mDuration;

		if (factor > 1.0f)
		{
			if (mTargetScale.IsZero())
			{
				mObject->DestroySelf();
			}
			else
			{
				mObject->SetRelativeScale(mTargetScale);
				DestroySelf();
			}
		}
		else
		{
			Vector3f& scale = Interpolation::Hermite(mStartScale, mTargetScale, factor);
			mObject->SetRelativeScale(scale);
		}
	}
}