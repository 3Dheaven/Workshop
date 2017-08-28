cmake_minimum_required(VERSION 2.8)

project( Application )
set(EXE_NAME Application)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/modules/")
message("CMAKE_MODULE_PATH : " ${CMAKE_MODULE_PATH})

message("PROJECT_SOURCE_DIR : ${PROJECT_SOURCE_DIR}" )

macro(GroupSources curdir relativePath)

   file(GLOB children RELATIVE 
   ${PROJECT_SOURCE_DIR}/${relativePath}/${curdir}
   ${PROJECT_SOURCE_DIR}/${relativePath}${curdir}/*) 
   
   foreach(child ${children})

		if(IS_DIRECTORY ${PROJECT_SOURCE_DIR}/${relativePath}/${curdir}/${child})
			   GroupSources(${curdir}/${child} ${relativePath})
		else()
			string(REPLACE "/" "\\" groupname ${curdir})
			source_group(${groupname} FILES
			${PROJECT_SOURCE_DIR}/${relativePath}/${curdir}/${child})
		endif()
    endforeach()
	
endmacro()

# Sources.
file(
	GLOB_RECURSE
	SRC_FILES
	src/*
	workshop/*
)

GroupSources(src "//")
GroupSources(workshop "//")

# Exclude ZERO_CHECK project
SET(CMAKE_SUPPRESS_REGENERATION TRUE)

# Set C++11
set (CMAKE_CXX_STANDARD 11)

# FIND REQUIRED PACKAGES
find_package(GLEW REQUIRED)
find_package(wxWidgets REQUIRED)
find_package(SOIL REQUIRED)
find_package(ASSIMP REQUIRED)

link_directories(${wxWidgets_LIB_DIR})
include_directories(${wxWidgets_ROOT_DIR}/include)
include_directories(${wxWidgets_ROOT_DIR}/include/msvc)
include_directories(${CMAKE_MODULE_PATH}/../../deps/glm/)
include_directories(${GLEW_INCLUDE_PATH})
include_directories(${SOIL_INCLUDE_DIR})
include_directories(${ASSIMP_INCLUDE_DIR})
include_directories(${CMAKE_MODULE_PATH}/../../deps/vulkan/include)
include_directories(../../deps/YEngine/src)

#http://stackoverflow.com/questions/33400777/error-lnk2019-unresolved-external-symbol-main-referenced-in-function-int-cde
# force Unicode over Multi-byte
add_definitions(-DUNICODE -D_UNICODE)

#wxwidget seems to use some depracated functions : disable warnings on visual studio about it
add_definitions(-D_CRT_SECURE_NO_DEPRECATE)
add_definitions(-D_CRT_NONSTDC_NO_DEPRECATE)

add_subdirectory(../../deps/YEngine ${PROJECT_SOURCE_DIR}/../../deps/YEngine/build) # add project of yengine in our solution

add_executable(${EXE_NAME} ${SRC_FILES})
set(LIBS opengl32) 
target_link_libraries(${EXE_NAME} YEngine ${LIBS} ${GLEW_LIBRARY} ${SOIL_LIBRARY} ${ASSIMP_LIBRARY})
set_target_properties(${EXE_NAME} PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS")
set_target_properties(${EXE_NAME} PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:WINDOWS")
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Application)

# Copy gelw32.dll 64 bits with .exe
MESSAGE("PROJECT_SOURCE_DIR : ${PROJECT_SOURCE_DIR}")
set( CMAKE_CURRENT_BINARY_DIR "${PROJECT_SOURCE_DIR}/build/Debug/" )
add_custom_command(TARGET ${EXE_NAME} POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_MODULE_PATH}/../../deps/glew/bin/Release/x64/glew32.dll ${CMAKE_CURRENT_BINARY_DIR})

# Copy assimp-vc140.dll 64 bits with .exe
set( CMAKE_CURRENT_BINARY_DIR "${PROJECT_SOURCE_DIR}/build/Debug/" )
add_custom_command(TARGET ${EXE_NAME} POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_MODULE_PATH}/../../deps/assimp/lib/Release/assimp-vc140-mt.dll ${CMAKE_CURRENT_BINARY_DIR})
