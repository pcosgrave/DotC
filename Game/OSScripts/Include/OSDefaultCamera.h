#pragma once

//============================================================================================================
//  Defense of the Citadel, Copyright (c) 2010 Michael Lyashenko and Philip Cosgrave. All rights reserved.
//											www.nextrevision.com
//============================================================================================================
// Custom camera used by DotC -- it's a third person camera that follows the terrain's elevation
//============================================================================================================

class OSDefaultCamera : public Script
{
protected:

	Vector3f	mDolly;				// Distance from the origin: X = Min, Y = Current, Z = Max
	Vector3f	mTargetPos;			// Target position the camera is moving towards
	Vector2f	mPosMovement;		// XY camera movement (camera scrolling at the edges of the window)
	float		mRotMovement;		// Rotational movement (orbit)
	float		mDollyMovement;		// Current dolly movement
	bool		mHasMovement;		// Whether there is potential movement in progress
	Vector3f	mSavedPos;			// Saved relative position, restored in post-update
	Quaternion	mSavedRot;			// Saved relative rotation, restored in post-update
	float		mEyeHeight;

	OSDefaultCamera();

private:

	// Updates the rotation and dolly from the input-based movement
	bool _ApplyMovement (Quaternion& relativeRot);

public:

	R5_DECLARE_INHERITED_CLASS("OSDefaultCamera", OSDefaultCamera, Script, Script);

	// Dolly vector contains how close to the camera it can get (X), current value (Y), and maximum distance (Z)
	const Vector3f& GetDolly() const	{ return mDolly; }
	void SetDolly (const Vector3f& val) { mDolly = val; }
	void SetDolly (float val)			{ mDolly.y = Float::Clamp(val, mDolly.x, mDolly.z); }

	// Sets the camera's LookAt target
	void SetTarget (const Vector3f& v);

protected:

	// Respond to mouse movement
	uint MouseMove (const Vector2i& pos, const Vector2i& delta);
	uint Scroll (const Vector2i& pos, float delta);

	// Register and unregister mouse movement and scroll event listeners
	virtual void OnInit();
	virtual void OnDestroy();

	// Update functions differ as they need to use the dolly-offset relative position
	virtual void OnPreUpdate();
	virtual void OnPostUpdate();

	// Serialization
	virtual void OnSerializeTo	 (TreeNode& root) const;
	virtual void OnSerializeFrom (const TreeNode& root);
};