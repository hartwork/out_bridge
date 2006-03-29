

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
    *                                                                 *
    *   Outbridge Winamp Plugin 2.2                                   *
    *   Copyright © 2006 Sebastian Pipping <webmaster@hartwork.org>   *
    *                                                                 *
    *   --> http://www.hartwork.org                                   *
    *                                                                 *
    *                                                                 *
    *   This source code is released under LGPL.                      *
    *   See LGPL.txt for details.                        2006-03-29   *
    *                                                                 *
    \* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


#include "Global.h"
#include "Console.h"
#include "DevilConfig.h"
#include "Winamp/Out.h"
#include <stdio.h>


typedef Out_Module * ( * WINAMPGETOUTMODULE )();


HINSTANCE g_hMasterInstance = NULL; // extern
DEVIL_CONFIG * g_pCONFIG = NULL;

HINSTANCE g_hSlaveInstance = NULL;
Out_Module * g_pModSlave = NULL;



bool bLogConfig; // extern
bool bLogAbout; // extern
bool bLogInit; // extern
bool bLogQuit; // extern
bool bLogOpen; // extern
bool bLogClose; // extern
bool bLogWrite; // extern
bool bLogCanWrite; // extern
bool bLogIsPlaying; // extern
bool bLogPause; // extern
bool bLogSetVolume; // extern
bool bLogSetPan; // extern
bool bLogFlush; // extern
bool bLogGetWrittenTime; // extern
bool bLogGetOutputTime; // extern

int iShowCmd; // extern
int xMaxPosition; // extern
int yMaxPosition; // extern
int xNormalPosition; // extern
int yNormalPosition; // extern
int cxNormalPosition; // extern
int cyNormalPosition; // extern


UINT_PTR hMainHandleTimer = 0;


void Config( HWND p );
void About( HWND p );
void Init();
void Quit();
int Open( int sr, int nch, int bps, int bufferlenms, int prebufferms );
void Close();
int Write( char * data, int size );
int CanWrite();
int IsPlaying();
int Pause( int new_state );
void SetVolume( int v );
void SetPan( int p );
void Flush( int pos );
int GetWrittenTime();
int GetOutputTime();



////////////////////////////////////////////////////////////////////////////////
//  GetCurrentModule
////////////////////////////////////////////////////////////////////////////////
HMODULE GetCurrentModule()
{
	MEMORY_BASIC_INFORMATION mbi;
	static int dummy;
	VirtualQuery( &dummy, &mbi, sizeof( MEMORY_BASIC_INFORMATION ) );
	return ( HMODULE )mbi.AllocationBase;
}



////////////////////////////////////////////////////////////////////////////////
//  ReadConfig
////////////////////////////////////////////////////////////////////////////////
void ReadConfig()
{
	if( !g_pCONFIG ) return;

	int res;
	g_pCONFIG->Read( "bLogConfig",          &res, 1 ); bLogConfig          = ( bool )res;
	g_pCONFIG->Read( "bLogAbout",           &res, 1 ); bLogAbout           = ( bool )res;
	g_pCONFIG->Read( "bLogInit",            &res, 1 ); bLogInit            = ( bool )res;
	g_pCONFIG->Read( "bLogQuit",            &res, 1 ); bLogQuit            = ( bool )res;
	g_pCONFIG->Read( "bLogOpen",            &res, 1 ); bLogOpen            = ( bool )res;
	g_pCONFIG->Read( "bLogClose",           &res, 1 ); bLogClose           = ( bool )res;
	g_pCONFIG->Read( "bLogWrite",           &res, 0 ); bLogWrite           = ( bool )res;
	g_pCONFIG->Read( "bLogCanWrite",        &res, 0 ); bLogCanWrite        = ( bool )res;
	g_pCONFIG->Read( "bLogIsPlaying",       &res, 0 ); bLogIsPlaying       = ( bool )res;
	g_pCONFIG->Read( "bLogPause",           &res, 1 ); bLogPause           = ( bool )res;
	g_pCONFIG->Read( "bLogSetVolume",       &res, 1 ); bLogSetVolume       = ( bool )res;
	g_pCONFIG->Read( "bLogSetPan",          &res, 1 ); bLogSetPan          = ( bool )res;
	g_pCONFIG->Read( "bLogFlush",           &res, 1 ); bLogFlush           = ( bool )res;
	g_pCONFIG->Read( "bLogGetWrittenTime",  &res, 0 ); bLogGetWrittenTime  = ( bool )res;
	g_pCONFIG->Read( "bLogGetOutputTime",   &res, 0 ); bLogGetOutputTime   = ( bool )res;
	
	g_pCONFIG->Read( "iShowCmd",          &iShowCmd,          SW_SHOWNORMAL );
	g_pCONFIG->Read( "xMaxPosition",      &xMaxPosition,      0 );
	g_pCONFIG->Read( "yMaxPosition",      &yMaxPosition,      0 );
	g_pCONFIG->Read( "xNormalPosition",   &xNormalPosition,   CW_USEDEFAULT );
	g_pCONFIG->Read( "yNormalPosition",   &yNormalPosition,   CW_USEDEFAULT );
	g_pCONFIG->Read( "cxNormalPosition",  &cxNormalPosition,  430 );
	g_pCONFIG->Read( "cyNormalPosition",  &cyNormalPosition,  240 );
}



