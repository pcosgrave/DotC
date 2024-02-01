//============================================================================================================
//  Defense of the Citadel, Copyright (c) 2010 Michael Lyashenko and Philip Cosgrave. All rights reserved.
//											www.nextrevision.com
//============================================================================================================

#include "../../Other/Include/_All.h"
using namespace R5;

//============================================================================================================
// Grow or shrink the widget as necessary
//============================================================================================================

void USGrowOnHover::OnUpdate (bool areaChanged)
{
	UIButton* btn = R5_CAST(UIButton, mWidget);
	if (btn != 0 && !btn->GetState(UIButton::State::Enabled)) return;

	UIRegion& r = mWidget->GetRegion();
	mTarget = (mWidget->GetUI()->GetHoverArea() == mWidget) ? mAmount : 0.0f;

	if (mCurrent != mTarget)
	{
		// If we're starting the process, save the corners
		if (mCurrent == 0.0f)
		{
			mLeft	= r.GetLeft();
			mRight	= r.GetRight();
			mTop	= r.GetTop();
			mBottom = r.GetBottom();
		}

		// Calculate the interpolated size
		float factor = Time::GetDelta() * 5.0f;
		mCurrent = Interpolation::Linear(mCurrent, mTarget, (factor < 1.0f) ? factor : 1.0f);

		// Set the adjusted region
		r.SetLeft	(mLeft.mRelative,	mLeft.mAbsolute		- mCurrent);
		r.SetRight	(mRight.mRelative,	mRight.mAbsolute	+ mCurrent);
		r.SetTop	(mTop.mRelative,	mTop.mAbsolute		- mCurrent);
		r.SetBottom	(mBottom.mRelative, mBottom.mAbsolute	+ mCurrent);
	}
}

//============================================================================================================
// Serialization -- Save
//============================================================================================================

void USGrowOnHover::OnSerializeTo (TreeNode& root) const
{
	root.AddChild("Amount", mAmount);
}

//============================================================================================================
// Serialization -- Load
//============================================================================================================

void USGrowOnHover::OnSerializeFrom (const TreeNode& node)
{
	if (node.mTag == "Amount") node.mValue >> mAmount;
}