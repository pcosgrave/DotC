#pragma once

//============================================================================================================
//  Defense of the Citadel, Copyright (c) 2010 Michael Lyashenko and Philip Cosgrave. All rights reserved.
//											www.nextrevision.com
//============================================================================================================
// UI script that sets its owner to be the chat log for the game
//============================================================================================================

class USChatLog : public UIScript
{
public:

	R5_DECLARE_INHERITED_CLASS("USChatLog", USChatLog, UIScript, UIScript);

	// Sets its owner to be the output widget for debugging
	virtual void OnInit();
};