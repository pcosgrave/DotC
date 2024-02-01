#pragma once

//============================================================================================================
//  Defense of the Citadel, Copyright (c) 2010 Michael Lyashenko and Philip Cosgrave. All rights reserved.
//											www.nextrevision.com
//============================================================================================================
// Sunlight is a more advanced version of a directional light source
//============================================================================================================

class Sunlight : public DirectionalLight
{
protected:

	ITexture* mTexture;
	IShader* mShader0;
	IShader* mShader1;

	Sunlight() : mTexture(0), mShader0(0), mShader1(0) {}

public:

	R5_DECLARE_INHERITED_CLASS("Sunlight", Sunlight, DirectionalLight, Object);

	// Draw the light using deferred rendering
	virtual void OnDrawLight (TemporaryStorage& storage, bool setStates);
};