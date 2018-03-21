#ifndef RADIUS_H
#define RADIUS_H

#include <string.h>
#include <radcli/radcli.h>

#include "common.h"

int radcli_init ( struct openvpn_plugin_args_open_in const* args );

void radcli_destroy ();

int radcli_avpair_get ( struct openvpn_plugin_args_func_in const* args, const char* const, const char* const, VALUE_PAIR** );

#endif
