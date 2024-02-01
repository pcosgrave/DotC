//============================================================================================================
//  Defense of the Citadel, Copyright (c) 2010 Michael Lyashenko and Philip Cosgrave. All rights reserved.
//											www.nextrevision.com
//============================================================================================================

#include "../../Other/Include/_All.h"
using namespace R5;

//============================================================================================================
// Create a new render target for the portrait's off-screen drawing
//============================================================================================================

void USPortrait::OnInit()
{
	UIPicture* pic = R5_CAST(UIPicture, mWidget);

	if (pic == 0)
	{
		DestroySelf();
		return;
	}

	// Create the off-screen resources
	mColor			= Game::Graphics->CreateRenderTexture();
	mDepth			= Game::Graphics->CreateRenderTexture();
	mRenderTarget	= Game::Graphics->CreateRenderTarget();

	// Set up the off-screen render target
	mRenderTarget->AttachColorTexture(0, mColor, ITexture::Format::RGBA);
	mRenderTarget->AttachDepthTexture(mDepth);

	Game::Core->Lock();
	{
		// Create the root object
		Object* coreRoot = Game::Core->GetRoot();
		mRoot = coreRoot->AddObject<Object>(String("%s: Root", GetClassID()));
		mRoot->AddScript<OSSceneRoot>();

		// Create the camera
		mCamera = mRoot->AddObject<Camera>(String("%s: Camera", GetClassID()));
		mCamera->SetAbsoluteRange(Vector3f(0.1f, 20.0f, 45.0f));

		// The camera should draw into our render target
		OSDrawDeferred* draw = mCamera->AddScript<OSDrawDeferred>();
		draw->SetRenderTarget(mRenderTarget);
		draw->SetFogRange(1.0f);
		draw->SetBackgroundColor(Color4f(0.0f, 0.0f, 0.0f, 0.0f));

		// Create an off-screen light source
		DirectionalLight* light = mCamera->AddObject<DirectionalLight>(String("%s: Light", GetClassID()));
		light->SetAmbient(Color3f(0.35f));
		light->SetDiffuse(Color3f(0.75f));
		light->SetSpecular(Color3f(1.0f));
		light->SetRelativeRotation(Vector3f(-1.0f, 1.0f, -1.0f));
	}
	Game::Core->Unlock();

	// The picture should be using the final off-screen texture
	pic->SetTexture(mColor);
	pic->SetEventHandling(0);
}

//============================================================================================================
// Clear the local resources
//============================================================================================================

void USPortrait::OnDestroy()
{
	if (mRoot != 0)
	{
		mRoot->DestroySelf();
		mRoot = 0;
	}

	if (mRenderTarget != 0)
	{
		Game::Graphics->DeleteRenderTarget(mRenderTarget);
		mRenderTarget = 0;
	}

	if (mDepth != 0)
	{
		Game::Graphics->DeleteTexture(mDepth);
		mDepth = 0;
	}

	if (mColor != 0)
	{
		Game::Graphics->DeleteTexture(mColor);
		mColor = 0;
	}
}

//============================================================================================================
// Draw the model
//============================================================================================================

void USPortrait::OnUpdate (bool areaChanged)
{
	mRenderTarget->SetSize(mSize);
}