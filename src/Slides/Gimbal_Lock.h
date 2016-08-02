#pragma once

#include "./OPengine.h"

struct GimbalLock {
	OPscene scene;

	OPmodel arrow;
	OPmodel ringX;
	OPmodel ringY;
	OPmodel ringZ;
	OPmodel ringGrey;

	OPeffect effect;
	OPeffect effectTextured;

	OPtexture* texture;

	OPmaterial materialRed;
	OPmaterial materialGreen;
	OPmaterial materialBlue;
	OPmaterial materialColored;
	OPmaterial materialGrey;

	OPvec4 colorRed;
	OPvec4 colorGreen;
	OPvec4 colorBlue;
	OPvec4 colorGrey;

	OPfloat rotationGrey;
	OPvec3 rotation;
	OPvec3 rotationTarget;
	OPuint rotationTime;
	bool interpolateRotation;

	OPfontManager* FontManager;

	void Init();
	OPint Update(OPtimer* timer);
	void Render(OPfloat delta);
	void RenderMain(OPfloat delta);
	void RenderSecondary();
};

extern OPgameState GIMBAL_LOCK;