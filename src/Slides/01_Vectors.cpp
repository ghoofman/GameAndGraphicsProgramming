#include "./01_Vectors.h"
#include "../Main.h"

#ifdef ADDON_imgui
#include "OPimgui.h"
#endif

Slide slides[VECTORS_STATES];

void VectorsSlide::Init() {

	OPcmanLoad("Ubuntu.opf");

	OPfontSystemLoadEffects();
	FontManager = OPfontManagerSetup("Ubuntu.opf", NULL, 0);
	FontManager->_align = OPfontAlign::OPFONT_ALIGN_CENTER;
	FontManager->scale = 0.5;

	//mesh = (OPmesh*)OPcmanLoadGet("sphere.opm");

	plane.Init("plane.opm");
	model.Init("sphere.opm");
	arrow.Init("arrow.opm");
	effect = OPeffectGen(
		"Shadeless.vert",
		"Shadeless.frag",
		OPATTR_POSITION,
		"Model Effect",
		model.mesh->vertexLayout.stride);

	material.Init(&effect);

	camera.SetPerspective(OPvec3(0, 0, 10), OPVEC3_ZERO);
	//camera.SetOrtho(OPvec3(10, 10, 10), OPVEC3_ONE, 1000.0f);

	lightDirection = OPvec3(1, 1, 1);

	grid.Init(10, 10);

	OPrenderDepth(1);
	OPrenderCull(0);

	position2 = OPvec3(1, 1, 0);
	state = 0;

	for (ui32 i = 0; i < VECTORS_STATES; i++) {
		stateTime[i] = 0;
	}
	OPcmanLoad("Vectors_01.png");
	OPcmanLoad("Vectors_02.png");
	OPcmanLoad("Vectors_03.png");
	OPcmanLoad("Vectors_04.png");
	OPcmanLoad("Vectors_05.png");
	Texture = (OPtexture*)OPcmanGet("Vectors_01.png");
	Tex = OPtexture2DCreate(Texture, NULL);

	slideManager.Init(slides, VECTORS_STATES);
	slides[6].texture = (OPtexture*)OPcmanGet("Vectors_01.png"); slides[6].disabled = 0; slides[6].slideType = 1;
	slides[7].texture = (OPtexture*)OPcmanGet("Vectors_02.png"); slides[7].disabled = 0;
	slides[8].texture = (OPtexture*)OPcmanGet("Vectors_03.png"); slides[8].disabled = 0;
	slides[9].texture = (OPtexture*)OPcmanGet("Vectors_04.png"); slides[9].disabled = 0;
	slides[10].texture = (OPtexture*)OPcmanGet("Vectors_05.png"); slides[10].disabled = 0;
}

OPint VectorsSlide::Update(OPtimer* timer) {
	OPuint prevTime = stateTime[state];
	stateTime[state] += timer->Elapsed;

	if (state == 4) {
		OPfloat perc = stateTime[4] / 500.0f;
		if (perc > 1) perc = 1.0;
		
		camera.pos.y = 5.0 * perc;
		//camera.pos.z = 10 + 5.0 * perc;

		if (perc >= 1) {
			OPfloat lastPerc = (prevTime - 500) / 3000.f;
			perc = (stateTime[4] - 500) / 3000.f;

			if (perc >= 1) {
				if (lastPerc < 1.0) {
					camera.pos = OPvec3(0, 5, 10);
				}
			}
			else {
				OPvec3 pos = OPvec3(0, 5, 10);
				pos = OPmat4Transform(pos, OPmat4RotY(perc * OPpi2));
				camera.pos = pos;
			}
		}
	}
	
	slideManager.Update(timer);

	camera.Update();

	return false;
}

void VectorsSlide::Render(OPfloat delta) {
	RenderMain();
	RenderSecondary();
}

