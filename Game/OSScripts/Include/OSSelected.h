#pragma once

//============================================================================================================
//  Defense of the Citadel, Copyright (c) 2010 Michael Lyashenko and Philip Cosgrave. All rights reserved.
//											www.nextrevision.com
//============================================================================================================
// Current in-game selection that will be receiving events
//============================================================================================================

class OSSelected : public OSSelection
{
public:

	R5_DECLARE_INHERITED_CLASS("OSSelected", OSSelected, OSSelection, Script);

protected:

	virtual const char* GetObjectName()  const { return "OSSelected Projector"; }
	virtual const char* GetTextureName() const { return "Textures/Selection/Selected.png"; }
	virtual float		GetRadius()		 const { return 1.0f; }

	virtual void OnInit();
	virtual void OnDestroy();
	virtual void OnUpdate();
};