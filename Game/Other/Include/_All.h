#pragma once

//============================================================================================================
//  Defense of the Citadel, Copyright (c) 2010 Michael Lyashenko and Philip Cosgrave. All rights reserved.
//											www.nextrevision.com
//============================================================================================================
// Precompiled header
//============================================================================================================

#include "../../../Engine/Serialization/Include/_All.h"
#include "../../../Engine/OpenGL/Include/_All.h"
#include "../../../Engine/Sound/Include/_All.h"
#include "../../../Engine/Core/Include/_All.h"
#include "../../../Engine/UI/Include/_All.h"
#include "../../../Engine/Noise/Include/_All.h"
#include "../../../Engine/Network/Include/_All.h"

namespace R5
{
	// Shared components
	#include "../../../Common/Vector3i.h"
	#include "../../../Common/Entity.h"
	#include "../../../Common/Packets.h"

	// Objects
	#include "../../Objects/Include/FireEmitter.h"
	#include "../../Objects/Include/SmokeEmitter.h"
	#include "../../Objects/Include/FirepitEmitter.h"
	#include "../../Objects/Include/Projector.h"
	#include "../../Objects/Include/Sunlight.h"

	// Widgets
	#include "../../UIWidgets/Include/UIFrameText.h"
	#include "../../UIWidgets/Include/UIHealthBar.h"

	// Object scripts
	#include "../../OSScripts/Include/OS3DGame.h"
	#include "../../OSScripts/Include/OSTorchlight.h"
	#include "../../OSScripts/Include/OSSelectable.h"
	#include "../../OSScripts/Include/OSSelection.h"
	#include "../../OSScripts/Include/OSScale.h"
	#include "../../OSScripts/Include/OSDayNight.h"
	#include "../../OSScripts/Include/OSNameTag.h"
	#include "../../OSScripts/Include/OSPrefab.h"
	#include "../../OSScripts/Include/OSMove.h"
	#include "../../OSScripts/Include/OSFace.h"
	#include "../../OSScripts/Include/OSEntity.h"
	#include "../../OSScripts/Include/OSCameraTarget.h"
	#include "../../OSScripts/Include/OSClickToFollow.h"
	#include "../../OSScripts/Include/OSClickToControl.h"
	#include "../../OSScripts/Include/OSSelected.h"
	#include "../../OSScripts/Include/OSControlled.h"
	#include "../../OSScripts/Include/OSCombatText.h"
	#include "../../OSScripts/Include/OSHealthBar.h"
	#include "../../OSScripts/Include/OSStats.h"
	#include "../../OSScripts/Include/OSCombatAnimations.h"
	#include "../../OSScripts/Include/OSMelee.h"
	#include "../../OSScripts/Include/OSDestroy.h"
	#include "../../OSScripts/Include/OSDefaultCamera.h"
	#include "../../OSScripts/Include/OSCopyModel.h"
	#include "../../OSScripts/Include/OSServerConnect.h"
	#include "../../OSScripts/Include/OSPlaceable.h"
	#include "../../OSScripts/Include/OSTerrainTile.h"
	#include "../../OSScripts/Include/OSTiledTerrain.h"
	#include "../../OSScripts/Include/OSCreateSurface.h"
	#include "../../OSScripts/Include/OSObjectSize.h"
	#include "../../OSScripts/Include/OSSway.h"
	#include "../../OSScripts/Include/OSStopParticleEffects.h"

	// Widget scripts
	#include "../../UIScripts/Include/USRoot.h"
	#include "../../UIScripts/Include/USChangeLayout.h"
	#include "../../UIScripts/Include/USDisconnect.h"
	#include "../../UIScripts/Include/USStatusBar.h"
	#include "../../UIScripts/Include/USServerDetails.h"
	#include "../../UIScripts/Include/USPreviewMap.h"
	#include "../../UIScripts/Include/USServerInfo.h"
	#include "../../UIScripts/Include/USServerDescription.h"
	#include "../../UIScripts/Include/USJoinGame.h"

	#include "../../UIScripts/Include/USFrameAware.h"
	#include "../../UIScripts/Include/USToggleWidget.h"
	#include "../../UIScripts/Include/USHideFrame.h"
	#include "../../UIScripts/Include/USConnect.h"
	#include "../../UIScripts/Include/USChatLog.h"
	#include "../../UIScripts/Include/USChatInput.h"
	#include "../../UIScripts/Include/USHealthColor.h"
	#include "../../UIScripts/Include/USGrowOnHover.h"
	#include "../../UIScripts/Include/USLoadingScreen.h"
	#include "../../UIScripts/Include/USStartBuilding.h"

	// Widget Portrait scripts
	#include "../../UIScripts/Include/USPortrait.h"
	#include "../../UIScripts/Include/USControlledPortrait.h"
	#include "../../UIScripts/Include/USSelectedPortrait.h"

	// Generic classes
	#include "../../Other/Include/World.h"
	#include "../../Other/Include/Server.h"
	#include "../../Other/Include/Debug.h"
	#include "../../Other/Include/Game.h"
};