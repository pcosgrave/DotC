//============================================================================================================
//  Defense of the Citadel, Copyright (c) 2010 Michael Lyashenko and Philip Cosgrave. All rights reserved.
//											www.nextrevision.com
//============================================================================================================

#include "../../Other/Include/_All.h"
using namespace R5;

//============================================================================================================
// Scans the name of the tile for its indexed XYZ positioned
//============================================================================================================

bool OSTerrainTile::_Rescan()
{
	uint x, y, z;

	if (sscanf(mObject->GetName().GetBuffer(), "%u %u %u", &x, &y, &z) == 3)
	{
		mRescan = false;
		mPos.Set(x, y, z);
		return true;
	}
	return false;
}

//============================================================================================================
// Updates the current selection
//============================================================================================================

bool OSTerrainTile::OnKeyPress (const Vector2i& pos, byte key, bool isDown)
{
	if (!mRescan || _Rescan())
	{
		if (key == Key::MouseLeft)
		{
			if (!isDown)
			{
				// The position is offset slightly because the terrain tiles lie directly on the floor
				Vector3f myPos (mObject->GetAbsolutePosition());
				myPos += 0.25f;

				Object* target = 0;
				OSEntity* entity = 0;

				// All static objects should be selectable by clicking on the ground below them
				FOREACH(i, Game::Entities)
				{
					entity = Game::Entities[i];

					if (entity->GetOwner()->GetAbsoluteBounds().Contains(myPos))
					{
						target = entity->GetOwner();
						break;
					}
				}

				// Try to delegate selection to objects on top of the terrain
				if (target != 0)
				{
					// Try to get the object's selectable script, if it has one (NPCs do)
					OSSelectable* sel = target->GetScript<OSSelectable>();
					if (sel != 0) return sel->OnKeyPress(pos, key, isDown);

					// No special script -- this must be a generic object
					if (Game::Selected != target) target->AddScript<OSSelected>()->Show(2.0f, 1.5f, 1.0f);
				}
				else if (Game::Selected != mObject)
				{
					// No object on top of the terrain -- select the terrain itself
					mObject->AddScript<OSSelected>()->Show(mColor.r, mColor.g, mColor.b);
				}
			}
			return true;
		}
		else if (key == Key::T)
		{
			if (!isDown) 
			{
				uint type = Entity::Type::Static + (ushort)(Game::Random.GenerateUint() % 3);
				float rot = Game::Random.GenerateFloat() * TWOPI;
				float scale = 1.0f + 0.15f * Game::Random.GenerateFloat();
				Game::Server->AddEntity(type, mPos, rot, scale, Vector3i(), Vector3i());
			}
			return true;
		}
		else if (key == Key::P)
		{
			if (!isDown)
			{
				float rot = Game::Random.GenerateFloat() * TWOPI;
				Game::Server->AddEntity(Entity::Type::Player, mPos, rot, 1.0f, Vector3i(), Vector3i());
			}
			return true;
		}
		else if (key == Key::O)
		{
			if (!isDown) 
			{
				float rot = Game::Random.GenerateFloat() * TWOPI;
				Game::Server->AddEntity(Entity::Type::Enemy, mPos, rot, 1.0f, Vector3i(), Vector3i());
			}
			return true;
		}
		else if (key == Key::G)
		{
			if (!isDown)
			{
				uint id = World::GetControlledID();
				if (id != 0) Game::Server->SetObjective(id, Entity::Objective::Move, mPos);
			}
			return true;
		}
		else if (key == Key::S)
		{
			if (!isDown)
			{
				uint id = World::GetControlledID();
				if (id != 0) Game::Server->SetObjective(id, Entity::Objective::None, mPos);
			}
			return true;
		}
		else if (key == Key::D)
		{
			if (!isDown)
			{
				Game::Server->SetObjective(World::GetControlledID(), Entity::Objective::Defend, mPos);
			}
			return true;
		}
		else if (key == Key::Grave)
		{
			if (!isDown)
			{
				Game::Server->SetObjective(World::GetControlledID(), Entity::Objective::Test, mPos);
			}
			return true;
		}
	}
	else
	{
		Debug::LogError("Expected the node's name to be in format of 'Node # # #'");
		ASSERT(false, "Invalid tile name");
	}
	return false;
}