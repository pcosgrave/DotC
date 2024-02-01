//============================================================================================================
//  Defense of the Citadel, Copyright (c) 2010 Michael Lyashenko and Philip Cosgrave. All rights reserved.
//											www.nextrevision.com
//============================================================================================================

#include "../../Other/Include/_All.h"
using namespace R5;

//============================================================================================================
// Helper functions used in OSTiledTerrain::SetTiles() below
//============================================================================================================

inline uint GetIndex (uint i0, uint i1, uint i2, uint i3)
{
	return i0 * 1000 + i1 * 100 + i2 * 10 + i3;
}

//============================================================================================================

byte GetHeight (uint i0, uint i1, uint i2, uint i3)
{
	uint lowest = i0, highest = 0;

	if (lowest > i1) lowest = i1;
	if (lowest > i2) lowest = i2;
	if (lowest > i3) lowest = i3;

	if (highest < i0 && i0 != 9) highest = i0;
	if (highest < i1 && i1 != 9) highest = i1;
	if (highest < i2 && i2 != 9) highest = i2;
	if (highest < i3 && i3 != 9) highest = i3;

	if (lowest == 0) lowest = 1;

	if (i0 == 9 || i1 == 9 || i2 == 9 || i3 == 9) ++highest;

	return (highest > lowest) ? highest - lowest : 1;
}

//============================================================================================================
// Adds a new entry to the hash table
//============================================================================================================

void OSTiledTerrain::AddEntry (uint index, Model* model, byte height, float rotation)
{
	if (mEntries.GetIfExists(index) == 0)
	{
		TileEntry& entry = mEntries[index];
		entry.model = model;
		entry.height = height;
		entry.rotation = rotation;
	}
}

//============================================================================================================
// Sets the 'mTiles' and creates the hash table
//============================================================================================================

void OSTiledTerrain::SetTiles (const Array<ushort>& tiles)
{
	mInitialized = false;
	mTiles = tiles;

	mEntries.Clear();
	String filename, temp;
	Core* core = mObject->GetCore();

	for (uint i = mTiles.GetSize(); i > 0; )
	{
		temp.Set("%4u", mTiles[--i]);
		ASSERT(temp.GetLength() == 4, "Invalid entry -- must be 4 digits!");
		if (temp.GetLength() != 4) continue;

		// Only allow 0-9 digits to be a part of the filename
		for (uint b = temp.GetLength(); b > 0; )
		{
			if (temp[--b] < '0' || temp[b] > '9') temp[b] = '0';
		}

		// Set the filename
		filename.Set("Models/Terrain/%s", temp.GetBuffer());

		// Load the model
		Model* model = core->GetModel(filename);

		// Try to load the model if necessary
		if (!model->IsValid())
		{
			ASSERT(false, "Failed to load the specified model!");
		}

		// Only continue if the model has been loaded
		if (model->IsValid())
		{
			uint i0 = temp[0] - '0';
			uint i1 = temp[1] - '0';
			uint i2 = temp[2] - '0';
			uint i3 = temp[3] - '0';

			uint idx0	= GetIndex(i0, i1, i2, i3);
			uint idx1	= GetIndex(i1, i2, i3, i0);
			uint idx2	= GetIndex(i2, i3, i0, i1);
			uint idx3	= GetIndex(i3, i0, i1, i2);
			byte height = GetHeight(i0, i1, i2, i3);

			AddEntry(idx0, model, height, 0.0f);
			AddEntry(idx1, model, height, -90.0f);
			AddEntry(idx2, model, height, -180.0f);
			AddEntry(idx3, model, height, 90.0f);
		}
	}
	mInitialized = true;
}

//============================================================================================================
// Create the terrain
//============================================================================================================

void OSTiledTerrain::Create()
{
	if (Game::World->IsValid())
	{
		_Create();
	}
}

//============================================================================================================
// INTERNAL: Create all terrain tiles
//============================================================================================================

void OSTiledTerrain::_Create()
{
	uint width  = Game::World->GetWidth();
	uint height = Game::World->GetHeight();

	for (uint y = height; y > 1; )
	{
		uint y1 = --y;

		for (uint x = width; x > 1; )
		{
			_Create(--x, y1);
		}
	}
}

//============================================================================================================
// INTERNAL: Creates a single column of tiles at the specified position
//============================================================================================================

