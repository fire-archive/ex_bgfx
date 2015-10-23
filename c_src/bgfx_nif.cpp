#ifdef __GNUC__
#define UNUSED(x) UNUSED_##x __attribute__((__unused__))
#else
#define UNUSED(x) UNUSED_##x
#endif

#include "nifpp.h"
#include <stdio.h>
#ifndef _WIN32
#include <strings.h>
#include <unistd.h>
#else
#endif

//#include "SDL.h"
//#include "SDL_syswm.h"
#include "bgfx_entry.hpp"
#include "common.h"
#include "entry/entry_p.h"
#include "bgfx/bgfxplatform.h"
#include "bgfx/bgfx.h"
#include "logo.h"
#include <thread>
#include <chrono>
#include <stdexcept>
#include <memory>

using namespace nifpp;
using namespace std;

extern "C" {
//	int _main_(int argc, char * argv[])
//	{
//		return 0;
//	}
/*
Create a sdl window. If you require no sdl windows for server purposes, create a new nif.
*/
static int load(ErlNifEnv* env, void** priv, ERL_NIF_TERM load_info)
{
	nifpp::register_resource< bgfx::CallbackI* >(env, nullptr,
												 "callback_interface_ptr");
	nifpp::register_resource< bx::ReallocatorI* >(env, nullptr,
												  "reallocator_interface_ptr");
	//nifpp::register_resource< SDL_Window* >(env, nullptr, "sdl_window_ptr");
	//nifpp::register_resource< entry::Context >(env, nullptr, "context_ptr");

	//SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER);

	const uint32_t width = 1280;
	const uint32_t height = 768;

	//auto window = nifpp::construct_resource< SDL_Window* >(SDL_CreateWindow("bgfx", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE));
	//m_flags[0] = 0
	//	| ENTRY_WINDOW_FLAG_ASPECT_RATIO
	//	| ENTRY_WINDOW_FLAG_FRAME
	//	;

	//s_userEventStart = SDL_RegisterEvents(7);
	//SDL_Delay(500);
	return 0; //!bgfx::sdlSetWindow(*window);
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
		//auto _callback = nifpp::construct_resource< bgfx::CallbackI* >(
		//	nifpp::get< bgfx::CallbackI* >(env, argv[3]));
		//auto _reallocator = nifpp::construct_resource< bx::ReallocatorI* >(
		//	nifpp::get< bx::ReallocatorI* >(env, argv[4]));
		//auto context = nifpp::construct_resource< entry::Context >();
		//context->run(0, nullptr);
		return nifpp::make(env, bgfx::init(static_cast< bgfx::RendererType::Enum >(_type), _vendor_id, _device_id, nullptr,
										   nullptr)
									? nifpp::str_atom("ok")
									: nifpp::str_atom("error")); //nifpp::make(env, nifpp::str_atom("ok"));
	}
	catch (nifpp::badarg)
	{
	}
	return enif_make_badarg(env);
}
//void bgfx::reset(uint32_t _width, uint32_t _height, uint32_t _flags)
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

