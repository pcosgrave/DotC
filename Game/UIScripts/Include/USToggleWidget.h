#pragma once

//============================================================================================================
//  Defense of the Citadel, Copyright (c) 2010 Michael Lyashenko and Philip Cosgrave. All rights reserved.
//											www.nextrevision.com
//============================================================================================================
// UI script that shows or hides the specified widget
//============================================================================================================

class USToggleWidget : public UIScript
{
	String mTarget;

public:

	R5_DECLARE_INHERITED_CLASS("USToggleWidget", USToggleWidget, UIScript, UIScript);

	const String& GetTarget() const { return mTarget; }
	void SetTarget (const String& target) { mTarget = target; }

	virtual void OnStateChange	(uint state, bool isSet);
	virtual void OnSerializeTo	(TreeNode& root) const;
	virtual void OnSerializeFrom(const TreeNode& node);
};