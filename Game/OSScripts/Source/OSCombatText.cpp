//============================================================================================================
//  Defense of the Citadel, Copyright (c) 2010 Michael Lyashenko and Philip Cosgrave. All rights reserved.
//											www.nextrevision.com
//============================================================================================================

#include "../../Other/Include/_All.h"
using namespace R5;

//============================================================================================================
// Add some text above the Entity
//============================================================================================================

void OSCombatText::Add (const String& text, const Color4ub& color)
{
	if (mFont == 0) mFont = Game::UI->GetDefaultFont();
	if (mFont != 0)
	{
		Entry* ent = mUnused.Shrink(false);
		if (ent == 0) ent = new Entry();
		ent->mStart = Time::GetMilliseconds();
		ent->mText  = text;
		ent->mCurrentOffset = 0.0f;
		ent->mColor = color;
		mUsed.Expand() = ent;
	}
}

//============================================================================================================
// Initialize the splines
//============================================================================================================

void OSCombatText::OnInit()
{
	// Set up the alpha spline
	mAlpha.AddKey(0.0f, 0.1f);
	mAlpha.AddKey(0.15f, 1.0f);
	mAlpha.AddKey(1.8f, 1.0f);
	mAlpha.AddKey(2.8f, 0.0f);

	// Set up the scale spline
	mScale.AddKey(0.0f, 0.5f);
	mScale.AddKey(0.2f, 1.25f);
	mScale.AddKey(0.3f, 1.0f);

	// Set up the offset spline
	mOffset.AddKey(0.0f, 0.0f);
	mOffset.AddKey(0.2f, 0.0f);
	mOffset.AddKey(2.8f, 90.0f);
}

//============================================================================================================
// Adjust all current label widgets
//============================================================================================================

void OSCombatText::OnFill (FillParams& params)
{
	if (params.mEye != Game::Camera) return;
	if (mUsed.IsEmpty()) return;

	const Bounds&	bounds	= mObject->GetAbsoluteBounds();
	const Vector3f& center	= bounds.GetCenter();
	const Vector3f& max		= bounds.GetMax();

	// Point right above the top of the object
	Vector3f pos3 (center.x, center.y, max.z);

	// Check to see whether the point is visible
	bool isVisible = params.mFrustum.IsVisible(pos3);

	// Convert the 3D position to on-screen coordinates
	Vector2i pos2 (mObject->GetCore()->GetGraphics()->ConvertTo2D(pos3));

	for (uint i = mUsed.GetSize(); i > 0;)
	{
		// Calculate the current fade-out factor
		Entry* ent = mUsed[--i];

		float elapsed = 0.001f * (Time::GetMilliseconds() - ent->mStart);
		float alpha = mAlpha.Sample(elapsed, 1);

		if (alpha > 0.001f)
		{
			if (!isVisible) continue;
			Vector2f pos (pos2);
			float offset = mOffset.Sample(elapsed, 1);

			if (i < mUsed.GetSize() - 1)
			{
				Entry* nextEnt = mUsed [i + 1];
				float diff = offset - nextEnt->mCurrentOffset;

				if (diff < mFont->GetSize())
				{
					offset += (mFont->GetSize() - diff);
				}
			}

			ent->mTargetOffset = offset;
			ent->mCurrentOffset = Interpolation::Linear(ent->mCurrentOffset, ent->mTargetOffset, 0.5f);
			pos.y -= ent->mCurrentOffset;
			ent->mColor.SetAlpha(alpha);
			Game::SCT->Add(ent->mText, pos, mFont, mScale.Sample(elapsed, 1), ent->mColor);
		}
		else
		{
			mUsed.RemoveAt(i);
			mUnused.Expand() = ent;
		}
	}
}