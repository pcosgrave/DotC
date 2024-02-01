#pragma once

//============================================================================================================
//  Defense of the Citadel, Copyright (c) 2010 Michael Lyashenko and Philip Cosgrave. All rights reserved.
//											www.nextrevision.com
//============================================================================================================
// A base class for UIScripts that knows about the widgets parent frame
//============================================================================================================

class USFrameAware : public UIScript
{
public:

	R5_DECLARE_INHERITED_CLASS("USFrameAware", USFrameAware, UIScript, UIScript);

	virtual UIWidget* GetParentFrame();
};