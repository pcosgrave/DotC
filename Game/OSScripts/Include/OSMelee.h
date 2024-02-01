#pragma once

//============================================================================================================
//  Defense of the Citadel, Copyright (c) 2010 Michael Lyashenko and Philip Cosgrave. All rights reserved.
//											www.nextrevision.com
//============================================================================================================
// Moves the object up to the melee range with the target
//============================================================================================================

class OSMelee : public Script
{
	Model*		mModel;
	Vector3f	mOrigin;
	Object*		mTarget;

public:

	R5_DECLARE_INHERITED_CLASS("OSMelee", OSMelee, Script, Script);

	OSMelee() : mModel(0), mTarget(0) {}

	void SetTarget (const Vector3f& origin, Object* target) { mOrigin = origin; mTarget = target; }

	virtual void OnInit();
	virtual void OnUpdate();
};