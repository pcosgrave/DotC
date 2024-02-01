//============================================================================================================
//  Defense of the Citadel, Copyright (c) 2010 Michael Lyashenko and Philip Cosgrave. All rights reserved.
//											www.nextrevision.com
//============================================================================================================

#include "../../Other/Include/_All.h"
using namespace R5;

//============================================================================================================
// Helper function that gets the terrain bounds-restricted position
//============================================================================================================

Vector3f GetRestrictedPos (Vector3f pos, float offset = 0.5f)
{
	float w = (float)Game::World->GetWidth();
	float h = (float)Game::World->GetHeight();

	float width  = (w > offset ? w - offset : 0.0f);
	float height = (h > offset ? h - offset : 0.0f);

	if		(pos.x >  width)	pos.x =  width;
	else if (pos.x < -width)	pos.x = -width;

	if		(pos.y >  height)	pos.y =  height;
	else if (pos.y < -height)	pos.y = -height;

	return pos;
}

//============================================================================================================
// Strangely enough placing this in the header file causes it to not recognize 'mDolly'
//============================================================================================================

OSDefaultCamera::OSDefaultCamera() :
	mDolly			(10.0f, 50.0f, 50.0f),
	mRotMovement	(0.0f),
	mDollyMovement	(0.0f),
	mHasMovement	(true),
	mEyeHeight		(1000.0f) {}

//============================================================================================================
// Updates the rotation and dolly from the input-based movement
//============================================================================================================

bool OSDefaultCamera::_ApplyMovement (Quaternion& relativeRot)
{
	uint delta = (uint)Time::GetDeltaMS();
	float rot (0.0f), dolly(0.0f);

	// Apply movement for each millisecond of change
	for (uint i = 0; i < delta; ++i)
	{
		rot		+= mRotMovement;
		dolly	+= mDollyMovement;

		mRotMovement	*= 0.97f;
		mDollyMovement	*= 0.99f;
	}

	// Determine if there has been any movement
	if (Float::IsNotZero(rot) || Float::IsNotZero(dolly))
	{
		// Horizontal rotation
		Vector3f dir  (relativeRot.GetForward());
		Vector3f flat (relativeRot.GetFlatForward());

		// Vertical angle
		float vt = Float::Acos(dir.Dot(flat));
		if (dir.z < 0.0f) vt = -vt;

		// Horizontal angle
		float hz = Float::Acos(flat.y);
		if (dir.x > 0.0f) hz = -hz;

		// Adjusted rotation
		relativeRot.SetFromEuler( Vector3f(vt, 0.0f, hz + rot) );

		// Dolly
		mDolly.y = Float::Clamp(mDolly.y + dolly, mDolly.x, mDolly.z);
		mObject->SetDirty();
		return true;
	}
	return false;
}

//============================================================================================================
// Sets the camera's LookAt target
//============================================================================================================

void OSDefaultCamera::SetTarget (const Vector3f& v)
{
	mTargetPos = GetRestrictedPos(v, 7.0f);
}

//============================================================================================================
// Respond to mouse movement
//============================================================================================================

uint OSDefaultCamera::MouseMove (const Vector2i& pos, const Vector2i& delta)
{
	if (Game::Core->IsKeyDown(Key::MouseLeft))
	{
		mPosMovement.Set(0.0f, 0.0f);
		mHasMovement = true;
		mRotMovement -= delta.x * 0.0001f * Game::Sensitivity;
		return 1;
	}
	else if (Game::Core->IsKeyDown(Key::MouseRight))
	{
		OSCameraTarget::Cancel();
		mPosMovement.Set(0.0f, 0.0f);

		const Quaternion& rot (mObject->GetAbsoluteRotation());
		Vector2f fw (rot.GetFlatForward());
		Vector2f rt (rot.GetFlatRight());

		rt *= (float)delta.x;
		fw *= (float)(-delta.y);

		mTargetPos.x += (rt.x + fw.x) * 0.035f * Game::Sensitivity;
		mTargetPos.y += (rt.y + fw.y) * 0.035f * Game::Sensitivity;
		mTargetPos.z  = Game::World->GetDepth( OS3DGame::GetServerPos(mTargetPos) );

		// Restrict the target position to be within the play area
		SetTarget(mTargetPos);
		return 1;
	}
	else if (Game::Window->GetStyle() == IWindow::Style::FullScreen || Game::Building != 0)
	{
		Vector2i size (Game::Window->GetSize());

		// Mouse close to the left or right sides of the screen should move the camera left/right
		if (pos.x < 25) mPosMovement.x = -1.0f + (pos.x / 25.0f);
		else if (pos.x > (size.x - 25)) mPosMovement.x = 1.0f - (size.x - pos.x) / 25.0f;
		else mPosMovement.x = 0.0f;

		// Mouse close to the top or bottom sides of the screen should move the camera forward/back
		if (pos.y < 25) mPosMovement.y = 1.0f - (pos.y / 25.0f);
		else if (pos.y > (size.y - 25)) mPosMovement.y = -1.0f + (size.y - pos.y) / 25.0f;
		else mPosMovement.y = 0.0f;

		// If we have movement, cancel the camera's follow target
		if (mPosMovement.Sum() != 0.0f) OSCameraTarget::Cancel();
	}
	return 0;
}

