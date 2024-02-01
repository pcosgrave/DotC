//============================================================================================================
//  Defense of the Citadel, Copyright (c) 2010 Michael Lyashenko and Philip Cosgrave. All rights reserved.
//											www.nextrevision.com
//============================================================================================================

#include "../../Other/Include/_All.h"
using namespace R5;

//============================================================================================================
// State change notification
//============================================================================================================

void USToggleWidget::OnStateChange (uint state, bool isSet)
{
	if (state == UIButton::State::Pressed && isSet)
	{
		UIWidget* target = GetUI()->FindWidget<UIWidget>(mTarget);

		if (target != 0)
		{
			target->SetAlpha( target->GetAlpha() < 1.0f ? 1.0f : 0.0f, 0.25f );
		}
	}
}

//============================================================================================================
// Serialization -- Save
//============================================================================================================

void USToggleWidget::OnSerializeTo (TreeNode& root) const
{
	root.AddChild("Target", mTarget);
}

//============================================================================================================
// Serialization -- Load
//============================================================================================================

void USToggleWidget::OnSerializeFrom (const TreeNode& node)
{
	if (node.mTag == "Target")
	{
		mTarget = node.mValue.AsString();
	}
}