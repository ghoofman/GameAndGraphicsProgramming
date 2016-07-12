#pragma once

#include "./OPengine.h"

struct Grid {
	OPmesh mesh;
	OPeffect effect;

	void Init(ui32 width, ui32 depth);
	void Render(OPcam* camera);
};