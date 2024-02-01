#pragma once

//============================================================================================================
//  Defense of the Citadel, Copyright (c) 2010 Michael Lyashenko and Philip Cosgrave. All rights reserved.
//											www.nextrevision.com
//============================================================================================================
// Keeps setting the camera's target to the object it's attached to
//============================================================================================================

class OSCameraTarget : public Script
{
public:

	R5_DECLARE_INHERITED_CLASS("OSCameraTarget", OSCameraTarget, Script, Script);

	// Removes the current camera target
	static void Cancel();

	virtual void OnInit();
	virtual void OnUpdate();
	virtual void OnDestroy();
};