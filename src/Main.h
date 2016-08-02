#pragma once

#include "./OPengine.h"

#define RUN_SECONDARY_WINDOW 0

extern OPwindow* MainWindow;

#ifdef RUN_SECONDARY_WINDOW
extern OPwindow* SecondaryWindow;
#endif

void ApplicationSetup();
