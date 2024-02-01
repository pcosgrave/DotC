//============================================================================================================
//  Defense of the Citadel, Copyright (c) 2010 Michael Lyashenko and Philip Cosgrave. All rights reserved.
//											www.nextrevision.com
//============================================================================================================

#include "../../Other/Include/_All.h"
using namespace R5;

//============================================================================================================
// Helper function that copies an attachment from one place to another
//============================================================================================================

void OSCopyModel::CopyAttachment (Object* myRoot, Object* remoteRoot, const String& handName)
{
	ModelInstance* remoteHand = remoteRoot->FindObject<ModelInstance>(handName);

	if (remoteHand != 0)
	{
		const Object* remoteObj = remoteHand->GetParent();

		if (remoteObj != 0)
		{
			const OSAttachToBone* remoteBone = remoteObj->GetScript<OSAttachToBone>();

			if (remoteBone != 0)
			{
				// Create an attachment point and copy the remote object's orientation
				Object* myObj = myRoot->AddObject<Object>(String("Attachment - %s", handName.GetBuffer()));
				myObj->SetRelativePosition(remoteObj->GetRelativePosition());
				myObj->SetRelativeRotation(remoteObj->GetRelativeRotation());

				// Copy the attachment script
				OSAttachToBone* myBone = myObj->AddScript<OSAttachToBone>();
				myBone->SetBone(remoteBone->GetBone());

				// Create a model instance and attach a script that will copy the model
				ModelInstance* myHand = myObj->AddObject<ModelInstance>(handName);
				myHand->AddScript<OSCopyModel>()->SetWatchedInstance(remoteHand);
			}
		}
	}
}

//============================================================================================================
// Ensure that the script can only be attached to a model instance
//============================================================================================================

void OSCopyModel::OnInit()
{
	mAttachment = R5_CAST(ModelInstance, mObject);
	if (mAttachment == 0) DestroySelf();
}

//============================================================================================================
// If the watched model changes, copy it over
//============================================================================================================

void OSCopyModel::OnUpdate()
{
	Model* watched = (mWatched == 0) ? 0 : mWatched->GetModel();

	if (watched != mAttachment->GetModel())
	{
		Object::Children& children = mAttachment->GetChildren();

		FOREACH(i, children)
		{
			children[i]->DestroySelf();
		}
		mAttachment->SetModel(watched);
	}
}