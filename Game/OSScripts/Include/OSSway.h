#pragma once

//============================================================================================================
//  Defense of the Citadel, Copyright (c) 2010 Michael Lyashenko and Philip Cosgrave. All rights reserved.
//											www.nextrevision.com
//============================================================================================================
// Script that makes the owner sway back and forth
//============================================================================================================

class OSSway : public OSRotate
{
public:

	R5_DECLARE_INHERITED_CLASS("OSSway", OSSway, OSRotate, Script);

	// Rotate the owner
	virtual void OnPreUpdate();
};