

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


#include "Console.h"
#include <time.h>


#define MENU_CONFIG           1
#define MENU_ABOUT            2
#define MENU_INIT             3
#define MENU_QUIT             4
#define MENU_OPEN             5
#define MENU_CLOSE            6
#define MENU_WRITE            7
#define MENU_CANWRITE         8
#define MENU_ISPLAYING        9
#define MENU_PAUSE           10
#define MENU_SETVOLUME       11
#define MENU_SETPAN          12
#define MENU_FLUSH           13
#define MENU_GETWRITTENTIME  14
#define MENU_GETOUTPUTTIME   15



HWND WindowConsole = NULL;
HMENU pupup_menu = NULL;
UINT uNext = 0;

const UINT uMaxEntries = 10000;


WNDPROC WndprocConsoleBackup = NULL;
LRESULT CALLBACK WndprocConsole( HWND hwnd, UINT message, WPARAM wp, LPARAM lp );


BOOL Console::Create( char * szTitle )
{
	WindowConsole = CreateWindowEx(
		WS_EX_TOOLWINDOW |                  // DWORD dwExStyle,
			WS_EX_CLIENTEDGE |              //
			WS_EX_APPWINDOW,                //
		"LISTBOX",                          // LPCTSTR lpClassName
		szTitle,                            // LPCTSTR lpWindowName
		WS_VSCROLL |                        // DWORD dwStyle
			LBS_DISABLENOSCROLL |           //
			LBS_EXTENDEDSEL |               //
			LBS_HASSTRINGS |                //
			LBS_NOTIFY |                    //
			LBS_NOINTEGRALHEIGHT |          //
			WS_VISIBLE |                    //
			WS_POPUP |                      //
			WS_BORDER |                     //
			WS_CAPTION |                    //
			WS_THICKFRAME |                 //
			WS_MAXIMIZEBOX ,                //
		xNormalPosition,                    // int x
		yNormalPosition,                    // int y
		cxNormalPosition,                   // int nWidth
		cyNormalPosition,                   // int nHeight
		NULL,                               // HWND hWndParent
		NULL,                               // HMENU hMenu
		g_hMasterInstance,                  // HINSTANCE hInstance
		NULL                                // LPVOID lpParam
	);
	
	if( !WindowConsole ) return FALSE;

	RECT rcNormalPosition;
	GetWindowRect( WindowConsole, &rcNormalPosition );

	// Window placement
	POINT ptMinPosition = { 0, 0 };
	POINT ptMaxPosition = { xMaxPosition, yMaxPosition };

	WINDOWPLACEMENT placement = {
		sizeof( WINDOWPLACEMENT ),  // UINT length
		0,                          // UINT flags
		( UINT )iShowCmd,           // UINT showCmd
		ptMinPosition,              // POINT ptMinPosition
		ptMaxPosition,              // POINT ptMaxPosition
		rcNormalPosition            // RECT rcNormalPosition
	};

	SetWindowPlacement( WindowConsole, &placement );
	
	
	// So we can always fully read the last line...
	SendMessage( WindowConsole, LB_INSERTSTRING, 0, ( LPARAM )"" );
	
	// Default font
	SendMessage( WindowConsole, WM_SETFONT, ( WPARAM )GetStockObject( DEFAULT_GUI_FONT ), ( LPARAM )TRUE );
	
	
	// Exchange window procedure
	WndprocConsoleBackup = ( WNDPROC )GetWindowLong( WindowConsole, GWL_WNDPROC );
	if( WndprocConsoleBackup != NULL )
	{
		SetWindowLong( WindowConsole, GWL_WNDPROC, ( LONG )WndprocConsole );
	}


	// Create menu
	pupup_menu = CreatePopupMenu();
	AppendMenu( pupup_menu, MF_STRING | ( bLogConfig          ? MF_CHECKED : MF_UNCHECKED ), MENU_CONFIG,          "Config" );
	AppendMenu( pupup_menu, MF_STRING | ( bLogAbout           ? MF_CHECKED : MF_UNCHECKED ), MENU_ABOUT,           "About" );
	AppendMenu( pupup_menu, MF_STRING | ( bLogInit            ? MF_CHECKED : MF_UNCHECKED ), MENU_INIT,            "Init" );
	AppendMenu( pupup_menu, MF_STRING | ( bLogQuit            ? MF_CHECKED : MF_UNCHECKED ), MENU_QUIT,            "Quit" );
	AppendMenu( pupup_menu, MF_STRING | ( bLogOpen            ? MF_CHECKED : MF_UNCHECKED ), MENU_OPEN,            "Open" );
	AppendMenu( pupup_menu, MF_STRING | ( bLogClose           ? MF_CHECKED : MF_UNCHECKED ), MENU_CLOSE,           "Close" );
	AppendMenu( pupup_menu, MF_STRING | ( bLogWrite           ? MF_CHECKED : MF_UNCHECKED ), MENU_WRITE,           "Write" );
	AppendMenu( pupup_menu, MF_STRING | ( bLogCanWrite        ? MF_CHECKED : MF_UNCHECKED ), MENU_CANWRITE,        "CanWrite" );
	AppendMenu( pupup_menu, MF_STRING | ( bLogIsPlaying       ? MF_CHECKED : MF_UNCHECKED ), MENU_ISPLAYING,       "IsPlaying" );
	AppendMenu( pupup_menu, MF_STRING | ( bLogPause           ? MF_CHECKED : MF_UNCHECKED ), MENU_PAUSE,           "Pause" );
	AppendMenu( pupup_menu, MF_STRING | ( bLogSetVolume       ? MF_CHECKED : MF_UNCHECKED ), MENU_SETVOLUME,       "SetVolume" );
	AppendMenu( pupup_menu, MF_STRING | ( bLogSetPan          ? MF_CHECKED : MF_UNCHECKED ), MENU_SETPAN,          "SetPan" );
	AppendMenu( pupup_menu, MF_STRING | ( bLogFlush           ? MF_CHECKED : MF_UNCHECKED ), MENU_FLUSH,           "Flush" );
	AppendMenu( pupup_menu, MF_STRING | ( bLogGetWrittenTime  ? MF_CHECKED : MF_UNCHECKED ), MENU_GETWRITTENTIME,  "GetWrittenTime" );
	AppendMenu( pupup_menu, MF_STRING | ( bLogGetOutputTime   ? MF_CHECKED : MF_UNCHECKED ), MENU_GETOUTPUTTIME,   "GetOutputTime" );


	return TRUE;
}


