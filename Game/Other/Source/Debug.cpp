//============================================================================================================
//  Defense of the Citadel, Copyright (c) 2010 Michael Lyashenko and Philip Cosgrave. All rights reserved.
//											www.nextrevision.com
//============================================================================================================

#include "../../Other/Include/_All.h"
#include <stdarg.h>
using namespace R5;

//============================================================================================================
// Text widget that will be used to log all incoming messages
//============================================================================================================

UITextArea* g_widget = 0;
Thread::Lockable g_lock;
String g_text;

//============================================================================================================
// Sets the widget that will be used to store the logged messages
//============================================================================================================

void Debug::SetWidget (UITextArea* text)
{
	if (g_widget != text)
	{
		if (g_widget != 0)
		{
			g_widget->AddParagraph( String("Debug target is changing to %s",
				(text != 0) ? text->GetName().GetBuffer() : "<null>"), Color3f(1.0f, 1.0f, 0.2f) );
		}

		g_widget = text;

		if (g_widget != 0)
		{
			g_widget->AddParagraph( String("Debug target has been set to this widget."),
				Color3f(1.0f, 1.0f, 0.2f) );
		}
	}
}

//============================================================================================================
// Adds a new entry to the log
//============================================================================================================

void Debug::Log (const char* format, ...)
{
	g_lock.Lock();
	{
		va_list args;
		va_start(args, format);

#ifdef _WINDOWS
		uint iLength = _vscprintf(format, args) + 1;
		g_text.Resize(iLength);
#else
		g_text.Resize(2048);
#endif

		vsprintf(g_text.GetBuffer(), format, args);
		va_end(args);

		if (g_widget != 0)
		{
			g_widget->AddParagraph(g_text, Color3f(1.0f, 1.0f, 1.0f));
		}
#ifdef _DEBUG
		else
		{
			System::Log(g_text.GetBuffer());
		}
#endif
	}
	g_lock.Unlock();
}

//============================================================================================================
// Logs a warning message
//============================================================================================================

void Debug::LogWarning (const char* format, ...)
{
	g_lock.Lock();
	{
		va_list args;
		va_start(args, format);

#ifdef _WINDOWS
		uint iLength = _vscprintf(format, args) + 1;
		g_text.Resize(iLength);
#else
		g_text.Resize(2048);
#endif

		vsprintf(g_text.GetBuffer(), format, args);
		va_end(args);

		if (g_widget != 0)
		{
			g_widget->AddParagraph(g_text, Color3f(1.0f, 1.0f, 0.0f));
		}
		else
		{
			USStatusBar::Show(g_text);
			System::Log("WARNING: %s", g_text.GetBuffer());
		}
	}
	g_lock.Unlock();
}

//============================================================================================================
// Logs an error message
//============================================================================================================

void Debug::LogError (const char* format, ...)
{
	g_lock.Lock();
	{
		va_list args;
		va_start(args, format);

#ifdef _WINDOWS
		uint iLength = _vscprintf(format, args) + 1;
		g_text.Resize(iLength);
#else
		g_text.Resize(2048);
#endif

		vsprintf(g_text.GetBuffer(), format, args);
		va_end(args);

		if (g_widget != 0)
		{
			g_widget->AddParagraph(g_text, Color3f(1.0f, 0.2f, 0.0f));
		}
		else
		{
			USStatusBar::Show(g_text);
			System::Log("ERROR: %s", g_text.GetBuffer());
		}
	}
	g_lock.Unlock();
}