////////////////////////////////////////////////////////////////////////////////
//  WriteConfig
////////////////////////////////////////////////////////////////////////////////
void WriteConfig()
{
	if( !g_pCONFIG ) return;

	g_pCONFIG->Write( "bLogConfig",          ( int )bLogConfig );
	g_pCONFIG->Write( "bLogAbout",           ( int )bLogAbout );
	g_pCONFIG->Write( "bLogInit",            ( int )bLogInit );
	g_pCONFIG->Write( "bLogQuit",            ( int )bLogQuit );
	g_pCONFIG->Write( "bLogOpen",            ( int )bLogOpen );
	g_pCONFIG->Write( "bLogClose",           ( int )bLogClose );
	g_pCONFIG->Write( "bLogWrite",           ( int )bLogWrite );
	g_pCONFIG->Write( "bLogCanWrite",        ( int )bLogCanWrite );
	g_pCONFIG->Write( "bLogIsPlaying",       ( int )bLogIsPlaying );
	g_pCONFIG->Write( "bLogPause",           ( int )bLogPause );
	g_pCONFIG->Write( "bLogSetVolume",       ( int )bLogSetVolume );
	g_pCONFIG->Write( "bLogSetPan",          ( int )bLogSetPan );
	g_pCONFIG->Write( "bLogFlush",           ( int )bLogFlush );
	g_pCONFIG->Write( "bLogGetWrittenTime",  ( int )bLogGetWrittenTime );
	g_pCONFIG->Write( "bLogGetOutputTime",   ( int )bLogGetOutputTime );

	g_pCONFIG->Write( "iShowCmd",          iShowCmd );
	g_pCONFIG->Write( "xMaxPosition",      xMaxPosition );
	g_pCONFIG->Write( "yMaxPosition",      yMaxPosition );
	g_pCONFIG->Write( "xNormalPosition",   xNormalPosition );
	g_pCONFIG->Write( "yNormalPosition",   yNormalPosition );
	g_pCONFIG->Write( "cxNormalPosition",  cxNormalPosition );
	g_pCONFIG->Write( "cyNormalPosition",  cyNormalPosition );
}



////////////////////////////////////////////////////////////////////////////////
//  g_OutModMaster
////////////////////////////////////////////////////////////////////////////////
Out_Module g_OutModMaster = {
	OUT_VER,            // int version
	NULL,               // char * description
	0,                  // int id
	0,                  // HWND hMainWindow
	0,                  // HINSTANCE hDllInstance
	Config,             // void ( * Config )( HWND hwndParent )
	About,              // void ( * About )( HWND hwndParent )
	Init,               // void ( * Init )()
	Quit,               // void ( * Quit )()
	Open,               // int ( *Open )( int samplerate, int numchannels, int bitspersamp, int bufferlenms, int prebufferms )
	Close,              // void ( * Close )()
	Write,              // int ( * Write )( char * buf, int len )
	CanWrite,           // int ( * CanWrite )()
	IsPlaying,          // int ( * IsPlaying )()
	Pause,              // int ( * Pause )( int pause )
	SetVolume,          // void ( * SetVolume )( int volume )
	SetPan,             // void ( * SetPan )( int pan )
	Flush,              // void ( * Flush )( int t )
	GetOutputTime,      // int ( * GetOutputTime )()
	GetWrittenTime,     // int ( * GetWrittenTime )()
};



