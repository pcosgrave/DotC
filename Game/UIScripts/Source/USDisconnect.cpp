//============================================================================================================
//  Defense of the Citadel, Copyright (c) 2010 Michael Lyashenko and Philip Cosgrave. All rights reserved.
//											www.nextrevision.com
//============================================================================================================

#include "../../Other/Include/_All.h"
using namespace R5;

//============================================================================================================

void USDisconnect::OnKeyPress (const Vector2i& pos, byte key, bool isDown)
{
	if (mLayout.IsValid() && !isDown && key == Key::MouseLeft && mWidget->GetRegion().Contains(pos))
	{
		Game::Server->Disconnect();
		Game::World->Release();
		USRoot::Get(mWidget)->ChangeLayout(mLayout);
	}
}