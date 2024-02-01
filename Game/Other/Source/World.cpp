//============================================================================================================
//  Defense of the Citadel, Copyright (c) 2010 Michael Lyashenko and Philip Cosgrave. All rights reserved.
//											www.nextrevision.com
//============================================================================================================

#include "../../Other/Include/_All.h"
using namespace R5;

//============================================================================================================
// Modifies the terrain's heightmap data
//============================================================================================================

void World::ModifyHeightmap (ushort x, ushort y, ushort width, ushort height, const byte* data)
{
	if (x == 0 && y == 0 && !IsValid())
	{
		mWidth = width;
		mHeight = height;
		uint size = (uint)mWidth * mHeight;
		memcpy(mHeightmap.ExpandTo(size), data, size);
	}
	else
	{
		ASSERT(false, "TODO: Implement heightmap changes");
	}
}

//============================================================================================================
// Helper function that returns the ID of the selected entity
//============================================================================================================

uint World::GetSelectedID()
{
	if (Game::Selected != 0)
	{
		OSEntity* ent = Game::Selected->GetScript<OSEntity>();
		if (ent != 0) return ent->GetID();
	}
	return 0;
}

//============================================================================================================
// Helper function that returns the ID of the controlled entity
//============================================================================================================

uint World::GetControlledID()
{
	if (Game::Controlled != 0)
	{
		OSEntity* ent = Game::Controlled->GetScript<OSEntity>();
		if (ent != 0) return ent->GetID();
	}
	return 0;
}