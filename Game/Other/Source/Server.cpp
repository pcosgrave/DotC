//============================================================================================================
//  Defense of the Citadel, Copyright (c) 2010 Michael Lyashenko and Philip Cosgrave. All rights reserved.
//											www.nextrevision.com
//============================================================================================================

#include "../../Other/Include/_All.h"
using namespace R5;

//============================================================================================================
// Set up the network callbacks
//============================================================================================================

Server::Server() : mSocketID(0)
{
	mNet.SetOnConnect( bind(&Server::OnConnect, this) );
	mNet.SetOnReceive( bind(&Server::OnReceive, this) );
	mNet.SetOnClose	 ( bind(&Server::OnClose,	this) );
	mNet.SetOnError	 ( bind(&Server::OnError,	this) );
	mNet.SetOnLog	 ( bind(&Server::OnLog,		this) );
	mNet.SpawnWorkerThread();
}

//============================================================================================================
// Clear the callbacks
//============================================================================================================

Server::~Server()
{
	mNet.SetOnConnect(0);
	mNet.SetOnReceive(0);
	mNet.SetOnClose	 (0);
	mNet.SetOnError	 (0);
	mNet.SetOnLog	 (0);
}

//============================================================================================================
// Connection established callback
//============================================================================================================

void Server::OnConnect (const Address& addr, uint socketID, VoidPtr& param, const String& destination)
{
	Debug::Log("[FF9933]NET:[FFFFFF] Connected to %s (%u)", addr.ToString().GetBuffer(), socketID);

	BeginSend(Packet::ID::Greeting);
	{
		mOut.Append(GetVersion());
		mOut.Append(Game::Name);
	}
	EndSend();
}

//============================================================================================================
// Connection closed callback
//============================================================================================================

void Server::OnClose (const Address& addr, uint socketID, VoidPtr& param)
{
	Debug::Log("[FF9933]NET:[FFFFFF] Disconnected from the server (%s)", addr.ToString().GetBuffer());
	if (mSocketID != 0 && onConnect) onConnect(false);
	mSocketID = 0;
}

//============================================================================================================
// Received data callback
//============================================================================================================

void Server::OnReceive (const Address& addr, uint socketID, VoidPtr& param, const byte* buffer, uint size, Thread::IDType threadID)
{
	mIn.Lock();
	{
		mIn.Append(buffer, size);
		if (!Process(socketID)) mNet.Close(socketID);
	}
	mIn.Unlock();
}

//============================================================================================================
// Callback triggered when an error occurs
//============================================================================================================

void Server::OnError (const Address& addr, uint socketID, VoidPtr& param, const char* msg)
{
	Debug::LogError("NET: %s", msg);
	if (mSocketID == 0 && onConnect) onConnect(false);
}

//============================================================================================================
// Callback triggered when an error occurs
//============================================================================================================

void Server::OnLog (const String& log)
{
	Debug::Log("[FF9933]NET:[FFFFFF] %s", log.GetBuffer());
}

//============================================================================================================
// Add this message to the chat window
//============================================================================================================

void Server::OnNotification (const String& msg)
{
	if (Game::Chat != 0)
	{
		Game::Chat->AddParagraph(msg);
	}
	else USStatusBar::Show(msg);
}

//============================================================================================================
// Add this message to the chat window
//============================================================================================================

void Server::OnMessage (const Packet::ChatMessage& packet)
{
	if (Game::Chat != 0)
	{
		String full;
		full << "[00FF55]";
		full << packet.name;
		full << "[FFFFFF]: ";
		full << packet.message;
		Game::Chat->AddParagraph(full);
	}
}

//============================================================================================================
// Adds a message to chat informing the player name change
//============================================================================================================

void Server::OnPlayerRenamed (const Packet::PlayerName& packet)
{
	if (Game::Chat != 0)
	{
		String full;
		full << "[00FF55]";
		full << packet.before;
		full << "[FFFFFF] is now known as [00FF55]";
		full << packet.after;
		full << "[FFFFFF].";
		Game::Chat->AddParagraph(full);
	}

	// Update the local name if it's ours
	if (packet.before == Game::Name) Game::Name = packet.after;
}

//============================================================================================================
// Convenience functionality -- starts the send process, locks the buffer, and enters the ID
//============================================================================================================

void Server::BeginSend (byte eventID)
{
	mOut.Lock();
	mOut.Clear();
	mOut.Expand(4);
	*mOut.Expand(1) = eventID;
}

