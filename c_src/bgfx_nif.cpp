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

#include <memory>
#include "SDL.h"
#include "SDL_syswm.h"
#include "entry/entry_p.h"
#include "bgfx/bgfxplatform.h"
#include "bgfx/bgfx.h"

using namespace nifpp;
using namespace std;

extern "C" {
static int load(ErlNifEnv* env, void** priv, ERL_NIF_TERM load_info)
{
	//nifpp::register_resource< bgfx::RendererType::Enum >(env, nullptr,
	//														 "renderer_type");
	nifpp::register_resource< bgfx::CallbackI* >(env, nullptr,
												 "callback_interface_ptr");
	nifpp::register_resource< bx::ReallocatorI* >(env, nullptr,
												  "reallocator_interface_ptr");
	return 0;
}

static ERL_NIF_TERM _hello(ErlNifEnv* env, int UNUSED(arc),
						   const ERL_NIF_TERM UNUSED(argv[]))
{
	return make(env, 0.0);
}

static ERL_NIF_TERM _bgfx_init(ErlNifEnv* env, int argc,
							   const ERL_NIF_TERM argv[])
{
	try
	{
		SDL_Init(0
			| SDL_INIT_VIDEO
			| SDL_INIT_GAMECONTROLLER
			);

		auto window = SDL_CreateWindow("bgfx"
			, SDL_WINDOWPOS_UNDEFINED
			, SDL_WINDOWPOS_UNDEFINED
			, ENTRY_DEFAULT_WIDTH
			, ENTRY_DEFAULT_HEIGHT
			, SDL_WINDOW_SHOWN
			| SDL_WINDOW_RESIZABLE
			);

		//m_flags[0] = 0
		//	| ENTRY_WINDOW_FLAG_ASPECT_RATIO
		//	| ENTRY_WINDOW_FLAG_FRAME
		//	;

		//s_userEventStart = SDL_RegisterEvents(7);
		bgfx::sdlSetWindow(window);
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
		auto output = nifpp::make(env, bgfx::init(static_cast< bgfx::RendererType::Enum >(_type), _vendor_id, _device_id, nullptr,
			nullptr));
		return output;
	}
	catch (nifpp::badarg)
	{
	}
	return enif_make_badarg(env);
}

static ErlNifFunc nif_funcs[] = {
	{"_hello", 0, _hello, 0},
	{ "_bgfx_init", 5, _bgfx_init, ERL_NIF_DIRTY_JOB_CPU_BOUND}
};

ERL_NIF_INIT(Elixir.ExBgfx.Nif, nif_funcs, load, NULL, NULL, NULL)
} // extern C