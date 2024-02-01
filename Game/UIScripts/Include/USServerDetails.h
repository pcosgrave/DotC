#pragma once

//============================================================================================================
//  Defense of the Citadel, Copyright (c) 2010 Michael Lyashenko and Philip Cosgrave. All rights reserved.
//											www.nextrevision.com
//============================================================================================================
// Server details script updates the server information window
//============================================================================================================

class USServerDetails : public UIScript
{
	Array<Color4ub> mColors;
	uint			mWidth;
	uint			mHeight;
	uint			mDepth;
	UIPicture*		mMap;
	UITextArea*		mInfo;
	UITextArea*		mDesc;
	bool			mIsDirty;

	USServerDetails() : mWidth(0), mHeight(0), mDepth(0), mMap(0), mInfo(0), mDesc(0), mIsDirty(false) {}

public:

	R5_DECLARE_INHERITED_CLASS("USServerDetails", USServerDetails, UIScript, UIScript);

	virtual void OnInit();
	virtual void OnDestroy();

public:

	void SetMap  (UIPicture*  area) { mMap  = area; }
	void SetInfo (UITextArea* area) { mInfo = area; }
	void SetDesc (UITextArea* area) { mDesc = area; }

	// Changes the terrain
	void OnServerInfo (const Packet::ServerInfo& packet, const byte* data);

	// Callback triggered when an entity gets added
	void OnAddEntity (const Entity& ent);

public:

	// Update the map's texture
	virtual void OnUpdate (bool areaChanged);
};