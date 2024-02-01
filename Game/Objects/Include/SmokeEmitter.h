#pragma once

//============================================================================================================
//  Defense of the Citadel, Copyright (c) 2010 Michael Lyashenko and Philip Cosgrave. All rights reserved.
//											www.nextrevision.com
//============================================================================================================
// Smoke Emitter
//============================================================================================================

class SmokeEmitter : public Emitter
{
private:

	Vector3f	mWindDirection;	// The direction of the wind
	Color3f		mColor;			// The color of the smoke
	bool		mFadeAlpha;		// If the smoke fades in and out

public:

	R5_DECLARE_INHERITED_CLASS("Smoke Emitter", SmokeEmitter, Emitter, Object);

	SmokeEmitter();

protected:

	// Virtual functionality allows custom particle behavior
	virtual void InitParticle   (Particle& particle);
	virtual void UpdateParticle (Particle& particle);
	virtual void SetRenderStates(IGraphics* graphics);

	// Serialization to and from the scenegraph tree
	virtual void OnSerializeTo	 (TreeNode& root) const;
	virtual bool OnSerializeFrom (const TreeNode& node);
};