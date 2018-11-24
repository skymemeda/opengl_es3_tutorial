#pragma once
#include <GLES3/gl3.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>

struct EGLContextInfo {
	EGLNativeWindowType  nativeWindow; // HWND
	EGLNativeDisplayType nativeDisplay; // HDC
	//
	EGLContext context;
	EGLSurface surface;
	EGLDisplay display;
	//
	EGLint majorVersion;
	EGLint minorVersion;
	//
	EGLint contextAttribs[3] = { 
		EGL_CONTEXT_CLIENT_VERSION, 3, EGL_NONE 
	};
};

bool InitOpenGLES30Context(EGLContextInfo& _context);