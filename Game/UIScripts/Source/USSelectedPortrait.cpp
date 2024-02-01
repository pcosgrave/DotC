//============================================================================================================
//  Defense of the Citadel, Copyright (c) 2010 Michael Lyashenko and Philip Cosgrave. All rights reserved.
//											www.nextrevision.com
//============================================================================================================

#include "../../Other/Include/_All.h"
using namespace R5;

//============================================================================================================
// Update the local model
//============================================================================================================

void USSelectedPortrait::OnUpdate(bool areaChanged)
{
	ModelInstance* remoteInst = R5_CAST(ModelInstance, Game::Selected);
	UpdateModel(remoteInst, areaChanged);
}