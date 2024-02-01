#pragma once

//============================================================================================================
//  Defense of the Citadel, Copyright (c) 2010 Michael Lyashenko and Philip Cosgrave. All rights reserved.
//											www.nextrevision.com
//============================================================================================================
// Change the color of the slider based on value
//============================================================================================================

class USHealthColor : public UIScript
{
private:

	float   mCurrentValue;
	SplineV mColors;

public:

	R5_DECLARE_INHERITED_CLASS("USHealthColor", USHealthColor, UIScript, UIScript);

	USHealthColor() : mCurrentValue (0.0f) {}

	virtual void OnInit();
	virtual void OnUpdate(bool areaChanged);
};
