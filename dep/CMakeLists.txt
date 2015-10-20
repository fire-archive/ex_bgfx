cmake_minimum_required(VERSION 2.8)

set(PROJECT_NAME bgfx)
project(${PROJECT_NAME})
#include (InstallRequiredSystemLibraries)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/CMake/Module)
find_package(OpenGL)

add_definitions(-DBGFX_CONFIG_RENDERER_DIRECT3D12=0)

if (NOT PNACL)
  include_directories(bgfx/3rdparty/khronos)
endif()
include_directories(bgfx/3rdparty)
# Add everything to the build process, bgfx is smart
# to know what to compile on each platform
set(MAIN_DIR bgfx/src)
set(MAIN_SOURCE
  ${MAIN_DIR}/bgfx.cpp ${MAIN_DIR}/bgfx_p.h
  ${MAIN_DIR}/charset.h ${MAIN_DIR}/config.h
  ${MAIN_DIR}/glimports.h
  ${MAIN_DIR}/image.cpp ${MAIN_DIR}/image.h
  ${MAIN_DIR}/ovr.cpp ${MAIN_DIR}/ovr.h
  ${MAIN_DIR}/renderdoc.cpp ${MAIN_DIR}/renderdoc.h
  ${MAIN_DIR}/renderer.h
  ${MAIN_DIR}/renderer_null.cpp
  ${MAIN_DIR}/vertexdecl.cpp ${MAIN_DIR}/vertexdecl.h
  ${MAIN_DIR}/varying.def.sc

  # hmm looks like these files must be included
  # on all platforms, otherwise linker errors
  # from Bgfx::s_rendererCreator result:
  ${MAIN_DIR}/renderer_gl.cpp ${MAIN_DIR}/renderer_gl.h
  ${MAIN_DIR}/renderer_d3d11.cpp ${MAIN_DIR}/renderer_d3d11.h
  ${MAIN_DIR}/renderer_d3d12.cpp
  ${MAIN_DIR}/renderer_d3d9.cpp ${MAIN_DIR}/renderer_d3d9.h
  ${MAIN_DIR}/renderer_vk.cpp
  )

set(BGFX_SHADERS
  fs_clear0.sc
  fs_clear1.sc
  fs_clear2.sc
  fs_clear3.sc
  fs_clear4.sc
  fs_clear5.sc
  fs_clear6.sc
  fs_clear7.sc
  fs_debugfont.sc
  vs_clear.sc
  vs_debugfont.sc
  )

if (MACOS)
  fips_files(glcontext_nsgl.mm glcontext_nsgl.h)
elseif (IOS)
  fips_files(glcontext_eagl.mm glcontext_eagl.h)
elseif (LINUX)
  fips_files(glcontext_glx.cpp glcontext_glx.h)
elseif (PNACL)
  fips_files(glcontext_ppapi.cpp glcontext_ppapi.h)
elseif (WIN32)
  set(MAIN_SOURCE ${MAIN_SOURCE}  ${MAIN_DIR}/glcontext_wgl.cpp  ${MAIN_DIR}/glcontext_wgl.h)
else()
  fips_files(glcontext_egl.cpp glcontext_egl.h)
endif()

set(MAIN_INCLUDE bgfx/include/bgfx)
# Includes
set(MAIN_SOURCE ${MAIN_SOURCE}
  ${MAIN_INCLUDE}/c99/bgfx.h
  ${MAIN_INCLUDE}/bgfx.h
  ${MAIN_INCLUDE}/bgfxdefines.h
  ${MAIN_INCLUDE}/c99/bgfxplatform.h
  ${MAIN_INCLUDE}/bgfxplatform.h
  )

# untested, note: OSX and iOS currently
# automatically link against GL
if (WIN32)
#  add_library(opengl32)
elseif (LINUX)
#  add_library(X11 Xrandr Xi Xinerama Xxf86vm Xcursor GL dl)
elseif (ANDROID)
  # FIXME!
endif()

add_library(${PROJECT_NAME} ${MAIN_SOURCE})
#set(CXX clang++)
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
#install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION priv)
#install(FILES ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS} DESTINATION priv COMPONENT Libraries)

include_directories(bx/include bx/include/compat/msvc bgfx/include ${OPENGL_INCLUDE_DIR})
