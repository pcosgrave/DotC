#pragma once

//============================================================================================================
//  Defense of the Citadel, Copyright (c) 2010 Michael Lyashenko and Philip Cosgrave. All rights reserved.
//											www.nextrevision.com
//============================================================================================================
// Health bar that changes color based on value
//============================================================================================================

class UIHealthBar : public UIHighlight
{
private:

	UIAnimatedSlider* mSlider;

	UIHealthBar() : mSlider(0) {}

public:

	R5_DECLARE_INHERITED_CLASS("UIHealthBar", UIHealthBar, UIHighlight, UIWidget);

	float GetValue() { return mSlider->GetValue(); }
	void SetValue (float value) { mSlider->SetValue(value); }

	virtual void OnLayerChanged() { if (mSlider != 0) mSlider->SetLayer(mLayer + 1); }
	virtual void OnInit();
};
