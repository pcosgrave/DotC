#pragma once

//============================================================================================================
//  Defense of the Citadel, Copyright (c) 2010 Michael Lyashenko and Philip Cosgrave. All rights reserved.
//											www.nextrevision.com
//============================================================================================================
// Chat input handler script
//============================================================================================================

class USStatusBar : public UIScript
{
public:

	R5_DECLARE_INHERITED_CLASS("USStatusBar", USStatusBar, UIScript, UIScript);

	static void Show(const String& text);
	static void Hide();

	virtual void OnInit();
	virtual void OnDestroy();
	virtual void OnUpdate (bool changed);
};