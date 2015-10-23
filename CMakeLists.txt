cmake_minimum_required(VERSION 2.8)

set(PROJECT_NAME lib_bgfx)
project(${PROJECT_NAME})
#include (InstallRequiredSystemLibraries)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/CMake/Module)
find_package(OpenGL)
#set(SDL2_BUILDING_LIBRARY ON)
#find_package(SDL2)
find_package(glfw)

set(MAIN_SOURCE
    C_src/bgfx_entry.hpp
    c_src/bgfx_nif.cpp
    )

add_definitions(-DENTRY_CONFIG_USE_GLFW -DGLEW_STATIC)
  
add_subdirectory(dep)

add_library(${PROJECT_NAME} SHARED ${MAIN_SOURCE})
set(CXX clang++)
set(ERL_ARG erl -eval "io:format(\"~s~n\", [lists:concat([code:root_dir(), \"/erts-\", erlang:system_info(version), \"/include\"])])" -s init stop -noshell)

execute_process(COMMAND erl ARGS ${ERL_ARG} OUTPUT_VARIABLE ERLANG_PATH)
message(STATUS "Detecting Erlang Path -- ${ERLANG_PATH}")

if(MSVC)
add_definitions(-DWIN32
			-D_WIN32
			-D_HAS_EXCEPTIONS=0
			-D_HAS_ITERATOR_DEBUGGING=0
            -D_SCL_SECURE=0
            -D_SECURE_SCL=0
            -D_SCL_SECURE_NO_WARNINGS
            -D_CRT_SECURE_NO_WARNINGS
            -D_CRT_SECURE_NO_DEPRECATE)
endif()
include_directories(dep/bx/include/compat/msvc dep/bgfx/examples/common dep/bgfx/3rdparty dep/bgfx/include dep/bx/include dep/nifpp ${ERLANG_PATH} ${OPENGL_INCLUDE_DIR} ${SDL2_INCLUDE_DIR} ${GLFW3_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME} bgfx ${SDL2_LIBRARY} ${GLFW3_LIBRARIES})

install(FILES ${SDL2_INCLUDE_DIR}/../../bin/SDL2.dll DESTINATION ${CMAKE_SOURCE_DIR})
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION .)
