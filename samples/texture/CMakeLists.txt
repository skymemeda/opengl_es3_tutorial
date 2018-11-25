if(MSVC)
	add_definitions( -DUNICODE )
endif()

project( Texture )

set( Source
#project file
	${CMAKE_CURRENT_SOURCE_DIR}/delegate.cpp
# cpp code
	${CMAKE_CURRENT_SOURCE_DIR}/../common/ES30Initializer.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/../common/UniformAllocator.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/../common/main.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/../common/Resource.h
	${CMAKE_CURRENT_SOURCE_DIR}/../common/VulkanWindow.rc
)

add_executable(
	Texture
	WIN32
	${Source}
)

target_link_libraries( Texture
	${EGL}
	${GLESv2}
	${MaliEmulator}
)