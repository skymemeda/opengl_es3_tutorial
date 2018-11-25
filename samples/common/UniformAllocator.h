#include <EGL/egl.h>
#include <GLES3/gl3.h>
#include <GLES3/gl3platform.h>
#include <cstdint>
#include <mutex>
#include <vector>

const int MAX_UNIFORM_BLOCK_SIZE = 1024;
const int UNIFORM_CHUNK_SIZE = MAX_UNIFORM_BLOCK_SIZE * 1024;

struct UniformBlock {
	GLuint buffer;
	GLuint offset;
};

class UniformAllocator 
{
private:
	std::vector<GLuint> buffers;
	std::vector<UniformBlock> freeList;
	std::mutex mtx;
public:
	UniformAllocator(){
	}
	UniformBlock alloc() {
		mtx.lock();
		if (!freeList.size())
		{
			GLuint buffer;
			glGenBuffers(1, &buffer);
			glBindBuffer(GL_UNIFORM_BUFFER, buffer);
			glBufferData(GL_UNIFORM_BUFFER, UNIFORM_CHUNK_SIZE, nullptr, GL_DYNAMIC_DRAW);
			glBindBuffer(GL_UNIFORM_BUFFER, 0);
			for (uint32_t i = 0; i < UNIFORM_CHUNK_SIZE / MAX_UNIFORM_BLOCK_SIZE; ++i)
			{
				freeList.push_back({ buffer, MAX_UNIFORM_BLOCK_SIZE * i	});
			}
			buffers.push_back(buffer);
		}
		auto unif = freeList.back();
		freeList.pop_back();
		mtx.unlock();
		return unif;
	}
	void free( const UniformBlock& _unif ) {
		mtx.lock();
		freeList.push_back(_unif);
		mtx.unlock();
	}
};