void VectorsSlide::RenderMain() {
	OPchar buffer[1024];

	MainWindow->Bind();
	OPrenderClear(0, 0, 0, 1);

	OPfontRenderBegin(FontManager);
	FontManager->scale = 0.5;
	sprintf(buffer, "State %d", state);
	OPfontRender(buffer, OPvec2(150, 10));
	FontManager->scale = 0.5;
	OPfontRenderEnd();

	if (state == 0) {
		OPfontRenderBegin(FontManager);
		FontManager->scale = 1.0;
		OPfontRender("Vectors", OPvec2(OPRENDER_WIDTH / 2, OPRENDER_HEIGHT / 2 - 100));
		OPfontRenderEnd();

		FontManager->scale = 0.5;
	}
	// Render a vector arrow to the screen
	if (state >= 1) {
		OPfloat perc = stateTime[1] / 200.0f;
		if (perc > 1) perc = 1.0;
		model.world.SetIdentity()->Scl(scaled * perc)->Translate(position)->RotX(rotate.x)->RotY(rotate.y)->RotZ(rotate.z);
		model.Draw(&material, &camera);

		if (perc >= 1 && state < 4) {
			OPfontRenderBegin(FontManager);
			if (state >= 4) {
				sprintf(buffer, "%d, %d, %d", (OPint)position.x, (OPint)position.y, (OPint)position.z);
			} else{
				sprintf(buffer, "%0.2f, %0.2f", position.x, position.y);
			}
			OPfontRender(buffer, OPvec2(OPRENDER_WIDTH / 2, OPRENDER_HEIGHT / 2 + 25));
			OPfontRenderEnd();
		}
	}
	if (state >= 2) {
		OPfloat perc = stateTime[2] / 200.0f;
		if (perc > 1) perc = 1.0;
		model.world.SetIdentity()->Scl(scaled * perc)->Translate(position2)->RotX(rotate.x)->RotY(rotate.y)->RotZ(rotate.z);
		model.Draw(&material, &camera);

		if (perc >= 1 && state < 4) {
			OPfontRenderBegin(FontManager);
			if (state >= 4) {
				sprintf(buffer, "%d, %d, %d", (OPint)position2.x, (OPint)position2.y, (OPint)position2.z);
			}
			else {
				sprintf(buffer, "%0.2f, %0.2f", position2.x, position2.y);
			}
			OPfontRender(buffer, OPvec2(OPRENDER_WIDTH / 2 + 80, OPRENDER_HEIGHT / 2 - 120));
			OPfontRenderEnd();
		}
	}
	if (state >= 3 && state < 6) {
		OPfloat perc = stateTime[3] / 600.0f;
		if (perc > 1) perc = 1.0;

		OPfloat tweened = OPtween_cubicEaseIn(perc);

		//plane.world.SetIdentity()->Translate(.5, 0.5,0)->Scl(0.1)->RotZ(OPpi_4)->Scl(15, 1, 1);
		//plane.Draw(&material, &camera);
		OPfloat angle = OPvec2Angle(OPvec2(0, 1), OPvec2(position2.x, position2.y));
		OPfloat len = OPvec3Len(position2);
		if (position2.x < 0) {
			angle *= -1;
		}
		arrow.world.SetIdentity()->RotZ(-angle)->Scl(len * tweened);
		arrow.Draw(&material, &camera);

		if (perc >= 1 && state < 4) {
			OPfontRenderBegin(FontManager);
			sprintf(buffer, "[ %0.2f, %0.2f ]", position2.x, position2.y);
			OPfontRender(buffer, OPvec2(OPRENDER_WIDTH / 2 - 25, OPRENDER_HEIGHT / 2 - 75));
			OPfontRenderEnd();
		}
	}
	if (state >= 4) {
		grid.Render(&camera);
		OPfloat perc = ((OPint)stateTime[4] - 500) / 3000.f;
		if (perc >= 1 && state == 4) {
			state = 5;
			slideManager.Next();
		}
	}
	if (state == 5) {
		OPfontRenderBegin(FontManager);
		sprintf(buffer, "[ %0.2f, %0.2f, %0.2f]", position2.x, position2.y, position2.z);
		OPfontRender(buffer, OPvec2(OPRENDER_WIDTH / 2, OPRENDER_HEIGHT / 2 - 150));
		OPfontRenderEnd();
	}
	if (state >= 6 && state < 11) {
		OPfloat perc = stateTime[6] / 500.0f;
		if (perc > 1.0) perc = 1;
		Tex->Position.x = (-1280.0f * 2.0f) * (1.0 - perc);

		Tex->Texture = Texture;
		OPtexture2DPrepRender(Tex);

		OPmat4 world = OPMAT4_IDENTITY;
		OPmat4 size = OPMAT4_IDENTITY;
		size.Translate(Tex->Position.x / (OPfloat)OPRENDER_WIDTH, Tex->Position.y / (OPfloat)OPRENDER_HEIGHT, 0);
		size.Scl(Tex->Texture->Description.Width * Tex->Scale.x, Tex->Texture->Description.Height * Tex->Scale.y, 1.0);
		OPmat4 view = OPMAT4_IDENTITY;
		view.Scl(1.0f / (OPfloat)OPRENDER_WIDTH, 1.0f / (OPfloat)OPRENDER_HEIGHT, 1.0f);

		world = size * view;

		world.cols[0][1] += 2.0 * (1.0 - perc);

		OPeffectParamMat4v("uWorld", 1, &world);
		//OPtextureClearActive();
		//OPeffectParami("uColorTexture", OPtextureBind(Texture));
		OPmeshRender();

		//OPtexture2DRender(Tex);
	}
	if (state == 11) {
		OPfloat perc = stateTime[11] / 500.0f;
		if (perc > 1.0) perc = 1;
		camera.pos.y = 5.0 - (5.0 * perc);

		OPfloat angle = OPvec2Angle(OPvec2(0, 1), OPvec2(position2.x, position2.y));
		OPfloat len = OPvec3Len(position2);
		OPfloat scl = 1.0 + (len - 1.0);
		arrow.world.SetIdentity()->RotZ(-angle)->Scl(scl);
		arrow.Draw(&material, &camera);

		OPfontRenderBegin(FontManager);
		OPfontRender("1.414", OPvec2(OPRENDER_WIDTH / 2, OPRENDER_HEIGHT / 2 - 150));
		OPfontRenderEnd();
	}
	if (state >= 12) {
		OPfloat perc = stateTime[12] / 600.0f;
		if (perc > 1) perc = 1.0;

		OPfloat angle = OPvec2Angle(OPvec2(0, 1), OPvec2(position2.x, position2.y));
		OPfloat len = OPvec3Len(position2);

		OPfloat invPer = (1.0 - perc);
		OPfloat scl = 1.0 + invPer * (len - 1.0);

		OPvec3 vecNorm = OPvec3Norm(position2);
		OPvec3 inbetween = OPvec3(
			vecNorm.x + (position2.x - vecNorm.x) * invPer,
			vecNorm.y + (position2.y - vecNorm.y) * invPer,
			vecNorm.z + (position2.z - vecNorm.z) * invPer
		);


		if (position2.x < 0) {
			angle *= -1;
		}
		arrow.world.SetIdentity()->RotZ(-angle)->Scl(scl);
		arrow.Draw(&material, &camera);

		OPfontRenderBegin(FontManager);
		sprintf(buffer, "[ %0.2f, %0.2f, %0.2f ]", inbetween.x, inbetween.y, inbetween.z);
		OPfontRender(buffer, OPvec2(OPRENDER_WIDTH / 2, OPRENDER_HEIGHT / 2 - 150));
		OPfontRender("Normalize", OPvec2(OPRENDER_WIDTH / 2, OPRENDER_HEIGHT / 2 - 200));
		OPfontRenderEnd();
	}
	
	slideManager.Render();

	OPfontRenderBegin(FontManager);
	FontManager->scale = 0.5;
	sprintf(buffer, "Current %d", slideManager.current);
	OPfontRender(buffer, OPvec2(150, 60));
	FontManager->scale = 0.5;
	OPfontRenderEnd();

	OPrenderPresent();
}

