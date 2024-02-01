//============================================================================================================
//  Defense of the Citadel, Copyright (c) 2010 Michael Lyashenko and Philip Cosgrave. All rights reserved.
//											www.nextrevision.com
//============================================================================================================

#include "../../Other/Include/_All.h"
using namespace R5;

//============================================================================================================
// Serialize -- Save
//============================================================================================================

void OSObjectSize::OnSerializeTo(TreeNode& node)
{
	node.AddChild("Size", mSize);
}

//============================================================================================================
// Serialize -- Load
//============================================================================================================

void OSObjectSize::OnSerializeFrom(const TreeNode& node)
{
	if (node.mTag == "Size") node.mValue >> mSize;
}

//============================================================================================================
// Gets the size the object takes up in world space.
//============================================================================================================

const Vector4f OSObjectSize::GetWorldSize() const
{
	Vector4f newSize (mSize * 2);
	newSize.x -= 1;
	newSize.y -= 1;
	newSize.z += 1;
	newSize.w += 1;
	return newSize;
}