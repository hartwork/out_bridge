

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


#ifndef GLOBAL_H
#define GLOBAL_H


#define WIN32_LEAN_AND_MEAN
#include <Windows.h>


extern HINSTANCE g_hMasterInstance;

extern bool bLogConfig;
extern bool bLogAbout;
extern bool bLogInit;
extern bool bLogQuit;
extern bool bLogOpen;
extern bool bLogClose;
extern bool bLogWrite;
extern bool bLogCanWrite;
extern bool bLogIsPlaying;
extern bool bLogPause;
extern bool bLogSetVolume;
extern bool bLogSetPan;
extern bool bLogFlush;
extern bool bLogGetWrittenTime;
extern bool bLogGetOutputTime;

extern int iShowCmd;
extern int xMaxPosition;
extern int yMaxPosition;
extern int xNormalPosition;
extern int yNormalPosition;
extern int cxNormalPosition;
extern int cyNormalPosition;



#endif // GLOBAL_H
