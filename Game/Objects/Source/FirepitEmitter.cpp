//============================================================================================================
//  Defense of the Citadel, Copyright (c) 2010 Michael Lyashenko and Philip Cosgrave. All rights reserved.
//											www.nextrevision.com
//============================================================================================================

#include "../../Other/Include/_All.h"
using namespace R5;

//============================================================================================================
// Fire emitter constructor
//============================================================================================================

FirepitEmitter::FirepitEmitter()
{
	mLifetime		= 700;
	mMaxParticles	= 7;
	mFrequency		= mLifetime / mMaxParticles;
	mActiveTime		= -1;

	// We don't want to allow random flipping the flame texture vertically (by default it randomly flips both)
	mFlags.Set(Flag::FlipV, false);
}

//============================================================================================================
// Initializes a particle, setting it to default (spawn time) values
//============================================================================================================

void FirepitEmitter::InitParticle (Particle& particle)
{
	particle.mSpawnPos	    = mAbsolutePos;
	particle.mSpawnPos.x   += mAbsoluteScale.x * (Game::Random.GenerateRangeFloat() * 0.1f);
	particle.mSpawnPos.y   += mAbsoluteScale.y * (Game::Random.GenerateRangeFloat() * 0.1f);
	particle.mSpawnDir.x	= mAbsoluteScale.x * (Game::Random.GenerateRangeFloat());
	particle.mSpawnDir.y	= mAbsoluteScale.y * (Game::Random.GenerateRangeFloat());
	particle.mSpawnDir.z	= mAbsoluteScale.z * (Game::Random.GenerateRangeFloat() * 0.5f + 3.0f);
	particle.mRotation		= 0.0f;
}

//============================================================================================================
// Updates the particle
//============================================================================================================

void FirepitEmitter::UpdateParticle (Particle& particle)
{
	float time			= (float)(mLifetime - particle.mRemaining);
	float progress		= Interpolation::Linear( 0.0f, 1.0f, Float::Clamp(time / mLifetime, 0.0f, 1.0f ) );
	float movement		= pow(progress, 3.0f);
	particle.mPos		= particle.mSpawnPos + particle.mSpawnDir * movement;
	particle.mRadius	= mAbsoluteScale.y * (1.0f - movement);

	// Make the flame get narrower as it gets higher
	//Vector3f pos ( particle.mSpawnPos.x, particle.mSpawnPos.y, particle.mPos.z );
	//particle.mPos = Interpolation::Linear( particle.mPos, pos, progress * 0.5f );

	float alpha = (progress < 0.25f) ? progress * 4.0f : 1.0f - (progress - 0.25f) * 1.333333f;

	// Color is based on the same factor value
	particle.mColor.r	= Float::ToRangeByte(alpha);
	particle.mColor.g	= particle.mColor.r;
	particle.mColor.b	= particle.mColor.r;
	particle.mColor.a	= particle.mColor.r;
}

//============================================================================================================
// Set up the optional render states
//============================================================================================================

void FirepitEmitter::SetRenderStates (IGraphics* graphics)
{
	graphics->SetBlending   ( IGraphics::Blending::Add );
	graphics->SetAlphaTest  ( false );
	graphics->SetFog		( false );
}