void VectorsSlide::Reset() {

}

void VectorsSlide::RenderSecondary() {
#if RUN_SECONDARY_WINDOW
	SecondaryWindow->Bind();
#ifdef ADDON_imgui
	//OPlog("Frame Started");
	OPimguiNewFrame();
	{
		OPfloat angle = OPvec2Angle(OPvec2(0, 1), OPvec2(position2.x, position2.y));
		ImGui::Text("01 Vectors : %f", angle);
		if (ImGui::DragInt("State", (int*)&state, 0.25f, 0, VECTORS_STATES - 1)) {
			stateTime[state] = 0;
			Reset();
		}
		//ImGui::DragFloat3
		ImGui::DragFloat3("Cam", (float*)&camera.pos, 0.25f, -25.0f, 25.0f);
		//ImGui::DragFloat3("Position", (float*)&position, 0.25f, -5.0f, 5.0f);
		ImGui::DragFloat3("Position2", (float*)&position2, 0.25f, -5.0f, 5.0f);
		ImGui::DragFloat("Scale", &scaled, 0.1f, 0.01, 5.0f);
		ImGui::SliderFloat3("Rotate", (float*)&rotate, -OPpi, OPpi);
		if (ImGui::Button("Prev")) {
			if (state > 0) {
				stateTime[state] = 0;
				state--;
				stateTime[state] = 0;
				Reset();
			}
		}
		if (ImGui::Button("Re-Animate")) {
			stateTime[state] = 0;
		}
		if (ImGui::Button("Next")) {
			if (state < VECTORS_STATES - 1) {
				state++;
				Reset();
			}
		}
	}

#endif
	OPrenderClear(0.2, 0.2, 0.2);
#ifdef ADDON_imgui
	ImGui::Render();
#endif
	OPrenderPresent();
#endif
	if (OPkeyboardWasReleased(OPKEY_UP)) {
		stateTime[state] = 0;
		Reset();
	}
	if (OPkeyboardWasReleased(OPKEY_LEFT)) {
		if (state > 0) {
			stateTime[state] = 0;
			state--;
			stateTime[state] = 0;
			slideManager.Prev();
			Reset();
		}
	}
	if (OPkeyboardWasReleased(OPKEY_RIGHT)) {
		if (state < VECTORS_STATES - 1) {
			state++;
			slideManager.Next();
			Reset();
		}
	}
}


VectorsSlide vectorsSlide;
void VectorsInit(OPgameState* last) { vectorsSlide.Init(); }
OPint VectorsUpdate(OPtimer* time) { return vectorsSlide.Update(time); }
void VectorsRender(OPfloat delta) { vectorsSlide.Render(delta); }

OPgameState GS_01_VECTORS = {
	VectorsInit,
	VectorsUpdate,
	VectorsRender,
	NULL
};
