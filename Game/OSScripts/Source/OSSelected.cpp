//============================================================================================================
//  Defense of the Citadel, Copyright (c) 2010 Michael Lyashenko and Philip Cosgrave. All rights reserved.
//											www.nextrevision.com
//============================================================================================================

#include "../../Other/Include/_All.h"
using namespace R5;

//============================================================================================================
// Handle key events on the selected object
//============================================================================================================

uint SelectedOnKey (const Vector2i& pos, byte key, bool isDown)
{
	if (key == Key::Escape)
	{
		if (!isDown)
		{
			if (Game::Selected != 0)
			{
				OSSelection* sel = Game::Selected->GetScript<OSSelected>();
				if (sel != 0) sel->Hide();
				Game::Selected = 0;
				return 1;
			}
		}
	}
	else if (Game::Selected != 0)
	{
		// Forward this event to the selected object
		uint retVal = Game::Selected->KeyPress(pos, key, isDown);
		return retVal;
	}
	return 0;
}

//============================================================================================================
// Handle mouse move events on the selected object
//============================================================================================================

uint SelectedOnMouseMove(const Vector2i& pos, const Vector2i& delta)
{
	if (Game::Selected != 0)
	{
		// Forward this event to the selected object
		uint retVal = Game::Selected->MouseMove(pos, delta);
		return retVal;
	}
	return 0;
}

//============================================================================================================
// This object is now our target object
//============================================================================================================

void OSSelected::OnInit()
{
	if (Game::Selected != mObject)
	{
		if (Game::Selected != 0)
		{
			OSSelected* s = Game::Selected->GetScript<OSSelected>();
			if (s != 0) s->Hide();
		}
		Game::Selected = mObject;
	}
}

//============================================================================================================
// Remove the selection
//============================================================================================================

void OSSelected::OnDestroy()
{
	if (Game::Selected == mObject) Game::Selected = 0;
	OSSelection::OnDestroy();
}

//============================================================================================================
// One-time update callback
//============================================================================================================

void OSSelected::OnUpdate()
{
	static bool doOnce = true;

	if (doOnce)
	{
		doOnce = false;
		Game::Core->AddOnKey(&SelectedOnKey, 1049);
	}
	Script::OnUpdate();
}