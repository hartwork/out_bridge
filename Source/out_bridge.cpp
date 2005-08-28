

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


#include "Global.h"
#include "Console.h"
#include "DevilConfig.h"
#include "Winamp/Out.h"
#include <stdio.h>


typedef Out_Module * ( * WINAMPGETOUTMODULE )();


HINSTANCE g_hMasterInstance = NULL; // extern
DEVIL_CONFIG * g_pCONFIG = NULL;

bool bSlaveLoaded = false;
bool bWindowSet = false;
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
///  ReadConfig
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
///  WriteConfig
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
///  g_ModMaster
////////////////////////////////////////////////////////////////////////////////

Out_Module g_ModMaster = {
	OUT_VER,                    // int version
	"Outbridge Winamp Plugin",  // char * description
	66666,                      // int id
	0,                          // HWND hMainWindow
	0,                          // HINSTANCE hDllInstance
	Config,
	About,
	Init,
	Quit,
	Open,
	Close,
	Write,
	CanWrite,
	IsPlaying,
	Pause,
	SetVolume,
	SetPan,
	Flush,
	GetOutputTime,
	GetWrittenTime,
};


////////////////////////////////////////////////////////////////////////////////
///  Config
////////////////////////////////////////////////////////////////////////////////

void Config( HWND p )
{
	if( !bSlaveLoaded ) return;

	// Set window handle as soon as available
	if( !bWindowSet && g_ModMaster.hMainWindow )
	{
		g_pModSlave->hMainWindow = g_ModMaster.hMainWindow;
		bWindowSet = true;
	}

	if( bLogConfig )
	{
		char szBuffer[ 500 ];
		sprintf( szBuffer, "Config( p = %i )", ( int )p );
		Console::Append( szBuffer );
	}
	
	g_pModSlave->Config( p );
}


////////////////////////////////////////////////////////////////////////////////
///  About
////////////////////////////////////////////////////////////////////////////////

void About( HWND p )
{
	if( !bSlaveLoaded ) return;

	// Set window handle as soon as available
	if( !bWindowSet && g_ModMaster.hMainWindow )
	{
		g_pModSlave->hMainWindow = g_ModMaster.hMainWindow;
		bWindowSet = true;
	}

	if( bLogAbout )
	{
		char szBuffer[ 500 ];
		sprintf( szBuffer, "About( p = %i )", ( int )p );
		Console::Append( szBuffer );
	}

	g_pModSlave->About( p );
}


////////////////////////////////////////////////////////////////////////////////
///  Init
////////////////////////////////////////////////////////////////////////////////

void Init()
{
	// Instance handle is now filled
	// but window handle is still missing
	g_hMasterInstance = g_ModMaster.hDllInstance;


	// Get master path
	char szFullpath[ 1024 ] = "\0";
	GetModuleFileName( g_hMasterInstance, szFullpath, 1024 - 6 - 1 );


	// Get slave path
	const int iFullLathLen = strlen( szFullpath );
	char * walk = szFullpath + iFullLathLen;
	while( ( walk > szFullpath ) && ( *walk != '\\' ) ) walk--;
	walk++;
	char * szSlaveBasename = new char[ szFullpath + iFullLathLen - walk + 1 ];
	szSlaveBasename[ 0 ] = '\0';
	strcpy( szSlaveBasename, walk );      // Save master szSlaveBasename
	strcpy( walk, "slave_" );
	strcpy( walk + 6, szSlaveBasename );  // Append master szSlaveBasename to make full slave filename
    delete [] szSlaveBasename;
	szSlaveBasename = walk;


	// Load slave dll
	g_hSlaveInstance = LoadLibrary( szFullpath );
	if( !g_hSlaveInstance )
	{
		_strlwr( szSlaveBasename );
		char szBuffer[ 1000 ];
		wsprintf(
			szBuffer,
			"Slave plugin could not be loaded:\n"
			"    %s  \n"
			"\n"
			"NOTE:\tRenaming the master file also affects  \n"
			"\tthe expected filename of the slave.",
			szSlaveBasename
		);
		MessageBox( NULL, szBuffer, "Slave plugin error", MB_ICONINFORMATION );
		return;
	}


	// Find export
	WINAMPGETOUTMODULE winampGetOutModule =
		( WINAMPGETOUTMODULE )GetProcAddress( g_hSlaveInstance, "winampGetOutModule" );
	if( !winampGetOutModule )
	{
		FreeLibrary( g_hSlaveInstance );
		return;
	}


	// Get module
	g_pModSlave = winampGetOutModule();
	if( !g_pModSlave )
	{
		FreeLibrary( g_hSlaveInstance );
		return;
	}


	// Version mismatch?
	if( g_pModSlave->version != OUT_VER )
	{
		FreeLibrary( g_hSlaveInstance );
		return;

	}
	

	// Modify slave
	g_pModSlave->hDllInstance = g_hSlaveInstance;
	
	// Modfiy master
	g_ModMaster.description  = g_pModSlave->description;

	
	// Read config
	g_pCONFIG = new DEVIL_CONFIG( "Outbridge", g_hMasterInstance );
	ReadConfig();
	

	Console::Create( walk + 6 );
	Console::Append( "Outbridge Winamp Plugin 1.0" );
	Console::Append( "http://www.hartwork.org" );
	Console::Append( " " );
	Console::Append( "Right-click for settings" );
	Console::Append( " " );


	bSlaveLoaded = true;
		
	
	if( bLogInit )
	{
		Console::Append( "Init()" );
	}

	g_pModSlave->Init();
}