//============================================================================================================
// Respond to scrolling events
//============================================================================================================

uint OSDefaultCamera::Scroll (const Vector2i& pos, float delta)
{
	mHasMovement = true;
	mDollyMovement -= delta * 0.02f;
	return 1;
}

//============================================================================================================
// Register mouse movement and scroll event listeners
//============================================================================================================

void OSDefaultCamera::OnInit()
{
	if (!mObject->IsOfClass(Camera::ClassID()))
	{
		DestroySelf();
		return;
	}

	Game::Camera = R5_CAST(R5::Camera, mObject);
	Game::Core->AddOnMouseMove	( bind(&OSDefaultCamera::MouseMove,	this), 1010 );
	Game::Core->AddOnScroll		( bind(&OSDefaultCamera::Scroll,	this), 1010 );
}

//============================================================================================================
// Unregister mouse movement and scroll event listeners
//============================================================================================================

void OSDefaultCamera::OnDestroy()
{
	Game::Core->RemoveOnMouseMove	( bind(&OSDefaultCamera::MouseMove,	this) );
	Game::Core->RemoveOnScroll		( bind(&OSDefaultCamera::Scroll,	this) );
	Game::Camera = 0;
}

//============================================================================================================
// PreUpdate function has to calculate the dolly-offset position
//============================================================================================================

void OSDefaultCamera::OnPreUpdate()
{
	if (Game::Core->IsInUIOnlyMode())
	{
		mSavedPos = mObject->GetRelativePosition();
		mSavedRot = mObject->GetRelativeRotation();
	}
	else
	{
		const Quaternion& absoluteRot (mObject->GetAbsoluteRotation());

		// If there is positional movement happening we should apply it
		if (mPosMovement.Sum() != 0.0f)
		{
			Vector2f fw (absoluteRot.GetFlatForward());
			Vector2f rt (absoluteRot.GetFlatRight());

			rt *= mPosMovement.x;
			fw *= mPosMovement.y;

			float factor = 20.0f * Time::GetDelta();
			mTargetPos.x += (rt.x + fw.x) * factor;
			mTargetPos.y += (rt.y + fw.y) * factor;
			mTargetPos.z  = Game::World->GetDepth( OS3DGame::GetServerPos(mTargetPos) );

			// Restrict the target position to be within the play area
			SetTarget(mTargetPos);
		}

		Vector3f parentPos (mObject->GetParent()->GetAbsolutePosition());
		Vector3f relativePos (mObject->GetRelativePosition());
		Quaternion relativeRot (mObject->GetRelativeRotation());

		// Calculate the smoothly interpolated new position
		float delta = Float::Min(1.0f, 5.0f * Time::GetDelta());
		relativePos = Interpolation::Linear(relativePos, mTargetPos - parentPos, delta);

		// Updates the rotation and dolly from the input-based movement
		if (mHasMovement) mHasMovement = _ApplyMovement(relativeRot);

		// Save the position and rotation so they can be restored in post-update
		mSavedPos	= relativePos;
		mSavedRot	= relativeRot;

		// Offset the position by dolly amount
		Vector3f forward (relativeRot.GetForward());
		relativePos = relativePos - forward * mDolly.y;

		// Ensure that the camera is always above the terrain
		// NOTE: Second sample accounts for the tile immediately in front (near clipping issues)
		Vector3f eyePos (parentPos + relativePos);
		float z0 = Game::World->GetDepth( OS3DGame::GetServerPos(eyePos) );
		float z1 = Game::World->GetDepth( OS3DGame::GetServerPos(eyePos + forward) );

		// Smooth interpolation for smooth height changes
		float targetHeight = Float::Max(Float::Min(z0, z1) + 5.0f, relativePos.z);
		delta = Float::Min(1.0f, Time::GetDelta() * (targetHeight < mEyeHeight ? 3.0f : 10.0f));
		mEyeHeight = Interpolation::Linear(mEyeHeight, targetHeight, delta);
		relativePos.z = mEyeHeight;

		// Look at the LookAt point
		relativeRot.SetFromDirection(mSavedPos - relativePos);

		// Update the object's position and rotation
		mObject->SetRelativePosition(relativePos);
		mObject->SetRelativeRotation(relativeRot);
	}
}

//============================================================================================================
// Restore the relative position for the next update
//============================================================================================================

void OSDefaultCamera::OnPostUpdate()
{
	mObject->SetRelativePosition(mSavedPos);
	mObject->SetRelativeRotation(mSavedRot);
}

//============================================================================================================
// Serialization -- Save
//============================================================================================================

void OSDefaultCamera::OnSerializeTo (TreeNode& node) const
{
	node.AddChild("Dolly", mDolly);
}

//============================================================================================================
// Serialization -- Load
//============================================================================================================

void OSDefaultCamera::OnSerializeFrom (const TreeNode& node)
{
	if (node.mTag == "Dolly") node.mValue >> mDolly;
}