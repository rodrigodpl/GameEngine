#pragma once

#define LIBRARY_BASE_PATH "Library"
#define LIBRARY_MESHES_PATH "Library/Meshes"
#define LIBRARY_TEXTURES_PATH "Library/Textures"
#define ASSETS_BASE_PATH "Assets"
#define SETTINGS_BASE_PATH "Settings"

// Warning disabled ---
#pragma warning( disable : 4577 ) // Warning that exceptions are disabled
#pragma warning( disable : 4530 )

#include <windows.h>
#include <stdio.h>

#define CAP(n) ((n <= 0.0f) ? n=0.0f : (n >= 1.0f) ? n=1.0f : n=n)

#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f
#define HAVE_M_PI

typedef unsigned int uint;

enum update_status
{
	UPDATE_CONTINUE = 1,
	UPDATE_STOP,
	UPDATE_ERROR
};

#define TITLE "Space Engine"