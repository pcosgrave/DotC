//============================================================================================================
//  Defense of the Citadel, Copyright (c) 2010 Michael Lyashenko and Philip Cosgrave. All rights reserved.
//											www.nextrevision.com
//============================================================================================================

#include "../../Other/Include/_All.h"
using namespace R5;

//============================================================================================================
// Smoke emitter constructor
//============================================================================================================

SmokeEmitter::SmokeEmitter() : mWindDirection (0.0f), mColor (0.0f), mFadeAlpha (true)
{
	mLifetime		= 3000;
	mMaxParticles	= 45;
	mFrequency		= mLifetime / mMaxParticles;
	mActiveTime		= -1;
}

//============================================================================================================
// Initializes a particle, setting it to default (spawn time) values
//============================================================================================================

void SmokeEmitter::InitParticle (Particle& particle)
{
	particle.mSpawnPos		= mAbsolutePos;
	particle.mSpawnPos.x   += mAbsoluteScale.x * (Game::Random.GenerateRangeFloat() * 0.15f);
	particle.mSpawnPos.y   += mAbsoluteScale.y * (Game::Random.GenerateRangeFloat() * 0.15f);
	particle.mSpawnDir.x	= mAbsoluteScale.x * (Game::Random.GenerateRangeFloat() * 0.35f);
	particle.mSpawnDir.y	= mAbsoluteScale.y * (Game::Random.GenerateRangeFloat() * 0.35f);
	particle.mSpawnDir.z	= mAbsoluteScale.z * (Game::Random.GenerateRangeFloat() + 20.0f);
	particle.mParam			= (Game::Random.GenerateRangeFloat() < 0.0f) ? 0 : 1;
}

//============================================================================================================
// Updates the particle
//============================================================================================================

void SmokeEmitter::UpdateParticle (Particle& particle)
{
	float time			= (float)(mLifetime - particle.mRemaining);
	float progress		= Interpolation::Linear( 0.0f, 1.0f, Float::Clamp(time / mLifetime, 0.0f, 1.0f ) );
	float movement		= pow(progress * 2.0f, 0.75f);
	particle.mPos		= particle.mSpawnPos + particle.mSpawnDir * movement;

	// Wind offset should take the vertex position into account
	float currentTime = (float) Time::GetSeconds();
	float windTime = (float)((0.6 * sin(currentTime * 0.421) +
					   0.3 * sin(currentTime * 1.737) +
					   0.1 * cos(currentTime * 2.786)) * 0.5 + 0.5);
	float vertexOffset 	= particle.mPos.Dot(Game::Wind) * (0.5f / mAbsoluteScale.y);
	float windOffset 	= sin(currentTime - vertexOffset);
	float windHeight	= (particle.mSpawnPos - particle.mPos).z;
	float windStrenth	= windTime * windHeight * windHeight;

 	// Offset the vertex in world space
	Vector3f offset (Game::Wind * (windOffset * 0.25f + 0.25f) * windStrenth * mAbsoluteScale);
	particle.mPos		+= offset + (Game::Wind * windHeight * windHeight * 0.05f);
	particle.mRadius	= mAbsoluteScale.y * (1.0f + movement * 2.0f);
	particle.mRotation	= (particle.mParam == 0 ? PI : -PI) * Float::Sin( Float::Sqrt(progress) );
	particle.mColor		= mColor;

	if (mFadeAlpha)
	{
		particle.mColor.a	= Float::ToRangeByte( 0.35f * (0.5f - Float::Abs(progress - 0.5f)) );
	}
	else
	{
		particle.mColor.a	= Float::ToRangeByte( 0.35f * (1.0f - progress));
	}
	
}

//============================================================================================================
// Set up the optional render states
//============================================================================================================

void SmokeEmitter::SetRenderStates(IGraphics* graphics)
{
	graphics->SetBlending	( IGraphics::Blending::Replace );
	graphics->SetAlphaTest	( true );
	graphics->SetFog		( true );
}

//============================================================================================================
// Serialization -- Save
//============================================================================================================

void SmokeEmitter::OnSerializeTo (TreeNode& root) const
{
	root.AddChild("Wind", mWindDirection);
	root.AddChild("Color", mColor);
	root.AddChild("Fade", mFadeAlpha);
	Emitter::OnSerializeTo(root);
}

//============================================================================================================
// Serialization -- Load
//============================================================================================================

bool SmokeEmitter::OnSerializeFrom (const TreeNode& node)
{
	if (node.mTag == "Wind")
	{
		node.mValue >> mWindDirection;
		return true;
	}
	else if (node.mTag == "Color")
	{
		node.mValue >> mColor;
		return true;
	}
	else if (node.mTag == "Fade")
	{
		node.mValue >> mFadeAlpha;
		return true;
	}
	return Emitter::OnSerializeFrom(node);
}