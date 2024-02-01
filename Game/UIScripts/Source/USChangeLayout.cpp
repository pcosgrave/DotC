//============================================================================================================
//  Defense of the Citadel, Copyright (c) 2010 Michael Lyashenko and Philip Cosgrave. All rights reserved.
//											www.nextrevision.com
//============================================================================================================

#include "../../Other/Include/_All.h"
using namespace R5;

//============================================================================================================

void USChangeLayout::OnKeyPress (const Vector2i& pos, byte key, bool isDown)
{
	if (mLayout.IsValid() && !isDown && key == Key::MouseLeft && mWidget->GetRegion().Contains(pos))
	{
		USRoot::Get(mWidget)->ChangeLayout(mLayout);
	}
}

//============================================================================================================

void USChangeLayout::OnSerializeTo (TreeNode& root) const
{
	root.AddChild("Layout", mLayout);
}

//============================================================================================================

void USChangeLayout::OnSerializeFrom (const TreeNode& node)
{
	if (node.mTag == "Layout") mLayout = node.mValue.AsString();
}