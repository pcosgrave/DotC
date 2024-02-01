//============================================================================================================
//  Defense of the Citadel, Copyright (c) 2010 Michael Lyashenko and Philip Cosgrave. All rights reserved.
//											www.nextrevision.com
//============================================================================================================

#include "../../Other/Include/_All.h"
using namespace R5;

//============================================================================================================
// Set the model template, creating a new instance of that model
//============================================================================================================

void OSPrefab::SetTemplate (const String& name, bool unique)
{
	if (mModel == 0 && mInst != 0)
	{
		Core* core = mObject->GetCore();
		ModelTemplate* temp = core->GetModelTemplate(name, false);

		if (temp != 0)
		{
			Model* model = core->GetModel(unique ? String("NPC %s", mObject->GetName().GetBuffer()) : name);
			model->SetSource(temp);
			mInst->SetModel(model, true);

			// Save the model if we've created it so we can destroy it when the script gets destroyed
			if (unique) mModel = model;
		}
	}
}

//============================================================================================================
// Sets the peasant's position
//============================================================================================================

void OSPrefab::OnInit()
{
	mSerializable = false;
	mInst = R5_CAST(ModelInstance, mObject);
}

//============================================================================================================
// Destroy the associated peasant model as well
//============================================================================================================

void OSPrefab::OnDestroy()
{
	if (mInst != 0)
	{
		mInst->SetModel(0, false);
	}

	if (mModel != 0)
	{
		mModel->DestroySelf();
		mModel = 0;
	}
}