////////////////////////////////////////////////////////////////////////////////
//  Config
////////////////////////////////////////////////////////////////////////////////
void Config( HWND p )
{
	if( bLogConfig )
	{
		Console::Append( "Config( HWND p )" );
		char szBuffer[ 500 ];
		sprintf( szBuffer, "   p = %i", ( int )p );
		Console::Append( szBuffer );
		Console::Append( " " );
	}
	
	g_pModSlave->Config( p );
}



////////////////////////////////////////////////////////////////////////////////
//  About
////////////////////////////////////////////////////////////////////////////////
void About( HWND p )
{
	if( bLogAbout )
	{
		Console::Append( "About( HWND p )" );
		char szBuffer[ 500 ];
		sprintf( szBuffer, "   p = %i", ( int )p );
		Console::Append( szBuffer );
		Console::Append( " " );
	}

	g_pModSlave->About( p );
}



////////////////////////////////////////////////////////////////////////////////
//  Init
////////////////////////////////////////////////////////////////////////////////
void Init()
{
	if( bLogInit )
	{
		Console::Append( "Init()" );
		Console::Append( " " );
	}

	g_pModSlave->Init();
}



////////////////////////////////////////////////////////////////////////////////
//  Quit
////////////////////////////////////////////////////////////////////////////////
void Quit()
{
	// In case Quit() is called right after Init()
	// which can happen when Plainamp scans the plugin
	// the timer is still running which means we are
	// unloading the plugin although is still running.
	// Bad idea so we have to stop the timer.
	if( hMainHandleTimer )
	{
		KillTimer( NULL, hMainHandleTimer );
		hMainHandleTimer = 0;
	}

	if( bLogQuit )
	{
		Console::Append( "Quit()" );
		Console::Append( " " );
	}

	g_pModSlave->Quit();

	// Unload DLL
	FreeLibrary( g_hSlaveInstance );

	// Destroy console
	Console::Destroy();
	
	// Save config
	WriteConfig();
}



////////////////////////////////////////////////////////////////////////////////
//  Open
////////////////////////////////////////////////////////////////////////////////
int Open( int sr, int nch, int bps, int len_ms, int pre_len_ms )
{
	if( bLogOpen )
	{
		Console::Append( "Open( int sr, int nch, int bps, int len_ms, int pre_len_ms )" );
		char szBuffer[ 500 ];
		sprintf( szBuffer, "   sr = %i", sr );
		Console::Append( szBuffer );
		sprintf( szBuffer, "   nch = %i", nch );
		Console::Append( szBuffer );
		sprintf( szBuffer, "   bps = %i", bps );
		Console::Append( szBuffer );
		sprintf( szBuffer, "   len_ms = %i", len_ms );
		Console::Append( szBuffer );
		sprintf( szBuffer, "   pre_len_ms = %i", pre_len_ms );
		Console::Append( szBuffer );
	}

	int res = g_pModSlave->Open( sr, nch, bps, len_ms, pre_len_ms );
	
	if( bLogOpen )
	{
		char szBuffer[ 500 ];
		sprintf( szBuffer, "result = %i", res );
		Console::Append( szBuffer );
		Console::Append( " " );
	}
	
	return res;
}



////////////////////////////////////////////////////////////////////////////////
//  Close
////////////////////////////////////////////////////////////////////////////////
void Close()
{
	if( bLogClose )
	{
		Console::Append( "Close()" );
		Console::Append( " " );
	}

	g_pModSlave->Close();
}



////////////////////////////////////////////////////////////////////////////////
//  Write
////////////////////////////////////////////////////////////////////////////////
int Write( char * data, int size )
{
	if( bLogWrite )
	{
		Console::Append( "Write( char * data, int size )" );
		Console::Append( "   data = ..." );
		char szBuffer[ 500 ];
		sprintf( szBuffer, "   size = %i", size );
		Console::Append( szBuffer );
	}

	int res = g_pModSlave->Write( data, size );
	
	if( bLogWrite )
	{
		char szBuffer[ 500 ];
		sprintf( szBuffer, "result = %i", res );
		Console::Append( szBuffer );
		Console::Append( " " );
	}
	
	return res;
}



