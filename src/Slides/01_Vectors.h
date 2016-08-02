#pragma once

#include "./OPengine.h"
#include "../Grid.h"
#include "../SlideManager.h"

extern OPgameState GS_01_VECTORS;

#define VECTORS_STATES 13

struct VectorsSlide {
	SlideManager slideManager;

	OPeffect effect;
	OPmodel model;
	OPmodel plane;
	OPmodel arrow;
	OPmaterial material;
	//OPmesh* mesh;
	OPcam camera;
	OPvec3 lightDirection;
	OPvec3 position;
	OPvec3 position2;
	OPvec3 rotate;
	OPfloat scaled = 0.15;
	OPfontManager* FontManager;
	Grid grid;
	ui32 state;

	OPtexture2D* Tex;
	OPtexture* Texture;

	OPuint stateTime[VECTORS_STATES];


	// Public
	void Init();
	OPint Update(OPtimer* timer);
	void Render(OPfloat delta);

	// Private
	void Reset();
	void RenderMain();
	void RenderSecondary();
};
