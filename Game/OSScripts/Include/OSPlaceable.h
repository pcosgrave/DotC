#pragma once

//============================================================================================================
//  Defense of the Citadel, Copyright (c) 2010 Michael Lyashenko and Philip Cosgrave. All rights reserved.
//											www.nextrevision.com
//============================================================================================================
// Defines a object that is placeable and the placement rules.
//============================================================================================================

class OSPlaceable : public Script
{
private:

	Vector3f	mTargetPosition;
	float		mTargetRotation;
	float		mCurrentRotation;
	uint		mEntityType;
	Vector3f	mWorldOffset;
	Vector4f	mSize;
	bool		mUpdate;

public:

	R5_DECLARE_INHERITED_CLASS("OSPlaceable", OSPlaceable, Script, Script);

	OSPlaceable();

protected:

	virtual void OnInit		();
	virtual void OnDestroy	();
	virtual void OnPreUpdate();
	virtual bool OnMouseMove(const Vector2i& pos, const Vector2i& delta);
	virtual bool OnKeyPress (const Vector2i& pos, byte key, bool isDown);
	virtual bool OnScroll	(const Vector2i& pos, float delta);

public:

	void SetEntityType (uint type);

private:

	float GetMaxTerrainHeight(const Vector3i& serverPos, const Quaternion& rotation);
	bool  ValidatePlacement  (const Vector3i& serverPos, const Quaternion& rotation);
};
