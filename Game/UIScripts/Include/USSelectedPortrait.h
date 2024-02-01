#pragma once

//============================================================================================================
//  Defense of the Citadel, Copyright (c) 2010 Michael Lyashenko and Philip Cosgrave. All rights reserved.
//											www.nextrevision.com
//============================================================================================================
// The portrait for the controlled player.
//============================================================================================================

class USSelectedPortrait : public USControlledPortrait
{
public:

	R5_DECLARE_INHERITED_CLASS("USSelectedPortrait", USSelectedPortrait, USControlledPortrait, UIScript);

	virtual void OnUpdate(bool areaChanged);
};
