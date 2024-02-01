#pragma once

//============================================================================================================
//  Defense of the Citadel, Copyright (c) 2010 Michael Lyashenko and Philip Cosgrave. All rights reserved.
//											www.nextrevision.com
//============================================================================================================
// When the object gets clicked on it will make the camera follow it
//============================================================================================================

class OSClickToControl : public Script
{
	ulong mTimestamp;

	OSClickToControl() : mTimestamp(0) {}

public:

	R5_DECLARE_INHERITED_CLASS("OSClickToControl", OSClickToControl, Script, Script);

	// In order for selection to work, a box collider must be present
	virtual void OnInit() { mObject->SetFlag(Object::Flag::BoxCollider, true); }

	// Start following the object when it gets clicked on
	virtual bool OnKeyPress (const Vector2i& pos, byte key, bool isDown);
};