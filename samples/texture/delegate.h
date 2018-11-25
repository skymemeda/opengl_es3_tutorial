#pragma once
#include <stdint.h>
#include <EGL/egl.h>
#include <GLES3/gl3.h>
#include <vector>
#include <string>
#include "../common/IApplication.h"

class AppDelegate:public IApplication {
public:
	struct SamplerSlot {
		std::string name;
		int uniformLoc;
		uint32_t textureUnit;
	};
private:
	uint32_t _width;
	uint32_t _height;
	// opengl resource
	GLuint _vbo;
	GLuint _ibo;
	GLuint _vao;
	//
	GLuint _texture;
	GLuint _texture2;
	//
	GLuint _sampler;
	int _samplerUniformLoc;
	uint32_t _textureUnit;
	int _samplerUniformLoc2;
	uint32_t _textureUnit2;
	//
	std::vector< SamplerSlot > _unifTex2D;
	std::vector< SamplerSlot > _unifTexCube;
	std::vector< SamplerSlot > _unifTexDepth;
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