// void bgfx::setViewClear(uint8_t _id, uint16_t _flags, uint32_t _rgba, float _depth, uint8_t _stencil)
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
		bgfx::setViewClear(_id, _flags, _rgba, static_cast< float >(_depth), _stencil);
		return nifpp::make(env, nifpp::str_atom("ok"));
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
		using namespace entry;
		auto contex = std::make_unique< entry::Context >();
		char fakeParam[] = "erl";
		char* fakeargv[] = {fakeParam, NULL};
		int fakeargc = 1;
		//entry::initMain(fakeargc, fakeargv);
		contex->run([]() {
			uint32_t width = 1280;
			uint32_t height = 720;
			uint32_t debug = BGFX_DEBUG_TEXT;
			uint32_t reset = BGFX_RESET_VSYNC;

			bgfx::init();
			bgfx::reset(width, height, reset);

			// Enable debug text.
			bgfx::setDebug(debug);

			// Set view 0 clear state.
			bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x303030ff, 1.0f, 0);

			while (true)//!entry::processEvents(width, height, debug, reset))
			{
				// Set view 0 default viewport.
				bgfx::setViewRect(0, 0, 0, width, height);

				// This dummy draw call is here to make sure that view 0 is cleared
				// if no other draw calls are submitted to view 0.
				bgfx::touch(0);

				// Use debug font to print information about this example.
				bgfx::dbgTextClear();
				bgfx::dbgTextImage(bx::uint16_max(width / 2 / 8, 20) - 20, bx::uint16_max(height / 2 / 16, 6) - 6, 40, 12, s_logo, 160);
				bgfx::dbgTextPrintf(0, 1, 0x4f, "bgfx/examples/lib_bgfx");
				bgfx::dbgTextPrintf(0, 2, 0x6f, "Description: Initialization and debug text in Elixir and SDL2.");

				// Advance to next frame. Rendering thread will be kicked to
				// process submitted rendering primitives.
				bgfx::frame();
			}
			bgfx::shutdown();
			return 0;
		}, fakeargc, fakeargv);
		//cmdInit();
		//cmdAdd("mouselock", cmdMouseLock);
		//cmdAdd("graphics", cmdGraphics);
		//cmdAdd("exit", cmdExit);

		//inputInit();
		//inputAddBindings("bindings", s_bindings);

		//entry::WindowHandle defaultWindow = { 0 };
		//entry::setWindowTitle(defaultWindow, bx::baseName(_argv[0]));
		//
		/*uint32_t width = 1280;
		uint32_t height = 720;
		uint32_t debug = BGFX_DEBUG_TEXT;
		uint32_t reset = BGFX_RESET_VSYNC;
		auto active = true;*/
		/*SDL_Event event;
		while (active)
		{
			bgfx::setViewRect(0, 0, 0, width, height);
			bgfx::dbgTextPrintf(0, 1, 0x4f, std::to_string(static_cast<int>(bgfx::getRendererType())).c_str());
						
			// This dummy draw call is here to make sure that view 0 is cleared
			// if no other draw calls are submitted to view 0.
			bgfx::touch(0);

			// Use debug font to print information about this example.
			bgfx::dbgTextClear();
			bgfx::dbgTextImage(bx::uint16_max(width / 2 / 8, 20) - 20
				, bx::uint16_max(height / 2 / 16, 6) - 6
				, 40
				, 12
				, s_logo
				, 160
				);
			bgfx::dbgTextPrintf(0, 1, 0x4f, "bgfx/examples/00-helloworld");
			bgfx::dbgTextPrintf(0, 2, 0x6f, "Description: Initialization and debug text.");

			// Advance to next frame. Rendering thread will be kicked to
			// process submitted rendering primitives.
			bgfx::frame();
		}
		bgfx::shutdown();*/
		/*
		inputRemoveBindings("bindings");
		inputShutdown();

		cmdShutdown();
		*/
		return nifpp::make(env, nifpp::str_atom("ok"));
	}
	catch (nifpp::badarg)
	{
	}
	return enif_make_badarg(env);
}

static ErlNifFunc nif_funcs[] = {
	{"_bgfx_init", 5, _bgfx_init, ERL_NIF_DIRTY_JOB_CPU_BOUND},
	{"_bgfx_reset", 3, _bgfx_reset, ERL_NIF_DIRTY_JOB_CPU_BOUND},
	{"_bgfx_set_view_clear", 5, _bgfx_set_view_clear, ERL_NIF_DIRTY_JOB_CPU_BOUND},
	{"_bgfx_run", 0, _bgfx_run, ERL_NIF_DIRTY_JOB_CPU_BOUND},
};

ERL_NIF_INIT(Elixir.Bgfx.Nif, nif_funcs, load, NULL, NULL, NULL)
} // extern C