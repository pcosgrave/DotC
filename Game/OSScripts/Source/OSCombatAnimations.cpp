//============================================================================================================
//  Defense of the Citadel, Copyright (c) 2010 Michael Lyashenko and Philip Cosgrave. All rights reserved.
//											www.nextrevision.com
//============================================================================================================

#include "../../Other/Include/_All.h"
using namespace R5;

//============================================================================================================
// Plays a random attack animation
//============================================================================================================

void OSCombatAnimations::Attack()
{
	if (mCombatAttackAnims.IsValid())
	{
		uint index = Game::Random.GenerateUint() % mCombatAttackAnims.GetSize();
		mModel->PlayAnimation(mCombatAttackAnims[index]);
		mNextPlay = Time::GetMilliseconds() + 500;
	}
}

//============================================================================================================

void OSCombatAnimations::Block()
{
	if (mNextPlay < Time::GetMilliseconds())
	{
		mNextPlay = Time::GetMilliseconds() + 500;
		mModel->PlayAnimation("Combat: Block");
	}
}

//============================================================================================================

void OSCombatAnimations::Dodge()
{
	if (mNextPlay < Time::GetMilliseconds())
	{
		mNextPlay = Time::GetMilliseconds() + 500;
		mModel->PlayAnimation("Combat: Dodge");
	}
}

//============================================================================================================

void OSCombatAnimations::Hit()
{
	if (mNextPlay < Time::GetMilliseconds())
	{
		mNextPlay = Time::GetMilliseconds() + 500;
		mModel->PlayAnimation("Combat: Hit");
	}
}

//============================================================================================================
// When the script initializes, gather all idle animations
//============================================================================================================

void OSCombatAnimations::OnInit()
{
	ModelInstance* inst = R5_CAST(ModelInstance, mObject);

	if (inst != 0)
	{
		mModel = inst->GetModel();

		if (mModel != 0)
		{
			const Skeleton* skel = mModel->GetSkeleton();

			if (skel != 0)
			{
				const Skeleton::Animations& anims = skel->GetAllAnimations();

				if (anims.IsValid())
				{
					// Collect all idle animations
					for (uint i = anims.GetSize(); i > 0; )
					{
						Animation* anim = anims[--i];

						if (anim->GetName().BeginsWith("Combat: Attack"))
						{
							// It's a combat attack animation -- add it to the list
							mCombatAttackAnims.Expand() = anim;
						}
					}

					// If we have one or more animations to work with, all is well
					if (mCombatAttackAnims.IsValid())
					{
						mModel->PlayAnimation("Combat: Idle");
						mModel->PlayAnimation("Combat: Start");
						return;
					}
				}
			}
		}
	}
	// If anything goes wrong, destroy this script
	DestroySelf();
}

//============================================================================================================
// On destroy stop playing the idle animation and play the end combat
//============================================================================================================

void OSCombatAnimations::OnDestroy()
{
	Model* model = ((ModelInstance*)mObject)->GetModel();

	if (model != 0)
	{
		model->StopAnimation("Combat: Idle", 0.5f);
		model->StopAnimation("Combat: Start", 0.5f);
	}
}