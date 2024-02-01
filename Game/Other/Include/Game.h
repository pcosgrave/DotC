#pragma once

//============================================================================================================
//  Defense of the Citadel, Copyright (c) 2010 Michael Lyashenko and Philip Cosgrave. All rights reserved.
//											www.nextrevision.com
//============================================================================================================
// Singleton-like access to all game components, accessible from anywhere within the project
//============================================================================================================

namespace Game
{
	// Various managers, accessible from anywhere within the project. Defined in Game.cpp.
	extern R5::IWindow*			Window;
	extern R5::IGraphics*		Graphics;
	extern R5::UI*				UI;
	extern R5::IAudio*			Audio;
	extern R5::Core*			Core;

	extern byte					Mode;			// Game mode (0 not initialized, 2 for 2D, 3 for 3D)
	extern R5::Scene*			Scene;			// Local scene
	extern R5::OSDraw*			DrawScript;		// Main draw script
	extern R5::Camera*			Camera;			// Camera used for visualization
	extern R5::Object*			Terrain;		// Object that holds the terrain
	extern R5::Object*			Controlled;		// Object that's currently being controlled
	extern R5::Object*			Selected;		// Selected game object (receives events)
	extern R5::Object*			Building;		// The building that is currently being placed in the world
	extern R5::Random			Random;			// Local random number generator
	extern R5::World*			World;			// Game world
	extern R5::Server*			Server;			// All in-game actions should go through the server
	extern R5::String			Name;			// Player name
	extern R5::UITextArea*		Chat;			// Chat area
	extern R5::UIInput*			Input;			// Chat input
	extern R5::UIFrameText*		SCT;			// Scrolling combat text
	extern float				Brightness;		// Current scene brightness outdoors
	extern float				Sensitivity;	// Mouse sensitivity
	extern Vector3f				Wind;			// The direction of the wind

	extern R5::Array<OSEntity*>	Entities;		// List of in-game entities
};
