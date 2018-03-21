#include "common.h"

const char* logprefix = "radccd";

const char* get_env ( const char* name, const char* envp[] )
{
	if ( envp )
	{
		int i;
		const int namelen = strlen ( name );

		for ( i = 0; envp[i]; ++ i )
		{
			if ( ! strncmp ( envp[i], name, namelen ) )
			{
				const char* cp = envp[i] + namelen;

				if ( *cp == '=' )
					return cp + 1;
			}
		}
	}

	return NULL;
}
