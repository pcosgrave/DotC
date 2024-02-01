//============================================================================================================
//  Defense of the Citadel, Copyright (c) 2010 Michael Lyashenko and Philip Cosgrave. All rights reserved.
//											www.nextrevision.com
//============================================================================================================

#include "../../Other/Include/_All.h"
using namespace R5;

//============================================================================================================
// Callback for the connection attempt
//============================================================================================================

void USConnect::OnConnect (bool successful)
{
	if (successful)
	{
		USStatusBar::Hide();
		GetParentFrame()->AddScript<USFadeOut>()->SetDestroyWhenDone(true);
		GetParentFrame()->SetEventHandling(UIWidget::EventHandling::None);
		USRoot::Get(mWidget)->ChangeLayout(mLayout);
	}
	else
	{
		// Re-enable the button
		mButton->SetState(UIButton::State::Enabled, true);
	}
}

//============================================================================================================
// Ensure that the script is attached to a button
//============================================================================================================

void USConnect::OnInit()
{
	mButton = R5_CAST(UIButton, mWidget);
	if (mButton == 0) DestroySelf();
}

//============================================================================================================
// Ensure that the server is no longer referencing this script
//============================================================================================================

void USConnect::OnDestroy()
{
	if (Game::Server != 0)
	{
		Game::Server->onConnect.clear();
	}
}

//============================================================================================================
// Start the connection process
//============================================================================================================

void USConnect::OnKeyPress (const Vector2i& pos, byte key, bool isDown)
{
	if (mLayout.IsValid() && key == Key::MouseLeft && !isDown && mWidget->GetRegion().Contains(pos))
	{
		UIInput* server = mWidget->GetParent()->FindWidget<UIInput>("Server Address", false);
		if (server == 0) return;

		UIInput* name = mWidget->GetParent()->FindWidget<UIInput>("User Name", false);
		//UIInput* pass = mWidget->GetParent()->FindWidget<UIInput>("User Password", false);

		// Bind a callback to trigger on the result of the connection attempt
		Game::Server->onConnect = bind(&USConnect::OnConnect, this);

		// Use the name player specified
		if (name != 0) Game::Name = name->GetText();

		// If no name was specified, use a default name
		if (Game::Name.IsEmpty()) Game::Name = "Guest";

		// If the name is 'Guest', make it unique
		if (Game::Name == "Guest")
		{
			Game::Random.SetSeed(Time::GetMilliseconds());
			Game::Name << Game::Random.GenerateUint();
		}

		// Try to establish a connection
		Game::Server->Connect(server->GetText());

		// Disable the button
		mButton->SetState(UIButton::State::Highlighted, false);
		mButton->SetState(UIButton::State::Enabled, false);
	}
}

//============================================================================================================
// Serialization -- Save
//============================================================================================================

void USConnect::OnSerializeTo (TreeNode& root)
{
	root.AddChild("Layout", mLayout);
}

//============================================================================================================
// Serialization -- Load
//============================================================================================================

void USConnect::OnSerializeFrom (const TreeNode& node)
{
	if (node.mTag == "Layout")
	{
		mLayout = node.mValue.AsString();
	}
}