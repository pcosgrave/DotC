//============================================================================================================
//  Defense of the Citadel, Copyright (c) 2010 Michael Lyashenko and Philip Cosgrave. All rights reserved.
//											www.nextrevision.com
//============================================================================================================

#include "../../Other/Include/_All.h"
using namespace R5;

//============================================================================================================
// Returns the server position of the object
//============================================================================================================

Vector3i OSEntity::GetServerPosition() const
{
	const Vector3f& pos = mObject->GetAbsolutePosition();
	return OS3DGame::GetServerPos(pos);
}

//============================================================================================================
// Entities must add themselves to the Game entity list
//============================================================================================================

void OSEntity::OnInit()		{ Game::Entities.Expand() = this; }
void OSEntity::OnDestroy()	{ Game::Entities.Remove(this); }

//============================================================================================================
// Handles key events
//============================================================================================================

bool OSEntity::OnKeyPress (const Vector2i& pos, byte key, bool isDown)
{
	if (mId != 0)
	{
		if (key == Key::Delete || key == Key::Backspace)
		{
			if (!isDown) Game::Server->RemoveEntity(mId);
			return true;
		}
		else if (key == Key::F)
		{
			if (!isDown) mObject->AddScript<OSCameraTarget>();
			return true;
		}
		else if (key == Key::U)
		{
			if (!isDown && IsUsable())
			{
				Game::Server->SetObjective(World::GetControlledID(), Entity::Objective::Use, mId);
			}
			return true;
		}
		else if (key == Key::G)
		{
			if (!isDown)
			{
				uint id = World::GetControlledID();

				if (id != mId)
				{
					Game::Server->SetObjective(id, Entity::Objective::Move,
						OS3DGame::GetServerPos(mObject->GetAbsolutePosition()));
				}
			}
		}
		else if (key == Key::S)
		{
			// 'Stop' command always affects only the selected entity
			if (!isDown) Game::Server->SetObjective(mId, Entity::Objective::None);
			return true;
		}
	}
	return false;
}