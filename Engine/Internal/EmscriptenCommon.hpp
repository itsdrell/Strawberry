#pragma once
//===============================================================================================
//	Get all the emscripten goodies
//===============================================================================================
#ifdef EMSCRIPTEN_PORT
	#include <emscripten.h>
	#include <GLES2/gl2.h>
	#include <GLES2/gl2ext.h>
	
	#include "emscripten/emscripten.h"
	#include "emscripten.h"
	#include <emscripten/emscripten.h>
#endif