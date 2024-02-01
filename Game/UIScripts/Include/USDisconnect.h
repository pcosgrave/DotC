#pragma once

//============================================================================================================
//  Defense of the Citadel, Copyright (c) 2010 Michael Lyashenko and Philip Cosgrave. All rights reserved.
//											www.nextrevision.com
//============================================================================================================
// Disconnect button functionality
//============================================================================================================

class USDisconnect : public USChangeLayout
{
public:

	R5_DECLARE_INHERITED_CLASS("USDisconnect", USDisconnect, USChangeLayout, UIScript);

	virtual void OnKeyPress (const Vector2i& pos, byte key, bool isDown);
};