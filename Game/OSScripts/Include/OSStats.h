#pragma once

//============================================================================================================
//  Defense of the Citadel, Copyright (c) 2010 Michael Lyashenko and Philip Cosgrave. All rights reserved.
//											www.nextrevision.com
//============================================================================================================
// The stats that the NPC has. Health, defence, attack, etc.
//============================================================================================================

class OSStats : public Script
{
private:

	OSHealthBar* mHealth;

public:

	R5_DECLARE_INHERITED_CLASS("OSStats", OSStats, Script, Script);

	OSStats() : mHealth(0) {}

	virtual void OnInit();
};
