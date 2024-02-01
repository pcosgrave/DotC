#pragma once

//============================================================================================================
//  Defense of the Citadel, Copyright (c) 2010 Michael Lyashenko and Philip Cosgrave. All rights reserved.
//											www.nextrevision.com
//============================================================================================================
// Script that controls visible selection around the object
//============================================================================================================

class OSSelection : public Script
{
protected:

	Projector*	mProjector;
	Color4f		mStartColor;
	Color4f		mTargetColor;
	Vector3f	mRadius;
	ulong		mStartMS;
	float		mDuration;

public:

	R5_DECLARE_ABSTRACT_CLASS("OSSelection", Script);

	OSSelection();
	virtual ~OSSelection() { Release(); }

	// Releases the projector associated with this script
	void Release();

	// Fade in the projection, giving it the appropriate color
	void Show(float r, float g, float b);

	// Fade out and destroy the projection
	void Hide();

protected:

	// Derived classes must override
	virtual const char* GetObjectName()  const=0;
	virtual const char* GetTextureName() const=0;
	virtual float		GetRadius()		 const=0;

	// Destroys the projector in addition to the script
	virtual void OnDestroy() { Release(); }

	// Animate the selection
	virtual void OnPreUpdate();
};