#pragma once

//============================================================================================================
//  Defense of the Citadel, Copyright (c) 2010 Michael Lyashenko and Philip Cosgrave. All rights reserved.
//											www.nextrevision.com
//============================================================================================================
// Simple debugger functionality
//============================================================================================================

namespace Debug
{
	// Sets the widget that will be used to store the logged messages
	void SetWidget (UITextArea* text);

	// Adds a new entry to the log
	void Log (const char* format, ...);

	// Logs a warning message
	void LogWarning (const char* format, ...);

	// Logs an error message
	void LogError (const char* format, ...);
};