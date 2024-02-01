//============================================================================================================
//  Defense of the Citadel, Copyright (c) 2010 Michael Lyashenko and Philip Cosgrave. All rights reserved.
//											www.nextrevision.com
//============================================================================================================

#include "../../Other/Include/_All.h"
using namespace R5;

//============================================================================================================
// Converts a photoshop RGB value into a floating point vec3
//============================================================================================================

Vector3f GetColor (uint rgb)
{
	uint r = (rgb >> 16) & 0xFF;
	uint g = (rgb >> 8) & 0xFF;
	uint b = rgb & 0xFF;

	return Vector3f( Float::FromRangeByte((byte)r),
					 Float::FromRangeByte((byte)g),
					 Float::FromRangeByte((byte)b));
}

//============================================================================================================
// Sets the diffuse color gradient
//============================================================================================================

void OSDayNight::SetDiffuse (float r, float g, float b)
{
	Vector3f diffuse (r, g, b);
	mDiffuse.Set(r, g, b);

	// Diffuse color gradient
	mDiffuseV.Clear();
	mDiffuseV.AddKey(-0.0625f, 0.0f);
	mDiffuseV.AddKey( 0.0000f, 0.0f);
	mDiffuseV.AddKey( 0.0625f, 0.0f);
	mDiffuseV.AddKey( 0.1250f, 0.0f);
	mDiffuseV.AddKey( 0.1875f, 0.0f);
	mDiffuseV.AddKey( 0.2500f, diffuse * Vector3f(1.0f, 0.35f, 0.45f));
	mDiffuseV.AddKey( 0.3125f, diffuse * Vector3f(1.0f, 0.75f, 0.75f));
	mDiffuseV.AddKey( 0.3750f, diffuse * Vector3f(1.0f, 0.85f, 1.0f));
	mDiffuseV.AddKey( 0.4375f, diffuse);
	mDiffuseV.AddKey( 0.5000f, diffuse);
	mDiffuseV.AddKey( 0.5625f, diffuse);
	mDiffuseV.AddKey( 0.6250f, diffuse * Vector3f(1.0f, 1.0f, 0.8f));
	mDiffuseV.AddKey( 0.6875f, diffuse * Vector3f(1.0f, 0.8f, 0.35f));
	mDiffuseV.AddKey( 0.7500f, diffuse * Vector3f(1.0f, 0.5f, 0.05f));
	mDiffuseV.AddKey( 0.8125f, 0.0f);
	mDiffuseV.AddKey( 0.8750f, 0.0f);
	mDiffuseV.AddKey( 0.9375f, 0.0f);
	mDiffuseV.AddKey( 1.0000f, 0.0f);
	mDiffuseV.AddKey( 1.0625f, 0.0f);
}

//============================================================================================================
// This script should be attached to a directional light source
//============================================================================================================

void OSDayNight::OnInit()
{
	mLight = R5_CAST(DirectionalLight, mObject);
	if (mLight == 0) return;

	// Ambient color gradient
	mAmbientV.AddKey(-0.0625f, GetColor(0x4a5066) * 0.25f);
	mAmbientV.AddKey( 0.0000f, GetColor(0x4a5066) * 0.25f);	// Midnight
	mAmbientV.AddKey( 0.0625f, GetColor(0x494f65) * 0.25f);
	mAmbientV.AddKey( 0.1250f, GetColor(0x4a5163) * 0.25f);
	mAmbientV.AddKey( 0.1875f, GetColor(0x545871) * 0.30f);
	mAmbientV.AddKey( 0.2500f, GetColor(0x816b99) * 0.35f);	// 6 AM
	mAmbientV.AddKey( 0.3125f, GetColor(0xb5c2d3) * 0.37f);
	mAmbientV.AddKey( 0.3750f, GetColor(0xdefafe) * 0.40f);
	mAmbientV.AddKey( 0.4375f, GetColor(0xa9e6ff) * 0.42f);
	mAmbientV.AddKey( 0.5000f, GetColor(0xa9e6ff) * 0.45f);	// Noon
	mAmbientV.AddKey( 0.5625f, GetColor(0xa9e6ff) * 0.42f);
	mAmbientV.AddKey( 0.6250f, GetColor(0xe0fcff) * 0.40f);
	mAmbientV.AddKey( 0.6875f, GetColor(0xb5c2d3) * 0.37f);
	mAmbientV.AddKey( 0.7500f, GetColor(0x816b99) * 0.35f);	// 6 PM
	mAmbientV.AddKey( 0.8125f, GetColor(0x545871) * 0.30f);
	mAmbientV.AddKey( 0.8750f, GetColor(0x495168) * 0.25f);
	mAmbientV.AddKey( 0.9375f, GetColor(0x4a5066) * 0.25f);
	mAmbientV.AddKey( 1.0000f, GetColor(0x4a5066) * 0.25f);	// Midnight
	mAmbientV.AddKey( 1.0625f, GetColor(0x494f65) * 0.25f);

	// Use the light's diffuse color as the starting diffuse color
	const Color4f& dc = mLight->GetDiffuse();
	SetDiffuse(dc.r, dc.g, dc.b);
}

