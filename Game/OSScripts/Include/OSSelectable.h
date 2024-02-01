#pragma once

//============================================================================================================
//  Defense of the Citadel, Copyright (c) 2010 Michael Lyashenko and Philip Cosgrave. All rights reserved.
//											www.nextrevision.com
//============================================================================================================
// When attached to an object it will make this object selectable
//============================================================================================================

class OSSelectable : public Script
{
protected:

	Color3f mColor;

public:

	R5_DECLARE_INHERITED_CLASS("OSSelectable", OSSelectable, Script, Script);

	// Selection color
	const Color3f& GetColor() const { return mColor; }
	void SetColor (const Color3f& color) { mColor = color; }
	void SetColor (float r, float g, float b) { mColor.Set(r, g, b); }

	// In order for selection to work, a box collider must be present
	virtual void OnInit() { mObject->SetFlag(Object::Flag::BoxCollider, true); }

	// Updates the current selection
	virtual bool OnKeyPress (const Vector2i& pos, byte key, bool isDown);
};