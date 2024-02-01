#pragma once

//============================================================================================================
//  Defense of the Citadel, Copyright (c) 2010 Michael Lyashenko and Philip Cosgrave. All rights reserved.
//											www.nextrevision.com
//============================================================================================================
// Script that plays combat animations on the model instance it's attached to
//============================================================================================================

class OSCombatAnimations : public Script
{
	Model*	mModel;
	ulong	mNextPlay;

	// All possible attack animations
	Array<Animation*> mCombatAttackAnims;

public:

	R5_DECLARE_INHERITED_CLASS("OSCombatAnimations", OSCombatAnimations, Script, Script);

	OSCombatAnimations() : mModel(0), mNextPlay(0) {}

	// Plays a random attack animation
	void Attack();
	void Block();
	void Dodge();
	void Hit();

protected:

	// When the script initializes, gather all idle animations
	virtual void OnInit();

	// When the script is destroyed play stop combat 
	virtual void OnDestroy();
};