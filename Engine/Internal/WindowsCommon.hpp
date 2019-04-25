#pragma once
#ifndef EMPSCRIPTEN_PORT
	#define WIN32_LEAN_AND_MEAN
	#include <WinSock2.h>
	#include <WS2tcpip.h>
	#include <Windows.h>
#endif

//====================================================================================
//									NOTES:
//____________________________________________________________________________________
// 
// This is for people that ever include windows, they can just use this header
//====================================================================================