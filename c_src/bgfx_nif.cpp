#ifdef __GNUC__
#define UNUSED(x) UNUSED_##x __attribute__((__unused__))
#else
#define UNUSED(x) UNUSED_##x
#endif

#ifndef _WIN32
#include <strings.h>
#include <unistd.h>
#else
#endif

#include "common.h"
#include "SDL.h"
#include "SDL_syswm.h"
#include "bx/uint32_t.h"
#include "nifpp.h"
#include <stdio.h>
#include "bgfx/bgfxplatform.h"
#include "bx/macros.h"
#include "bgfx/bgfx.h"
#include "logo.h"
#include <thread>
#include <chrono>
#include <stdexcept>
#include <memory>

using namespace nifpp;
using namespace std;

extern "C" {
/*
Create a sdl window. If you require removal of sdl windows for server purposes, create a new nif.
*/
const uint32_t Width = 1280;
const uint32_t Height = 720;
static int load(ErlNifEnv* env, void** priv, ERL_NIF_TERM load_info)
{
	nifpp::register_resource< bgfx::CallbackI* >(env, nullptr,
												 "callback_interface_ptr");
	nifpp::register_resource< bx::ReallocatorI* >(env, nullptr,
												  "reallocator_interface_ptr");
	nifpp::register_resource< SDL_Window* >(env, nullptr,
											"sdl_window_ptr");

	return 0;
}

static ERL_NIF_TERM _bgfx_sdl_create_window(ErlNifEnv* env, int argc,
											const ERL_NIF_TERM argv[])
{
	try
	{
		auto Window = nifpp::construct_resource< SDL_Window* >(SDL_CreateWindow("bgfx", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, Width, Height, SDL_WINDOW_HIDDEN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI));
		return nifpp::make(env, Window);
	}
	catch (nifpp::badarg)
	{
	}
	return enif_make_badarg(env);
}

static ERL_NIF_TERM _bgfx_init(ErlNifEnv* env, int argc,
							   const ERL_NIF_TERM argv[])
{
	try
	{
		auto _type = nifpp::get< int >(env, argv[0]);
		auto _vendor_id = nifpp::get< unsigned int >(env, argv[1]);
		auto _device_id = nifpp::get< unsigned int >(env, argv[2]);
		nifpp::str_atom callback_atom;
		nifpp::get_throws(env, argv[3], callback_atom);
		nifpp::str_atom reallocate_atom;
		nifpp::get_throws(env, argv[4], reallocate_atom);
		nifpp::resource_ptr< SDL_Window* > Window;
		nifpp::get< SDL_Window* >(env, argv[5], Window);
		SDL_SysWMinfo wmInfo;
		SDL_VERSION(&wmInfo.version);
		SDL_GetWindowWMInfo(*Window, &wmInfo);
		HWND hwnd = wmInfo.info.win.window;
		bgfx::winSetHwnd(hwnd);
		bgfx::renderFrame();
		bgfx::init(static_cast< bgfx::RendererType::Enum >(_type), _vendor_id, _device_id, nullptr,
				   nullptr);
		return nifpp::make(env, nifpp::str_atom("ok"));
	}
	catch (nifpp::badarg)
	{
	}
	return enif_make_badarg(env);
}

static ERL_NIF_TERM _bgfx_reset(ErlNifEnv* env, int argc,
								const ERL_NIF_TERM argv[])
{
	try
	{
		auto _width = nifpp::get< uint32_t >(env, argv[0]);
		auto _height = nifpp::get< uint32_t >(env, argv[1]);
		auto _flags = nifpp::get< uint32_t >(env, argv[2]);
		bgfx::reset(_width, _height, _flags);
		return nifpp::make(env, nifpp::str_atom("ok"));
	}
	catch (nifpp::badarg)
	{
	}
	return enif_make_badarg(env);
}

static ERL_NIF_TERM _bgfx_set_view_clear(ErlNifEnv* env, int argc,
										 const ERL_NIF_TERM argv[])
{
	try
	{
		auto _id = nifpp::get< uint8_t >(env, argv[0]);
		auto _flags = nifpp::get< uint16_t >(env, argv[1]);
		auto _rgba = nifpp::get< uint32_t >(env, argv[2]);
		auto _depth = nifpp::get< double >(env, argv[3]);
		auto _stencil = nifpp::get< uint8_t >(env, argv[4]);
//		bgfx::setViewClear(_id, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x303030ff, static_cast< float >(_depth), _stencil);
		return nifpp::make(env, nifpp::str_atom("error"));
	}
	catch (nifpp::badarg)
	{
	}
	return enif_make_badarg(env);
}

static ERL_NIF_TERM _bgfx_run(ErlNifEnv* env, int argc,
							  const ERL_NIF_TERM argv[])
{
	try
	{
		nifpp::resource_ptr< SDL_Window* > Window;
		nifpp::get< SDL_Window* >(env, argv[0], Window);
		bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x303030ff, 0.0f);
		uint32_t debug = BGFX_DEBUG_TEXT;
		// Enable debug text.
		bgfx::setDebug(debug);
		SDL_ShowWindow(*Window);
		SDL_Event e;
		auto running = true;
		while (running)
		{
			while (SDL_PollEvent(&e))
			{
				if (e.type == SDL_QUIT)
				{
					running = false;
				}
				if (e.type == SDL_KEYDOWN)
				{
					running = false;
				}
				if (e.type == SDL_WINDOWEVENT)
				{
					switch (e.window.event)
					{
					case SDL_WINDOWEVENT_CLOSE:
						running = false;
						break;
					}
				}
			}

			// Set view 0 default viewport.
			bgfx::setViewRect(0, 0, 0, Width, Height);

			// This dummy draw call is here to make sure that view 0 is cleared
			// if no other draw calls are submitted to view 0.
			bgfx::touch(0);

			// Use debug font to print information about this example.
			bgfx::dbgTextClear();
			bgfx::dbgTextImage(bx::uint16_max(Width / 2 / 8, 20) - 20, bx::uint16_max(Height / 2 / 16, 6) - 6, 40, 12, s_logo, 160);
			bgfx::dbgTextPrintf(0, 1, 0x4f, "bgfx/examples/lib_bgfx");
			bgfx::dbgTextPrintf(0, 2, 0x6f, "Description: Initialization and debug text in Elixir and SDL2.");
			// Advance to next frame. Rendering thread will be kicked to
			// process submitted rendering primitives.
			bgfx::frame();
		}
		SDL_DestroyWindow(*Window);
		bgfx::shutdown();
		return nifpp::make(env, nifpp::str_atom("ok"));
	}
	catch (nifpp::badarg)
	{
	}
	return enif_make_badarg(env);
}

static ErlNifFunc nif_funcs[] = {
	{"_bgfx_init", 6, _bgfx_init, ERL_NIF_DIRTY_JOB_CPU_BOUND},
	{"_bgfx_reset", 3, _bgfx_reset, ERL_NIF_DIRTY_JOB_CPU_BOUND},
	{"_bgfx_set_view_clear", 5, _bgfx_set_view_clear, ERL_NIF_DIRTY_JOB_CPU_BOUND},
	{"_bgfx_run", 1, _bgfx_run, ERL_NIF_DIRTY_JOB_CPU_BOUND},
	{"_bgfx_sdl_create_window", 0, _bgfx_sdl_create_window, ERL_NIF_DIRTY_JOB_CPU_BOUND}};

ERL_NIF_INIT(Elixir.Bgfx.Nif, nif_funcs, load, NULL, NULL, NULL)
} // extern C