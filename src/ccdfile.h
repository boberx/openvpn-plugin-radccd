#ifndef CCDFILE_H
#define CCDFILE_H

#include "radius.h"
#include "common.h"

/*!
	*\brief эта функция должна вызывать, если запорс к радиусу прошёл, так как она перезатрёт файлы настроек
*/
int createccdfile ( const char* const, VALUE_PAIR* const, struct openvpn_plugin_args_func_in const* );

#endif
