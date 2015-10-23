cmake_minimum_required(VERSION 2.8)

set(PROJECT_NAME bgfx)
project(${PROJECT_NAME})
#include (InstallRequiredSystemLibraries)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/CMake/Module)
find_package(OpenGL)

add_definitions(-DBGFX_CONFIG_RENDERER_DIRECT3D12 -DBGFX_CONFIG_RENDERER_NULL -DBGFX_CONFIG_RENDERER_DIRECT3D11 -DBGFX_CONFIG_RENDERER_DIRECT3D9 -DBGFX_CONFIG_RENDERER_OPENGL=0 -DBGFX_CONFIG_MULTITHREADED -DENTRY_CONFIG_USE_SDL)

if (NOT PNACL)
  include_directories(bgfx/3rdparty/khronos)
endif()
include_directories(bgfx/3rdparty)
# Add everything to the build process, bgfx is smart
# to know what to compile on each platform
set(MAIN_DIR bgfx/src)
seT(ENTRY_DIR bgfx/examples/common/entry)
set(MAIN_SOURCE
  ${MAIN_DIR}/amalgamated.cpp
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

set(MAIN_INCLUDE ${MAIN_INCLUDE} bgfx/include/bgfx)
# Includes
set(MAIN_SOURCE ${MAIN_SOURCE} 
  #${MAIN_INCLUDE}/c99/bgfx.h
  #${MAIN_INCLUDE}/bgfx.h
  #${MAIN_INCLUDE}/bgfxdefines.h
  #${MAIN_INCLUDE}/c99/bgfxplatform.h
  #${MAIN_INCLUDE}/bgfxplatform.h
  ${EXAMPLE_COMMON}
  )


set(MAIN_INCLUDE ${MAIN_INCLUDE} bgfx/examples/common bgfx/3rdparty/dxsdk/include  ${SDL2_INCLUDE_DIR})
set(COMMON_DIR bgfx/examples/common)
set(COMMON_IMGUI_DIR ${COMMON_DIR}/imgui)

set(COMMON_NANOVG_DIR ${COMMON_DIR}/nanovg)

set(COMMON_ENTRY_DIR ${COMMON_DIR}/entry)
set(COMMON_FONT_DIR ${COMMON_DIR}/font)
set(EXAMPLE_COMMON 
        ${COMMON_DIR}/aviwriter.h
        ${COMMON_DIR}/bgfx_utils.cpp ${COMMON_DIR}/bgfx_utils.h
        ${COMMON_DIR}/bounds.cpp ${COMMON_DIR}/bounds.h
        ${COMMON_DIR}/camera.cpp ${COMMON_DIR}/camera.h
        ${COMMON_DIR}/common.h
        ${COMMON_DIR}/cube_atlas.cpp ${COMMON_DIR}/cube_atlas.h

        ${COMMON_IMGUI_DIR}/fs_imgui_color.sc   ${COMMON_IMGUI_DIR}/vs_imgui_color.sc
        ${COMMON_IMGUI_DIR}/fs_imgui_cubemap.sc ${COMMON_IMGUI_DIR}/vs_imgui_cubemap.sc
        ${COMMON_IMGUI_DIR}/fs_imgui_image.sc   ${COMMON_IMGUI_DIR}/vs_imgui_image.sc
        ${COMMON_IMGUI_DIR}/fs_imgui_texture.sc ${COMMON_IMGUI_DIR}/vs_imgui_texture.sc
        ${COMMON_IMGUI_DIR}/fs_ocornut_imgui.sc ${COMMON_IMGUI_DIR}/vs_ocornut_imgui.sc
        ${COMMON_IMGUI_DIR}/fs_imgui_image_swizz.sc
        ${COMMON_IMGUI_DIR}/droidsans.ttf.h
        ${COMMON_IMGUI_DIR}/imgui.cpp ${COMMON_IMGUI_DIR}/imgui.h
        ${COMMON_IMGUI_DIR}/ocornut_imgui.cpp ${COMMON_IMGUI_DIR}/ocornut_imgui.h
        ${COMMON_IMGUI_DIR}/varying.def.sc
        ${COMMON_NANOVG_DIR}/fs_nanovg_fill.sc ${COMMON_NANOVG_DIR}/vs_nanovg_fill.sc
        ${COMMON_NANOVG_DIR}/fontstash.h
        ${COMMON_NANOVG_DIR}/nanovg.cpp ${COMMON_NANOVG_DIR}/nanovg.h
        ${COMMON_NANOVG_DIR}/nanovg_bgfx.cpp
        ${COMMON_NANOVG_DIR}/varying.def.sc
        ${COMMON_FONT_DIR}/fs_font_basic.sc
        ${COMMON_FONT_DIR}/vs_font_basic.sc
        ${COMMON_FONT_DIR}/fs_font_distance_field.sc
        ${COMMON_FONT_DIR}/vs_font_distance_field.sc
        ${COMMON_FONT_DIR}/fs_font_distance_field_subpixel.sc
        ${COMMON_FONT_DIR}/vs_font_distance_field_subpixel.sc
        ${COMMON_FONT_DIR}/font_manager.cpp ${COMMON_FONT_DIR}/font_manager.h
        ${COMMON_FONT_DIR}/text_buffer_manager.cpp ${COMMON_FONT_DIR}/text_buffer_manager.h
        ${COMMON_FONT_DIR}/text_metrics.cpp ${COMMON_FONT_DIR}/text_metrics.h
        ${COMMON_FONT_DIR}/utf8.cpp ${COMMON_FONT_DIR}/utf8.h ${COMMON_FONT_DIR}/varying.def.sc
        ${COMMON_ENTRY_DIR}/cmd.cpp  ${COMMON_ENTRY_DIR}/cmd.h
        ${COMMON_ENTRY_DIR}/dbg.cpp  ${COMMON_ENTRY_DIR}/dbg.h
        #${COMMON_ENTRY_DIR}/entry.cpp  ${COMMON_ENTRY_DIR}/entry.h  ${COMMON_ENTRY_DIR}/entry_p.h
        ${COMMON_ENTRY_DIR}/input.cpp  ${COMMON_ENTRY_DIR}/input.h
    )

 # set(EXAMPLE_COMMON ${EXAMPLE_COMMON} ${COMMON_ENTRY_DIR}/entry_sdl.cpp)

# untested, note: OSX and iOS currently
# automatically link against GL
if (WIN32)
#  add_library(opengl32)
elseif (LINUX)
#  add_library(X11 Xrandr Xi Xinerama Xxf86vm Xcursor GL dl)
elseif (ANDROID)
  # FIXME!
endif()

add_library(${PROJECT_NAME} STATIC ${EXAMPLE_COMMON} ${MAIN_SOURCE})
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES})
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

include_directories(${MAIN_INCLUDE} bx/include bx/include/compat/msvc bgfx/include ${OPENGL_INCLUDE_DIR})