////////////////////////////////////////////////////////////////////////////////
///  Quit
////////////////////////////////////////////////////////////////////////////////

void Quit()
{
	if( !bSlaveLoaded )
	{
		WriteConfig();
		return;
	}
	
	if( bLogQuit )
	{
		Console::Append( "Quit()" );
	}

	g_pModSlave->Quit();

	// Destroy console
	Console::Destroy();
	
	// Save config
	WriteConfig();
	
	// Unload DLL
	FreeLibrary( g_hSlaveInstance );
}


////////////////////////////////////////////////////////////////////////////////
///  Open
////////////////////////////////////////////////////////////////////////////////

int Open( int sr, int nch, int bps, int bufferlenms, int prebufferms )
{
	if( !bSlaveLoaded ) return -1; // Error

	// Set window handle as soon as available
	if( !bWindowSet && g_ModMaster.hMainWindow )
	{
		g_pModSlave->hMainWindow = g_ModMaster.hMainWindow;
		bWindowSet = true;
	}

	if( bLogOpen )
	{
		char szBuffer[ 500 ];
		sprintf( szBuffer, "Open( sr = %i, nch = %i, bps = %i, bufferlenms = %i, prebufferms = %i )", sr, nch, bps, bufferlenms, prebufferms );
		Console::Append( szBuffer );
	}

	int res = g_pModSlave->Open( sr, nch, bps, bufferlenms, prebufferms );
	
	if( bLogOpen )
	{
		char szBuffer[ 500 ];
		sprintf( szBuffer, "   = %i", res );
		Console::Append( szBuffer );
	}
	
	return res;
}


////////////////////////////////////////////////////////////////////////////////
///  Close
////////////////////////////////////////////////////////////////////////////////

void Close()
{
	if( !bSlaveLoaded ) return;
	
	if( bLogClose )
	{
		Console::Append( "Close()" );
	}

	g_pModSlave->Close();
}


////////////////////////////////////////////////////////////////////////////////
///  Write
////////////////////////////////////////////////////////////////////////////////

int Write( char * data, int size )
{
	if( !bSlaveLoaded ) return 1; // Not able to write

	if( bLogWrite )
	{
		char szBuffer[ 500 ];
		sprintf( szBuffer, "Write( data = ..., size = %i )", size );
		Console::Append( szBuffer );
	}

	int res = g_pModSlave->Write( data, size );
	
	if( bLogWrite )
	{
		char szBuffer[ 500 ];
		sprintf( szBuffer, "   = %i", res );
		Console::Append( szBuffer );
	}
	
	return res;
}


////////////////////////////////////////////////////////////////////////////////
///  CanWrite
////////////////////////////////////////////////////////////////////////////////

int CanWrite()
{
	if( !bSlaveLoaded ) return 0; // Not able to write
	
	if( bLogCanWrite )
	{
		Console::Append( "CanWrite()" );
	}

	int res = g_pModSlave->CanWrite();
	
	if( bLogCanWrite )
	{
		char szBuffer[ 500 ];
		sprintf( szBuffer, "   = %i", res );
		Console::Append( szBuffer );
	}
	
	return res;
}


////////////////////////////////////////////////////////////////////////////////
///  IsPlaying
////////////////////////////////////////////////////////////////////////////////

