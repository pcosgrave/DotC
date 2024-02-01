#pragma once

//============================================================================================================
//  Defense of the Citadel, Copyright (c) 2010 Michael Lyashenko and Philip Cosgrave. All rights reserved.
//											www.nextrevision.com
//============================================================================================================
// Loading screen script
//============================================================================================================

class USLoadingScreen : public UIScript
{
	Core*		mCore;
	UISlider*	mProgress;
	UILabel*	mCaption;
	bool		mIsFading;
	bool		mJustStarted;
	ITexture*	mStartTex;
	ITexture*	mLastTex;

	USLoadingScreen() : mCore(0), mProgress(0), mCaption(0),
		mIsFading(false), mJustStarted(false), mStartTex(0), mLastTex(0) {}

public:

	R5_DECLARE_INHERITED_CLASS("USLoadingScreen", USLoadingScreen, UIScript, UIScript);

	static void Create();

	void Show();

	virtual void OnUpdate (bool areaChanged);
};