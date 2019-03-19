// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <thread>

#include <stdio.h>
#include <tchar.h>
#include <iostream>

#include "MouseController.h"
#include "pubsub.h"

int eyeTrackerLoop(PubSubHandler *pubSubHandler, bool *killSignal);
// TODO: reference additional headers your program requires here
