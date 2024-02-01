//============================================================================================================
//  Defense of the Citadel, Copyright (c) 2010 Michael Lyashenko and Philip Cosgrave. All rights reserved.
//											www.nextrevision.com
//============================================================================================================

#include "../../Other/Include/_All.h"
using namespace R5;

//============================================================================================================
// Handle '/' and 'return' keys
//============================================================================================================

uint USChatInput::OnGlobalKey (const Vector2i& pos, byte key, bool isDown)
{
	if (key == Key::Slash)
	{
		Game::Input->SetText("");
		Game::Input->SetFocus(true);
		return 1;
	}
	else if (key == Key::Return)
	{
		// When the enter key is pressed, start typing
		if (!isDown)
		{
			Game::Input->SetFocus(true);
			Game::Input->SelectAll();
		}
		return 1;
	}
	return 0;
}

//============================================================================================================
// Set the game input field
//============================================================================================================

void USChatInput::OnInit()
{
	Game::Input = R5_CAST(UIInput, mWidget);

	if (Game::Input != 0)
	{
		Game::Input->SetText("<press Enter to start typing>");
		Game::Core->AddOnKey( bind(&USChatInput::OnGlobalKey, this), 100000 );
	}
}

//============================================================================================================
// Lose focus
//============================================================================================================

void USChatInput::OnDestroy()
{
	Game::Input = 0;
	Game::Core->RemoveOnKey( bind(&USChatInput::OnGlobalKey, this) );
}

//============================================================================================================
// When the chat input gets focus, show the complete chat log
//============================================================================================================

void USChatInput::OnFocus (bool gotFocus)
{
	if (gotFocus && Game::Chat != 0)
	{
		Game::Chat->ResetFadeTimer();
	}
}

//============================================================================================================
// Triggered when the input's value changes
//============================================================================================================

void USChatInput::OnValueChange()
{
	static String defaultText ("<press Enter to start typing>");
	UIInput* input = R5_CAST(UIInput, mWidget);

	if (input != 0 && input->GetText() != defaultText)
	{
		const String& text = input->GetText();

		if (text.IsValid())
		{
			// In-game commands begin with a slash
			if (text[0] == '/')
			{
				String command, arg;
				uint offset = text.GetWord(command, 1);

				if (command.IsValid())
				{
					if (command == "name" || command == "SetName")
					{
						text.GetWord(arg, offset);
						if (arg.IsValid()) Game::Server->SetPlayerName(arg);
					}
					else if (command == "disc" || command == "disconnect")
					{
						Game::Server->Disconnect();
					}
					else if (command == "conn" || command == "connect")
					{
						text.GetWord(arg, offset);
						if (arg.IsValid()) Game::Server->Connect(arg);
					}
				}
			}
			else
			{
				Game::Server->SendMessage(text);
			}
		}
		input->SetText(defaultText);
	}
}