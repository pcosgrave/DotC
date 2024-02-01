//============================================================================================================
//  Defense of the Citadel, Copyright (c) 2010 Michael Lyashenko and Philip Cosgrave. All rights reserved.
//											www.nextrevision.com
//============================================================================================================

#include "../../Other/Include/_All.h"
using namespace R5;

//============================================================================================================
// Sets its owner to be the output widget for debugging
//============================================================================================================

void USChatLog::OnInit()
{
	Game::Chat = R5_CAST(UITextArea, mWidget);
	Debug::SetWidget(Game::Chat);
}