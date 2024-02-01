#pragma once

//============================================================================================================
//  Defense of the Citadel, Copyright (c) 2010 Michael Lyashenko and Philip Cosgrave. All rights reserved.
//											www.nextrevision.com
//============================================================================================================
// Keeps track of the model on the watched object, making sure current object has the same one
//============================================================================================================

class OSCopyModel : public Script
{
	ModelInstance* mAttachment;
	ModelInstance* mWatched;

	OSCopyModel() : mAttachment(0), mWatched(0) {}

	void SetWatchedInstance (ModelInstance* inst) { mWatched = inst; }

public:

	R5_DECLARE_INHERITED_CLASS("OSCopyModel", OSCopyModel, Script, Script);

	// Copies the specified attachment
	static void CopyAttachment (Object* myRoot, Object* remoteRoot, const String& handName);

	virtual void OnInit();
	virtual void OnUpdate();
};