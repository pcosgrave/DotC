#pragma once

//============================================================================================================
//  Defense of the Citadel, Copyright (c) 2010 Michael Lyashenko and Philip Cosgrave. All rights reserved.
//											www.nextrevision.com
//============================================================================================================
// Stops all particle effect on the connected object.
//============================================================================================================

class OSStopParticleEffects : public Script
{
public:

	R5_DECLARE_INHERITED_CLASS("OSStopParticleEffects", OSStopParticleEffects, Script, Script);

	// Stop all particle effects.
	virtual void OnInit();
	virtual void OnDestroy();

private:

	void EnableParticles(const Object* parent, bool enabled);
};
