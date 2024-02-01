//============================================================================================================
//  Defense of the Citadel, Copyright (c) 2010 Michael Lyashenko and Philip Cosgrave. All rights reserved.
//											www.nextrevision.com
//============================================================================================================
// Starting point for the game.
//============================================================================================================

#include "../../Other/Include/_All.h"
using namespace R5;

//============================================================================================================
// Globally accessible managers
//============================================================================================================

namespace R5
{
	namespace Game
	{
		R5::IWindow*		Window		= 0;
		R5::IGraphics*		Graphics	= 0;
		R5::UI*				UI			= 0;
		R5::IAudio*			Audio		= 0;
		R5::Core*			Core		= 0;

		byte				Mode		= 0;
		R5::Scene*			Scene		= 0;
		R5::OSDraw*			DrawScript	= 0;
		R5::Camera*			Camera		= 0;
		R5::Object*			Terrain		= 0;
		R5::Object*			Controlled	= 0;
		R5::Object*			Selected	= 0;
		R5::Object*			Building	= 0;
		R5::Random			Random;
		R5::World*			World		= 0;
		R5::Server*			Server		= 0;
		R5::String			Name;
		R5::UITextArea*		Chat		= 0;
		R5::UIInput*		Input		= 0;
		R5::UIFrameText*	SCT			= 0;
		float				Brightness	= 1.0f;
		float				Sensitivity	= 1.0f;
		Vector3f			Wind (-0.783f, 0.535f, -0.07f);

		R5::Array<OSEntity*> Entities;
	};
};

//============================================================================================================
// Script that sets the game root
//============================================================================================================

class OSDrawGame : public OSDrawDeferred
{
public:

	R5_DECLARE_INHERITED_CLASS("OSDrawGame", OSDrawGame, OSDrawDeferred, Script);

	virtual void OnInit()
	{
		Game::Scene = &mScene;
		Game::DrawScript = this;
		OSDrawDeferred::OnInit();
	}

	virtual void OnDestroy()
	{
		OSDrawDeferred::OnDestroy();
		Game::DrawScript = 0;
		Game::Scene = 0;
	}
};

//============================================================================================================
// Generic key handler -- keys that fall through everything else
//============================================================================================================

uint OnGenericKey (const Vector2i& pos, byte key, bool isDown)
{
	if (key == Key::Escape)
	{
		if (!isDown) Game::Core->Shutdown();
		return 1;
	}
	else if (key == Key::F5)
	{
		if (!isDown) Game::Window->SetStyle( (Game::Window->GetStyle() &
			IWindow::Style::FullScreen) == 0 ?
			IWindow::Style::FullScreen :
			IWindow::Style::Normal );
		return 1;
	}
	else if (key == Key::F6)
	{
		if (!isDown)
		{
			Game::Window->SetPosition( Vector2i(100, 100) );
			Game::Window->SetSize( Vector2i(900, 600) );
			Game::Window->SetStyle(IWindow::Style::Normal);
		}
		return 1;
	}
	return 0;
}

//============================================================================================================
// Callback function that gets called to set the wind value for the uniform.
//============================================================================================================

void SetWindUniform (const String& name, Uniform& data) { data = Game::Wind; }

//============================================================================================================
// Registers all shaders that use the wind uniform
//============================================================================================================

void RegisterWindUniforms()
{
	R5::IShader* shader;

	// TreeCanopy shader
	shader = Game::Graphics->GetShader("Deferred/TreeCanopy");
	shader->RegisterUniform("_wind", &SetWindUniform);

	// TreeCanopyDepth shader
	shader = Game::Graphics->GetShader("Deferred/TreeCanopyDepth");
	shader->RegisterUniform("_wind", &SetWindUniform);

	// Sunlight shader (clouds)
	shader = Game::Graphics->GetShader("Shaders/Other/Sunlight");
	shader->RegisterUniform("_wind", &SetWindUniform);
	shader = Game::Graphics->GetShader("Shaders/Other/SunlightShadow");
	shader->RegisterUniform("_wind", &SetWindUniform);
}

//============================================================================================================
// Initializes the managers, registers scripts, and serializes the configuration
//============================================================================================================

