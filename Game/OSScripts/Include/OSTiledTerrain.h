#pragma once

//============================================================================================================
//  Defense of the Citadel, Copyright (c) 2010 Michael Lyashenko and Philip Cosgrave. All rights reserved.
//											www.nextrevision.com
//============================================================================================================
// Script that contains tiles used for terrains
//============================================================================================================

class OSTiledTerrain : public Script
{
public:

	// When tiles get parsed they create tile entries -- 4 per tile (at 90 degree rotation intervals)
	struct TileEntry
	{
		byte	height;		// Tile's calculated height in units
		Model*	model;		// Model used to visualize this tile
		float	rotation;	// Tile's rotation in degrees

		TileEntry() : height(0), model(0), rotation(0.0f) {}
	};

	// Each tile is made up of 4 single-byte corners (heights)
	struct Corners
	{
		union
		{
			struct { byte a, b, c, d; };
			uint32 mVal;
		};

		Corners (byte aa, byte bb, byte cc, byte dd) : a(aa), b(bb), c(cc), d(dd) {}

		bool IsValid() const { return (a | b | c | d) != 0; }

		bool operator == (byte i) const { return (a == i) || (b == i) || (c == i) || (d == i); }
		bool operator >  (byte i) const { return (a  > i) || (b  > i) || (c  > i) || (d  > i); }

		Corners operator - (byte offset)
		{
			return Corners( a > offset ? a - offset : 0,
							b > offset ? b - offset : 0,
							c > offset ? c - offset : 0,
							d > offset ? d - offset : 0 );
		}

		uint ToIndex() const { return (a * 1000 + b * 100 + c * 10 + d); }

		void Cap (byte i)
		{
			if (a > i) a = 9;
			if (b > i) b = 9;
			if (c > i) c = 9;
			if (d > i) d = 9;
		}

		void GetLowHigh (byte& low, byte& high)
		{
			low  = a;
			high = 0;

			if (low > b) low = b;
			if (low > c) low = c;
			if (low > d) low = d;

			if (high < a) high = a;
			if (high < b) high = b;
			if (high < c) high = c;
			if (high < d) high = d;
		}
	};

protected:

	Array<ushort>		mTiles;
	Hash<TileEntry>		mEntries;
	bool				mInitialized;
	IMaterial*			mMat;

	OSTiledTerrain() : mInitialized(false), mMat(0) {}

private:

	// Adds a new entry to the hash table
	void AddEntry (uint index, Model* model, byte height, float rotation);

public:

	R5_DECLARE_INHERITED_CLASS("OSTiledTerrain", OSTiledTerrain, Script, Script);

	// Sets the 'mTiles' and creates the hash table
	void SetTiles (const Array<ushort>& tiles);

	// Create the terrain
	void Create();

private:

	// INTERNAL: Create all terrain tiles
	void _Create();

	// INTERNAL: Creates a single column of tiles at the specified position
	void _Create (uint x, uint y);

	// INTERNAL: Creates a single tile at the specified position. Returns the tile's height.
	byte _Create (const Vector3f& pos, const Corners& tile);

protected:

	// OSTiledTerrain is technically a singleton
	virtual void OnInit();
	virtual void OnDestroy();

	// Serialization
	virtual void OnSerializeTo	(TreeNode& node) const;
	virtual void OnSerializeFrom(const TreeNode& node);
};