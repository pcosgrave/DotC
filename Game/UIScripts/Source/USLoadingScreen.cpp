//============================================================================================================
//  Defense of the Citadel, Copyright (c) 2010 Michael Lyashenko and Philip Cosgrave. All rights reserved.
//											www.nextrevision.com
//============================================================================================================

#include "../../Other/Include/_All.h"
using namespace R5;

//============================================================================================================
// Show the loading screen -- globally accessible static function
//============================================================================================================

void USLoadingScreen::Create()
{
	if (Game::UI	== 0) return;
	if (Game::Core	== 0) return;

	UIWidget* screen = Game::UI->AddWidget<UIAnimatedFrame>("Loading Screen", true);
	USLoadingScreen* script = screen->AddScript<USLoadingScreen>();
	script->Show();
	screen->Update(Game::UI->GetSize(), true);
}

//============================================================================================================
// Show the loading screen
//============================================================================================================

void USLoadingScreen::Show()
{
	mIsFading = false;
	mJustStarted = true;
	mWidget->SetAlpha(1.0f);
	Game::Core->SetUIOnlyMode(true);

	if (mProgress == 0)
	{
		UIHighlight* hlt = mWidget->AddWidget<UIHighlight>("Loading Screen Background", false);
		mProgress = mWidget->AddWidget<UISlider>("Loading Screen Progress Bar", false);
		mProgress->SetLayer(1);
		mProgress->SetValue(0.0f);
		mProgress->SetBackColor(Color4f(0.5f, 0.9f, 0.35f, 1.0f));
		{
			UIRegion& rgn = mProgress->GetRegion();
			rgn.SetLeft	 (0.5f, -200.0f);
			rgn.SetRight (0.5f, 200.0f);
			rgn.SetTop	 (0.5f, -10.0f);
			rgn.SetBottom(0.5f, -4.0f);
		}

		hlt = mProgress->AddWidget<UIHighlight>("Loading Slider Background", false);
		hlt->SetTopColor(Color4ub(0, 0, 0, 150));
		hlt->SetBottomColor(Color4ub(0, 0, 0, 200));
		{
			UIRegion& rgn = hlt->GetRegion();
			rgn.SetLeft	 (0.0f, -1.0f);
			rgn.SetRight (1.0f,  1.0f);
			rgn.SetTop	 (0.0f, -1.0f);
			rgn.SetBottom(1.0f,  1.0f);
		}

		mCaption = mWidget->AddWidget<UILabel>("Loading Screen Caption", false);
		mCaption->SetLayer(1);

		mCaption->SetTextColor(Color4f(0.70f, 0.85f, 0.50f, 1.0f));
		{
			UIRegion& rgn = mCaption->GetRegion();
			rgn.SetLeft	 (0.5f, -200.0f);
			rgn.SetRight (0.5f, 200.0f);
			rgn.SetTop	 (0.5f, 5.0f);
			rgn.SetBottom(0.5f, 25.0f);
		}
	}

	if (mProgress->GetValue() == 1.0f) mProgress->SetValue(0.0f);
}

//============================================================================================================
// Update the loading screen
//============================================================================================================

void USLoadingScreen::OnUpdate (bool areaChanged)
{
	if (Game::Core == 0) return;

	uint threads = Game::Core->GetNumberOfThreads();

	if (mJustStarted || threads > 0)
	{
		// The core is currently loading something
		mWidget->SetAlpha(1.0f);
		mWidget->BringToFront();
		Game::Core->SetUIOnlyMode(true);
		mJustStarted = false;
		mIsFading = false;
		return;
	}

	// If we have a texture we're supposed to load, do so now
	if (mLastTex != 0) mLastTex->Activate();

	float progress = 0.0f;
	IGraphicsManager::Textures& textures = Game::Graphics->GetAllTextures();

	// Run through all textures and activate the next texture in the list, thus uploading it to the GPU
	textures.Lock();
	{
		if (textures.IsValid() && textures.Back() == mLastTex)
		{
			// If the last texture is still at the end of the list, there is nothing left to do
			progress = 1.0f;
			mStartTex = mLastTex;
		}
		else
		{
			uint start = 0;
			uint current = 0;

			// Run through all textures
			FOREACH(i, textures)
			{
				ITexture* tex = textures[i];

				// Just in case...
				if (tex == 0) continue;

				// If we've found the last starting texture -- remember its index
				if (tex == mStartTex) start = i;

				if (current != 0 || mLastTex == 0)
				{
					// This texture immediately follows the last texture -- make it the last texture
					mLastTex = tex;
					progress = mProgress->GetValue() + 1.0f / (textures.GetSize() - start);
					progress = Float::Clamp(progress, 0.0f, 1.0f);
					break;
				}
				else if (tex == mLastTex)
				{
					// Once we find the last texture, remember its index
					current  = i + 1;
					progress = 1.0f;
				}
			}
		}
	}
	textures.Unlock();

	// Update the progress bar
	mProgress->SetValue(progress);
	mCaption->SetText((mLastTex != 0) ? mLastTex->GetName() : "");

	if (Game::Core->IsInUIOnlyMode())
	{
		// If the progress reaches 1, disable the UI-only mode
		if (progress == 1.0f)
		{
			Game::Core->SetUIOnlyMode(false);
			Game::Core->SetUIOnlyModeSleepDelay(10);
		}
		else
		{
			Game::Core->SetUIOnlyModeSleepDelay(1);
		}
	}
	else if (!mIsFading)
	{
		// Start the fading process
		mIsFading = true;
		mWidget->SetAlpha(0.0f, 0.5f);
	}
	else if (mWidget->GetRegion().GetCalculatedAlpha() == 0.0f)
	{
		// If the widget has faded out, destroy it
		mWidget->DestroySelf();
	}
}