////////////////////////////////////////////////////////////////////////////////
//  CanWrite
////////////////////////////////////////////////////////////////////////////////
int CanWrite()
{
	if( bLogCanWrite )
	{
		Console::Append( "CanWrite()" );
	}

	int res = g_pModSlave->CanWrite();
	
	if( bLogCanWrite )
	{
		char szBuffer[ 500 ];
		sprintf( szBuffer, "result = %i", res );
		Console::Append( szBuffer );
		Console::Append( " " );
	}
	
	return res;
}



////////////////////////////////////////////////////////////////////////////////
//  IsPlaying
////////////////////////////////////////////////////////////////////////////////
int IsPlaying()
{
	if( bLogIsPlaying )
	{
		Console::Append( "IsPlaying()" );
	}

	int res = g_pModSlave->IsPlaying();
	
	if( bLogIsPlaying )
	{
		char szBuffer[ 500 ];
		sprintf( szBuffer, "result = %i", res );
		Console::Append( szBuffer );
		Console::Append( " " );
	}
	
	return res;
}



////////////////////////////////////////////////////////////////////////////////
//  Pause
////////////////////////////////////////////////////////////////////////////////
int Pause( int new_state )
{
	if( bLogPause )
	{
		Console::Append( "Pause( int new_state )" );
		char szBuffer[ 500 ];
		sprintf( szBuffer, "   new_state = %i", new_state );
		Console::Append( szBuffer );
	}

	int res = g_pModSlave->Pause( new_state );
	
	if( bLogPause )
	{
		char szBuffer[ 500 ];
		sprintf( szBuffer, "result = %i", res );
		Console::Append( szBuffer );
		Console::Append( " " );
	}
	
	return res;
}	



////////////////////////////////////////////////////////////////////////////////
//  SetVolume
////////////////////////////////////////////////////////////////////////////////
void SetVolume( int v )
{
	if( bLogSetVolume )
	{
		Console::Append( "SetVolume( int v )" );
		char szBuffer[ 500 ];
		sprintf( szBuffer, "   v = %i", v );
		Console::Append( szBuffer );
		Console::Append( " " );
	}
	
	g_pModSlave->SetVolume( v );
}



////////////////////////////////////////////////////////////////////////////////
//  SetPan
////////////////////////////////////////////////////////////////////////////////
void SetPan( int p )
{
	if( bLogSetPan )
	{
		Console::Append( "SetPan( int p )" );
		char szBuffer[ 500 ];
		sprintf( szBuffer, "   p = %i", p );
		Console::Append( szBuffer );
		Console::Append( " " );
	}
	
	g_pModSlave->SetPan( p );
}



////////////////////////////////////////////////////////////////////////////////
//  Flush
////////////////////////////////////////////////////////////////////////////////
void Flush( int pos )
{
	if( bLogFlush )
	{
		Console::Append( "Flush( int pos )" );
		char szBuffer[ 500 ];
		sprintf( szBuffer, "   pos = %i", pos );
		Console::Append( szBuffer );
		Console::Append( " " );
	}
	
	g_pModSlave->Flush( pos );
}



////////////////////////////////////////////////////////////////////////////////
//  GetWrittenTime
////////////////////////////////////////////////////////////////////////////////
int GetWrittenTime()
{
	if( bLogGetWrittenTime )
	{
		Console::Append( "GetWrittenTime()" );
	}

	int res = g_pModSlave->GetWrittenTime();

	if( bLogGetWrittenTime )
	{
		char szBuffer[ 500 ];
		sprintf( szBuffer, "result = %i", res );
		Console::Append( szBuffer );
		Console::Append( " " );
	}
	
	return res;
}



////////////////////////////////////////////////////////////////////////////////
//  GetOutputTime
////////////////////////////////////////////////////////////////////////////////
int GetOutputTime()
{
	if( bLogGetOutputTime )
	{
		Console::Append( "GetOutputTime()" );
	}

	int res = g_pModSlave->GetOutputTime();
	
	if( bLogGetOutputTime )
	{
		char szBuffer[ 500 ];
		sprintf( szBuffer, "result = %i", res );
		Console::Append( szBuffer );
		Console::Append( " " );
	}
	
	return res;
}



