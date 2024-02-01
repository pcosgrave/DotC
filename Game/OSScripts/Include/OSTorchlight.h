#pragma once

//============================================================================================================
//  Defense of the Citadel, Copyright (c) 2010 Michael Lyashenko and Philip Cosgrave. All rights reserved.
//											www.nextrevision.com
//============================================================================================================
// Script that adds torchlight-like flickering to the point light it's attached to
//============================================================================================================

class OSTorchlight : public Script
{
	PointLight* mLight;

public:

	R5_DECLARE_INHERITED_CLASS("OSTorchlight", OSTorchlight, Script, Script);

	virtual void OnInit();
	virtual void OnUpdate();
};