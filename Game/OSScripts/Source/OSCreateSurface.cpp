//============================================================================================================
//  Defense of the Citadel, Copyright (c) 2010 Michael Lyashenko and Philip Cosgrave. All rights reserved.
//											www.nextrevision.com
//============================================================================================================

#include "../../Other/Include/_All.h"
using namespace R5;

//============================================================================================================
// Create a selectable tile at the specified XYZ
//============================================================================================================

void OSCreateSurface::_Create (uint x, uint y, uint width, uint height)
{
	// TODO: Add support for various sections
	ushort z = Game::World->GetDepth(x + y * width);

	Vector3f center (OS3DGame::GetGamePos(Vector3i(x, y, z)));

	Bounds bounds;
	bounds.Include( Vector3f(-1.0f, -1.0f, -1.0f) );
	bounds.Include( Vector3f( 1.0f,  1.0f,  0.0f) );

	Object* obj = mObject->AddObject<Object>(String("%u %u %u", x, y, z));
	obj->SetAbsolutePosition(center);
	obj->SetRelativeBounds(bounds);
	obj->AddScript<OSTerrainTile>()->SetColor(1.5f, 1.5f, 1.5f);
	obj->AddScript<OSClickToFollow>();
	obj->SetSerializable(false);
	//obj->SetShowOutline(true);
}

//============================================================================================================
// Create all surface tiles
//============================================================================================================

void OSCreateSurface::Create()
{
	if (Game::World->IsValid())
	{
		uint width  = Game::World->GetWidth();
		uint height = Game::World->GetHeight();

		for (uint y = 1, ymax = height - 1; y < ymax; ++y)
		{
			for (uint x = 1, xmax = width - 1; x < xmax; ++x)
			{
				_Create(x, y, width, height);
			}
		}
	}
}