BOOL Console::Destroy()
{
	if( !WindowConsole ) return FALSE;

	// Window placement
	WINDOWPLACEMENT placement = { sizeof( WINDOWPLACEMENT ), };
	GetWindowPlacement( WindowConsole, &placement );

	iShowCmd          = placement.showCmd;
	xMaxPosition      = placement.ptMaxPosition.x;
	yMaxPosition      = placement.ptMaxPosition.y;
	xNormalPosition   = placement.rcNormalPosition.left;
	yNormalPosition   = placement.rcNormalPosition.top;
	cxNormalPosition  = placement.rcNormalPosition.right - placement.rcNormalPosition.left;
	cyNormalPosition  = placement.rcNormalPosition.bottom - placement.rcNormalPosition.top;


	DestroyWindow( WindowConsole );
	DestroyMenu( pupup_menu );

	return TRUE;
}


BOOL Console::Append( char * szText )
{
	if( !WindowConsole ) return FALSE;
	if( uNext > uMaxEntries - 1 )
	{
		SendMessage(
			WindowConsole,
			LB_DELETESTRING,
			0,
			0
		);
		uNext--;
	}
	
	const UINT uTextLen = strlen( szText );
	char * szBuffer = new char[ 11 + uTextLen + 1 ];
	time_t now_time_t = time( NULL );
	struct tm * now_tm = localtime( &now_time_t );
	strftime( szBuffer, 12, "%H:%M:%S   ", now_tm );
	memcpy( szBuffer + 11, szText, uTextLen );
	szBuffer[ 11 + uTextLen ] = TEXT( '\0' );
	
	SendMessage( WindowConsole, LB_INSERTSTRING, uNext, ( LPARAM )szBuffer );
	SendMessage( WindowConsole, LB_SETSEL, FALSE, -1 );
	SendMessage( WindowConsole, LB_SETSEL, TRUE, uNext );
	SendMessage( WindowConsole, LB_SETTOPINDEX, uNext, 0 );
	uNext++;
	
	delete [] szBuffer;
	
	return TRUE;
}


