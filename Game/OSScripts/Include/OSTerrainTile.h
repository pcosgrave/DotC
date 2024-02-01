#pragma once

//============================================================================================================
//  Defense of the Citadel, Copyright (c) 2010 Michael Lyashenko and Philip Cosgrave. All rights reserved.
//											www.nextrevision.com
//============================================================================================================
// Terrain tiles need to respond to raycast and key events
//============================================================================================================

class OSTerrainTile : public OSSelectable
{
protected:

	Vector3i mPos;
	bool mRescan;

	bool _Rescan();

	OSTerrainTile() : mRescan(true) {}

public:

	R5_DECLARE_INHERITED_CLASS("OSTerrainTile", OSTerrainTile, OSSelectable, Script);

	// Returns the XY index of this tile
	const Vector3i& GetPos() { if (mRescan) _Rescan(); return mPos; }

	// Updates the current selection
	virtual bool OnKeyPress (const Vector2i& pos, byte key, bool isDown);
};