void OSTiledTerrain::_Create (uint x, uint y)
{
	uint width  = Game::World->GetWidth();
	uint height = Game::World->GetHeight();

	// Tiles are 2x2x1 and are centered at the middle of XY
	Vector3f pos (2.0f * x - width, 2.0f * y - height, 0.0f);

	uint yw0 = (y - 1) * width;
	uint yw1 = y * width;
	uint bl  = yw0 + (x - 1);
	uint br  = yw0 + x;
	uint tr  = yw1 + x;
	uint tl  = yw1 + (x - 1);

	Corners corners ((byte)Game::World->GetDepth(bl),
					 (byte)Game::World->GetDepth(br),
					 (byte)Game::World->GetDepth(tr),
					 (byte)Game::World->GetDepth(tl));

	// Get the lowest and highest bytes
	byte lowest, highest;
	corners.GetLowHigh(lowest, highest);

	if (lowest == highest)
	{
		// If the tile is completely flat, just place a flat tile
		pos.z = corners.a;
		_Create(pos, Corners(1, 1, 1, 1));
	}
	else
	{
		// Adjust the sampled points so they start at '1'
		byte offset = (lowest - 1);

		for(;;)
		{
			// Currently sampled values
			Corners current (corners - offset);

			// Always useful to have an escape clause!
			if (!current.IsValid()) break;

			bool zero	= (current == 0);
			bool one	= (current == 1);
			bool two	= (current == 2);
			bool three	= (current == 3);
			bool four	= (current == 4);
			bool max	= (current  > 4);

			// '1' can't exist next to '0'. Diagonally is fine, however.
			if ((current.a == 0 || current.c == 0) && (current.b == 1 || current.d == 1)) { ++offset; continue; }
			if ((current.a == 1 || current.c == 1) && (current.b == 0 || current.d == 0)) { ++offset; continue; }

			// The piece's height is always based on the lowest corner's height
			if (current.mVal == 0x01010101)
			{
				// The only piece that doesn't have a height is the flat piece
				pos.z = (float)offset;
			}
			else
			{
				// All other pieces have a height of 1 or more
				pos.z = (float)(offset + 1);
			}

			byte retVal (0);

			if (two)
			{
				if (three)
				{
					if (four)
					{
						// Example: 1234
						current.Cap(4);
						retVal = _Create(pos, current);
					}
					else
					{
						// Example: 1239
						current.Cap(3);
						retVal = _Create(pos, current);
					}
				}
				else
				{
					// Example: 1299
					current.Cap(2);
					retVal = _Create(pos, current);
				}
			}

			if (retVal == 0)
			{
				if (one)
				{
					// Example: 1999
					current.Cap(1);
					retVal = _Create(pos, current);
				}
				else
				{
					// Example: 0909
					current.Cap(0);
					retVal = _Create(pos, current);
				}
			}

			if (retVal == 0)
			{
				System::Log("WARNING: Missing piece: %u", current.ToIndex());
				retVal = 1;
			}
			offset += retVal;
		}
	}
}

//============================================================================================================
// INTERNAL: Creates a single tile at the specified position. Returns the tile's height.
//============================================================================================================

byte OSTiledTerrain::_Create (const Vector3f& pos, const Corners& tile)
{
	TileEntry* ent = mEntries.GetIfExists(tile.ToIndex());

	if (ent != 0)
	{
		ModelInstance* inst = mObject->AddObject<ModelInstance>(String("%.0f %.0f %.0f", pos.x, pos.y, pos.z));

		inst->SetRelativePosition(pos);
		inst->SetRelativeRotation( Quaternion(0.0f, 0.0f, DEG2RAD(ent->rotation)) );
		inst->SetModel(ent->model);
		inst->Update();
		inst->SetDirty();
		inst->SetLayer(1);
		inst->SetSerializable(false);

		Model::Limbs& limbs = ent->model->GetAllLimbs();

		FOREACH(i, limbs)
		{
			Limb* limb = limbs[i];
			limb->SetMaterial(mMat);
		}

		return ent->height;
	}
	return 0;
}

//============================================================================================================
// Remember the terrain object
//============================================================================================================

void OSTiledTerrain::OnInit()
{
	Game::Terrain = mObject;
}

//============================================================================================================

void OSTiledTerrain::OnDestroy()
{
	if (Game::Terrain == mObject) Game::Terrain = 0;
}

//============================================================================================================
// Serialization -- Save
//============================================================================================================

void OSTiledTerrain::OnSerializeTo (TreeNode& node) const
{
	if (mMat != 0) node.AddChild("Material", mMat->GetName());
	node.AddChild("Tiles", mTiles);
}

//============================================================================================================
// Serialization -- Load
//============================================================================================================

void OSTiledTerrain::OnSerializeFrom (const TreeNode& node)
{
	if (node.mTag == "Material")
	{
		mMat = mObject->GetGraphics()->GetMaterial(node.mValue.AsString());
	}
	else if (node.mTag == "Tiles")
	{
		// Don't allow initialization of tiles more than once. This just makes the code easier.
		if (mTiles.IsEmpty() && node.mValue.IsUShortArray())
		{
			SetTiles(node.mValue.AsUShortArray());
		}
	}
}