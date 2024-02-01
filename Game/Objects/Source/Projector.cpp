//============================================================================================================
//  Defense of the Citadel, Copyright (c) 2010 Michael Lyashenko and Philip Cosgrave. All rights reserved.
//											www.nextrevision.com
//============================================================================================================

#include "../../Other/Include/_All.h"
using namespace R5;

//============================================================================================================
// Sets the texture used by the projector
//============================================================================================================

void Projector::SetTexture (const String& tex)
{
	IGraphics* graphics = GetGraphics();
	if (graphics != 0) SetTexture( graphics->GetTexture(tex) );
}

//============================================================================================================
// Sets the texture used by the projector
//============================================================================================================

void Projector::SetTexture (ITexture* tex)
{
	mTexture = tex;

	if (mTextures.IsEmpty())
	{
		mTextures.Expand() = tex;
	}
	else
	{
		mTextures[0] = tex;
	}
}

//============================================================================================================
// Called prior to object's Update function, before absolute coordinates are calculated
//============================================================================================================

void Projector::OnPreUpdate()
{
	// Projector should always point straight down
	Quaternion rot ( Vector3f(0.0f, 0.0f, -1.0f) );

	// If we've specified a spin amount, apply that
	if (mSpin != 0) rot = Quaternion(0.0f, 0.0f, mSpin) * rot;

	// Update the absolute rotation
	SetAbsoluteRotation(rot);

	// Default shader replaces the diffuse color of whatever it affects
	if (mShader == 0) SetShader("Decal/Replace_D");
}

//============================================================================================================
// Serialization -- Save
//============================================================================================================

void Projector::OnSerializeTo (TreeNode& node) const
{
	if (mShader  != 0) node.AddChild("Shader",  mShader->GetName());
	if (mTexture != 0) node.AddChild("Texture", mTexture->GetName());
	node.AddChild("Color", mColor);
}

//============================================================================================================
// Serialization -- Load
//============================================================================================================

bool Projector::OnSerializeFrom (const TreeNode& node)
{
	if (node.mTag == "Texture")
	{
		SetTexture(node.mValue.IsString() ? node.mValue.AsString() : node.mValue.GetString());
	}
	else if (node.mTag == "Shader")
	{
		SetShader(node.mValue.IsString() ? node.mValue.AsString() : node.mValue.GetString());
	}
	else if (node.mTag == "Color")
	{
		if (node.mValue.IsColor4f()) mColor = node.mValue.AsColor4f();
	}
	else return false;
	return true;
}