//============================================================================================================
// Convenience functionality -- finishes the send process,
// updating the size of the packet and actually sends the data
//============================================================================================================

void Server::EndSend()
{
	uint size = mOut.GetSize();
	byte* buffer = mOut.GetBuffer();
	*((uint32*)buffer) = (uint32)(size - 4);

	mNet.Send(buffer, size);
	mOut.Unlock();
}

//============================================================================================================
// Process all packets
//============================================================================================================

bool Server::Process (uint socketID)
{
	const byte* buffer = mIn.GetBuffer();
	uint size = mIn.GetSize();
	uint original = size;
	uint packetSize;

	// Keep extracting and processing packets until we can't process any more
	while (size > 4)
	{
		packetSize = (*(const uint32*)buffer);
		if (size < packetSize) break;

		// Skip past the packet size
		buffer += 4;
		size   -= 4;

		// The first packet must be small
		if (mSocketID == 0 && packetSize > 32)
			return false;

		// Process this single packet
		if (!ProcessPacket(socketID, buffer, packetSize))
		{
			Debug::LogError("NET: Failed to process a packet (%u bytes)", size);
			return false;
		}

		// Move on to the next packet
		buffer	+= packetSize;
		size	-= packetSize;
	}

	// Remove the processed data
	if (size < original) mIn.Remove(original - size);
	return true;
}

//============================================================================================================
// Process a single packet on the client side
//============================================================================================================

bool Server::ProcessPacket (uint socketID, const byte* buffer, uint size)
{
	byte packetID;
	if (!Memory::Extract(buffer, size, packetID)) return false;

	if (mSocketID != 0)
	{
		if (packetID == Packet::ID::UpdateEntity)
		{
			Packet::UpdateEntity packet;
			if (!Memory::Extract(buffer, size, packet)) return false;
			if (onUpdateEntity) onUpdateEntity(packet);
		}
		else if (packetID == Packet::ID::CombatMessage)
		{
			Packet::CombatMessage packet;
			if (!Memory::Extract(buffer, size, packet)) return false;
			if (onCombatMessage) onCombatMessage(packet);
		}
		else if (packetID == Packet::ID::UpdateStats)
		{
			Packet::UpdateStats packet;
			if (!Memory::Extract(buffer, size, packet)) return false;
			if (onUpdateStats) onUpdateStats(packet);
		}
		else if (packetID == Packet::ID::AddEntity)
		{
			Entity ent;
			if (!Memory::Extract(buffer, size, ent)) return false;
			if (onAddEntity) onAddEntity(ent);
		}
		else if (packetID == Packet::ID::RemoveEntity)
		{
			uint id;
			if (!Memory::Extract(buffer, size, id)) return false;
			if (onRemoveEntity) onRemoveEntity(id);
		}
		else if (packetID == Packet::ID::ChatMessage)
		{
			Packet::ChatMessage packet;

			if (!Memory::Extract(buffer, size, packet.name) ||
				!Memory::Extract(buffer, size, packet.message)) return false;

			OnMessage(packet);
		}
		else if (packetID == Packet::ID::PlayerName)
		{
			Packet::PlayerName packet;

			if (!Memory::Extract(buffer, size, packet.before) ||
				!Memory::Extract(buffer, size, packet.after)) return false;

			OnPlayerRenamed(packet);
		}
		else if (packetID == Packet::ID::Notification)
		{
			String msg;
			if (!Memory::Extract(buffer, size, msg)) return false;
			OnNotification(msg);
		}
		else if (packetID == Packet::ID::ModifyHeightmap)
		{
			Packet::ModifyHeightmap packet;
			if (!Memory::Extract(buffer, size, packet)) return false;

			uint expected ((uint)packet.width * packet.height);
			if (size < expected) return false;

			// Modify the world data
			Game::World->ModifyHeightmap(packet.x, packet.y, packet.width, packet.height, (const byte*)buffer);

			// Call the implementation-set listener
			if (onModifyHeightmap) onModifyHeightmap(packet, (const byte*)buffer);
		}
		else if (packetID == Packet::ID::ServerInfo)
		{
			Packet::ServerInfo packet;
			if (!Memory::Extract(buffer, size, packet.description) ||
				!Memory::Extract(buffer, size, packet.players) ||
				!Memory::Extract(buffer, size, packet.uptime) ||
				!Memory::Extract(buffer, size, packet.width) ||
				!Memory::Extract(buffer, size, packet.height)) return false;

			uint expected ((uint)packet.width * packet.height);
			if (size < expected) return false;

			// Set the world height map
			Game::World->ModifyHeightmap(0, 0, packet.width, packet.height, (const byte*)buffer);

			// Notify the registered listener
			if (onServerInfo) onServerInfo(packet, (const byte*)buffer);
		}
		else if (packetID == Packet::ID::EntityList)
		{
			uint count(0);

			if (!Memory::Extract(buffer, size, count)) return false;

			Entity ent;

			for (uint i = 0; i < count; ++i)
			{
				if (!Memory::Extract(buffer, size, &ent, sizeof(Entity))) return false;
				if (onAddEntity) onAddEntity(ent);
			}
		}
		else return false;
	}
	else
	{
		uint version;
		String name;

		if (packetID == Packet::ID::Greeting && Memory::Extract(buffer, size, version))
		{
			if (onConnect) onConnect(true);

			// Local notification
			String msg;
			msg << "Connected to the server version ";
			msg << version;
			OnNotification(msg);

			if (version != GetVersion())
			{
				Debug::LogError("NET: The server's version number is different than yours! (Your version: %u)", version);
			}
			mSocketID = socketID;
			return true;
		}
		else
		{
			Debug::LogError("NET: The server never sent a greeting!");
			return false;
		}
	}
	return true;
}

