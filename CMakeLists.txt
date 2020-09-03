cmake_minimum_required(VERSION 3.14.0 FATAL_ERROR)

project(PacketQueue)

set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17")


if (WIN32)
	add_compile_options(/W4 /std:c++17 $<$<CONFIG:Release>:/GL>)
	add_link_options($<$<CONFIG:Release>:/LTCG:INCREMENTAL>)
endif()

set(SOURCES
		main.cpp
		PacketQueue.cpp
		)

set(HEADER 
		PacketQueue.h
		PacketQueue.inl
		)

add_executable(PacketQueue  ${SOURCES} ${HEADER})