//============================================================================================================
// Change the light color, adjusting it to the time
//============================================================================================================

void OSDayNight::OnPreUpdate()
{
	if (mLight != 0)
	{
		// TODO: Use network time, not local time
		ulong ms = 25478;
		//ulong ms = 1234;
		// TODO: use a longer cycle
		ulong cycle = 18000;

		// Cycling 0-1 time range
		float time = (float)(ms % cycle) / cycle;
		float timePi2 = TWOPI * time;
		float x = Float::Sin(timePi2);
		float y = Float::Cos(timePi2);

		Vector3f a (mAmbientV.Sample(time));
		Vector3f d (mDiffuseV.Sample(time, 1));

		// 360 degree rotation around the vertical axis
		Quaternion spin;
		spin.SetFromEuler(Vector3f(0.0f, 0.0f, -timePi2 + PI));
		Vector3f dir (spin.GetForward());

		// At night the moon should be shining ambient-like light onto the world
		if (y > 0.4f)
		{
			dir = -dir;

			float moonlight = ((y - 0.4f) / 0.6f);
			moonlight = 1.0f - moonlight;
			moonlight *= moonlight;
			moonlight = 1.0f - moonlight;

			d = Interpolation::Linear(d, a * 1.5f, moonlight);

			x = -x;
			y = -y;
		}

		// Rotate the light around the world
		// NOTE: The sun's rotation is always at an angle and is never horizontal.
		// This is because of shadows. Shadows look terrible when the light is horizontal.
		mLight->SetRelativeRotation(Vector3f(0.0f, 0.0f, -1.5f + y * 1.5f) + dir);

		// Adjust the light colors
		mLight->SetAmbient(Color3f(a.x, a.y, a.z));
		mLight->SetDiffuse(Color3f(d.x, d.y, d.z));
		mLight->SetSpecular(mLight->GetDiffuse());

		// Outdoor brightness should average the sunlight
		Game::Brightness = Float::Clamp((d.x + d.y + d.z) / 3.0f, 0.0f, 1.0f);

		// Adjust the background color (this is also the fog color)
		if (Game::DrawScript != 0)
		{
			Vector3f fogColor (Interpolation::Linear(a, d, Game::Brightness));
			Game::DrawScript->SetBackgroundColor(Color4f(fogColor.x, fogColor.y, fogColor.z, 1.0f));
		}
	}
}

//============================================================================================================
// Serialization -- Save
//============================================================================================================

void OSDayNight::OnSerializeTo (TreeNode& node) const
{
	node.AddChild("Diffuse", mDiffuse);
}

//============================================================================================================
// Serialization -- Load
//============================================================================================================

void OSDayNight::OnSerializeFrom (const TreeNode& node)
{
	if (node.mTag == "Diffuse")
	{
		Vector3f v;
		if (node.mValue >> v) SetDiffuse(v.x, v.y, v.z);
	}
}