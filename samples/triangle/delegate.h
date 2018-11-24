#pragma once
#include <stdint.h>
#include <EGL/egl.h>
#include <GLES3/gl3.h>
#include "../common/IApplication.h"

class AppDelegate:public IApplication {
private:
	uint32_t _width;
	uint32_t _height;
	// opengl resource
	GLuint _vbo;
	GLuint _ibo;
	GLuint _vao;
	//
	GLuint _prog;
public:
	virtual bool initialize(void* _wnd);
	virtual void resize(uint32_t _width, uint32_t _height);
	virtual void release();
	virtual void tick();
	virtual const char * title();

};

IApplication * GetApplication();