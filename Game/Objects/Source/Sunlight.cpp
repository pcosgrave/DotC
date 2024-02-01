//============================================================================================================
//  Defense of the Citadel, Copyright (c) 2010 Michael Lyashenko and Philip Cosgrave. All rights reserved.
//											www.nextrevision.com
//============================================================================================================

#include "../../Other/Include/_All.h"
using namespace R5;

//============================================================================================================
// Draw the light using deferred rendering
//============================================================================================================

void Sunlight::OnDrawLight (TemporaryStorage& storage, bool setStates)
{
	if (mShader0 == 0) mShader0 = mGraphics->GetShader("Shaders/Other/Sunlight");
	if (mShader1 == 0) mShader1 = mGraphics->GetShader("Shaders/Other/SunlightShadow");

	IShader* shader = mProperties.mShadows ? mShader1 : mShader0;

	if (mTexture == 0)
	{
		mTexture = mGraphics->GetTexture("[Generated] Clouds");

		Noise noise;
		noise.SetSize( Vector2i(256, 256) );
		noise.SetSeamless(true);
		noise.SetSeed(123456789);
		noise.ApplyFilter("Perlin").Set(6.0f);
		noise.ApplyFilter("Clamp").Set(0.35f, 0.45f);
		noise.ApplyFilter("Normalize");

		mTexture->Set(noise.GetBuffer(), noise.GetWidth(), noise.GetHeight(), 1,
			ITexture::Format::Float,
			ITexture::Format::Alpha);
		mTexture->SetFiltering(ITexture::Filter::Linear);
	}

	mGraphics->SetActiveTexture(0, storage.GetDepth());
	mGraphics->SetActiveTexture(1, storage.GetNormal());
	mGraphics->SetActiveTexture(2, storage.GetAO());

	if (mProperties.mShadows)
	{
		mGraphics->SetActiveTexture(3, storage.GetShadow());
		mGraphics->SetActiveTexture(4, mTexture);
	}
	else
	{
		mGraphics->SetActiveTexture(3, mTexture);
	}

	mGraphics->SetDepthTest(false);
	mGraphics->SetScreenProjection(true);
	mGraphics->ResetModelViewMatrix();

	// Draw the light
	mGraphics->SetActiveShader(shader);
	mGraphics->SetActiveLight(0, &mProperties);
	mGraphics->Draw( IGraphics::Drawable::InvertedQuad );
}