void PopupMenu()
{
	if( !pupup_menu ) return;

	POINT p;
	GetCursorPos( &p );

	BOOL iIndex = TrackPopupMenu(
		pupup_menu,              // HMENU hMenu
		TPM_LEFTALIGN |          // UINT uFlags
			TPM_TOPALIGN |       // .
			TPM_NONOTIFY |       // .
			TPM_RETURNCMD |      // .
			TPM_RIGHTBUTTON,     // .
		p.x,                     // int x
		p.y,                     // int y
		0,                       // int nReserved
		WindowConsole,           // HWND hWnd
		NULL                     // HWND prcRect
	);


	if( iIndex )
	{
		BOOL bChecked = MF_CHECKED & GetMenuState( pupup_menu, iIndex, MF_BYCOMMAND );
		CheckMenuItem( pupup_menu, iIndex, bChecked ? MF_UNCHECKED : MF_CHECKED );
		
		switch( iIndex )
		{
		case MENU_CONFIG:          bLogConfig          = !bLogConfig;          break;
		case MENU_ABOUT:           bLogAbout           = !bLogAbout;           break;
		case MENU_INIT:            bLogInit            = !bLogInit;            break;
		case MENU_QUIT:            bLogQuit            = !bLogQuit;            break;
		case MENU_OPEN:            bLogOpen            = !bLogOpen;            break;
		case MENU_CLOSE:           bLogClose           = !bLogClose;           break;
		case MENU_WRITE:           bLogWrite           = !bLogWrite;           break;
		case MENU_CANWRITE:        bLogCanWrite        = !bLogCanWrite;        break;
		case MENU_ISPLAYING:       bLogIsPlaying       = !bLogIsPlaying;       break;
		case MENU_PAUSE:           bLogPause           = !bLogPause;           break;
		case MENU_SETVOLUME:       bLogSetVolume       = !bLogSetVolume;       break;
		case MENU_SETPAN:          bLogSetPan          = !bLogSetPan;          break;
		case MENU_FLUSH:           bLogFlush           = !bLogFlush;           break;
		case MENU_GETWRITTENTIME:  bLogGetWrittenTime  = !bLogGetWrittenTime;  break;
		case MENU_GETOUTPUTTIME:   bLogGetOutputTime   = !bLogGetOutputTime;   break;
		}
	}	
}


LRESULT CALLBACK WndprocConsole( HWND hwnd, UINT message, WPARAM wp, LPARAM lp )
{
	static BOOL bContextOpen = FALSE;

	switch( message )
	{
	case WM_CONTEXTMENU:
		if( bContextOpen ) break;
		bContextOpen = TRUE;

		PopupMenu();

		bContextOpen = FALSE;
		break;

	case WM_GETMINMAXINFO:
		{
			MINMAXINFO * mmi = ( MINMAXINFO * )lp;
			mmi->ptMinTrackSize.x = 300;
			mmi->ptMinTrackSize.y = 200;
			return 0;
		}
		
	case WM_SYSCOMMAND:
		if( wp == SC_CLOSE )
		{
			return 0;
		}
		break;
		
	}
	return CallWindowProc( WndprocConsoleBackup, hwnd, message, wp, lp );
}
