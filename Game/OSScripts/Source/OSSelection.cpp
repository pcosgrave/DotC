//============================================================================================================
//  Defense of the Citadel, Copyright (c) 2010 Michael Lyashenko and Philip Cosgrave. All rights reserved.
//											www.nextrevision.com
//============================================================================================================

#include "../../Other/Include/_All.h"
using namespace R5;

//============================================================================================================
// Initialize the default values
//============================================================================================================

OSSelection::OSSelection() :
	mProjector	(0),
	mStartColor	(1.0f, 1.0f, 1.0f, 0.0f),
	mTargetColor(1.0f),
	mStartMS	(0),
	mDuration	(0)
{
	// This is a local script and should not be serialized
	SetSerializable(false);
}

//============================================================================================================
// Releases the associated projector
//============================================================================================================

void OSSelection::Release()
{
	if (mProjector != 0 && mObject != 0)
	{
		if (mObject->GetChildren().Contains(mProjector))
		{
			mProjector->DestroySelf();
			mProjector = 0;
		}
	}
}

//============================================================================================================
// Show the selection ring
//============================================================================================================

void OSSelection::Show (float r, float g, float b)
{
	// Ensure that OnInit gets called each time so that the previous selection gets hidden properly
	OnInit();

	// If we have not yet created the projector, let's do that now
	if (mProjector == 0)
	{
		mProjector = mObject->AddObject<Projector>(GetObjectName());
		ASSERT(mProjector != 0, "Missing projector registration?");

		ITexture* ring  = mObject->GetGraphics()->GetTexture(GetTextureName());
		IShader* shader = mObject->GetGraphics()->GetShader("Decal/Glow_D");

		mProjector->SetTexture(ring);
		mProjector->SetShader(shader);
		mProjector->SetColor(Color4f(1.0f, 1.0f, 1.0f, 0.0f));
		mProjector->SetSerializable(false);

		// We want to start receiving update events
		mIgnore.Set(Ignore::PreUpdate, false);
	}

	// Create the color and match it against what's already set
	Color4f color(r, g, b, 1.0f);

	if (color != mTargetColor)
	{
		// If the color is different, start the fading process
		mStartMS	 = Time::GetMilliseconds();
		mDuration	 = 0.25f;
		mStartColor	 = mProjector->GetColor();
		mTargetColor = color;
		mRadius.x	 = mRadius.y;
		mRadius.z	 = 1.0f;
	}
}

//============================================================================================================
// Hide the selection ring
//============================================================================================================

void OSSelection::Hide()
{
	if (mTargetColor.a != 0.0f && mProjector != 0)
	{
		mStartMS		= Time::GetMilliseconds();
		mDuration		= 0.25f;
		mStartColor		= mProjector->GetColor();
		mTargetColor	= mStartColor;
		mTargetColor.a	= 0.0f;
		mRadius.x		= mRadius.y;
		mRadius.z		= 0.0f;
	}
}

//============================================================================================================
// Animate the selection
//============================================================================================================

void OSSelection::OnPreUpdate()
{
	if (mProjector != 0)
	{
		Color4f color (mTargetColor);

		if (mDuration != 0)
		{
			float delta  = 0.001f * (Time::GetMilliseconds() - mStartMS);
			float factor = delta / mDuration;

			if (factor > 1.0f)
			{
				// If the projector has been fading out, we can now safely destroy it
				if ( Float::IsZero(mTargetColor.a) )
				{
					DestroySelf();
					return;
				}
				else
				{
					// The projector as fully faded in
					mDuration = 0;
					factor = 1.0f;
				}
			}

			// Interpolate the color
			color = Interpolation::Hermite(mStartColor, mTargetColor, factor);

			// Interpolate the radius
			mRadius.y = 1.0f - Interpolation::Hermite(mRadius.x, mRadius.z, factor);
			mRadius.y *= mRadius.y;
			mRadius.y *= mRadius.y;
			mRadius.y = 1.0f - mRadius.y;
		}

		// Update the projector parameters
		mProjector->SetColor(color);
		mProjector->SetRelativeScale( GetRadius() * mRadius.y * (0.95f + 0.05f * (float)sin(Time::GetSeconds() * 5.0)) );
		mProjector->SetSpin( (float)(Time::GetSeconds() * 0.1) );
	}
	else
	{
		// Ignore all future updates
		mIgnore.Set(Ignore::PreUpdate, true);
	}
}