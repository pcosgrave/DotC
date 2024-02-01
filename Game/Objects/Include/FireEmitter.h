#pragma once

//============================================================================================================
//  Defense of the Citadel, Copyright (c) 2010 Michael Lyashenko and Philip Cosgrave. All rights reserved.
//											www.nextrevision.com
//============================================================================================================
// Fire emitter
//============================================================================================================

class FireEmitter : public Emitter
{
public:

	R5_DECLARE_INHERITED_CLASS("Fire Emitter", FireEmitter, Emitter, Object);

	FireEmitter();

protected:

	// Virtual functionality allows custom particle behavior
	virtual void InitParticle   (Particle& particle);
	virtual void UpdateParticle (Particle& particle);
	virtual void SetRenderStates(IGraphics* graphics);
};