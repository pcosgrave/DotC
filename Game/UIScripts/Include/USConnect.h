#pragma once

//============================================================================================================
//  Defense of the Citadel, Copyright (c) 2010 Michael Lyashenko and Philip Cosgrave. All rights reserved.
//											www.nextrevision.com
//============================================================================================================
// Script that initiates loading of required resources and connects to the server
//============================================================================================================

class USConnect : public USFrameAware
{
private:

	UIButton* mButton;
	String mLayout;

	USConnect() : mButton(0) {}

public:

	R5_DECLARE_INHERITED_CLASS("USConnect", USConnect, USFrameAware, UIScript);

	// Callback for the connection attempt
	void OnConnect (bool successful);

	// Ensure that the script is attached to a button
	virtual void OnInit();

	// Ensure that the server is no longer referencing this script
	virtual void OnDestroy();

	// Start the connection process on click
	virtual void OnKeyPress (const Vector2i& pos, byte key, bool isDown);

	// Serialization
	virtual void OnSerializeFrom (const TreeNode& node);
	virtual void OnSerializeTo (TreeNode& root);
};