#pragma once

#include "./OPengine.h"

struct Slide {
	OPtexture* texture;
	OPuint aliveTime;
	OPuint maxTime;
	ui8 slideType;
	ui8 disabled;
};

struct SlideManager {
	OPtexture2D* tex2d;
	Slide* slides;
	ui8 count;
	ui8 current;

	void Init(Slide* slidesArray, ui8 slidesCount);
	void Update(OPtimer* timer);
	void Next();
	void Prev();
	void Render();
};