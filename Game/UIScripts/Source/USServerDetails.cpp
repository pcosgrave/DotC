//============================================================================================================
//  Defense of the Citadel, Copyright (c) 2010 Michael Lyashenko and Philip Cosgrave. All rights reserved.
//											www.nextrevision.com
//============================================================================================================

#include "../../Other/Include/_All.h"
using namespace R5;

//============================================================================================================
// Bind event listener callbacks
//============================================================================================================

void USServerDetails::OnInit()
{
	Game::Server->onServerInfo = bind(&USServerDetails::OnServerInfo, this);
}

//============================================================================================================
// Remove all listener callbacks
//============================================================================================================

void USServerDetails::OnDestroy()
{
	Game::Server->onServerInfo.clear();
}

//============================================================================================================
// Changes the terrain
//============================================================================================================

void USServerDetails::OnServerInfo (const Packet::ServerInfo& packet, const byte* data)
{
	mColors.Lock();

	// Find the highest point
	uint size = (uint)packet.width * packet.height;
	for (uint i = 0; i < size; ++i) if (mDepth < data[i]) mDepth = data[i];

	// Create the color array that will be used to create the map texture
	if (mColors.IsEmpty())
	{
		mWidth  = packet.width;
		mHeight = packet.height;
		mColors.ExpandTo(size, true);
	}

	// Server information
	if (mInfo != 0)
	{
		uint seconds = packet.uptime % 60;
		uint hours	 = packet.uptime / 3600;
		uint minutes = packet.uptime / 60 - hours * 60;

		mInfo->AddParagraph(String("Terrain: [996633]%u x %u x %u", mWidth, mHeight, mDepth));
		mInfo->AddParagraph(String("Server up time: [996633]%u %s, %u %s, %u %s",
			hours, (hours == 1 ? "hour" : "hours"),
			minutes, (minutes == 1 ? "minute" : "minutes"),
			seconds, (seconds == 1 ? "second" : "seconds")));

		String players ("Players: [996633]");
		players << ((packet.players.IsEmpty()) ? "<None>" : packet.players);
		mInfo->AddParagraph(players);
	}

	// Server description
	if (mDesc != 0) mDesc->AddParagraph(packet.description);

	float invDepth = (mDepth == 0) ? 1.0f : 1.0f / mDepth;

	SplineV colorSpline;
	colorSpline.AddKey(0.0f, Vector3f(0.05f, 0.1f, 0.15f));
	colorSpline.AddKey(0.5f, Vector3f(0.15f, 0.5f, 0.15f));
	colorSpline.AddKey(1.0f, Vector3f(1.0f, 1.0f, 0.25f));

	// Fill the color array
	for (uint y = 0, ymax = packet.width; y < ymax; ++y)
	{
		for (uint x = 0, xmax = packet.width; x < xmax; ++x)
		{
			uint index = y * mWidth + x;
			byte height = data[index];
			Color4ub& c = mColors[index];
			float val = height * invDepth;
			Vector3f v = colorSpline.Sample(val, 1);
			c.r = Float::ToRangeByte(v.x);
			c.g = Float::ToRangeByte(v.y);
			c.b = Float::ToRangeByte(v.z);
			c.a = 255;
		}
	}

	mIsDirty = true;
	mColors.Unlock();
}

//============================================================================================================
// Update the map's texture
//============================================================================================================

void USServerDetails::OnUpdate (bool areaChanged)
{
	if (mIsDirty && mMap != 0)
	{
		mColors.Lock();
		{
			mIsDirty = false;
			ITexture* tex = Game::Graphics->GetTexture("Map Texture");
			tex->SetFiltering(ITexture::Filter::Nearest);
			tex->Set(mColors.GetBuffer(), mWidth, mHeight, 1, ITexture::Format::RGBA, ITexture::Format::RGB);
			mMap->SetTexture(tex);
		}
		mColors.Unlock();
	}
}