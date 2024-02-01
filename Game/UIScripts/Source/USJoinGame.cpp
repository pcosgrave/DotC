//============================================================================================================
//  Defense of the Citadel, Copyright (c) 2010 Michael Lyashenko and Philip Cosgrave. All rights reserved.
//											www.nextrevision.com
//============================================================================================================

#include "../../Other/Include/_All.h"
using namespace R5;

//============================================================================================================
// Ensure that the script is attached to a button
//============================================================================================================

void USJoinGame::OnInit()
{
	mButton = R5_CAST(UIButton, mWidget);
	if (mButton == 0) DestroySelf();
}

//============================================================================================================
// The button should only be enabled while we're still connected
//============================================================================================================

void USJoinGame::OnUpdate (bool areaChanged)
{
	mButton->SetState(UIButton::State::Enabled, Game::Server->IsConnected());
}

//============================================================================================================
// Start the process of joining the game
//============================================================================================================

void USJoinGame::OnKeyPress (const Vector2i& pos, byte key, bool isDown)
{
	if (mLayout.IsValid() && !isDown && key == Key::MouseLeft && mWidget->GetRegion().Contains(pos))
	{
		USRoot::Get(mWidget)->ChangeLayout(mLayout);

		if (mScene.IsValid())
		{
			Game::Core->SerializeFrom(mScene, true, false);
			USLoadingScreen::Create();
		}
	}
}

//============================================================================================================

void USJoinGame::OnSerializeTo (TreeNode& root) const
{
	root.AddChild("Layout", mLayout);
	root.AddChild("Scene", mScene);
}

//============================================================================================================

void USJoinGame::OnSerializeFrom (const TreeNode& node)
{
	if		(node.mTag == "Layout") mLayout = node.mValue.AsString();
	else if (node.mTag == "Scene")	mScene  = node.mValue.AsString();
}