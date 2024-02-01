//============================================================================================================
//  Defense of the Citadel, Copyright (c) 2010 Michael Lyashenko and Philip Cosgrave. All rights reserved.
//											www.nextrevision.com
//============================================================================================================

#include "../../Other/Include/_All.h"
using namespace R5;

//============================================================================================================
// Only one camera target can exist at a time
//============================================================================================================

OSCameraTarget* g_camTargetInstance = 0;

//============================================================================================================
// Removes the current camera target
//============================================================================================================

void OSCameraTarget::Cancel()
{
	if (g_camTargetInstance != 0) g_camTargetInstance->DestroySelf();
	g_camTargetInstance = 0;
}

//============================================================================================================
// Ensure that this script is the only one of its kind in existence
//============================================================================================================

void OSCameraTarget::OnInit()
{
	Cancel();
	g_camTargetInstance = this;
}

//============================================================================================================
// Set the camera's target each update
//============================================================================================================

void OSCameraTarget::OnUpdate()
{
	if (Game::Camera != 0)
	{
		OSDefaultCamera* def = Game::Camera->GetScript<OSDefaultCamera>();
		if (def != 0) def->SetTarget(mObject->GetAbsolutePosition());
	}
}

//============================================================================================================
// Ensure that the camera target doesn't stick around
//============================================================================================================

void OSCameraTarget::OnDestroy()
{
	if (g_camTargetInstance == this) g_camTargetInstance = 0;
}