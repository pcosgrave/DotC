//============================================================================================================
//  Defense of the Citadel, Copyright (c) 2010 Michael Lyashenko and Philip Cosgrave. All rights reserved.
//											www.nextrevision.com
//============================================================================================================

#include "../../Other/Include/_All.h"
using namespace R5;

//============================================================================================================
// Update the local model
//============================================================================================================

void USControlledPortrait::OnUpdate (bool areaChanged)
{
	ModelInstance* remoteInst = R5_CAST(ModelInstance, Game::Controlled);
	UpdateModel(remoteInst, areaChanged);
}

//============================================================================================================
// Update the model used by the portrait window
//============================================================================================================

void USControlledPortrait::UpdateModel (ModelInstance* remoteInst, bool areaChanged)
{
	Model* remoteModel	= (remoteInst == 0) ? 0 : remoteInst->GetModel();
	Model* myModel		= (mModelInst == 0) ? 0 : mModelInst->GetModel();

	// If the model has changed
	if (myModel != remoteModel)
	{
		Game::Core->Lock();

		// If the model is not null
		if (remoteModel != 0)
		{
			if (mModelInst == 0)
			{
				// Create a new model instance that will be used to draw the off-screen model
				mModelInst = mRoot->AddObject<ModelInstance>(String("%s: Model", GetClassID()));
			}
			else
			{
				mModelInst->Clear();
			}

			// Change the model
			mModelInst->SetModel(remoteModel, false);

			// Copy the attachments
			OSCopyModel::CopyAttachment(mModelInst, remoteInst, "Right Hand");
			OSCopyModel::CopyAttachment(mModelInst, remoteInst, "Left Hand");

			// Update the instance, recalculating the bounds
			mModelInst->Update();

			// Position the camera according to the bounds
			const Vector3f& center = mModelInst->GetAbsoluteBounds().GetCenter();
			float radius = mModelInst->GetAbsoluteBounds().GetRadius();

			Vector3f pos (center);
			pos.x += radius * 1.5f;
			pos.y += radius * 1.5f;
			pos.z += radius * 0.5f;

			pos *= mModelInst->GetAbsoluteRotation();

			mCamera->SetAbsolutePosition(pos);
			mCamera->SetAbsoluteRotation(center - pos);

			// Refresh the widget
			mWidget->SetDirty();
		}
		else if (mModelInst != 0)
		{
			mModelInst->DestroySelf();
			mModelInst = 0;
		}
		Game::Core->Unlock();
	}

	// Update the render target's dimensions
	const UIRegion& rgn = mWidget->GetRegion();
	mSize.Set((short)Float::RoundToUInt(rgn.GetCalculatedWidth()),
		(short)Float::RoundToUInt(rgn.GetCalculatedHeight()));

	// Update the base class
	USPortrait::OnUpdate(areaChanged);
}

//============================================================================================================
// Ensure to release the model
//============================================================================================================

void USControlledPortrait::OnDestroy()
{
	if (mModelInst != 0)
	{
		mModelInst->Clear();
		mModelInst->SetModel(0, false);
		mModelInst->DestroySelf();
		mModelInst = 0;
	}
}