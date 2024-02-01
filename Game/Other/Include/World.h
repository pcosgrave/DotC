#pragma once

//============================================================================================================
//  Defense of the Citadel, Copyright (c) 2010 Michael Lyashenko and Philip Cosgrave. All rights reserved.
//											www.nextrevision.com
//============================================================================================================
// Game world contains all the data for the current world -- from terrain to objects to pathfinding
//============================================================================================================

class World
{
	ushort	mWidth;
	ushort	mHeight;

	Array<byte> mHeightmap;

public:

	World() : mWidth(0), mHeight(0) {}

	// Returns whether the terrain is valid
	bool	IsValid()	const { return ((uint)mWidth * mHeight) > 1; }
	ushort	GetWidth()	const { return mWidth;  }
	ushort	GetHeight()	const { return mHeight; }

	// Releases the heightmap information
	void Release() { mHeightmap.Release(); mWidth = 0; mHeight = 0; }

	// Returns the terrain's depth at the specified index
	ushort GetDepth (uint index) const { return (index < mHeightmap.GetSize()) ? mHeightmap[index] : 0; }

	// Samples the terrain's depth at the specified coordinates
	ushort GetDepth (const Vector3i& pos) const { return GetDepth((uint)mWidth * pos.y + pos.x); }

	// Modifies the terrain's heightmap data
	void ModifyHeightmap (ushort x, ushort y, ushort width, ushort height, const byte* data);

	// Helper function that returns the ID of the selected entity
	static uint GetSelectedID();

	// Helper function that returns the ID of the controlled entity
	static uint GetControlledID();
};