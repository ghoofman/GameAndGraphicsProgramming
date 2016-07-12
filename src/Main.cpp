//////////////////////////////////////
// Application Entry Point
//////////////////////////////////////
#include "Main.h"
#include "GameState.h"
#include "Slides/01_Vectors.h"

#ifdef ADDON_imgui
#include "OPimgui.h"
#endif

//////////////////////////////////////
// Application Methods
//////////////////////////////////////
OPwindow* MainWindow = NULL;
OPwindow* SecondaryWindow = NULL;

void ApplicationInit() {
    OP_LOG_LEVEL = 2000;
    
	OPloadersAddDefault();
	OPcmanInit(OPIFEX_ASSETS);
	OPrenderInit();

	i32 contain = 50;
	MainWindow = OPrenderCreateWindow(NULL, false, true, "GamedevGR", OPMONITOR_LIST[0].VideoModeCurrent.Width - contain - contain, OPMONITOR_LIST[0].VideoModeCurrent.Height - contain - contain);
	MainWindow->SetPosition(OPMONITOR_LIST[0].X + contain, OPMONITOR_LIST[0].Y + contain);

	SecondaryWindow = OPrenderCreateWindow(NULL, false, true, "GamedevGR", OPMONITOR_LIST[1].VideoModeCurrent.Width - contain - contain, OPMONITOR_LIST[1].VideoModeCurrent.Height - contain - contain);
	SecondaryWindow->SetPosition(OPMONITOR_LIST[1].X + contain, OPMONITOR_LIST[1].Y + contain);

#ifdef ADDON_imgui
	OPimguiInit(SecondaryWindow->Window, true);
#endif

	MainWindow->Bind();
	MainWindow->Focus();

	OPgameStateChange(&GS_01_VECTORS);
}

OPint ApplicationUpdate(OPtimer* timer) {
	SecondaryWindow->Bind();
	OPrenderUpdate();
	OPinputSystemUpdate(timer);
	if (OPkeyboardWasReleased(OPKEY_ESCAPE)) {
		return 1;
	}

	MainWindow->Bind();
	OPrenderUpdate();

	return ActiveState->Update(timer);
}

void ApplicationRender(OPfloat delta) {
	ActiveState->Render(delta);
}

void ApplicationDestroy() {

}

void ApplicationSetup() {
	OPinitialize = ApplicationInit;
	OPupdate = ApplicationUpdate;
	OPrender = ApplicationRender;
	OPdestroy = ApplicationDestroy;
}

//////////////////////////////////////
// Application Entry Point
//////////////////////////////////////
OP_MAIN {
	OP_LOG_LEVEL = 2000;
	OPlog("Starting up OPifex Engine");

	ApplicationSetup();

	OP_MAIN_START
	OP_MAIN_END
	OP_MAIN_SUCCESS
}
