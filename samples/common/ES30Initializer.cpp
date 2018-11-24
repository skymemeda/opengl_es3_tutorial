#include "ES30Initializer.h"
#include <Windows.h>

EGLint GetContextRenderableType(EGLDisplay eglDisplay)
{
#ifdef EGL_KHR_create_context
	const char *extensions = eglQueryString(eglDisplay, EGL_EXTENSIONS);
	// check whether EGL_KHR_create_context is in the extension string
	if (extensions != NULL && strstr(extensions, "EGL_KHR_create_context")) {
		// extension is supported
		return EGL_OPENGL_ES3_BIT_KHR;
	}
#endif
	// extension is not supported
	return EGL_OPENGL_ES2_BIT;
}

bool InitOpenGLES30Context(EGLContextInfo& _context)
{
	EGLConfig config;
	EGLDisplay display = eglGetDisplay(_context.nativeDisplay);
	if (display == EGL_NO_DISPLAY){
		return false;
	}
	// get the supported opengl es version
	if (!eglInitialize(display, &_context.majorVersion, &_context.minorVersion)){
		return false;
	}
	{
		EGLint numConfigs = 0;
		EGLint attribList[] = {
		   EGL_RED_SIZE,       8,
		   EGL_GREEN_SIZE,     8,
		   EGL_BLUE_SIZE,      8,
		   EGL_ALPHA_SIZE,     8,
		   //
		   EGL_DEPTH_SIZE,     24,
		   EGL_STENCIL_SIZE,   8,
		   EGL_SAMPLE_BUFFERS, 0,
// 		   EGL_ALPHA_SIZE,     (flags & ES_WINDOW_ALPHA) ? 8 : EGL_DONT_CARE,
// 		   EGL_DEPTH_SIZE,     (flags & ES_WINDOW_DEPTH) ? 24 : EGL_DONT_CARE,
// 		   EGL_STENCIL_SIZE,   (flags & ES_WINDOW_STENCIL) ? 8 : EGL_DONT_CARE,
// 		   EGL_SAMPLE_BUFFERS, (flags & ES_WINDOW_MULTISAMPLE) ? 1 : 0,
 		   EGL_RENDERABLE_TYPE, GetContextRenderableType( display ),
		   EGL_NONE
		};

		// Choose config
		if (!eglChooseConfig(display, attribList, &config, 1, &numConfigs)) {
			return false;
		}
		if (numConfigs < 1){
			return false;
		}
	}
	// Create a surface
	EGLSurface surface = eglCreateWindowSurface( display, config, _context.nativeWindow, NULL);
	//
	if (surface == EGL_NO_SURFACE) {
		return false;
	}
	// Create a GL context
	EGLContext context = eglCreateContext( display, config, EGL_NO_CONTEXT, _context.contextAttribs);
	if (context == EGL_NO_CONTEXT) {
		return false;
	}
	// Make the context current
	if (!eglMakeCurrent( display, surface, surface, context )) {
		return false;
	}
	_context.context = context;
	_context.display = display;
	_context.surface = surface;
	//
	return true;
}