//////////////////////////////////////////////////////////////////////////////// 
//  MainHandleTimerProc
//////////////////////////////////////////////////////////////////////////////// 
VOID CALLBACK MainHandleTimerProc( HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime )
{
	static bool bStayOut = false;

	if( bStayOut )
	{
		return;
	}
	
	if( g_OutModMaster.hMainWindow )
	{
		bStayOut = true;
//////////////////////////////////////////////////////////////////////////////// 

		KillTimer( NULL, hMainHandleTimer );
		hMainHandleTimer = 0;

		g_pModSlave->hMainWindow = g_OutModMaster.hMainWindow;

		Console::Append( ">> Main window handle found <<" );
		Console::Append( " " );

//////////////////////////////////////////////////////////////////////////////// 
		bStayOut = false;
	}
}



////////////////////////////////////////////////////////////////////////////////
//  winampGetOutModule
////////////////////////////////////////////////////////////////////////////////
extern "C" __declspec( dllexport ) Out_Module * winampGetOutModule()
{
	g_hMasterInstance = ( HINSTANCE )GetCurrentModule();
	g_OutModMaster.hDllInstance = g_hMasterInstance;


	// Get master path
	char szFullpath[ MAX_PATH ] = "";
	GetModuleFileName( g_hMasterInstance, szFullpath, MAX_PATH - 6 - 1 );
	const int iFullLathLen = strlen( szFullpath );

	// Get slave path
	char * walk = szFullpath + iFullLathLen - 1; // Last char
	while( ( walk > szFullpath ) && ( *walk != '\\' ) ) walk--;
	walk++;
	
	// Backup basename e.g. "out_ds.dll"
	const int iBackupLen = ( szFullpath + iFullLathLen ) - walk;
	char * szBackup = new char[ iBackupLen + 1 ];
	memcpy( szBackup, walk, iBackupLen );
	szBackup[ iBackupLen ] = '\0';
	
	// Make plugin.ini path for config
	memcpy( walk, "plugin.ini\0", 11 );

	// Make section name for config
	char * szSection = new char[ 10 + iBackupLen + 1 ];
	sprintf( szSection, "Outbridge, %s", szBackup );

	// Read config
	g_pCONFIG = new DEVIL_CONFIG( szSection, szFullpath );
	ReadConfig();

	// Make slave plugin path
	memcpy( walk, "slave_", 6 );
	walk += 6;
	memcpy( walk, szBackup, iBackupLen );
	walk[ iBackupLen ] = '\0';
	char * szBasename = walk;

	// Not needed anymore
    delete [] szBackup;
    delete [] szSection;


	// Load slave dll
	g_hSlaveInstance = LoadLibrary( szFullpath );
	if( !g_hSlaveInstance )
	{
		_strlwr( szBasename );
		char szBuffer[ 1000 ];
		wsprintf(
			szBuffer,
			"Slave plugin could not be loaded:\n"
			"    %s  \n"
			"\n"
			"NOTE:\tRenaming the master file also affects  \n"
			"\tthe expected filename of the slave.",
			szBasename
		);
		MessageBox( NULL, szBuffer, "Slave plugin error", MB_ICONINFORMATION );
		return NULL;
	}


	// Find export
	WINAMPGETOUTMODULE winampGetOutModule =
		( WINAMPGETOUTMODULE )GetProcAddress( g_hSlaveInstance, "winampGetOutModule" );
	if( !winampGetOutModule )
	{
		FreeLibrary( g_hSlaveInstance );
		return NULL;
	}


	// Get module
	g_pModSlave = winampGetOutModule();
	if( !g_pModSlave )
	{
		FreeLibrary( g_hSlaveInstance );
		return NULL;
	}


	// Version mismatch?
	if( g_pModSlave->version != OUT_VER )
	{
		FreeLibrary( g_hSlaveInstance );
		return NULL;
	}
	

	// Modify slave
	g_pModSlave->hDllInstance   = g_hSlaveInstance;
	
	// Modfiy master
	g_OutModMaster.description  = g_pModSlave->description;
	g_OutModMaster.id           = g_pModSlave->id;


	// Initialize console
	Console::Create( szBasename );
	Console::Append( "Outbridge Winamp Plugin 2.2" );
	Console::Append( "http://www.hartwork.org" );
	Console::Append( " " );
	Console::Append( "Right-click for settings" );
	Console::Append( "=================================================" );
	Console::Append( " " );


	// Start main window detection
	hMainHandleTimer = SetTimer( NULL, 0, 333, MainHandleTimerProc );

	return &g_OutModMaster;
}
