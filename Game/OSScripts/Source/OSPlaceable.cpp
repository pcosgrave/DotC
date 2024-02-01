//============================================================================================================
//  Defense of the Citadel, Copyright (c) 2010 Michael Lyashenko and Philip Cosgrave. All rights reserved.
//											www.nextrevision.com
//============================================================================================================

#include "../../Other/Include/_All.h"
using namespace R5;

//============================================================================================================

OSPlaceable::OSPlaceable() :
	mTargetRotation (0.0f), 
	mCurrentRotation(0.0f),
	mEntityType(Entity::Type::None),
	mSize(0.0f),
	mUpdate(true) {}

//============================================================================================================
// Find the initial position for the building
//============================================================================================================

void OSPlaceable::OnInit()
{
	// Change the placeable target
	if (Game::Building != 0) Game::Building->DestroySelf();
	Game::Building = mObject;

	// Set the starting position to be the position of the mouse
	Array<RaycastHit>& hits = Game::Scene->Raycast(Game::Core->GetMousePos());
	if (hits.GetSize() > 0) mTargetPosition = hits[0].mObject->GetRelativePosition();

	// If the building has a custom size set, use it
	OSObjectSize* objSize = mObject->GetScript<OSObjectSize>();
	if (objSize != 0) mSize = objSize->GetServerSize();

	// World-space offset is the center (*0.5), multiplied by tile scale (*2)
	mWorldOffset.Set(mSize.x + mSize.z, mSize.y + mSize.w, 0.0f);

	// Start the building at the target position right away
	Game::Building->SetRelativePosition(mTargetPosition);

	// Cancel the camera target
	OSCameraTarget::Cancel();

	mObject->SubscribeToKeyPress(2000000);
	mObject->SubscribeToMouseMove(2000000);
	mObject->SubscribeToScroll(2000000);

	mObject->AddScript<OSStopParticleEffects>();
}

//============================================================================================================
// Set the Game::Building to null as this script is the only one that sets that variable
//============================================================================================================

void OSPlaceable::OnDestroy()
{
	if (Game::Building == mObject) Game::Building = 0;
}

//============================================================================================================
// Interpolate the position / rotation values
//============================================================================================================

void OSPlaceable::OnPreUpdate()
{
	if (Game::Building != 0)
	{
		if (mUpdate)
		{
			mUpdate = false;
		
			Array<RaycastHit>& hits = Game::Scene->Raycast(Game::Core->GetMousePos());

			if (hits.GetSize() > 0)
			{
				mTargetPosition = hits[0].mObject->GetRelativePosition();

				// TODO: When the building that's being placed gets changed to use a transparent material,
				// the following code should be used instead:
				//mTargetPosition = Game::DrawScript->GetMousePos();
				//mTargetPosition.z = hits[0].mObject->GetRelativePosition().z;
			}
		}

		float delta = Float::Min(1.0f, Time::GetDelta() * 9.0f);

		// We want the rotation to be in 90 degree intervals
		float targetRot = TWOPI * Float::Round(mTargetRotation, 0.25f);

		// Smooth interpolation for rotation
		mCurrentRotation = Interpolation::Linear(mCurrentRotation, targetRot, delta);

		// Rotate the visible building
		Quaternion rot;
		rot.SetFromEuler(Vector3f(0.0f, 0.0f, mCurrentRotation));
		Game::Building->SetRelativeRotation(rot);

		// Calculate the final rotation
		rot.SetFromEuler(Vector3f(0.0f, 0.0f, targetRot));

		// Calculate the final offset position
		Vector3f pos (mTargetPosition - mWorldOffset * rot);

		// Convert to server coordinates, choosing the closest node
		Vector3i serverPos (OS3DGame::GetServerPos(pos));

		// Convert back to world coordinates
		pos = OS3DGame::GetGamePos(serverPos);

		// TODO: use some other way of showing the bounding box of the building being placed. Creating a dummy
		// entity with Show Outline set to true and bounds set to the building's would work.
		pos.z = GetMaxTerrainHeight(serverPos, rot);

		// Interpolate for extra smoothness
		pos = Interpolation::Linear(Game::Building->GetRelativePosition(), pos, delta);
		Game::Building->SetRelativePosition(pos);

		// TODO: Use return value to visualize the validity of the placement
		ValidatePlacement(serverPos, rot);
	}
}

//============================================================================================================
// Change stage if the the key press counted as a click
//============================================================================================================

bool OSPlaceable::OnKeyPress (const Vector2i& pos, byte key, bool isDown)
{
	if (key == Key::MouseLeft)
	{
		static Vector2i lastPos = pos;

		if (isDown)
		{
			lastPos = pos;
		}
		else
		{
			Vector2i delta (lastPos - pos);

			if (delta.x < 0) delta.x = -delta.x;
			if (delta.y < 0) delta.y = -delta.y;

			// If the mouse has moved too much, don't count it as a click
			if (delta.x > 3 || delta.y > 3) return 1;

			if (mEntityType != Entity::Type::None)
			{
				// Server-side position
				Vector3f worldPos (Game::Building->GetAbsolutePosition());
				Vector3i serverPos (OS3DGame::GetServerPos(worldPos));
				serverPos.z = Game::World->GetDepth(serverPos);

				if (ValidatePlacement(serverPos, Game::Building->GetAbsoluteRotation()))
				{
					// Rotation around the vertical axis
					float angle = WrapAngle(TWOPI * Float::Round(mTargetRotation, 0.25f));

					Vector3i min  ( (ushort)Float::RoundToUInt(-mSize.x),
									(ushort)Float::RoundToUInt(-mSize.y), 0 );
					Vector3i size ( (ushort)Float::RoundToUInt(mSize.z - mSize.x),
									(ushort)Float::RoundToUInt(mSize.w - mSize.y), 0 );

					// NOTE: Server's rotation is backwards. Ugh. I'll need to fix that at some point.
					Game::Server->AddEntity(mEntityType, serverPos, -angle, 1.0f, min, size);
					Game::Building->DestroySelf();
				}
			}
		}
		return true;
	}
	else if (key == Key::MouseRight || key == Key::Escape)
	{
		Game::Building->DestroySelf();
		return true;
	}
	return false;
}

