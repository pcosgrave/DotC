//============================================================================================================
//  Defense of the Citadel, Copyright (c) 2010 Michael Lyashenko and Philip Cosgrave. All rights reserved.
//											www.nextrevision.com
//============================================================================================================

#include "../../Other/Include/_All.h"
using namespace R5;

//============================================================================================================
// Stops the particle effects
//============================================================================================================

void OSStopParticleEffects::OnInit()
{
	EnableParticles(mObject, true);
}

//============================================================================================================
// Stops the particle effects
//============================================================================================================

void OSStopParticleEffects::OnDestroy()
{
	EnableParticles(mObject, false);
}

//============================================================================================================
// Enable/disable particle effect on all children.
//============================================================================================================

void OSStopParticleEffects::EnableParticles(const Object* parent, bool enabled)
{
	const Object::Children& children = parent->GetChildren();

	FOREACH(i, children)
	{
		Object* child = children[i];

		if (child->IsOfClass(Emitter::ClassID()))
		{
			Emitter* emitter = R5_CAST(Emitter, child);
			emitter->SetActive(enabled);
		}
		EnableParticles(child, enabled);
	}
}
