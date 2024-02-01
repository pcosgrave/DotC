//============================================================================================================
//  Defense of the Citadel, Copyright (c) 2010 Michael Lyashenko and Philip Cosgrave. All rights reserved.
//											www.nextrevision.com
//============================================================================================================

#include "../../Other/Include/_All.h"
using namespace R5;

//============================================================================================================
// Callback triggered when the death animation finishes playing
//============================================================================================================

void OSDestroy::OnAnimationFinished (Model* model, const Animation* anim, float timeToEnd)
{
	if (timeToEnd == 0.0f)
	{
		// Remember the starting values
		mStartPos		= mObject->GetRelativePosition();
		mStartTime		= Time::GetMilliseconds() + 1000;
		mDuration		= 5.0f;
		mStartRadius	= mObject->GetRelativeBounds().GetRadius();
		mStartScale		= mObject->GetRelativeScale();
		mIgnore.Set(Ignore::PreUpdate, false);
	}
}

//============================================================================================================
// Plays the death animation
//============================================================================================================

void OSDestroy::OnInit()
{
	// Turn off the box collider, disabling selection
	mObject->SetFlag(Object::Flag::BoxCollider, false);

	// Remove the animation scripts
	mObject->RemoveScript<OSPlayIdleAnimations>();
	mObject->RemoveScript<OSCombatAnimations>();
	mObject->RemoveScript<OSMelee>();
	mObject->RemoveScript<OSHealthBar>();

	// Stop ALL particles.
	mObject->AddScript<OSStopParticleEffects>();

	// Ensure we aren't controlling this object
	if (Game::Controlled == mObject)
	{
		Game::Controlled->RemoveScript<OSCameraTarget>();
		OSSelection* sel = Game::Controlled->GetScript<OSControlled>();
		if (sel != 0) sel->Hide();
		Game::Controlled = 0;
	}

	// Ensure we aren't selecting this object
	if (Game::Selected == mObject)
	{
		OSSelection* sel = Game::Selected->GetScript<OSSelected>();
		if (sel != 0) sel->Hide();
		Game::Selected = 0;
	}

	// Remove the entity script from the object
	mObject->RemoveScript<OSEntity>();

	// Find an instance of this object
	ModelInstance* inst = R5_CAST(ModelInstance, mObject);

	// If this is a model instance we should try playing a "death" animation on it
	if (inst != 0)
	{
		Model* model = inst->GetModel();

		if (model != 0)
		{
			// Stop all active animations
			model->StopAllAnimations(0.5f);

			// Play the death animation
			uint response = model->PlayAnimation("Combat: Death", 1.0f,
				bind(&OSDestroy::OnAnimationFinished, this));

			// If we succeed in playing the animation, wait for it to end
			if (response != Model::PlayResponse::Failed)
			{
				// We want to ignore updates until after the animation finishes
				mIgnore.Set(Ignore::PreUpdate, true);
				return;
			}
		}
	}

	// Can't play the death animation -- start the destroying process right away
	mStartTime		= Time::GetMilliseconds();
	mDuration		= 2.0f;
	mStartPos		= mObject->GetRelativePosition();
	mStartRadius	= mObject->GetRelativeBounds().GetRadius();
	mStartScale		= mObject->GetRelativeScale();
}

//============================================================================================================
// Should start the fading-out process after a certain amount of time has passed
//============================================================================================================

void OSDestroy::OnPreUpdate()
{
	ulong current = Time::GetMilliseconds();

	if (current > mStartTime)
	{
		float factor = (0.001f / mDuration) * (current - mStartTime);
		factor *= factor;

		Vector3f currentPos(mStartPos);
		currentPos.z -= mStartRadius * factor;
		mObject->SetRelativePosition(currentPos);
		mObject->SetRelativeScale(mStartScale * (1.0f - factor));

		// Once we reach the end, destroy the object
		if (factor >= 1.0f) mObject->DestroySelf();
	}
}