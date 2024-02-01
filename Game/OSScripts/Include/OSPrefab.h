#pragma once

//============================================================================================================
//  Defense of the Citadel, Copyright (c) 2010 Michael Lyashenko and Philip Cosgrave. All rights reserved.
//											www.nextrevision.com
//============================================================================================================
// Facilitates the creation of a new uniquely animated prefab
//============================================================================================================

class OSPrefab : public Script
{
	ModelInstance*	mInst;		// Model instance this script belongs to
	Model*			mModel;		// Model created and controlled by this script

	Thread::Lockable mLock;

public:

	R5_DECLARE_INHERITED_CLASS("OSPrefab", OSPrefab, Script, Script);

	OSPrefab() : mInst(0), mModel(0) {}
	virtual ~OSPrefab() {}

	// Thread safety
	void Lock()		{ mLock.Lock(); }
	void Unlock()	{ mLock.Unlock(); }

	// Set the model template, creating a new instance of that model
	void SetTemplate (const String& name, bool unique = true);

	// Ensures that the script is attached to a ModelInstance
	virtual void OnInit();

	// Destroy the associated model as well
	virtual void OnDestroy();
};