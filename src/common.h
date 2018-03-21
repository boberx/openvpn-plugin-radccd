#ifndef COMMON_H
#define COMMON_H

#include <string.h>
#include <radcli/radcli.h>
#include <openvpn/openvpn-plugin.h>

struct plugin_context {
	plugin_log_t log;
};

extern const char* logprefix;

extern rc_handle* rh;

const char* get_env ( const char* name, const char* envp[] );

#endif
