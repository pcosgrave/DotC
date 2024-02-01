//============================================================================================================
//  Defense of the Citadel, Copyright (c) 2010 Michael Lyashenko and Philip Cosgrave. All rights reserved.
//											www.nextrevision.com
//============================================================================================================

#include "../../Other/Include/_All.h"
using namespace R5;

//============================================================================================================
// Adds a new line to the font entry struct
//============================================================================================================

void UIFrameText::FontEntry::Add (const String& text, const Vector2i& pos, float scale, const Color4ub& color)
{
	Vector2f fp (pos.x, pos.y);
	Vector2f tl (fp.x - 0.5f * font->GetLength(text), fp.y - 0.5f * font->GetSize());

	uint start = verts.GetSize();
	font->Print(verts, tl, color, text, 0, 0xFFFFFFFF, IFont::Tags::Process);

	if (scale != 1.0f)
	{
		for (uint i = start; i < verts.GetSize(); ++i)
		{
			verts[i].mPos = fp + (verts[i].mPos - fp) * scale;
		}
	}
}

//============================================================================================================
// Adds a new text entry to be displayed next frame
//============================================================================================================

void UIFrameText::Add (const String& text, const Vector2i& pos, IFont* font, float scale, const Color4ub& color)
{
	if (font == 0) font = mUI->GetDefaultFont();
	if (font == 0) return;

	mEntries.Lock();
	{
		// Try to find an existing font entry
		FOREACH(i, mEntries)
		{
			FontEntryPtr& fe = mEntries[i];

			if (fe->font == font)
			{
				fe->Add(text, pos, scale, color);
				mEntries.Unlock();
				return;
			}
		}

		// Add a new entry
		FontEntry* fe = new FontEntry();
		fe->font = font;
		fe->Add(text, pos, scale, color);
		mEntries.Expand() = fe;
	}
	mEntries.Unlock();
}

//============================================================================================================
// Draws all text and discards it
//============================================================================================================

uint UIFrameText::OnDraw()
{
	uint tri (0);

	if (mEntries.IsValid())
	{
		mEntries.Lock();
		{
			Game::Graphics->SetBlending(IGraphics::Blending::Replace);

			FOREACH(i, mEntries)
			{
				FontEntry* fe = mEntries[i];

				if (fe->verts.IsValid())
				{
					// Activate the texture and the vertex arrays
					Game::Graphics->SetActiveTexture(0, fe->font->GetTexture());
					Game::Graphics->SetActiveVertexAttribute(IGraphics::Attribute::TexCoord0,
						&fe->verts[0].mTc, IGraphics::DataType::Float, 2, sizeof(IFont::Vertex));
					Game::Graphics->SetActiveVertexAttribute(IGraphics::Attribute::Color,
						&fe->verts[0].mColor, IGraphics::DataType::Byte, 4, sizeof(IFont::Vertex));
					Game::Graphics->SetActiveVertexAttribute(IGraphics::Attribute::Position,
						&fe->verts[0].mPos, IGraphics::DataType::Float, 2, sizeof(IFont::Vertex));

					// Draw the vertices
					tri += Game::Graphics->DrawVertices(IGraphicsManager::Primitive::Quad, fe->verts.GetSize());

					// Cleanup
					fe->verts.Clear();
				}
			}
		}
		mEntries.Unlock();
	}
	return tri;
}