#pragma once

//============================================================================================================
//  Defense of the Citadel, Copyright (c) 2010 Michael Lyashenko and Philip Cosgrave. All rights reserved.
//											www.nextrevision.com
//============================================================================================================
// Fade the widget in over the duration
//============================================================================================================

class USGrowOnHover : public UIScript
{
protected:

	float mAmount;
	float mCurrent;
	float mTarget;

	UIAnchor mLeft;
	UIAnchor mRight;
	UIAnchor mTop;
	UIAnchor mBottom;

public:

	R5_DECLARE_INHERITED_CLASS("USGrowOnHover", USGrowOnHover, UIScript, UIScript);

	USGrowOnHover() : mAmount(2.0f), mCurrent(0.0f), mTarget(0.0f) {}

	// The amount by which the widget will grow on mouse over
	void SetGrowAmount (float amount) { mAmount = amount; }
	float GetGrowAmount() const { return mAmount; }

	// Grow or shrink the widget as necessary
	virtual void OnUpdate(bool areaChanged);

	// Serialization
	virtual void OnSerializeTo (TreeNode& root) const;
	virtual void OnSerializeFrom (const TreeNode& node);
};
