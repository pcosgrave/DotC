#pragma once

//============================================================================================================
//  Defense of the Citadel, Copyright (c) 2010 Michael Lyashenko and Philip Cosgrave. All rights reserved.
//											www.nextrevision.com
//============================================================================================================
// Updates the preview window's map picture pointer
//============================================================================================================

class USServerInfo : public UIScript
{
public:

	R5_DECLARE_INHERITED_CLASS("USServerInfo", USServerInfo, UIScript, UIScript);

	virtual void OnInit()
	{
		USServerDetails* det = mWidget->GetParent()->GetParent()->GetScript<USServerDetails>();
		if (det != 0) det->SetInfo(R5_CAST(UITextArea, mWidget));
	}

	virtual void OnDestroy()
	{
		USServerDetails* det = mWidget->GetParent()->GetParent()->GetScript<USServerDetails>();
		if (det != 0) det->SetInfo(0);
	}
};