bool Init()
{
	Game::Window	= new R5::GLWindow();
	Game::Graphics	= new R5::GLGraphics();
	Game::World		= new R5::World();
	Game::Server	= new R5::Server();
	Game::UI		= new R5::UI(Game::Graphics, Game::Window);
	Game::Audio		= new R5::Audio();
	Game::Core		= new R5::Core(Game::Window, Game::Graphics, Game::UI, Game::Audio);
	Game::Name		= "Anonymous";

	// Create the environment cube map
	{
		Noise noise;
		noise.SetSeed(7654321);
		noise.SetSize(32, 32);
		noise.SetSeamless(false);
		noise.ApplyFilter("Simple");
		void* ptr = noise.GetBuffer();

		ITexture* tex = Game::Graphics->GetTexture("Environment Map");
		tex->Set(ptr, ptr, ptr, ptr, ptr, ptr, 32, 32, ITexture::Format::Float, ITexture::Format::Alpha);
		tex->SetWrapMode(ITexture::WrapMode::ClampToEdge);
	}

	// Create the noise map
	{
		Noise noise;
		noise.SetSeed(74625646);
		noise.SetSize(32, 32);
		noise.SetSeamless(true);
		noise.ApplyFilter("Perlin").Set(3.0f);

		ITexture* tex = Game::Graphics->GetTexture("Noise map");
		tex->Set(noise.GetBuffer(), 32, 32, 1, ITexture::Format::Float, ITexture::Format::Alpha);
		tex->SetWrapMode(ITexture::WrapMode::Repeat);
	}

	// Generic key handler -- keys that fall through everything else
	Game::Core->AddOnKey( &OnGenericKey, 0 );

	// Registers all shaders that use the wind property.
	RegisterWindUniforms();

	// Register the initialization scripts
	Script::Register<OS3DGame>();
	Script::Register<OSDrawGame>();

	// Register custom widgets
	UIWidget::Register<UIFrameText>();
	UIWidget::Register<UIHealthBar>();

	// Common UI components
	UIScript::Register<USRoot>();
	UIScript::Register<USChangeLayout>();
	UIScript::Register<USDisconnect>();
	UIScript::Register<USStatusBar>();
	UIScript::Register<USServerDetails>();
	UIScript::Register<USPreviewMap>();
	UIScript::Register<USServerInfo>();
	UIScript::Register<USServerDescription>();
	UIScript::Register<USJoinGame>();

	UIScript::Register<USFrameAware>();
	UIScript::Register<USChatLog>();
	UIScript::Register<USToggleWidget>();
	UIScript::Register<USHideFrame>();
	UIScript::Register<USConnect>();
	UIScript::Register<USChatInput>();
	UIScript::Register<USHealthColor>();
	UIScript::Register<USFadeOut>();
	UIScript::Register<USFadeIn>();
	UIScript::Register<USGrowOnHover>();
	UIScript::Register<USPortrait>();
	UIScript::Register<USControlledPortrait>();
	UIScript::Register<USSelectedPortrait>();
	UIScript::Register<USLoadingScreen>();
	UIScript::Register<USStartBuilding>();

	// Add the scrolling combat text widget
	Game::SCT = Game::UI->AddWidget<UIFrameText>("SCT");
	Game::SCT->SetEventHandling(UIWidget::EventHandling::None);
	Game::SCT->SetSerializable(false);

	// Create the game window
	if (!Game::Window->Create("Defense of the Citadel", 100, 100, 900, 600)) return false;

	// Use anisotropic filtering by default
	Game::Graphics->SetDefaultAF(8);
	Game::Graphics->SetBackgroundColor(Color4ub(255, 255, 255, 255));

	// Load the startup configuration
	return (*Game::Core << "Config/Startup.txt");
}

//============================================================================================================
// Cleans up the managers
//============================================================================================================

void Shutdown()
{
	Debug::SetWidget(0);

	Game::Chat		 = 0;
	Game::Input		 = 0;
	Game::Controlled = 0;

	if (Game::UI != 0) Game::UI->GetRoot().DestroySelf();

	if (Game::Core		!= 0)	delete Game::Core;
	if (Game::Audio		!= 0)	delete Game::Audio;
	if (Game::UI		!= 0)	delete Game::UI;
	if (Game::Server	!= 0)	delete Game::Server;
	if (Game::World		!= 0)	delete Game::World;
	if (Game::Graphics	!= 0)	delete Game::Graphics;
	if (Game::Window	!= 0)	delete Game::Window;
}

//============================================================================================================
// Locate resources, enter the main loop
//============================================================================================================

void Run()
{
	while (Game::Core->Update());

	//*Game::Core >> "out.r5a";
	//*Game::Core >> "out.r5b";
	//*Game::Core >> "out.r5c";
}

//============================================================================================================
// Application entry point
//============================================================================================================

R5_MAIN_FUNCTION
{
#ifdef _MACOS
	String path ( System::GetPathFromFilename(argv[0]) );
	System::SetCurrentPath(path.GetBuffer());
	System::SetCurrentPath("../../../");
#endif
	if (!System::SetCurrentPath("../../Resources/"))
		 System::SetCurrentPath("../../../Resources/");

	if (Init()) Run();
	Shutdown();
	return 0;
}