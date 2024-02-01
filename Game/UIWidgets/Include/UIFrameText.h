#pragma once

//============================================================================================================
//                  R5 Engine, Copyright (c) 2007-2010 Michael Lyashenko. All rights reserved.
//											www.nextrevision.com
//============================================================================================================
// Widget for one-shot text: it will draw text on the screen once and then discard it.
// It's meant to be used for text that's potentially changing or moving every single frame, such as
// Scrolling Combat Text, players names, debug output, etc.
//============================================================================================================

class UIFrameText : public UIWidget
{
private:

	// Each font has an associated vertex array
	struct FontEntry
	{
		IFont* font;
		IFont::Vertices verts;

		// Adds a new line to the font entry struct
		void Add (const String& text, const Vector2i& pos, float scale, const Color4ub& color);
	};

	typedef FontEntry* FontEntryPtr;

	// Array of font entries, each one containing a list of text entries valid for the next frame
	PointerArray<FontEntry> mEntries;

	// Temporary vertex array used for gathering all text entries into one draw call
	IFont::Vertices mVertices;

public:

	// Area creation
	R5_DECLARE_INHERITED_CLASS("UIFrameText", UIFrameText, UIWidget, UIWidget);

	// Adds a new text entry to be displayed next frame
	void Add (const String& text, const Vector2i& pos, IFont* font = 0, float scale = 1.0f,
		const Color4ub& color = Color4ub(0xFFFFFFFF));

	// Draws all text and then discards all entries
	virtual uint OnDraw();
};