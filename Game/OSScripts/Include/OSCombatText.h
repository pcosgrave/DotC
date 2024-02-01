#pragma once

//============================================================================================================
//  Defense of the Citadel, Copyright (c) 2010 Michael Lyashenko and Philip Cosgrave. All rights reserved.
//											www.nextrevision.com
//============================================================================================================
// Combat text display
//============================================================================================================

class OSCombatText : public Script
{
private:

	struct Entry
	{
		String	 mText;
		ulong	 mStart;
		Color4ub mColor;
		float	 mCurrentOffset;
		float	 mTargetOffset;
	};

	IFont* mFont;
	PointerArray<Entry> mUsed;
	PointerArray<Entry> mUnused;

	SplineF mScale;
	SplineF mOffset;
	SplineF mAlpha;

public:

	R5_DECLARE_INHERITED_CLASS("OSCombatText", OSCombatText, Script, Script);

	OSCombatText() : mFont(0) {}
	
	void Add (const String& text, const Color4ub& color = Color4ub(255, 255, 255));

	virtual void OnInit();
	virtual void OnFill(FillParams& params);
};