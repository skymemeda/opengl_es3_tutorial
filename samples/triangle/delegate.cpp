#include "delegate.h"
#include <GLES3/gl3.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>

const char * vertShader = R"(#version 300 es
layout ( location = 0 ) in vec3 vPos;
layout ( location = 1 ) in vec3 vColor;

out vec3 Color;

void main()
{
	Color = vColor;
	gl_Position = vec4( vPos, 1.0f );
}
)";

const char * fragShader = R"(#version 300 es
precision mediump float;
in vec3 Color;

out vec4 fragColor;

void main()
{
	fragColor = vec4( Color, 1.0f );
}
)";

float triangleVerts[] = {
	-1.0f, -1.f, 0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 1.f, 0.0f,0.0f, 1.0f, 0.0f,
	1.0f, -1.f, 0.0f,0.0f, 0.0f, 1.0f,
};

uint16_t indexBuffer[] = {
	0, 1, 2
};

bool AppDelegate::initialize(void* _wnd)
{
	//
	glGenBuffers(1, &_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVerts), triangleVerts, GL_STATIC_DRAW);
	glGenBuffers(1, &_ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexBuffer), indexBuffer, GL_STATIC_DRAW);

	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0); 
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(sizeof(float)*3));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	//
	auto vert = glCreateShader(GL_VERTEX_SHADER);
	auto frag = glCreateShader(GL_FRAGMENT_SHADER);
	//
	glShaderSource(vert, 1, &vertShader, NULL);
	glCompileShader(vert);
	int compiled;
	glGetShaderiv(vert, GL_COMPILE_STATUS, &compiled);
	if (!compiled) {
		GLint  logSize;
		glGetShaderiv(vert, GL_INFO_LOG_LENGTH, &logSize);
		char* logMsg = new char[logSize];
		glGetShaderInfoLog(vert, logSize, NULL, logMsg);
		delete[] logMsg;
		return NULL;
	}//
	glShaderSource(frag, 1, &fragShader, NULL);
	glCompileShader(frag);

	glGetShaderiv(frag, GL_COMPILE_STATUS, &compiled);
	if (!compiled)
	{
		GLint  logSize;
		glGetShaderiv(frag, GL_INFO_LOG_LENGTH, &logSize);
		char* logMsg = new char[logSize];
		glGetShaderInfoLog(frag, logSize, NULL, logMsg);
		//PXLOG_I("shader error : %s", logMsg);
		delete[] logMsg;
		return NULL;
	}
	_prog = glCreateProgram();
	glAttachShader(_prog, vert);
	glAttachShader(_prog, frag);
	glLinkProgram(_prog);
	int linked;
	glGetProgramiv(_prog, GL_LINK_STATUS, &linked);
	if (!linked)
	{
		GLint  logSize;
		glGetShaderiv(_prog, GL_INFO_LOG_LENGTH, &logSize);
		char* logMsg = new char[logSize];
		glGetProgramInfoLog(_prog, logSize, NULL, logMsg);
		//PXLOG_I("link error : %s", logMsg);
		delete[] logMsg;
		return NULL;
	}

	glDeleteShader(vert);
	glDeleteShader(frag);

	return true;
}

void AppDelegate::resize(uint32_t _width, uint32_t _height)
{
	this->_width = _width;
	this->_height = _height;
	return;
}

void AppDelegate::release()
{

}

void AppDelegate::tick()
{
	glViewport(0, 0, _width, _height);
	glClearColor(1.0f, 1.0f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindVertexArray(_vao);
	glUseProgram(_prog);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, nullptr);
}

const char * AppDelegate::title()
{
	return "triangle";
}

IApplication * GetApplication()
{
	return new AppDelegate();
}