//============================================================================================================
//  Defense of the Citadel, Copyright (c) 2010 Michael Lyashenko and Philip Cosgrave. All rights reserved.
//											www.nextrevision.com
//============================================================================================================

#include "../../Other/Include/_All.h"
using namespace R5;

//============================================================================================================
// Key event receiver
//============================================================================================================

uint SelectObject (const Vector2i& pos, byte key, bool isDown)
{	
	// Left mouse key should select objects	
	if (key == Key::MouseLeft || key == Key::MouseRight)
	{
		static Vector2i lastPos = pos;

		if (isDown)
		{
			lastPos = pos;
		}
		else if (Game::Scene != 0)
		{
			Vector2i delta (lastPos - pos);

			if (delta.x < 0) delta.x = -delta.x;
			if (delta.y < 0) delta.y = -delta.y;

			// If the mouse has moved too much, don't count it as a click
			if (delta.x > 3 || delta.y > 3) return 1;
			
			// Cast a ray into the screen
			Array<RaycastHit>& hits = Game::Scene->Raycast(pos);

			// Forward this event to the closest object
			for (uint i = 0; i < hits.GetSize(); ++i)
			{
				Object* obj = hits[i].mObject;

				// Forward this event to the object and see if one of its scripts handles it
				if (obj->KeyPress(pos, key, isDown))
				{
					const Vector3f& pos = obj->GetAbsolutePosition();
					Debug::Log("Clicked on [FF8833]%s[FFFFFF] (%.1f %.1f %.1f)", obj->GetName().GetBuffer(),
						pos.x, pos.y, pos.z);
					return 1;
				}
			}
			// MouseClick events should not be handled by anything else
			return 1;
		}
	}
	return 0;
}

//============================================================================================================
// Inverse view-projection matrix
//============================================================================================================

void SetUniform_WorldSize (const String& name, Uniform& uniform)
{
	Vector2f v;

	if (Game::Terrain != 0)
	{
		const Bounds& bounds = Game::Terrain->GetAbsoluteBounds();
		Vector2f center = bounds.GetCenter();
		v  = bounds.GetMax();
		v -= center;
		v -= 2.0f;
	}
	uniform = v;
}

//============================================================================================================
// Initialize the game to be played in 3D
//============================================================================================================

void OS3DGame::OnInit()
{
	Game::Mode = 3;

	Object::Register<FireEmitter>();
	Object::Register<SmokeEmitter>();
	Object::Register<Projector>();
	Object::Register<Sunlight>();
	Object::Register<FirepitEmitter>();

	Script::Register<OSTorchlight>();
	Script::Register<OSTiledTerrain>();
	Script::Register<OSCreateSurface>();
	Script::Register<OSSelectable>();
	Script::Register<OSTerrainTile>();
	Script::Register<OSScale>();
	Script::Register<OSDayNight>();
	Script::Register<OSNameTag>();
	Script::Register<OSPrefab>();
	Script::Register<OSMove>();
	Script::Register<OSFace>();
	Script::Register<OSEntity>();
	Script::Register<OSCameraTarget>();
	Script::Register<OSClickToFollow>();
	Script::Register<OSClickToControl>();
	Script::Register<OSSelected>();
	Script::Register<OSControlled>();
	Script::Register<OSCombatText>();
	Script::Register<OSStats>();
	Script::Register<OSHealthBar>();
	Script::Register<OSCombatAnimations>();
	Script::Register<OSMelee>();
	Script::Register<OSDestroy>();
	Script::Register<OSDefaultCamera>();
	Script::Register<OSCopyModel>();
	Script::Register<OSServerConnect>();
	Script::Register<OSPlaceable>();
	Script::Register<OSObjectSize>();
	Script::Register<OSSway>();
	Script::Register<OSStopParticleEffects>();

	// Register listener callbacks with the server
	Game::Server->onModifyHeightmap	= bind(&OS3DGame::OnModifyHeightmap, this);
	Game::Server->onAddEntity		= bind(&OS3DGame::OnAddEntity,		 this);
	Game::Server->onUpdateEntity	= bind(&OS3DGame::OnUpdateEntity,	 this);
	Game::Server->onCombatMessage	= bind(&OS3DGame::OnCombatMessage,	 this);
	Game::Server->onUpdateStats		= bind(&OS3DGame::OnUpdateStats,	 this);
	Game::Server->onRemoveEntity	= bind(&OS3DGame::OnRemoveEntity,	 this);

	// Key event listener handling object selection is the most important event and must be handled first
	Game::Core->AddOnKey(&SelectObject, 1000000);

	// Replace the deferred rendering combine stage shader
	String code;

	if (code.Load("Shaders/Deferred/Combine.frag"))
	{
		ISubShader* sub = Game::Graphics->GetSubShader("[R5] Combine Deferred");
		sub->SetCode(code);
		IShader* shader = Game::Graphics->GetShader("[R5] Combine Deferred");
		shader->RegisterUniform("worldSize", &SetUniform_WorldSize);
	}
}

