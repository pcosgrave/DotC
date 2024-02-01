#pragma once

//============================================================================================================
//  Defense of the Citadel, Copyright (c) 2010 Michael Lyashenko and Philip Cosgrave. All rights reserved.
//											www.nextrevision.com
//============================================================================================================
// The portrait for the controlled player.
//============================================================================================================

class USControlledPortrait : public USPortrait
{
protected:

	ModelInstance*	mModelInst;

	USControlledPortrait() : mModelInst(0) {}

public:

	R5_DECLARE_INHERITED_CLASS("USControlledPortrait", USControlledPortrait, USPortrait, UIScript);

	virtual void OnUpdate(bool areaChanged);
	virtual void OnDestroy();

protected:

	void UpdateModel(ModelInstance* remoteInst, bool areaChanged);
};
