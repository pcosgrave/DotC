#pragma once

//============================================================================================================
//  Defense of the Citadel, Copyright (c) 2010 Michael Lyashenko and Philip Cosgrave. All rights reserved.
//											www.nextrevision.com
//============================================================================================================
// UI script that changes the parent frame's alpha to 0
//============================================================================================================

class USHideFrame : public UIScript
{
public:

	R5_DECLARE_INHERITED_CLASS("USHideFrame", USHideFrame, UIScript, UIScript);

	virtual void OnStateChange (uint state, bool isSet);
};