//============================================================================================================
// Remove the registered listener
//============================================================================================================

void OS3DGame::OnDestroy()
{
	Game::Server->onModifyHeightmap.clear();
	Game::Server->onAddEntity.clear();
	Game::Server->onUpdateEntity.clear();
	Game::Server->onCombatMessage.clear();
	Game::Server->onUpdateStats.clear();
	Game::Server->onRemoveEntity.clear();

	Game::Core->RemoveOnKey(&SelectObject);
}

//============================================================================================================
// Update function waits until everything finishes loading, then creates the terrain
//============================================================================================================

void OS3DGame::OnUpdate()
{
	if (Game::Core->GetNumberOfThreads() == 0)
	{
		if (mTerrain == 0)
		{
			mTerrain = Game::Scene->FindObject<Object>("Terrain");

			if (mTerrain != 0)
			{
				OSTiledTerrain* script = mTerrain->GetScript<OSTiledTerrain>();
				if (script != 0) script->Create();
			}
		}

		if (mSurface == 0)
		{
			mSurface = Game::Scene->FindObject<Object>("Surface");

			if (mSurface != 0)
			{
				OSCreateSurface* script = mSurface->GetScript<OSCreateSurface>();
				if (script != 0) script->Create();

				if (Game::Camera != 0)
				{
					OSDefaultCamera* def = Game::Camera->GetScript<OSDefaultCamera>();

					if (def != 0) 
					{
						// Tile size is 2x2x1
						Vector3i pos (Game::World->GetWidth() >> 1, Game::World->GetHeight() >> 1, 0);
						pos.z = Game::World->GetDepth(pos);
						def->SetTarget(GetGamePos(pos));
					}
				}
			}
		}

		Game::Server->JoinGame();
		mIgnore.Set(Ignore::Update);
	}
}

//============================================================================================================
// Retrieves the specified entity
//============================================================================================================

ModelInstance* OS3DGame::GetEntity (uint id, bool createIfMissing, bool threadSafe)
{
	if (threadSafe) Game::Core->Lock();

	if (mEntityRoot == 0)
	{
		mEntityRoot = Game::Scene->FindObject<Object>("Entities", false);
		ASSERT(mEntityRoot != 0, "Expecting the placeables to exist by now");

		if (mEntityRoot == 0)
		{
			if (threadSafe) Game::Core->Unlock();
			return 0;
		}
	}

	String name ("%u", id);

	ModelInstance* inst = createIfMissing ?
		mEntityRoot->AddObject <ModelInstance>(name) :
		mEntityRoot->FindObject<ModelInstance>(name, false);

	if (threadSafe) Game::Core->Unlock();
	return inst;
}

//============================================================================================================
// Helper function that calculates the center position of the specified node
//============================================================================================================

inline Vector3f OS3DGame::GetGamePos (const Vector3i& v)
{
	return Vector3f(2.0f * v.x - Game::World->GetWidth()  + 1.0f,
					2.0f * v.y - Game::World->GetHeight() + 1.0f, (float)v.z);
}

//============================================================================================================
// Helper function that converts world coordinates to server coordinates
//============================================================================================================

Vector3i OS3DGame::GetServerPos (const Vector3f& v)
{
	int x = Float::RoundToInt((v.x - 1.0f + Game::World->GetWidth())  * 0.5f);
	int y = Float::RoundToInt((v.y - 1.0f + Game::World->GetHeight()) * 0.5f);
	int z = Float::RoundToInt(v.z);

	if (x < 0) x = 0;
	if (y < 0) y = 0;
	if (z < 0) z = 0;

	ushort ux = (ushort)x;
	ushort uy = (ushort)y;
	ushort uz = (ushort)z;

	if (ux >= Game::World->GetWidth())	ux = Game::World->GetWidth()  - 1;
	if (uy >= Game::World->GetHeight())	uy = Game::World->GetHeight() - 1;

	return Vector3i(ux, uy, uz);
}

//============================================================================================================
// Changes the terrain
//============================================================================================================

void OS3DGame::OnModifyHeightmap (const Packet::ModifyHeightmap& packet, const byte* data)
{
	ASSERT(false, "Implement terrain modification");
}

//============================================================================================================
// Callback triggered when a entity gets added
//============================================================================================================

