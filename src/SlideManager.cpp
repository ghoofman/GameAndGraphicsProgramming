#include "SlideManager.h"

void SlideManager::Init(Slide* slidesArray, ui8 slidesCount)
{
	current = 0;
	count = slidesCount;
	slides = slidesArray;
	for (ui8 i = 0; i < slidesCount; i++) {
		slides[i].aliveTime = 0;
		slides[i].disabled = 1;
		slides[i].maxTime = 500;
		slides[i].slideType = 0;
		slides[i].texture = NULL;
	}
	tex2d = OPtexture2DCreate((OPtexture*)OPcmanLoadGet("Vectors_01.png"), NULL);
}

void SlideManager::Update(OPtimer * timer)
{
	slides[current].aliveTime += timer->Elapsed;
}

void SlideManager::Next()
{
	if (current < count - 1) {
		current++;
		slides[current].aliveTime = 0;
	}
}

void SlideManager::Prev()
{
	if (current > 0) {
		current--;
		slides[current].aliveTime = 0;
	}
}

void SlideManager::Render()
{
	if (slides[current].disabled) return;

	tex2d->Texture = slides[current].texture;
	OPtexture2DPrepRender(tex2d);

	OPmat4 world = OPMAT4_IDENTITY;
	OPmat4 size = OPMAT4_IDENTITY;
	size.Translate(tex2d->Position.x / (OPfloat)OPRENDER_WIDTH, tex2d->Position.y / (OPfloat)OPRENDER_HEIGHT, 0);
	size.Scl(tex2d->Texture->Description.Width * tex2d->Scale.x, tex2d->Texture->Description.Height * tex2d->Scale.y, 1.0);
	OPmat4 view = OPMAT4_IDENTITY;
	view.Scl(1.0f / (OPfloat)OPRENDER_WIDTH, 1.0f / (OPfloat)OPRENDER_HEIGHT, 1.0f);

	world = size * view;


	OPfloat perc = slides[current].aliveTime / slides[current].maxTime;
	if (perc > 1.0) perc = 1.0;

	if (slides[current].slideType == 1) {
		tex2d->Position.x = (-1280.0f * 2.0f) * (1.0 - perc);
		//world.cols[0][1] += 2.0 * (1.0 - perc);
	}

	OPeffectParamMat4v("uWorld", 1, &world);
	OPmeshRender();
	
}
