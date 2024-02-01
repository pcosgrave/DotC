#pragma once

//============================================================================================================
//  Defense of the Citadel, Copyright (c) 2010 Michael Lyashenko and Philip Cosgrave. All rights reserved.
//											www.nextrevision.com
//============================================================================================================
// Server controller class -- all in-game actions go through the server
//============================================================================================================

class Server
{
private:

	uint		mSocketID;		// SocketID will be set to non-zero if the server authenticates
	Memory		mIn;			// Memory buffer used for the incoming data
	Memory		mOut;			// Temporary memory buffer used for writing
	Random		mRandom;		// Random number generator, server-side
	Network		mNet;			// Network communications layer

public:

	// Connection attempt callback
	typedef FastDelegate<void (bool successful)> OnConnectCallback;

	// Terrain modification callback
	typedef FastDelegate<void (const Packet::ModifyHeightmap& packet, const byte* data)> ModifyHeightmapCallback;

	// Server information callback
	typedef FastDelegate<void (const Packet::ServerInfo& packet, const byte* data)> ServerInfoCallback;

	// Callback triggered when an entity gets added
	typedef FastDelegate<void (const Entity& ent)> AddEntityCallback;

	// Callback triggered when the entity's objective changes
	typedef FastDelegate<void (const Packet::UpdateEntity& packet)> UpdateEntityCallback;

	// Callback triggered when the entity combat update message arrives
	typedef FastDelegate<void (const Packet::CombatMessage& packet)> CombatMessageCallback;

	// Callback triggered when the entity's stats change
	typedef FastDelegate<void (const Packet::UpdateStats& packet)> UpdateStatsCallback;

	// Callback triggered when an entity gets removed
	typedef FastDelegate<void (uint id)> RemoveEntityCallback;

	// Registered callback functions are exposed for simplicity's sake
	OnConnectCallback		onConnect;
	ServerInfoCallback		onServerInfo;
	ModifyHeightmapCallback	onModifyHeightmap;
	AddEntityCallback		onAddEntity;
	UpdateEntityCallback	onUpdateEntity;
	CombatMessageCallback	onCombatMessage;
	UpdateStatsCallback		onUpdateStats;
	RemoveEntityCallback	onRemoveEntity;

public:

	Server();
	~Server();

private:

	typedef Network::Address Address;

	// Connection established callback
	void OnConnect (const Address& addr, uint socketID, VoidPtr& param, const String& destination);

	// Connection closed callback
	void OnClose (const Address& addr, uint socketID, VoidPtr& param);

	// Received data callback
	void OnReceive (const Address& addr, uint socketID, VoidPtr& param, const byte* buffer, uint size, Thread::IDType threadID);

	// Callback triggered when an error occurs
	void OnError (const Address& addr, uint socketID, VoidPtr& param, const char* msg);

	// Callback triggered when an error occurs
	void OnLog (const String& log);

	// Adds a new paragraph to the chat window
	void OnNotification (const String& msg);

	// Adds a new paragraph to the chat window
	void OnMessage (const Packet::ChatMessage& packet);

	// Adds a message to chat informing the player name change
	void OnPlayerRenamed (const Packet::PlayerName& packet);

protected:

	// Convenience functionality -- starts the send process, locks the buffer, and enters the ID
	void BeginSend (byte eventID);

	// Convenience functionality -- finishes the send process, updating the
	// size of the packet and actually sends the data
	void EndSend();

	// Process all packets
	bool Process (uint socketID);

	// Process this single packet
	bool ProcessPacket (uint socketID, const byte* buffer, uint size);

public:

	// Communication protocol version
	uint GetVersion() const { return 170; }

	// Whether we're connected to anyone
	bool IsConnected() const { return mSocketID != 0; }

	// Connect to the specified destination
	void Connect (const String& addr);

	// Disconnect from all connected clients
	void Disconnect();

	// Sends a join game packet, notifying the server that we're ready to play
	void JoinGame();

	// Sends a public chat message to everyone
	void SendMessage (const String& message);

	// Changes the player's name
	void SetPlayerName (const String& name);

	// Removes the specified entity from the world
	void RemoveEntity (uint id);

	// Sets an objective for the specified entity
	void SetObjective (uint entity, byte objective, const Vector3i& pos);
	void SetObjective (uint entity, byte objective, uint target = 0);

	// Add a new entity to the world
	void AddEntity (uint type, const Vector3i& pos, const float rot, const float scale,
		const Vector3i& offset, const Vector3i& size);

	// TESTING: Add a tree at the specified location
	void AddTree (const Vector3i& v);

	// TESTING: Adds an NPC to the specified location
	void AddNPC (ushort type, const Vector3i& v);
};