void OS3DGame::OnAddEntity (const Entity& ent)
{
	Game::Core->Lock();
	{
		ModelInstance* inst = GetEntity(ent.mId, true, false);

		if (inst != 0)
		{
			inst->AddScript<OSEntity>()->Set(ent.mId, ent.mType);
			inst->SetSerializable(false);

			if (ent.mType != 0)
			{
				if (ent.mType >= Entity::Type::Objective)
				{
					// TODO: Add objectives (invisible)
				}
				else if (ent.mType >= Entity::Type::Enemy)
				{
					inst->SetRelativePosition( OS3DGame::GetGamePos(ent.mPos) );
					inst->SetRelativeRotation( Vector3f(Float::Sin(ent.mRot), Float::Cos(ent.mRot), 0.0f) );
					inst->SetRelativeScale(0.0f);

					inst->AddScript<OSScale>()->Start(Vector3f(ent.mScale), 0.5f);
					inst->AddScript<OSSelectable>()->SetColor(2.5f, 1.0f, 1.0f);
					inst->AddScript<OSClickToFollow>();
					inst->AddScript<OSClickToControl>(); // NOTE: Disable this when AI is implemented

					if (ent.mType == 512) inst->AddScript<OSPrefab>()->SetTemplate("Orc");

					// Currently defending
					if (ent.mObjective == 40) inst->AddScript<OSCombatAnimations>();
				}
				else if (ent.mType >= Entity::Type::Player)
				{
					inst->SetRelativePosition( OS3DGame::GetGamePos(ent.mPos) );
					inst->SetRelativeRotation( Vector3f(Float::Sin(ent.mRot), Float::Cos(ent.mRot), 0.0f) );
					inst->SetRelativeScale(0.0f);

					inst->AddScript<OSScale>()->Start(Vector3f(ent.mScale), 0.5f);
					inst->AddScript<OSSelectable>()->SetColor(1.0f, 1.5f, 2.5f);
					inst->AddScript<OSClickToFollow>();
					inst->AddScript<OSClickToControl>();

					if (ent.mType == 256) inst->AddScript<OSPrefab>()->SetTemplate("Peasant");

					// Currently defending
					if (ent.mObjective == 40) inst->AddScript<OSCombatAnimations>();
				}
				else if (ent.mType >= Entity::Type::Usable)
				{
					// TODO: Add usable objects such as levers, traps, torches, etc
				}
				else if (ent.mType >= Entity::Type::Static)
				{
					// Static objects
					Model* model = 0;

					// Static objects don't need OSPrefab script -- they can use static models
					if		(ent.mType == 1)	model = Game::Core->GetModel("Models/Nature/tree0.r5a");
					else if (ent.mType == 2)	model = Game::Core->GetModel("Models/Nature/tree1.r5a");
					else if (ent.mType == 3)	model = Game::Core->GetModel("Models/Nature/tree2.r5a");
					else if (ent.mType == Entity::Type::Barracks)	model = Game::Core->GetModel("Barracks");
					else if (ent.mType == Entity::Type::Farm)		model = Game::Core->GetModel("Farm");
					else if (ent.mType == Entity::Type::Forge)		model = Game::Core->GetModel("Forge");
					else if (ent.mType == Entity::Type::Mill)		model = Game::Core->GetModel("Mill");
					else if (ent.mType == Entity::Type::Mine)		model = Game::Core->GetModel("Mine");
					else if (ent.mType == Entity::Type::Shack)		model = Game::Core->GetModel("Shack");
					else if (ent.mType == Entity::Type::Tents)		model = Game::Core->GetModel("Tents");
					else if (ent.mType == Entity::Type::Townhall)	model = Game::Core->GetModel("Townhall");
					else if (ent.mType == Entity::Type::Workshop)	model = Game::Core->GetModel("Workshop");

					inst->SetRelativePosition( OS3DGame::GetGamePos(ent.mPos) );
					inst->SetRelativeRotation( Vector3f(Float::Sin(ent.mRot), Float::Cos(ent.mRot), 0.0f) );
					inst->SetRelativeScale(0.0f);
					inst->SetModel(model, true);
					inst->AddScript<OSScale>()->Start(Vector3f(ent.mScale), 0.5f);

					if (ent.mType < Entity::Type::Barracks)
					{
						ProjectedTexture* shadow = inst->AddObject<ProjectedTexture>("Shadow");
						shadow->SetColor( Color4f(0.08f, 0.065f, 0.0f, 1.0f) );
						shadow->SetTexture(mObject->GetGraphics()->GetTexture("Textures/Billboards/blob.png"));
						shadow->SetRelativeRotation(Vector3f(0.0f, 0.0f, -1.0f));
					}
				}
			}
			else
			{
				Debug::LogError("Unable to add a placeable object to %u %u %u", ent.mPos.x, ent.mPos.y, ent.mPos.z);
			}
		}
	}
	Game::Core->Unlock();
}

