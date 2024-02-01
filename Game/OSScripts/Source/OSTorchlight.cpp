//============================================================================================================
//  Defense of the Citadel, Copyright (c) 2010 Michael Lyashenko and Philip Cosgrave. All rights reserved.
//											www.nextrevision.com
//============================================================================================================

#include "../../Other/Include/_All.h"
using namespace R5;

//============================================================================================================
// This script can only be attached to a point light
//============================================================================================================

void OSTorchlight::OnInit()
{
	mLight = R5_CAST(PointLight, mObject);
	if (mLight == 0) DestroySelf();
}

//============================================================================================================
// Adjust the brightness
//============================================================================================================

void OSTorchlight::OnUpdate()
{
	float time = Time::GetTime();
	const Vector3f& pos (mObject->GetAbsolutePosition());
	float brightness = (float)sin(pos.x + 15.58213 * time) + (float)sin(pos.y + 6.4624 * time);
	brightness *= 0.25f;
	brightness += 0.5f;

	float invSunlight = Game::Brightness;
	invSunlight *= invSunlight;
	invSunlight *= invSunlight;
	invSunlight = 0.2f + 0.8f * (1.0f - invSunlight);

	// Set the final brightness, adjusted by the sunlight
	mLight->SetBrightness((1.5f + 0.35f * brightness) * invSunlight);
}