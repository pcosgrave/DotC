//============================================================================================================
//  Defense of the Citadel, Copyright (c) 2010 Michael Lyashenko and Philip Cosgrave. All rights reserved.
//											www.nextrevision.com
//============================================================================================================

#include "../../Other/Include/_All.h"
using namespace R5;

//============================================================================================================
// Connects when everything is loaded
//============================================================================================================

void OSServerConnect::OnUpdate()
{
	if (Game::Core->GetNumberOfThreads() == 0)
	{
		
		DestroySelf();
	}
}

//============================================================================================================
// Serialization -- Save
//============================================================================================================

void OSServerConnect::OnSerializeTo (TreeNode& node) const
{
	node.AddChild("ServerIP", mServerIP);
}

//============================================================================================================
// Serialization -- Load
//============================================================================================================

void OSServerConnect::OnSerializeFrom (const TreeNode& node)
{
	if (node.mTag == "ServerIP") node.mValue >> mServerIP;
}