//============================================================================================================
// Callback triggered when an entity's objective changes
//============================================================================================================

void OS3DGame::OnUpdateEntity (const Packet::UpdateEntity& packet)
{
	Game::Core->Lock();
	{
		ModelInstance* inst = GetEntity(packet.id, false, false);

		if (inst != 0)
		{
			// Movement objective overrides everything else
			if (packet.objective == Entity::Objective::Move)
			{
				Vector3f pos (GetGamePos(packet.position));
				OSMove* move = inst->AddScript<OSMove>();
				move->Move(pos, packet.delay * 0.001f, packet.haste);
			}
			else
			{
				// Show the objective above the NPC's head
				//OSCombatText* text = inst->GetScript<OSCombatText>();
				//if (text != 0) text->Add(String("* Objective is now %u *", packet.objective));

				if (packet.objective < 40)
				{
					// Not in combat
					inst->RemoveScript<OSCombatAnimations>();
					inst->RemoveScript<OSMelee>();
				}
				else
				{
					// In combat -- start playing appropriate animations
					inst->RemoveScript<OSMove>();
					inst->AddScript<OSCombatAnimations>();

					// Save the rotation as we' might change it
					float rot = packet.rotation;
					float delay = packet.delay * 0.001f;

					// Find the target
					ModelInstance* target = (packet.objectiveId == 0) ? 0 : GetEntity(packet.objectiveId, false, false);

					// Calculate the rotation necessary to face this target
					if (target != 0)
					{
						// Make the face target action faster
						inst->AddScript<OSMelee>()->SetTarget(GetGamePos(packet.position), target);
						inst->RemoveScript<OSFace>();
					}
					else
					{
						inst->RemoveScript<OSMelee>();
					}

					// If we need to rotate the entity to face the target, let's do that
					if (inst->GetScript<OSMelee>() == 0)
					{
						float yaw = inst->GetRelativeRotation().GetFlatYaw();
						float diff = Float::Abs(WrapAngle(yaw - rot));

						if (diff > DEG2RAD(5.0f))
						{
							inst->AddScript<OSFace>()->Face(rot, delay);
						}
					}
				}
			}
		}
	}
	Game::Core->Unlock();
}

//============================================================================================================
// Callback triggered when a combat message arrives
//============================================================================================================

void OS3DGame::OnCombatMessage (const Packet::CombatMessage& packet)
{
	Game::Core->Lock();
	{
		ModelInstance* inst = GetEntity(packet.id, false, false);

		if (inst != 0)
		{
			OSCombatText*		sct		= inst->AddScript<OSCombatText>();
			OSHealthBar*		bar		= inst->AddScript<OSHealthBar>();
			OSCombatAnimations* anims	= inst->AddScript<OSCombatAnimations>();

			bar->SetHealth(packet.health);

			if (packet.action == 0)
			{
				anims->Attack();
			}
			else if (packet.action == 1)
			{
				anims->Hit();
			}
			else if (packet.action == 2)
			{
				anims->Dodge();
				sct->Add("* Dodged *", Color4ub(100, 100, 100));
			}
			else if (packet.action == 3)
			{
				anims->Block();
				sct->Add("* Blocked *", Color4ub(100, 100, 100));
			}

			// Damage (or healing) on-hit effects
			if (packet.damage > 0.0f) sct->Add(String("+%.0f", packet.damage), Color4ub(0, 255, 0));
			if (packet.damage < 0.0f) sct->Add(String("%.0f",  packet.damage), Color4ub(255, 0, 0));
		}
	}
	Game::Core->Unlock();
}

//============================================================================================================
// Callback triggered when entity's stats get updated
//============================================================================================================

void OS3DGame::OnUpdateStats (const Packet::UpdateStats& packet)
{
	Game::Core->Lock();
	{
		ModelInstance* inst = GetEntity(packet.id, false, false);

		if (inst != 0)
		{
			OSHealthBar* health = inst->AddScript<OSHealthBar>();
			health->SetHealth(packet.health);
		}
	}
	Game::Core->Unlock();
}

//============================================================================================================
// Callback triggered when an entity gets removed
//============================================================================================================

void OS3DGame::OnRemoveEntity (uint id)
{
	Game::Core->Lock();
	{
		ModelInstance* inst = GetEntity(id, false, false);
		if (inst != 0) inst->AddScript<OSDestroy>();
	}
	Game::Core->Unlock();
}