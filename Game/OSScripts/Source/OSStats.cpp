//============================================================================================================
//  Defense of the Citadel, Copyright (c) 2010 Michael Lyashenko and Philip Cosgrave. All rights reserved.
//											www.nextrevision.com
//============================================================================================================

#include "../../Other/Include/_All.h"
using namespace R5;

//============================================================================================================
// Attach the HealthBar script to the owner
//============================================================================================================

void OSStats::OnInit()
{
	mHealth = mObject->AddScript<OSHealthBar>();
}
