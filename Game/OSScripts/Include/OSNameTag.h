#pragma once

//============================================================================================================
//  Defense of the Citadel, Copyright (c) 2010 Michael Lyashenko and Philip Cosgrave. All rights reserved.
//											www.nextrevision.com
//============================================================================================================
// Adding this script to an object will show its name above its head (or the overriding text if specified)
//============================================================================================================

class OSNameTag : public Script
{
	IFont* mFont;
	String mText;

public:

	R5_DECLARE_INHERITED_CLASS("OSNameTag", OSNameTag, Script, Script);

	OSNameTag() : mFont(0) {}

	const String& GetText() const { return mText; }
	void SetText (const String& text) { mText = text; }

	// OnInit function is called when the script has been created and all internal parameters have been set
	virtual void OnInit();

	// Called when the scene draw queue is being filled. This happens once per Scene::Cull call.
	virtual void OnFill (FillParams& params);
};