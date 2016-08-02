//////////////////////////////////////
// Application Entry Point
//////////////////////////////////////
#include "Main.h"
#include "GameState.h"
#include "Slides/01_Vectors.h"
#include "Slides\Gimbal_Lock.h"

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
	i32 width = OPMONITOR_LIST[0].VideoModeCurrent.Width;
	i32 height = OPMONITOR_LIST[0].VideoModeCurrent.Height;

	/*width = 1280;
	height = 720;*/
	contain = 0;

	i32 posX = OPMONITOR_LIST[0].X + (OPMONITOR_LIST[0].VideoModeCurrent.Width - width) / 2 + contain;
	i32 posY = OPMONITOR_LIST[0].Y + (OPMONITOR_LIST[0].VideoModeCurrent.Height - height) / 2 + contain;

	MainWindow = OPrenderCreateWindow(NULL, false, true, "GamedevGR", width - contain - contain, height - contain - contain);
	MainWindow->SetPosition(posX, posY);

#if RUN_SECONDARY_WINDOW
	SecondaryWindow = OPrenderCreateWindow(NULL, false, true, "GamedevGR", OPMONITOR_LIST[1].VideoModeCurrent.Width - contain - contain, OPMONITOR_LIST[1].VideoModeCurrent.Height - contain - contain);
	SecondaryWindow->SetPosition(OPMONITOR_LIST[1].X + contain, OPMONITOR_LIST[1].Y + contain);

	#ifdef ADDON_imgui
		OPimguiInit(SecondaryWindow->Window, true);
	#endif

	SecondaryWindow->Focus();
#else

	#ifdef ADDON_imgui
		OPimguiInit(MainWindow, true);
	#endif
#endif
	MainWindow->Bind();

	//OPgameStateChange(&GS_01_VECTORS);
	OPgameStateChange(&GIMBAL_LOCK);
}

OPint ApplicationUpdate(OPtimer* timer) {
#if RUN_SECONDARY_WINDOW
	SecondaryWindow->Bind();
	//SecondaryWindow->Focus();
	OPrenderUpdate();
#endif
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
OP_MAIN_START

	OP_LOG_LEVEL = 2000;
	OPlog("Starting up OPifex Engine");

	ApplicationSetup();

	OP_MAIN_RUN

OP_MAIN_END