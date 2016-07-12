#include "./01_Vectors.h"
#include "../Main.h"
#include "../Grid.h"

#ifdef ADDON_imgui
#include "OPimgui.h"
#endif

OPeffect effect;
OPmesh* mesh;
OPcam camera;
OPvec3 lightDirection;
OPvec3 position;
OPvec3 rotate;
OPfloat scaled = 1.0;
OPfontManager* FontManager;
Grid grid;

void VectorsInit(OPgameState* last) {
	OPcmanLoad("Ubuntu.opf");

	OPfontSystemLoadEffects();
	FontManager = OPfontManagerSetup("Ubuntu.opf", NULL, 0);

	mesh = (OPmesh*)OPcmanLoadGet("arrow.opm");
	effect = OPeffectGen(
		"ColoredModel.vert",
		"ColoredModel.frag",
		OPATTR_POSITION | OPATTR_COLOR,
		"Model Effect",
		mesh->vertexLayout.stride);

	camera.SetPerspective(OPvec3(0, 5, 10), OPVEC3_ZERO);
	//camera.SetOrtho(OPvec3(10, 10, 10), OPVEC3_ONE, 1000.0f);

	lightDirection = OPvec3(1, 1, 1);

	grid.Init(10, 10);
	
	OPrenderDepth(1);
	OPrenderCull(0);
}


OPint VectorsUpdate(OPtimer* time) {
	camera.Update();
	
	return false;
}

void VectorsRenderMain() {
	OPchar buffer[1024];

	MainWindow->Bind();
	OPrenderClear(0, 0, 0, 1);


	OPfontRenderBegin(FontManager);
	sprintf(buffer, "Position: [%f, %f, %f]", position.x, position.y, position.z);
	OPfontRender(buffer, OPvec2(50, OPRENDER_HEIGHT - 300));
	sprintf(buffer, "Scale: [%f, %f, %f]", scaled, scaled, scaled);
	OPfontRender(buffer, OPvec2(50, OPRENDER_HEIGHT - 200));
	sprintf(buffer, "Rotate: [%f, %f, %f]", rotate.x, rotate.y, rotate.z);
	OPfontRender(buffer, OPvec2(50, OPRENDER_HEIGHT - 100));
	OPfontRenderEnd();

	// Render a vector arrow to the screen
	OPmat4 world = OPMAT4_IDENTITY;
	world.Scl(scaled)->Translate(position)->RotX(rotate.x)->RotY(rotate.y)->RotZ(rotate.z);

	OPbindMeshEffectWorldCam(mesh, &effect, &world, &camera);

	// Sets the vLightDirection uniform on the Effect that is currently bound (modelExample->Effect)
	OPeffectParamVec3("vLightDirection", &lightDirection);

	// Renders to the screen the currently bound Mesh (modelExample->Mesh)
	OPmeshRender();

	grid.Render(&camera);

	OPrenderPresent();
}

void VectorsRenderSecondary() {
	SecondaryWindow->Bind();
#ifdef ADDON_imgui
	OPlog("Frame Started");
	OPimguiNewFrame();
	{
		ImGui::Text("01 Vectors");
		ImGui::SliderFloat3("Position", (float*)&position, -5.0f, 5.0f);
		ImGui::SliderFloat("Scale", &scaled, 0.01, 5.0f);
		ImGui::SliderFloat3("Rotate", (float*)&rotate, -OPpi, OPpi);
		ImGui::Button("Next Slide");
	}

#endif
	OPrenderClear(0.2, 0.2, 0.2);
#ifdef ADDON_imgui
	ImGui::Render();
#endif
	OPrenderPresent();
}

void VectorsRender(OPfloat delta) {
	VectorsRenderMain();
	VectorsRenderSecondary();
}


OPint VectorsExit(OPgameState* next) {
	return 0;
}


OPgameState GS_01_VECTORS = {
	VectorsInit,
	VectorsUpdate,
	VectorsRender,
	VectorsExit
};
