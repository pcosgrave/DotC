#pragma once

//============================================================================================================
//  Defense of the Citadel, Copyright (c) 2010 Michael Lyashenko and Philip Cosgrave. All rights reserved.
//											www.nextrevision.com
//============================================================================================================
// Scripts that initializes and registers all 3D-related game components
//============================================================================================================

class OS3DGame : public Script
{
	// List of in-game tiles (invisible objects)
	Array<Object*>	mObjects;
	Object*			mNodeRoot;
	Object*			mEntityRoot;
	Object*			mTerrain;
	Object*			mSurface;

public:

	R5_DECLARE_INHERITED_CLASS("OS3DGame", OS3DGame, Script, Script);

	OS3DGame() : mNodeRoot(0), mEntityRoot(0), mTerrain(0), mSurface(0) {}

	// Initialize the game to be played in 3D
	virtual void OnInit();

	// Remove the registered listeners
	virtual void OnDestroy();

	// Update function waits until everything finishes loading, then creates the terrain
	virtual void OnUpdate();

public:

	// Retrieves the specified entity
	ModelInstance* GetEntity (uint id, bool createIfMissing = false, bool threadSafe = true);

	// Helper function that calculates the center position of the specified node
	static Vector3f GetGamePos (const Vector3i& v);

	// Helper function that converts game coordinates to server coordinates
	static Vector3i GetServerPos (const Vector3f& v);

private:

	// Changes the terrain
	void OnModifyHeightmap (const Packet::ModifyHeightmap& packet, const byte* data);

	// Callback triggered when an entity gets added
	void OnAddEntity (const Entity& ent);

	// Callback triggered when an entity's objective changes
	void OnUpdateEntity (const Packet::UpdateEntity& packet);

	// Callback triggered when a combat message arrives
	void OnCombatMessage (const Packet::CombatMessage& packet);

	// Callback triggered when entity's stats get updated
	void OnUpdateStats (const Packet::UpdateStats& packet);

	// Callback triggered when an entity gets removed
	void OnRemoveEntity (uint id);
};