int IsPlaying()
{
	if( !bSlaveLoaded ) return 0; // Not playing

	if( bLogIsPlaying )
	{
		Console::Append( "IsPlaying()" );
	}

	int res = g_pModSlave->IsPlaying();
	
	if( bLogIsPlaying )
	{
		char szBuffer[ 500 ];
		sprintf( szBuffer, "   = %i", res );
		Console::Append( szBuffer );
	}
	
	return res;
}


////////////////////////////////////////////////////////////////////////////////
///  Pause
////////////////////////////////////////////////////////////////////////////////

int Pause( int new_state )
{
	if( !bSlaveLoaded ) return 0; // Not paused
	
	if( bLogPause )
	{
		char szBuffer[ 500 ];
		sprintf( szBuffer, "Pause( new_state = %i )", new_state );
		Console::Append( szBuffer );
	}

	int res = g_pModSlave->Pause( new_state );
	
	if( bLogPause )
	{
		char szBuffer[ 500 ];
		sprintf( szBuffer, "   = %i", res );
		Console::Append( szBuffer );
	}
	
	return res;
}	


////////////////////////////////////////////////////////////////////////////////
///  SetVolume
////////////////////////////////////////////////////////////////////////////////

void SetVolume( int v )
{
	if( !bSlaveLoaded ) return;

	// Set window handle as soon as available
	if( !bWindowSet && g_ModMaster.hMainWindow )
	{
		g_pModSlave->hMainWindow = g_ModMaster.hMainWindow;
		bWindowSet = true;
	}

	if( bLogSetVolume )
	{
		char szBuffer[ 500 ];
		sprintf( szBuffer, "SetVolume( v = %i )", v );
		Console::Append( szBuffer );
	}
	
	g_pModSlave->SetVolume( v );
}


////////////////////////////////////////////////////////////////////////////////
///  SetPan
////////////////////////////////////////////////////////////////////////////////

void SetPan( int p )
{
	if( !bSlaveLoaded ) return;

	// Set window handle as soon as available
	if( !bWindowSet && g_ModMaster.hMainWindow )
	{
		g_pModSlave->hMainWindow = g_ModMaster.hMainWindow;
		bWindowSet = true;
	}

	if( bLogSetPan )
	{
		char szBuffer[ 500 ];
		sprintf( szBuffer, "SetPan( p = %i )", p );
		Console::Append( szBuffer );
	}
	
	g_pModSlave->SetPan( p );
}


////////////////////////////////////////////////////////////////////////////////
///  Flush
////////////////////////////////////////////////////////////////////////////////

void Flush( int pos )
{
	if( !bSlaveLoaded ) return;
	
	if( bLogFlush )
	{
		char szBuffer[ 500 ];
		sprintf( szBuffer, "Flush( pos = %i )", pos );
		Console::Append( szBuffer );
	}
	
	g_pModSlave->Flush( pos );
}


////////////////////////////////////////////////////////////////////////////////
///  GetWrittenTime
////////////////////////////////////////////////////////////////////////////////

int GetWrittenTime()
{
	if( !bSlaveLoaded ) return 0; // Nothing written
	
	if( bLogGetWrittenTime )
	{
		Console::Append( "GetWrittenTime()" );
	}

	int res = g_pModSlave->GetWrittenTime();

	if( bLogGetWrittenTime )
	{
		char szBuffer[ 500 ];
		sprintf( szBuffer, "   = %i", res );
		Console::Append( szBuffer );
	}
	
	return res;
}


////////////////////////////////////////////////////////////////////////////////
///  GetOutputTime
////////////////////////////////////////////////////////////////////////////////

int GetOutputTime()
{
	if( !bSlaveLoaded ) return 0; // At the beginning
	
	if( bLogGetOutputTime )
	{
		Console::Append( "GetOutputTime()" );
	}

	int res = g_pModSlave->GetOutputTime();
	
	if( bLogGetOutputTime )
	{
		char szBuffer[ 500 ];
		sprintf( szBuffer, "   = %i", res );
		Console::Append( szBuffer );
	}
	
	return res;
}


////////////////////////////////////////////////////////////////////////////////
///  winampGetOutModule
////////////////////////////////////////////////////////////////////////////////

extern "C" __declspec( dllexport ) Out_Module * winampGetOutModule()
{
	return &g_ModMaster;
}
