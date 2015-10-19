#ifdef __GNUC__
  #define UNUSED(x) UNUSED_ ## x __attribute__((__unused__))
#else
  #define UNUSED(x) UNUSED_ ## x
#endif

#include "nifpp.h"
#include <stdio.h>
#ifndef _WIN32
	#include <strings.h>
	#include <unistd.h>
#else
    #include <string.h>	
#endif

#include "bgfx/c99/bgfx.h"

using namespace nifpp;

extern "C" {
static ERL_NIF_TERM _hello(ErlNifEnv *env, int UNUSED(arc),
                   const ERL_NIF_TERM UNUSED(argv[])) {
  return make(env, 0.0);
}

static ErlNifFunc nif_funcs[] = {{"_hello", 0, _hello, 0}};

ERL_NIF_INIT(Elixir.ExBgfx.Nif, nif_funcs, NULL, NULL, NULL, NULL)
} // extern C