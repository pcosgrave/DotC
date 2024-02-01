#pragma once

//============================================================================================================
//  Defense of the Citadel, Copyright (c) 2010 Michael Lyashenko and Philip Cosgrave. All rights reserved.
//											www.nextrevision.com
//============================================================================================================
// Script attached to a button on the server info page that allows the player to join the game
//============================================================================================================

class USJoinGame : public UIScript
{
	UIButton*	mButton;
	String		mLayout;
	String		mScene;

	USJoinGame() : mButton(0) {}

public:

	R5_DECLARE_INHERITED_CLASS("USJoinGame", USJoinGame, UIScript, UIScript);

	virtual void OnInit();
	virtual void OnUpdate (bool areaChanged);
	virtual void OnKeyPress (const Vector2i& pos, byte key, bool isDown);
	virtual void OnSerializeTo (TreeNode& root) const;
	virtual void OnSerializeFrom (const TreeNode& node);
};