//============================================================================================================
//  Defense of the Citadel, Copyright (c) 2010 Michael Lyashenko and Philip Cosgrave. All rights reserved.
//											www.nextrevision.com
//============================================================================================================

#include "../../Other/Include/_All.h"
using namespace R5;

//============================================================================================================
// Set the game input field
//============================================================================================================

void USHealthColor::OnInit()
{
	mColors.AddKey(0.0f, Vector3f(1.0f, 0.0f, 0.0f));
	mColors.AddKey(0.5f, Vector3f(1.0f, 1.0f, 0.0f));
	mColors.AddKey(1.0f, Vector3f(0.0f, 1.0f, 0.0f));
}

//============================================================================================================
// Change the color of the slider base on value
//============================================================================================================

void USHealthColor::OnUpdate(bool areaChanged)
{
	UISlider* slider = R5_CAST(UISlider, mWidget);

	if (slider != 0 && slider->GetValue() != mCurrentValue)
	{
		mCurrentValue = slider->GetValue();
		Color3f color (mColors.Sample(mCurrentValue));
		color.Normalize();
		slider->SetBackColor(color);
	}
}
