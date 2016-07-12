#include "./GameState.h"
#include "./Main.h"

#ifdef ADDON_imgui
#include "OPimgui.h"
#endif

void ExampleStateInit(OPgameState* last) {
}


OPint ExampleStateUpdate(OPtimer* time) {
	SecondaryWindow->Bind();
	OPrenderUpdate();

	MainWindow->Bind();
	OPrenderUpdate();
	return false;
}


void ExampleStateRender(OPfloat delta) {
	MainWindow->Bind();
	OPrenderClear(0, 0, 0, 1);
	OPrenderPresent();

	SecondaryWindow->Bind();
#ifdef ADDON_imgui
	OPlog("Frame Started");
	OPimguiNewFrame();
	{
		static float f = 0.0f;
		ImGui::Text("Hello, world!");
		ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}

#endif
	OPrenderClear(0.2, 0.2, 0.2);
#ifdef ADDON_imgui
	ImGui::Render();
#endif
	OPrenderPresent();
}

OPint ExampleStateExit(OPgameState* next) {
	return 0;
}


OPgameState GS_EXAMPLE = {
	ExampleStateInit,
	ExampleStateUpdate,
	ExampleStateRender,
	ExampleStateExit
};
