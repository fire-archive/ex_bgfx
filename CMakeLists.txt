cmake_minimum_required(VERSION 2.8)

# The project name will be the name of the binary executable created. The
# test code will also be named ${PROJECT_NAME}_test.
set(PROJECT_NAME lib_bgfx)
project(${PROJECT_NAME})

#set(${PROJECT_NAME}_VERSION_MAJOR 0)
#set(${PROJECT_NAME}_VERSION_MINOR 1)

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
