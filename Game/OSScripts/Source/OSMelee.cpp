//============================================================================================================
//  Defense of the Citadel, Copyright (c) 2010 Michael Lyashenko and Philip Cosgrave. All rights reserved.
//											www.nextrevision.com
//============================================================================================================

#include "../../Other/Include/_All.h"
using namespace R5;

//============================================================================================================
// Initialization should set the model pointer for convenience
//============================================================================================================

void OSMelee::OnInit()
{
	ModelInstance* inst = R5_CAST(ModelInstance, mObject);
	if (inst != 0) mModel = inst->GetModel();
}

//============================================================================================================
// Move up the object until it's within melee range of the target
//============================================================================================================

void OSMelee::OnUpdate()
{
	if (mTarget == 0) return;

	float factor = Float::Min(Time::GetDelta() * 3.0f, 1.0f);

	const Vector3f&		targetPos  (mTarget->GetRelativePosition());
	const Vector3f&		currentPos (mObject->GetRelativePosition());
	const Quaternion&	currentRot (mObject->GetRelativeRotation());

	Vector3f diff (targetPos - currentPos);
	float mag = Vector3f(diff.x, diff.y, 0.0f).Magnitude();

	// Tiles are 2m by 2m, but ideal distance from the target is 1.5 meters
	Vector3f dir (diff);
	dir /= mag;
	dir *= 1.5f;

	// TODO: It would be nice to account for terrain height... Currently NPCs go right through the
	// terrain if it happens to be curved along the way.

	Vector3f moveTo (targetPos - dir);
	Vector3f finalPos (Interpolation::Linear(currentPos, moveTo, factor));
	mObject->SetAbsolutePosition(finalPos);

	dir = targetPos - finalPos;
	dir.z = 0.0f;
	Quaternion rot (dir);

	rot = Interpolation::Slerp(currentRot, rot, factor);
	mObject->SetAbsoluteRotation(rot);
}