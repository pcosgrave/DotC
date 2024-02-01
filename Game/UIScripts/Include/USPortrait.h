#pragma once

//============================================================================================================
//  Defense of the Citadel, Copyright (c) 2010 Michael Lyashenko and Philip Cosgrave. All rights reserved.
//											www.nextrevision.com
//============================================================================================================
// Base class for desplaying a object through the UI
//============================================================================================================

class USPortrait : public UIScript
{
protected:

	Object*			mRoot;
	Camera*			mCamera;
	ITexture*		mColor;
	ITexture*		mDepth;
	IRenderTarget*	mRenderTarget;
	Vector2i		mSize;

public:

	R5_DECLARE_INHERITED_CLASS("USPortrait", USPortrait, UIScript, UIScript);

	USPortrait() : mRoot(0), mCamera(0), mColor(0), mDepth(0), mRenderTarget(0) {} 

	virtual void OnInit();
	virtual void OnDestroy();
	virtual void OnUpdate(bool areaChanged);
};
