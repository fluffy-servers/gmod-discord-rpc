cmake_minimum_required(VERSION 3.10)
project(gmod-discord-rpc)
set(CMAKE_CXX_STANDARD 14)

if(UNIX)
    set(CMAKE_CXX_FLAGS "-m32 -Os -s")
endif()


set(BUILD_EXAMPLES OFF CACHE BOOL "Build example apps")
set(USE_STATIC_CRT ON CACHE BOOL "Use /MT[d] for dynamic library")
add_subdirectory(discord-rpc)
include_directories(${PROJECT_SOURCE_DIR}/discord-rpc/include)

add_subdirectory(gmod-module-base)

add_library(fluffydiscord SHARED GRPCTest.cpp)

target_link_libraries(fluffydiscord discord-rpc)
target_link_libraries(fluffydiscord gmod-module-base)

SET_TARGET_PROPERTIES(fluffydiscord PROPERTIES PREFIX "gmcl_")
if(UNIX)
    SET_TARGET_PROPERTIES(fluffydiscord PROPERTIES SUFFIX "_linux.dll")
elseif(WIN32)
    if("${CMAKE_SIZEOF_VOID_P}" STREQUAL "4")
		SET_TARGET_PROPERTIES(${library} PROPERTIES SUFFIX "_win32.dll")
	else()
		SET_TARGET_PROPERTIES(${library} PROPERTIES SUFFIX "_win64.dll")
	endif()
endif()