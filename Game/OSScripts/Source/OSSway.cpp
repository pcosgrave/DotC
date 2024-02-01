//============================================================================================================
//  Defense of the Citadel, Copyright (c) 2010 Michael Lyashenko and Philip Cosgrave. All rights reserved.
//											www.nextrevision.com
//============================================================================================================

#include "../../Other/Include/_All.h"
using namespace R5;

//============================================================================================================
// Sways the owner
//============================================================================================================

void OSSway::OnPreUpdate()
{
	float time = Float::Sin(Float::Cos(Time::GetTime())) + 0.5f;
	Quaternion rot (mAxis, time * (mRate * TWOPI));
	mObject->SetRelativeRotation(rot);
}