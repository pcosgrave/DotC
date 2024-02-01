#pragma once

//============================================================================================================
//  Defense of the Citadel, Copyright (c) 2010 Michael Lyashenko and Philip Cosgrave. All rights reserved.
//											www.nextrevision.com
//============================================================================================================
// Firepit emitter (burns more slowly and is wider but shorter then the first emitter)
//============================================================================================================

class FirepitEmitter : public FireEmitter
{
public:

	R5_DECLARE_INHERITED_CLASS("Firepit Emitter", FirepitEmitter, FireEmitter, Object);

	FirepitEmitter();

protected:

	// Virtual functionality allows custom particle behavior
	virtual void InitParticle   (Particle& particle);
	virtual void UpdateParticle (Particle& particle);
	virtual void SetRenderStates(IGraphics* graphics);
};
