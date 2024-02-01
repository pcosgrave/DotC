//============================================================================================================
//  Defense of the Citadel, Copyright (c) 2010 Michael Lyashenko and Philip Cosgrave. All rights reserved.
//											www.nextrevision.com
//============================================================================================================

#include "../../Other/Include/_All.h"
using namespace R5;

//============================================================================================================
// Gets the root of the specified widget
//============================================================================================================

USRoot* USRoot::Get (UIWidget* widget)
{
	if (widget != 0)
	{
		USRoot* root = widget->GetScript<USRoot>();

		while (root == 0 && widget->GetParent() != 0)
		{
			widget = widget->GetParent();
			root   = widget->GetScript<USRoot>();
		}
		return (root != 0) ? root : widget->AddScript<USRoot>();
	}
	return 0;
}

//============================================================================================================
// Changes the currently active UI layout
//============================================================================================================

void USRoot::ChangeLayout (const String& layout)
{
	if (mLayout != layout)
	{
		mLayout = layout;

		UIWidget::Children& children = mWidget->GetAllChildren();

		FOREACH(i, children)
		{
			UIWidget* child = children[i];
			child->AddScript<USFadeOut>()->SetDestroyWhenDone(true);
		}

		TreeNode root;

		if (root.Load(layout))
		{
			TreeNode* ui = root.FindChild("UI", false);

			if (ui != 0)
			{
				TreeNode* node = ui->FindChild("Layout", false);
				if (node != 0) mWidget->SerializeFrom(*node);
			}
			else
			{
				mWidget->SerializeFrom(root);
			}
		}
	}
}

//============================================================================================================

void USRoot::OnSerializeTo (TreeNode& root) const
{
	root.AddChild("Layout", mLayout);
}

//============================================================================================================

void USRoot::OnSerializeFrom (const TreeNode& node)
{
	if (node.mTag == "Layout")
	{
		ChangeLayout(node.mValue.AsString());
	}
}