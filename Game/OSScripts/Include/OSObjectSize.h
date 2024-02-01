#pragma once

//============================================================================================================
//  Defense of the Citadel, Copyright (c) 2010 Michael Lyashenko and Philip Cosgrave. All rights reserved.
//											www.nextrevision.com
//============================================================================================================
// Defines a object that is placeable and the placement rules.
//============================================================================================================

class OSObjectSize : public Script
{
private:
	Vector4f mSize; // The size of the object (minX, minY, maxX, maxY)

public:

	R5_DECLARE_INHERITED_CLASS("OSObjectSize", OSObjectSize, Script, Script);

	OSObjectSize() : mSize (0.0f) { }

protected:

	virtual void OnSerializeFrom(const TreeNode& node);
	virtual void OnSerializeTo(TreeNode& node);

	//============================================================================================================
public:

	// Size the object takes up on server space (from the origin)
	const Vector4f& GetServerSize () const { return mSize; }
	const Vector4f GetWorldSize() const;
};
