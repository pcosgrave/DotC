//============================================================================================================
//  Defense of the Citadel, Copyright (c) 2010 Michael Lyashenko and Philip Cosgrave. All rights reserved.
//											www.nextrevision.com
//============================================================================================================

#include "../../Other/Include/_All.h"
using namespace R5;

//============================================================================================================
// Add a slider to the highlight
//============================================================================================================

void UIHealthBar::OnInit()
{
	SetColor(Color4ub(0, 0, 0, 255));

	mSlider = AddWidget<UIAnimatedSlider>("Health Bar", false);

	UIRegion& region = mSlider->GetRegion();
	region.SetLeft(0.0f, 1.0f);
	region.SetRight(1.0f, -1.0f);
	region.SetTop(0.0f, 1.0f);
	region.SetBottom(1.0f, -1.0f);

	mRegion.SetAlpha(0.0f);
	mSlider->SetAnimationTime(0.2f);
	mSlider->SetLayer(1);
	mSlider->AddScript<USHealthColor>();
}
