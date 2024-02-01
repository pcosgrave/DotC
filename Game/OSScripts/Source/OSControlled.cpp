//============================================================================================================
//  Defense of the Citadel, Copyright (c) 2010 Michael Lyashenko and Philip Cosgrave. All rights reserved.
//											www.nextrevision.com
//============================================================================================================

#include "../../Other/Include/_All.h"
using namespace R5;

//============================================================================================================
// Handle key events on the controlled object
//============================================================================================================

uint ControlledOnKey (const Vector2i& pos, byte key, bool isDown)
{
	if (key == Key::Escape)
	{
		if (!isDown)
		{
			if (Game::Controlled != 0)
			{
				Game::Controlled->RemoveScript<OSCameraTarget>();
				OSSelection* sel = Game::Controlled->GetScript<OSControlled>();
				if (sel != 0) sel->Hide();
				Game::Controlled = 0;
				return 1;
			}
		}
	}
	return 0;
}

//============================================================================================================
// This object is now our target object
//============================================================================================================

void OSControlled::OnInit()
{
	if (Game::Controlled != mObject)
	{
		if (Game::Controlled != 0)
		{
			OSControlled* s = Game::Controlled->GetScript<OSControlled>();
			if (s != 0) s->Hide();
		}
		Game::Controlled = mObject;
	}
}

//============================================================================================================
// Remove the selection
//============================================================================================================

void OSControlled::OnDestroy()
{
	if (Game::Controlled == mObject) Game::Controlled = 0;
	OSSelection::OnDestroy();
}

//============================================================================================================
// One-time update callback
//============================================================================================================

void OSControlled::OnUpdate()
{
	static bool doOnce = true;

	if (doOnce)
	{
		doOnce = false;
		Game::Core->AddOnKey(&ControlledOnKey, 1050);
	}
	Script::OnUpdate();
}