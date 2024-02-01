#pragma once

//============================================================================================================
//  Defense of the Citadel, Copyright (c) 2010 Michael Lyashenko and Philip Cosgrave. All rights reserved.
//											www.nextrevision.com
//============================================================================================================
// Updates the preview window's map picture pointer
//============================================================================================================

class USServerDescription : public UIScript
{
public:

	R5_DECLARE_INHERITED_CLASS("USServerDescription", USServerDescription, UIScript, UIScript);

	virtual void OnInit()
	{
		USServerDetails* det = mWidget->GetParent()->GetParent()->GetScript<USServerDetails>();
		if (det != 0) det->SetDesc(R5_CAST(UITextArea, mWidget));
	}

	virtual void OnDestroy()
	{
		USServerDetails* det = mWidget->GetParent()->GetParent()->GetScript<USServerDetails>();
		if (det != 0) det->SetDesc(0);
	}
};