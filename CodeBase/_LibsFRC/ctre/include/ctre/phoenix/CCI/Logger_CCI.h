#pragma once

#ifndef CCIEXPORT
	#define CCIEXPORT     __attribute__((visibility("default")))
#endif

extern "C" {
	CCIEXPORT void c_Logger_Close();
	CCIEXPORT void c_Logger_Open(int language, bool logDriverStation);
	CCIEXPORT ctre::phoenix::ErrorCode c_Logger_Log(ctre::phoenix::ErrorCode code, const char* origin, int hierarchy, const char *stacktrace);
	CCIEXPORT void c_Logger_Description(ctre::phoenix::ErrorCode code, std::string & shortDescripToFill, std::string & longDescripToFill);
}
