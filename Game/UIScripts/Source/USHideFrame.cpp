//============================================================================================================
//  Defense of the Citadel, Copyright (c) 2010 Michael Lyashenko and Philip Cosgrave. All rights reserved.
//											www.nextrevision.com
//============================================================================================================

#include "../../Other/Include/_All.h"
using namespace R5;

//============================================================================================================
// Hide the parent frame
//============================================================================================================

void USHideFrame::OnStateChange (uint state, bool isSet)
{
	if (state == UIButton::State::Pressed && isSet)
	{
		UIWidget* parent = mWidget->GetParent();

		while (parent != 0 && !parent->IsOfClass(UIFrame::ClassID()))
		{
			parent = parent->GetParent();
		}

		if (parent != 0)
		{
			parent->SetAlpha(0.0f, 0.25f);
		}
	}
}