cmake_minimum_required(VERSION 2.8)

set(PROJECT_NAME lib_bgfx)
project(${PROJECT_NAME})

set(MAIN_SOURCE
    c_src/bgfx_nif.cpp
    )

add_library(${PROJECT_NAME} SHARED ${MAIN_SOURCE})

set(ERL_ARG erl -eval "io:format(\"~s~n\", [lists:concat([\"ERTS_INCLUDE_PATH=\", code:root_dir(), \"/erts-\", erlang:system_info(version), \"/include\"])])" -s init stop -noshell)

execute_process(COMMAND erl ARGS ${ERL_ARG} OUTPUT_VARIABLE ERLANG_PATH)
string(REGEX REPLACE "ERTS_INCLUDE_PATH=" "" ERLANG_PATH ${ERLANG_PATH})
message(STATUS "Detecting Erlang Path -- ${ERLANG_PATH}")
include_directories(dep/bgfx/include dep/nifpp ${ERLANG_PATH})
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION priv)
