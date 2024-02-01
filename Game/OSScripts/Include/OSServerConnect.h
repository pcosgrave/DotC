#pragma once

//============================================================================================================
//  Defense of the Citadel, Copyright (c) 2010 Michael Lyashenko and Philip Cosgrave. All rights reserved.
//											www.nextrevision.com
//============================================================================================================
// Connects to the server when core execution is finished.
//============================================================================================================

class OSServerConnect : public Script
{
private:
	String mServerIP;

public:

	R5_DECLARE_INHERITED_CLASS("OSServerConnect", OSServerConnect, Script, Script);

	OSServerConnect() : mServerIP("127.0.0.1:3682") { }

	virtual void OnUpdate();

	// Serialization
	virtual void OnSerializeTo(TreeNode& node) const;
	virtual void OnSerializeFrom(const TreeNode& node);
};