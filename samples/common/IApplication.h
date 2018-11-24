#include <cstdint>

class IApplication {
public:
	virtual bool initialize( void* _wnd ) = 0;
	virtual void resize(uint32_t _width, uint32_t _height) = 0;
	virtual void release() = 0;
	virtual void tick() = 0;
	virtual const char * title() = 0;
};

IApplication * GetApplication();