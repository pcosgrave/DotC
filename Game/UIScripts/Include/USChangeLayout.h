#pragma once

//============================================================================================================
//  Defense of the Citadel, Copyright (c) 2010 Michael Lyashenko and Philip Cosgrave. All rights reserved.
//											www.nextrevision.com
//============================================================================================================
// Script that facilitates UI layout changes
//============================================================================================================

class USChangeLayout : public UIScript
{
protected:

	String mLayout;

public:

	R5_DECLARE_INHERITED_CLASS("USChangeLayout", USChangeLayout, UIScript, UIScript);

	const String& GetLayout() const { return mLayout; }
	void SetLayout (const String& val) { mLayout = val; }

	virtual void OnKeyPress (const Vector2i& pos, byte key, bool isDown);
	virtual void OnSerializeTo (TreeNode& root) const;
	virtual void OnSerializeFrom (const TreeNode& node);
};