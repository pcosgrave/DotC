#pragma once

//============================================================================================================
//  Defense of the Citadel, Copyright (c) 2010 Michael Lyashenko and Philip Cosgrave. All rights reserved.
//											www.nextrevision.com
//============================================================================================================
// Downward-pointing decal
//============================================================================================================

class Projector : public Decal
{
protected:

	ITexture*	mTexture;
	float		mSpin;

public:

	// Object creation
	R5_DECLARE_INHERITED_CLASS("Projector", Projector, Decal, Object);

	Projector() : mTexture(0), mSpin(0.0f) {}

	const ITexture* GetTexture() const { return mTexture; }

	void SetTexture (const String& tex);
	void SetTexture (ITexture* tex);

	float GetSpin() const { return mSpin; }
	void SetSpin (float val) { mSpin = val; }

protected:

	// Called prior to object's Update function, before absolute coordinates are calculated
	virtual void OnPreUpdate();

	// Serialization
	virtual void OnSerializeTo	  (TreeNode& node) const;
	virtual bool OnSerializeFrom  (const TreeNode& node);
};