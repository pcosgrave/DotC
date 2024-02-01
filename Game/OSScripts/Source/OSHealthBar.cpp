//============================================================================================================
//  Defense of the Citadel, Copyright (c) 2010 Michael Lyashenko and Philip Cosgrave. All rights reserved.
//											www.nextrevision.com
//============================================================================================================

#include "../../Other/Include/_All.h"
using namespace R5;

// Global frame to hold all health bars
static UIFrame* mFrame = 0;

//============================================================================================================
// Create the slide with a slider and background
//============================================================================================================

void OSHealthBar::OnInit()
{
	Game::UI->Lock();
	{
		if (mFrame == 0) mFrame = Game::UI->AddWidget<UIFrame>("Health bar BG", false);
		mFrame->SetIgnoreEvents(true);
	}
	Game::UI->Unlock();
}

//============================================================================================================
// When destroyed fade out and destroy the health bar.
//============================================================================================================

void OSHealthBar::OnDestroy()
{
	if (mHealthBar != 0)
	{
		Game::UI->Lock();
		{
			mHealthBar->AddScript<USFadeOut>();
			mHealthBar = 0;
		}
		Game::UI->Unlock();
	}
}

//============================================================================================================
// Display the health bar if required.
//============================================================================================================

void OSHealthBar::OnFill (FillParams& params)
{
	if (params.mEye != Game::Camera) return;

	const Bounds&	bounds	= mObject->GetAbsoluteBounds();
	const Vector3f& center	= bounds.GetCenter();
	const Vector3f& max		= bounds.GetMax();

	// Point right above the top of the object
	Vector3f pos3 (center.x, center.y, max.z);	
	bool visible = params.mFrustum.IsVisible(pos3);

	// We only want to draw the label if the 3D point is actually visible to begin with
	if (visible && (Game::Selected == mObject || Game::Controlled == mObject || mHealth < 1.0f))
	{
		// Convert the 3D position to on-screen coordinates
		Vector2i pos2 (Game::Graphics->ConvertTo2D(pos3));

		Game::UI->Lock();
		{
			if (mHealthBar == 0) 
			{
				mHealthBar = mFrame->AddWidget<UIHealthBar>("Health bar", false);
				mHealthBar->AddScript<USFadeIn>();
			}
			mHealthBar->GetRegion().SetRect(pos2.x - 25.0f, pos2.y - 10.0f, 52.0f, 7.0f);
			mHealthBar->SetValue(mHealth);
		}
		Game::UI->Unlock();
	}
	else
	{
		OnDestroy();
	}
}