//============================================================================================================
// Update the position or rotation on mouse move
//============================================================================================================

bool OSPlaceable::OnMouseMove(const Vector2i& pos, const Vector2i& delta)
{
	mUpdate = true;
	return false;
}

//============================================================================================================
// Update the rotation of the building
//============================================================================================================

bool OSPlaceable::OnScroll(const Vector2i& pos, float delta)
{
	mTargetRotation += delta * 0.125f;
	return true;
}

//============================================================================================================
// Set the entity type that will be created when placed.
//============================================================================================================

void OSPlaceable::SetEntityType (uint type)
{
	mEntityType = type;
}

//============================================================================================================
// Find the highest point under the building.
// NOTE: This assumes all buildings are placed on the terrain. This won't work for underground!
//============================================================================================================

float OSPlaceable::GetMaxTerrainHeight(const Vector3i& serverPos, const Quaternion& rotation)
{
	Vector3f minSize (mSize.x, mSize.y, 0.0f);
	Vector3f maxSize (mSize.z, mSize.w, 0.0f);
	float maxHeight = Game::World->GetDepth(serverPos);

	minSize *= rotation;
	maxSize *= rotation;

	if (minSize.x > maxSize.x) Swap(minSize.x, maxSize.x);
	if (minSize.y > maxSize.y) Swap(minSize.y, maxSize.y);

	minSize.x = Float::Round(minSize.x);
	minSize.y = Float::Round(minSize.y);
	maxSize.x = Float::Round(maxSize.x);
	maxSize.y = Float::Round(maxSize.y);

	for (int i = Float::RoundToInt(minSize.x),
		  imax = Float::RoundToInt(maxSize.x); i <= imax; ++i)
	{
		for (int j = Float::FloorToInt(minSize.y),
			  jmax = Float::FloorToInt(maxSize.y); j <= jmax; ++j)
		{
			if (i == 0 && j == 0) continue;
			float currentHeight = Game::World->GetDepth(Vector3i(serverPos.x + i, serverPos.y + j, serverPos.z));
			if (currentHeight > maxHeight) maxHeight = currentHeight;
		}
	}
	return maxHeight;
}

//============================================================================================================
// Validate that the positions under the building do not differ by more then 1 in height.
//============================================================================================================

bool OSPlaceable::ValidatePlacement(const Vector3i& serverPos, const Quaternion& rotation)
{
	Vector3f minSize (mSize.x, mSize.y, 0.0f);
	Vector3f maxSize (mSize.z, mSize.w, 0.0f);
	bool validLocation = true;

	minSize *= rotation;
	maxSize *= rotation;

	if (minSize.x > maxSize.x) Swap(minSize.x, maxSize.x);
	if (minSize.y > maxSize.y) Swap(minSize.y, maxSize.y);

	minSize.x = Float::Round(minSize.x);
	minSize.y = Float::Round(minSize.y);
	maxSize.x = Float::Round(maxSize.x);
	maxSize.y = Float::Round(maxSize.y);

	for (int i = Float::RoundToInt(minSize.x),
		  imax = Float::RoundToInt(maxSize.x); i <= imax; ++i)
	{
		for (int j = Float::FloorToInt(minSize.y),
			  jmax = Float::FloorToInt(maxSize.y); j <= jmax; ++j)
		{
			float currentHeight = Game::World->GetDepth(Vector3i(serverPos.x + i, serverPos.y + j, serverPos.z));
			float heightLeft = Game::World->GetDepth(Vector3i(serverPos.x + i, serverPos.y + j + 1, serverPos.z));
			float heightDown = Game::World->GetDepth(Vector3i(serverPos.x + i + 1, serverPos.y + j, serverPos.z));

			if (Float::Abs(currentHeight - heightLeft) > 1) validLocation = false; 
			else if (Float::Abs(currentHeight - heightDown) > 1) validLocation = false;
		}
	}

	// If the location is still valid check if you are able to enter the building. (One tile to the south)
	if (validLocation)
	{
		// One tile south of the building needs to be open and semi level as well for the placement to be valid.
		Vector3f enterancePosition (0.0f, -1.0f, 0.0f);
		enterancePosition *= rotation;

		float currentHeight = Game::World->GetDepth(serverPos);
		float enteranceHeight = Game::World->GetDepth(
			Vector3i(serverPos.x + (int)enterancePosition.x, serverPos.y + (int)enterancePosition.y, serverPos.z));

		if (Float::Abs(currentHeight - enteranceHeight) > 1) validLocation = false;
	}
	return validLocation;
}
