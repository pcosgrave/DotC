#pragma once

//============================================================================================================
//  Defense of the Citadel, Copyright (c) 2010 Michael Lyashenko and Philip Cosgrave. All rights reserved.
//											www.nextrevision.com
//============================================================================================================
// Defines a object that is placeable and the placement rules.
//============================================================================================================

class USStartBuilding : public UIScript
{
	String mOriginalText; // The original text of the owner

public:

	R5_DECLARE_INHERITED_CLASS("USStartBuilding", USStartBuilding, UIScript, UIScript);

protected:

	virtual void OnInit();
	virtual void OnValueChange();
};
