#include "openvpn-plugin-radccd.h"

OPENVPN_EXPORT int openvpn_plugin_open_v3 (
	__attribute__((unused)) const int version,
	struct openvpn_plugin_args_open_in const* args,
	struct openvpn_plugin_args_open_return* rv )
{
	int rc = OPENVPN_PLUGIN_FUNC_ERROR;

	struct plugin_context* plugin = calloc ( 1, sizeof ( *plugin ) );

	plugin->log = args->callbacks->plugin_log;

	rv->type_mask = OPENVPN_PLUGIN_MASK ( OPENVPN_PLUGIN_AUTH_USER_PASS_VERIFY );

	rv->handle = (void*) plugin;

	if ( radcli_init ( args ) != 0 )
		plugin->log ( PLOG_ERR, logprefix, "radcli_init failed" );
	else
		rc = OPENVPN_PLUGIN_FUNC_SUCCESS;

	return rc;
}

OPENVPN_EXPORT void openvpn_plugin_close_v1 ( openvpn_plugin_handle_t handle )
{
	struct plugin_context* plugin = (struct plugin_context*) handle;

	radcli_destroy ();

	free ( plugin );
}

OPENVPN_EXPORT int openvpn_plugin_func_v3 (
	__attribute__((unused)) const int version,
	struct openvpn_plugin_args_func_in const* args,
	__attribute__((unused)) struct openvpn_plugin_args_func_return* rv )
{
	int rc = OPENVPN_PLUGIN_FUNC_ERROR;

	struct plugin_context* plugin = (struct plugin_context*) args->handle;

	const char* username = get_env ( "username", args->envp );
	const char* password = get_env ( "password", args->envp );

	if ( username == NULL )
		plugin->log ( PLOG_ERR, logprefix, "envp[username] is not set" );
	else if ( password == NULL )
		plugin->log ( PLOG_ERR, logprefix, "envp[password] is not set" );
	else switch ( args->type )
	{
		default:
			plugin->log ( PLOG_ERR, logprefix, "openvpn_plugin_func_v3: unknown type" );
			break;
		case OPENVPN_PLUGIN_AUTH_USER_PASS_VERIFY:
			{
				VALUE_PAIR* received = NULL;

				if ( radcli_avpair_get ( args, username, password, &received ) != 0 )
					plugin->log ( PLOG_ERR, logprefix, "openvpn_plugin_func_v3: radcli_avpair_get failed" );
				else if ( createccdfile ( username, received, args ) != 0 )
					plugin->log ( PLOG_ERR, logprefix, "openvpn_plugin_func_v3: createccdfile failed" );
				else
					rc = OPENVPN_PLUGIN_FUNC_SUCCESS;
			}
			break;
	}

	return rc;
}
