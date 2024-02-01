//============================================================================================================
//  Defense of the Citadel, Copyright (c) 2010 Michael Lyashenko and Philip Cosgrave. All rights reserved.
//											www.nextrevision.com
//============================================================================================================

#include "../../Other/Include/_All.h"
using namespace R5;

//============================================================================================================
// Updates the current selection
//============================================================================================================

bool OSSelectable::OnKeyPress (const Vector2i& pos, byte key, bool isDown)
{
	if (key == Key::MouseLeft)
	{
		if (!isDown && Game::Selected != mObject)
		{
			mObject->AddScript<OSSelected>()->Show(mColor.r, mColor.g, mColor.b);
		}
		return true;
	}
	return false;
}