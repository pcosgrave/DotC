#pragma once

//============================================================================================================
//  Defense of the Citadel, Copyright (c) 2010 Michael Lyashenko and Philip Cosgrave. All rights reserved.
//											www.nextrevision.com
//============================================================================================================
// Current controlled in-game target
//============================================================================================================

class OSControlled : public OSSelection
{
public:

	R5_DECLARE_INHERITED_CLASS("OSControlled", OSControlled, OSSelection, Script);

protected:

	virtual const char* GetObjectName()  const { return "OSControlled Projector"; }
	virtual const char* GetTextureName() const { return "Textures/Selection/Controlled.png"; }
	virtual float		GetRadius()		 const { return 0.65f; }

	virtual void OnInit();
	virtual void OnDestroy();
	virtual void OnUpdate();
};