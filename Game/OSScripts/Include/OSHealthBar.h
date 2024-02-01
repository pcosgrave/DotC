#pragma once

//============================================================================================================
//  Defense of the Citadel, Copyright (c) 2010 Michael Lyashenko and Philip Cosgrave. All rights reserved.
//											www.nextrevision.com
//============================================================================================================
// Displays the players health bar when selected, controlled, or less than 100%
//============================================================================================================

class OSHealthBar : public Script
{
private:

	UIHealthBar* mHealthBar;
	float		 mHealth;

public:

	R5_DECLARE_INHERITED_CLASS("OSHealthBar", OSHealthBar, Script, Script);

	OSHealthBar() : mHealthBar(0), mHealth(1.0f) {}

	void SetHealth(float val) { mHealth = val; }

	virtual void OnInit();
	virtual void OnDestroy();
	virtual void OnFill(FillParams& params);
};
