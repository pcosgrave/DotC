#pragma once

//============================================================================================================
//  Defense of the Citadel, Copyright (c) 2010 Michael Lyashenko and Philip Cosgrave. All rights reserved.
//											www.nextrevision.com
//============================================================================================================
// Chat input handler script
//============================================================================================================

class USChatInput : public UIScript
{
public:

	R5_DECLARE_INHERITED_CLASS("USChatInput", USChatInput, UIScript, UIScript);

	uint OnGlobalKey (const Vector2i& pos, byte key, bool isDown);

	virtual void OnInit();
	virtual void OnDestroy();
	virtual void OnFocus (bool gotFocus);
	virtual void OnValueChange();
};