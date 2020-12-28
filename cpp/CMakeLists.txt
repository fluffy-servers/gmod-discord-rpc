cmake_minimum_required(VERSION 3.10)
project(gmod-discord-rpc)
set(CMAKE_CXX_STANDARD 14)

if(UNIX)
	if(64BIT)
        set(CMAKE_CXX_FLAGS "-Os -fPIC")
    else()
    	set(CMAKE_CXX_FLAGS "-m32 -Os -s")
    endif()
endif()


set(BUILD_EXAMPLES OFF CACHE BOOL "Build example apps")
set(USE_STATIC_CRT ON CACHE BOOL "Use /MT[d] for dynamic library")
add_subdirectory(discord-rpc)
include_directories(${PROJECT_SOURCE_DIR}/discord-rpc/include)

add_subdirectory(gmod-module-base)

add_library(gdiscord SHARED GRPCTest.cpp)

target_link_libraries(gdiscord discord-rpc)
target_link_libraries(gdiscord gmod-module-base)

SET_TARGET_PROPERTIES(gdiscord PROPERTIES PREFIX "gmcl_")
if(UNIX)
    if(64BIT)
        SET_TARGET_PROPERTIES(gdiscord PROPERTIES SUFFIX "_linux64.dll")
    else()
        SET_TARGET_PROPERTIES(gdiscord PROPERTIES SUFFIX "_linux.dll")
    endif()
elseif(WIN32)
    if("${CMAKE_SIZEOF_VOID_P}" STREQUAL "4")
		SET_TARGET_PROPERTIES(gdiscord PROPERTIES SUFFIX "_win32.dll")
	else()
		SET_TARGET_PROPERTIES(gdiscord PROPERTIES SUFFIX "_win64.dll")
	endif()
endif()
