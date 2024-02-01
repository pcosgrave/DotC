//============================================================================================================
//  Defense of the Citadel, Copyright (c) 2010 Michael Lyashenko and Philip Cosgrave. All rights reserved.
//											www.nextrevision.com
//============================================================================================================

#include "../../Other/Include/_All.h"
using namespace R5;

//============================================================================================================
// Updates the current selection
//============================================================================================================

bool OSClickToControl::OnKeyPress (const Vector2i& pos, byte key, bool isDown)
{
	if (key == Key::MouseLeft)
	{
		if (!isDown)
		{
			ulong time = Time::GetMilliseconds();

			if (IsKeyDown(Key::LeftShift) || IsKeyDown(Key::RightShift) || (time - mTimestamp < 250))
			{
				mObject->AddScript<OSControlled>()->Show(1.5f, 1.5f, 1.5f);
			}
			mTimestamp = time;
		}
		return true;
	}
	return false;
}