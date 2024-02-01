//============================================================================================================
//  Defense of the Citadel, Copyright (c) 2010 Michael Lyashenko and Philip Cosgrave. All rights reserved.
//											www.nextrevision.com
//============================================================================================================

#include "../../Other/Include/_All.h"
using namespace R5;

//============================================================================================================

void USStartBuilding::OnInit()
{
	UIList* list = R5_CAST(UIList, mWidget);

	if (list != 0)
	{
		mOriginalText = list->GetText();
	}
	else
	{
		DestroySelf();
	}
}

//============================================================================================================
// Set the placeable building
//============================================================================================================

void USStartBuilding::OnValueChange()
{
	if (Game::Scene == 0) return;
	UIList* list = R5_CAST(UIList, mWidget);

	ModelInstance* inst = Game::Scene->AddObject<ModelInstance>("Placing Building");
	if (inst == 0) return;

	String buildingType = list->GetText();
	inst->SetModel(Game::Core->GetModel(buildingType));
	OSPlaceable* placing = inst->AddScript<OSPlaceable>();

	if		(buildingType == "Barracks")	placing->SetEntityType(Entity::Type::Barracks);
	else if (buildingType == "Farm")		placing->SetEntityType(Entity::Type::Farm);
	else if (buildingType == "Forge")		placing->SetEntityType(Entity::Type::Forge);
	else if (buildingType == "Mill")		placing->SetEntityType(Entity::Type::Mill);
	else if (buildingType == "Mine")		placing->SetEntityType(Entity::Type::Mine);
	else if (buildingType == "Shack")		placing->SetEntityType(Entity::Type::Shack);
	else if (buildingType == "Tents")		placing->SetEntityType(Entity::Type::Tents);
	else if (buildingType == "Townhall")	placing->SetEntityType(Entity::Type::Townhall);
	else if (buildingType == "Workshop")	placing->SetEntityType(Entity::Type::Workshop);

	list->SetText(mOriginalText);	
}
