//============================================================================================================
//  Defense of the Citadel, Copyright (c) 2010 Michael Lyashenko and Philip Cosgrave. All rights reserved.
//											www.nextrevision.com
//============================================================================================================

#include "../../Other/Include/_All.h"
using namespace R5;

UIWidget* USFrameAware::GetParentFrame()
{
	UIWidget* widget = mWidget;

	do 
	{
		if (widget->IsOfClass(UIFrame::ClassID()))
		{
			break;
		}
		widget = widget->GetParent();
	} while (widget != 0);
	return widget;
}