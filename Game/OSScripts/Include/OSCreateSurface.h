#pragma once

//============================================================================================================
//  Defense of the Citadel, Copyright (c) 2010 Michael Lyashenko and Philip Cosgrave. All rights reserved.
//											www.nextrevision.com
//============================================================================================================
// Script that creates the terrain's surface by creating invisible tiles used for selection
//============================================================================================================

class OSCreateSurface : public Script
{
	void _Create (uint x, uint y, uint width, uint height);

public:

	R5_DECLARE_INHERITED_CLASS("OSCreateSurface", OSCreateSurface, Script, Script);

	void Create();
};