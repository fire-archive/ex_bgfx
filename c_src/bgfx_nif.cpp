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

//#include "bx/platform.h"
#include "bgfx/bgfx.h"

using namespace nifpp;

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
		//auto _type = nifpp::construct_resource< bgfx::RendererType::Enum >(
		//	nifpp::get< bgfx::RendererType::Enum >(env, argv[0]));
		auto _type = nifpp::get< int >(env, argv[0]);
		auto _vendor_id = nifpp::get< unsigned int >(env, argv[1]);
		auto _device_id = nifpp::get< unsigned int >(env, argv[2]);
		auto _callback = nifpp::construct_resource< bgfx::CallbackI* >(
			nifpp::get< bgfx::CallbackI* >(env, argv[3]));
		auto _reallocator = nifpp::construct_resource< bx::ReallocatorI* >(
			nifpp::get< bx::ReallocatorI* >(env, argv[4]));
		return make(env, bgfx::init(static_cast< bgfx::RendererType::Enum >(_type), _vendor_id, _device_id, *_callback,
									*_reallocator));
	}
	catch (nifpp::badarg)
	{
	}
	return enif_make_badarg(env);
}

static ErlNifFunc nif_funcs[] = {
	{"_hello", 0, _hello, 0},
	{ "_bgfx_init", 0, _bgfx_init, ERL_NIF_DIRTY_JOB_CPU_BOUND}
};

ERL_NIF_INIT(Elixir.ExBgfx.Nif, nif_funcs, load, NULL, NULL, NULL)
} // extern C