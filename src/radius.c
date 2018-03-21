#include "radius.h"

rc_handle* rh = NULL;

int radcli_init ( struct openvpn_plugin_args_open_in const* args )
{
	int rc = 1;

	if ( args->argv[1] == 0 )
		args->callbacks->plugin_log ( PLOG_ERR, logprefix, "libradcli configuration file is not set" );
	else if ( ( rh = rc_read_config ( args->argv[1] ) ) == 0 )
		args->callbacks->plugin_log ( PLOG_ERR, logprefix, "failed to initialze configuration" );
	else if ( rc_read_dictionary ( rh, rc_conf_str ( rh, "dictionary" ) ) != 0 )
		args->callbacks->plugin_log ( PLOG_ERR, logprefix, "failed to initialize radius dictionary" );
	else
		rc = 0;

	return rc;
}

void radcli_destroy ()
{
	rc_destroy ( rh );
}

int radcli_avpair_get (
	struct openvpn_plugin_args_func_in const* args,
	const char* const username,
	const char* const password,
	VALUE_PAIR** received )
{
	int rc = 1;
	int result;
	VALUE_PAIR* send = NULL;
	uint32_t service = PW_AUTHENTICATE_ONLY;
	char usernm[128];
	char passwd[AUTH_PASS_LEN + 1];

	struct plugin_context* plugin = (struct plugin_context*) args->handle;

	strcpy ( usernm, username );
	strcpy ( passwd, password );

	if ( rc_avpair_add ( rh, &send, PW_USER_NAME, usernm, -1, 0 ) == NULL )
		plugin->log ( PLOG_ERR, logprefix, "rc_avpair_add failed" );
	else if ( rc_avpair_add ( rh, &send, PW_USER_PASSWORD, passwd, -1, 0 ) == NULL )
		plugin->log ( PLOG_ERR, logprefix, "rc_avpair_add failed" );
	else if ( rc_avpair_add ( rh, &send, PW_SERVICE_TYPE, &service, -1, 0) == NULL )
		plugin->log ( PLOG_ERR, logprefix, "rc_avpair_add failed" );
	else if ( ( result = rc_auth ( rh, 0, send, received, NULL ) ) != OK_RC )
		plugin->log ( PLOG_ERR, logprefix, "radius authentication failure: rc: [%i] username: [%s]", result, usernm );
	else
	{
		plugin->log ( PLOG_NOTE, logprefix, "radius authentication ok: username: [%s]", usernm );
		rc = 0;
	}

	rc_avpair_free ( send );

	return rc;
}
