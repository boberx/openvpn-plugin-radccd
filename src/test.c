#include <stdio.h>

#include "radius.h"
#include "ccdfile.h"

int main ()
{
	printf ( "start\n" );

	const char* authserver = "127.0.0.1::testing123";

	if ( radcli_init ( authserver ) != 0 )
		printf ( "error: radcli_init\n" );
	else
	{
		printf ( "succes: radcli_init\n" );

		const char* username = "testusera";
		const char* password = "test";
		VALUE_PAIR* received = NULL;
		const char* envp[5] = { 0 };

		envp[0] = "ifconfig_netmask=255.255.255.0";
		envp[1] = 0;

		if ( radcli_avpair_get ( username, password, &received ) != 0 )
			printf ( "error: radcli_avpair_get\n" );
		else
		{
			printf ( "%ssucces: radcli_avpair_get\n", logprefix );

			if ( createccdfile ( username, received, envp ) != 0 )
				printf ( "%serror: createccdfile\n", logprefix );
			else
				printf ( "%ssucces: createccdfile\n", logprefix );
		}
	}

	return 0;
}
