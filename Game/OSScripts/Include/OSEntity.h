#pragma once

//============================================================================================================
//  Defense of the Citadel, Copyright (c) 2010 Michael Lyashenko and Philip Cosgrave. All rights reserved.
//											www.nextrevision.com
//============================================================================================================
// Generic entity script that contains an ID and responds to key events
//============================================================================================================

class OSEntity : public Script
{
protected:

	uint mId;
	ushort mType;

public:

	R5_DECLARE_INHERITED_CLASS("OSEntity", OSEntity, Script, Script);

	OSEntity() : mId(0), mType(0) {}

	// Initialization function
	void Set (uint val, ushort type) { mId = val; mType = type; }

	// Sets the entity's ID
	uint GetID() const { return mId; }

	// Entity's associated type
	ushort GetType() const { return mType; }

	// Whether the entity can be used (lever, door, etc)
	bool IsUsable() const { return (mType >= Entity::Type::Usable) && (mType < Entity::Type::Player); }

	// Returns the server position of the object
	Vector3i GetServerPosition() const;

	// Entities must add themselves to the Game entity list
	virtual void OnInit();
	virtual void OnDestroy();

	// Handles key events
	virtual bool OnKeyPress (const Vector2i& pos, byte key, bool isDown);
};