#pragma once

//============================================================================================================
//  Defense of the Citadel, Copyright (c) 2010 Michael Lyashenko and Philip Cosgrave. All rights reserved.
//											www.nextrevision.com
//============================================================================================================
// Identifies the widget it's attached to as the virtual root of the UI
//============================================================================================================

class USRoot : public UIScript
{
	String mLayout;

public:

	R5_DECLARE_INHERITED_CLASS("USRoot", USRoot, UIScript, UIScript);

	// Gets the root of the specified widget
	static USRoot* Get (UIWidget* widget);

	// Changes the current UI layout
	void ChangeLayout (const String& layout);

	// Serialization allows to specify the starting active layout right away
	virtual void OnSerializeTo (TreeNode& root) const;
	virtual void OnSerializeFrom (const TreeNode& node);
};