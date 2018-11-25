#include "delegate.h"
#include <GLES3/gl3.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

const char * vertShader = R"(#version 300 es
layout ( location = 0 ) in vec3 vPos;
layout ( location = 1 ) in vec2 vUv;

out vec2 uv;

void main()
{
	uv = vUv;
	gl_Position = vec4( vPos, 1.0f );
}
)";

const char * fragShader = R"(#version 300 es
precision mediump float;
uniform sampler2D samBase;
uniform sampler2D samGirl;
in vec2 uv;

out vec4 fragColor;
void main()
{
	fragColor = texture( samBase,uv ) * texture( samGirl,uv );
}
)";

float triangleVerts[] = {
	-0.5f, 0.5f, 0.0f, 0,1,//左上角
	0.5f, 0.5f, 0.0f,1,1,//右上角
	0.5f, -0.5f, 0.0f,1,0,//右下角
	-0.5f,-0.5f,0.0f,0,0//左下角
};

uint16_t indexBuffer[] = {
	3,0,2,1
};

uint32_t pixel[] = {
	0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,
	0xffffffff,0xffffffff,0xff00ff00,0xff001100,0xffffffff,0xffffffff,0xff0000ff,0xff0000ff,
	0xff000000,0xff000000,0xffffffff,0xffffffff,0xff000000,0xff000000,0xffffffff,0xffffffff,
	0xff000000,0xff000000,0xffffffff,0xffffffff,0xff000000,0xff000000,0xffffffff,0xffffffff,
	0xffffffff,0xffffffff,0xff000000,0xff000000,0xffffffff,0xffffffff,0xff000000,0xff000000,
	0xffffffff,0xffffffff,0xff000000,0xff000000,0xffffffff,0xffffffff,0xff000000,0xff000000,
	0xffff0000,0xffff0000,0xffffffff,0xffffffff,0xff000000,0xff000000,0xffffffff,0xffffffff,
	0xffff0000,0xffff0000,0xffffffff,0xffffffff,0xff000000,0xff000000,0xffffffff,0xffffffff,
};

bool AppDelegate::initialize(void* _wnd)
{
	auto error = glGetError();
	glGenTextures(1, &_texture);
	glBindTexture(GL_TEXTURE_2D, _texture);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, 8, 8);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 8, 8, GL_RGBA, GL_UNSIGNED_BYTE, pixel);
	error = glGetError();
	if (error != GL_NO_ERROR)
	{
		return false;
	}

	FILE* file = fopen("texture.jpg","rb");
	if (!file)
	{
		return false;
	}
	auto begin = ftell(file);
	fseek(file,0,SEEK_END);
	auto end = ftell(file);

	char* buffer = new char[end - begin];
	fseek(file, 0, SEEK_SET);
	fread(buffer, 1, end - begin, file);
	fclose(file);

	int x, y, n;
	stbi_uc * imageBytes = stbi_load_from_memory((const stbi_uc*)buffer, end-begin, &x, &y, &n, 4);
	//assert(x == 256 && y == 256);
	glGenTextures(1, &_texture2);
	glBindTexture(GL_TEXTURE_2D, _texture2);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, x, y);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, x, y, GL_RGBA, GL_UNSIGNED_BYTE, imageBytes);
	stbi_image_free(imageBytes);
	delete[]buffer;
	//
	glGenSamplers(1, &_sampler);
	glSamplerParameteri(_sampler, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glSamplerParameteri(_sampler, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	

	//
	glGenBuffers(1, &_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVerts), triangleVerts, GL_STATIC_DRAW);
	glGenBuffers(1, &_ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexBuffer), indexBuffer, GL_STATIC_DRAW);

	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0); 
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(sizeof(float)*3));
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

	//glBindSampler()
	//glBindSampler()

	glDeleteShader(vert);
	glDeleteShader(frag);

	// 获取shader uniform信息
	GLint nUniform;
	glGetProgramiv( _prog, GL_ACTIVE_UNIFORMS, &nUniform);
	GLsizei name_len;
	GLenum type;
	GLint size;
	GLint samplerLocation;
	char samplerName[1024];
	// 绑定槽计数
	uint32_t textureSlot = 0;

	for (GLint idx = 0; idx < nUniform; ++idx)
	{
		glGetActiveUniform(_prog, idx, 1024, &name_len, &size, &type, samplerName);
		samplerLocation = glGetUniformLocation(_prog, samplerName);
		switch (type)
		{
		case GL_SAMPLER_2D:
		{
			_unifTex2D.push_back({ samplerName, samplerLocation, textureSlot });
			++textureSlot;
			break;
		}
		case GL_SAMPLER_2D_SHADOW:
		{
			_unifTexDepth.push_back({ samplerName, samplerLocation, textureSlot });
			++textureSlot;
			break;
		}
		case GL_SAMPLER_CUBE:
		{
			_unifTexCube.push_back({ samplerName, samplerLocation, textureSlot });
			++textureSlot;
			break;
		}
		default:
		{
			break;
		}
		}
	}
	//
	for (auto& samplerSlot : _unifTex2D)
	{
		if (samplerSlot.name == "samBase")
		{
			_samplerUniformLoc = samplerSlot.uniformLoc;
			_textureUnit = samplerSlot.textureUnit;
			//break;
		}
		if (samplerSlot.name == "samGirl")
		{
			_samplerUniformLoc2 = samplerSlot.uniformLoc;
			_textureUnit2 = samplerSlot.textureUnit;
		}
	}

	error = glGetError();
	if (error != GL_NO_ERROR)
	{
		return false;
	}

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


	// 绑定 采样器<->纹理
	glActiveTexture(_textureUnit2 + GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _texture2);
	glBindSampler(_textureUnit2, _sampler);
	// 把采样器绑定到着色器上去
	glUniform1i(_samplerUniformLoc2, _textureUnit2);

	// 绑定 采样器<->纹理
	glActiveTexture(_textureUnit + GL_TEXTURE0 );
	glBindTexture(GL_TEXTURE_2D, _texture);
	glBindSampler(_textureUnit, _sampler);
	// 把采样器绑定到着色器上去
	glUniform1i(_samplerUniformLoc, _textureUnit );





	glBindVertexArray(_vao);
	glUseProgram(_prog);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
	glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_SHORT, nullptr);
}

const char * AppDelegate::title()
{
	return "texture mapping";
}

IApplication * GetApplication()
{
	return new AppDelegate();
}