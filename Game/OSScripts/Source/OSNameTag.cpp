//============================================================================================================
//  Defense of the Citadel, Copyright (c) 2010 Michael Lyashenko and Philip Cosgrave. All rights reserved.
//											www.nextrevision.com
//============================================================================================================

#include "../../Other/Include/_All.h"
using namespace R5;

//============================================================================================================
// Sets the font and the initial text
//============================================================================================================

void OSNameTag::OnInit()
{
	mFont = Game::UI->GetDefaultFont();
	mText = mObject->GetName();
}

//============================================================================================================
// Called when the scene draw queue is being filled. This happens once per Scene::Cull call.
//============================================================================================================

void OSNameTag::OnFill (FillParams& params)
{
	const Bounds&	bounds	= mObject->GetAbsoluteBounds();
	const Vector3f& center	= bounds.GetCenter();
	const Vector3f& max		= bounds.GetMax();

	// Point right above the top of the object
	Vector3f pos3 (center.x, center.y, max.z);

	// We only want to draw the label if the 3D point is actually visible to begin with
	if (params.mFrustum.IsVisible(pos3))
	{
		// Convert the 3D position to on-screen coordinates
		Vector2i pos2 (mObject->GetCore()->GetGraphics()->ConvertTo2D(pos3));
		pos2.y -= 10;
		Game::SCT->Add(mText, pos2, mFont, 1.0f, Color4ub(255, 255, 255, Float::ToRangeByte(1.0f)));
	}
}