#include "ccdfile.h"

int createccdfile (
	const char* const filename,
	VALUE_PAIR* const received,
	struct openvpn_plugin_args_func_in const* args )
{
	int rc = 1;

	FILE* ccd_f = NULL;

	char ccd_path[1024] = { 0 };
	char avp_path[1024] = { 0 };

	struct plugin_context* plugin = (struct plugin_context*) args->handle;

	snprintf ( ccd_path, sizeof ( ccd_path ), "/etc/openvpn/ccd/%s", filename );
	snprintf ( avp_path, sizeof ( avp_path ), "/etc/openvpn/ccd/%s.avp", filename );

	ccd_f = fopen ( ccd_path, "wb" );

	if  ( ccd_f == 0 )
		plugin->log ( PLOG_ERR, logprefix, "createccdfile: fopen: [%s]", ccd_path );
	else
	{
		FILE* avp_f = NULL;

		avp_f = fopen ( avp_path, "wb" );

		if  ( avp_f == 0)
			plugin->log ( PLOG_ERR, logprefix, "createccdfile: fopen: [%s]", avp_path );
		else
		{
			VALUE_PAIR* vp = received;

			char name[128];
			char value[128];

			while ( vp != NULL )
			{
				if ( rc_avpair_tostr ( rh, vp, name, sizeof(name), value, sizeof(value) ) == 0 )
				{
					char str[1024] = { 0 };
					int ccd_snr = 0;
					int avp_snr = 0;
					const char* lnm_str = 0;

					plugin->log ( PLOG_DEBUG, logprefix, "createccdfile: vp->name: %s vp->attribute: %d\n", vp->name, vp->attribute );

					switch ( vp->attribute )
					{
						default:
							avp_snr = snprintf ( str, sizeof ( str ), "%s \"%s\"\n", name, value );
							break;
						case PW_FRAMED_IP_ADDRESS:
							if ( ( lnm_str = get_env ( "ifconfig_netmask", args->envp ) ) == 0 )
								plugin->log ( PLOG_WARN, logprefix, "envp[ifconfig_netmask] is not set" );
							else
								ccd_snr = snprintf ( str, sizeof ( str ), "ifconfig-push %s %s\n", value, lnm_str );
							break;
						case 20381725: // MS-Secondary-DNS-Server
						case 20381724: // MS-Primary-DNS-Server
							ccd_snr = snprintf ( str, sizeof ( str ), "push \"dhcp-option DNS %s\"\n", value );
							break;
					}

					if ( ccd_snr <= 0 )
					{
						plugin->log ( PLOG_DEBUG, logprefix, "ccd_snr <= 0" );

						if ( avp_snr <= 0 )
							plugin->log ( PLOG_DEBUG, logprefix, "avp_snr <= 0" );
						else if ( fwrite ( str, avp_snr, 1, avp_f ) != 1 )
						{
							plugin->log ( PLOG_ERR, logprefix, "avp_f: fwrite failed" );
							break;
						}
					}
					else if ( fwrite ( str, ccd_snr, 1, ccd_f ) != 1 )
					{
						plugin->log ( PLOG_ERR, logprefix, "ccd_f: fwrite failed" );
						break;
					}
				}
				else
					plugin->log ( PLOG_DEBUG, logprefix, "rc_avpair_tostr != 0" );

				vp = vp->next;

				if ( vp == NULL ) rc = 0;
			}

			fclose ( avp_f );
		}

		fclose ( ccd_f );
	}

	return rc;
}