//============================================================================================================
// Connect to the specified destination
//============================================================================================================

void Server::Connect (const String& addr)
{
	Disconnect();
	mNet.Connect(addr);
	Debug::Log("[FF9933]NET:[FFFFFF] Connecting to %s...", addr.GetBuffer());
}

//============================================================================================================
// Disconnect from all connected clients
//============================================================================================================

void Server::Disconnect()
{
	mNet.Disconnect();
	mSocketID = 0;
}

//============================================================================================================
// Sends a join game packet, notifying the server that we're ready to play
//============================================================================================================

void Server::JoinGame()
{
	if (IsConnected())
	{
		BeginSend(Packet::ID::JoinGame);
		EndSend();
	}
}

//============================================================================================================
// Sends a public chat message to everyone
//============================================================================================================

void Server::SendMessage (const String& message)
{
	if (IsConnected())
	{
		BeginSend(Packet::ID::ChatMessage);
		{
			mOut.Append(Game::Name);
			mOut.Append(message);
		}
		EndSend();
	}
	else if (Game::Chat != 0)
	{
		Game::Chat->AddParagraph("You can't send messages while working offline. Try connecting!");
		return;
	}
}

//============================================================================================================
// Changes the player's name
//============================================================================================================

void Server::SetPlayerName (const String& name)
{
	if (IsConnected())
	{
		BeginSend(Packet::ID::PlayerName);
		{
			mOut.Append(Game::Name);
			mOut.Append(name);
		}
		EndSend();
	}
}

//============================================================================================================
// Add a entity to the sever
//============================================================================================================

void Server::AddEntity (uint type, const Vector3i& pos, const float rot, const float scale,
						const Vector3i& offset, const Vector3i& size)
{
	if (IsConnected())
	{
		Packet::AddEntity packet;
		packet.type		= type;
		packet.pos		= pos;
		packet.rot		= rot;
		packet.scale	= scale;
		packet.offset	= offset;
		packet.size		= size;

		BeginSend(Packet::ID::AddEntity);
		mOut.Append(packet);
		EndSend();
	}
}

//============================================================================================================
// Removes the specified entity from the world
//============================================================================================================

void Server::RemoveEntity (uint id)
{
	if (IsConnected())
	{
		BeginSend(Packet::ID::RemoveEntity);
		mOut.Append(id);
		EndSend();
	}
}

//============================================================================================================
// Sets an objective for the specified entity
//============================================================================================================

void Server::SetObjective (uint entity, byte objective, const Vector3i& pos)
{
	if (IsConnected())
	{
		Packet::SetObjective packet;

		packet.id		 = entity;
		packet.objective = objective;
		packet.position  = pos;
		packet.target	 = 0;

		BeginSend(Packet::ID::SetObjective);
		mOut.Append(packet);
		EndSend();
	}
}

//============================================================================================================
// Sets an objective for the specified entity
//============================================================================================================

void Server::SetObjective (uint entity, byte objective, uint target)
{
	if (IsConnected())
	{
		Packet::SetObjective packet;

		packet.id		 = entity;
		packet.objective = objective;
		packet.position	 = Vector3i();
		packet.target	 = target;

		BeginSend(Packet::ID::SetObjective);
		mOut.Append(packet);
		EndSend();
	}
}