

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
    *                                                                 *
    *   Outbridge Winamp Plugin 1.0                                   *
    *   Copyright © 2005 Sebastian Pipping <webmaster@hartwork.org>   *
    *                                                                 *
    *   --> http://www.hartwork.org                                   *
    *                                                                 *
    *                                                                 *
    *   This source code is released under LGPL.                      *
    *   See LGPL.txt for details.                        2005-08-28   *
    *                                                                 *
    \* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


#ifndef CONSOLE_H
#define CONSOLE_H


#include "Global.h"


namespace Console
{
	BOOL Create( char * szTitle );
	BOOL Destroy();
	
	BOOL Append( char * szText );
}


#endif // CONSOLE_H
