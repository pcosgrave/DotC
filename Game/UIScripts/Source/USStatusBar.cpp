//============================================================================================================
//  Defense of the Citadel, Copyright (c) 2010 Michael Lyashenko and Philip Cosgrave. All rights reserved.
//											www.nextrevision.com
//============================================================================================================

#include "../../Other/Include/_All.h"
using namespace R5;

//============================================================================================================
// There can only be one status bar
//============================================================================================================

UILabel* g_statusLabel = 0;
ulong g_fadeStart = 0;

//============================================================================================================
// Show the status bar with the specified message
//============================================================================================================

void USStatusBar::Show (const String& text)
{
	if (g_statusLabel != 0)
	{
		g_statusLabel->GetParent()->AddScript<USFadeIn>();
		g_statusLabel->SetText(text);
		g_fadeStart = Time::GetMilliseconds() + 4000;
	}
}

//============================================================================================================
// Hide the status bar
//============================================================================================================

void USStatusBar::Hide()
{
	if (g_statusLabel != 0 && g_fadeStart != 0)
	{
		g_fadeStart = Time::GetMilliseconds();
	}
}

//============================================================================================================
// Set the global pointer
//============================================================================================================

void USStatusBar::OnInit()
{
	UILabel* lbl = R5_CAST(UILabel, mWidget);

	if (lbl != 0)
	{
		g_statusLabel = lbl;
	}
	else
	{
		DestroySelf();
	}
}

//============================================================================================================
// Remove the global pointer reference
//============================================================================================================

void USStatusBar::OnDestroy()
{
	g_statusLabel = 0;
}

//============================================================================================================
// Fade out the status bar after some time has passed
//============================================================================================================

void USStatusBar::OnUpdate (bool changed)
{
	if (g_fadeStart != 0 && g_fadeStart < Time::GetMilliseconds())
	{
		g_fadeStart = 0;
		mWidget->GetParent()->AddScript<USFadeOut>()->SetDuration(4.0f);
	}
}