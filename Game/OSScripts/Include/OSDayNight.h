#pragma once

//============================================================================================================
//  Defense of the Citadel, Copyright (c) 2010 Michael Lyashenko and Philip Cosgrave. All rights reserved.
//											www.nextrevision.com
//============================================================================================================
// Day/night transition script
//============================================================================================================

class OSDayNight : public Script
{
	Color3f				mDiffuse;
	SplineV				mAmbientV;
	SplineV				mDiffuseV;
	DirectionalLight*	mLight;

public:

	R5_DECLARE_INHERITED_CLASS("OSDayNight", OSDayNight, Script, Script);

	OSDayNight() : mLight(0) {}

	void SetDiffuse	(float r, float g, float b);
	void SetDiffuse (const Color3f& color)		{ SetDiffuse(color.r, color.g, color.b); }

	const Color3f& GetDiffuse() const { return mDiffuse; }

	virtual void OnInit();
	virtual void OnPreUpdate();

	virtual void OnSerializeTo	(TreeNode& node) const;
	virtual void OnSerializeFrom(const TreeNode& node);
};