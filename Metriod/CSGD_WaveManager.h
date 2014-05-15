////////////////////////////////////////////////////////
//	File			:	CSGD_WaveManager.h
//
//	Author			:	Jensen Rivera (JR)
//
//	Creation Date	:	4/05/2004
//
//	Purpose			:	Loads wave files and creates
//						sound buffers for use in DirectSound.
////////////////////////////////////////////////////////

/* 
Disclaimer:
This source code was developed for and is the property of
Full Sail Real World Education Game Design Curriculum
2000, 2001, 2002, 2003, 2004, 2005, 2006, 2007
Full Sail students may not redistribute this code, 
but may use it in any project for educational purposes.
*/

#pragma once

#include <dsound.h>
#include <stdio.h>
#include <vector>
#include <list>
using std::vector;
using std::list;

#pragma comment (lib, "dsound.lib")
#pragma comment (lib, "dxguid.lib")

//	Macro to safely Release COM Objects.
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)		if (p) { p->Release(); p = NULL; }
#endif

//	Macro to initialize a DirectX Structure.
#ifndef INITSTRUCT
#define INITSTRUCT(s)	memset(&s, 0, sizeof(s)); s.dwSize = sizeof(s);
#endif

#ifndef WAVEERRBOX
#define WAVEERRBOX(a)	{ MessageBox(m_hWnd, a, "CSGD_WaveManager Error", MB_OK | MB_ICONEXCLAMATION); return false; }
#endif

struct tChunkHeader
{
	char cType[4];		//	Chunk ID.
	long lSize;			//	Size of the chunk, in bytes.
};

class CSGD_WaveManager
{
private:

	//	Struct contains data on the loaded sound files
	struct tSoundInfo 
	{
		char				filename[_MAX_PATH];		// The filename of this wave.
		int					ref;						// The number of times this wave has be loaded.
		WAVEFORMATEX				waveFormatEx;		//	The WaveFormat for the Sound File.
		LPDIRECTSOUNDBUFFER			dsBuffer;			//	The DirectSoundBuffer8 interface for the Wave File.

		//	Default constructor.
		tSoundInfo() 
		{ 
			filename[0] = '\0';
			ref = 0;
			memset(&waveFormatEx, 0, sizeof(waveFormatEx)); 
			dsBuffer = NULL; 
		}
	};

	struct tSoundChannel
	{
		LPDIRECTSOUNDBUFFER	dsBuffer;	//	The DirectSoundBuffer8 interface for the Wave File.
		HANDLE				event;		//	The event that was created by this buffer.
	};

	LPDIRECTSOUND8			m_dsObject;				//	The Main DirectSound Object.
	HWND					m_hWnd;					//	The Main Window Handle.
	
	vector<tSoundInfo>		m_waveList;				//	List of loaded wave files.
	list<tSoundChannel>		m_ChannelList;			//	List of created sound channels.

	int debugalloc;
	int debugdealloc;

	static CSGD_WaveManager m_Instance;			//	The Instance of this class.

	//	CSGD_WaveManager : Constructor for D3D setup
	CSGD_WaveManager();
	//	Copy Constructor.
	CSGD_WaveManager(const CSGD_WaveManager&);
	//	Assignment Operator.
	CSGD_WaveManager &operator = (const CSGD_WaveManager&);

public:

	//	~CSGD_WaveManager: Destructor
	virtual ~CSGD_WaveManager();

	///////////////////////////////////////////////////////////////////
	//	Function:	"GetInstance"
	//
	//	Last Modified:		8/04/2006
	//
	//	Input:		void
	//
	//	Return:		An instance to this class.
	//
	//	Purpose:	Gets an instance to this class.
	///////////////////////////////////////////////////////////////////
	static CSGD_WaveManager *GetInstance(void);

	////////////////////////////////////////////////////////////////////////////
	//	Function		:	"InitWaveManager"
	//
	//	Last Modified	:	4/05/2004
	//
	//	Input			:	hWnd		-	Main Window Handle
	//						dsObject	-	DirectSound Object for the wavemanager to use.
	//
	//	Return			:	true, if successful.
	//
	//	Purpose			:	Store the DirectSound object for use later on.
	////////////////////////////////////////////////////////////////////////////
	bool InitWaveManager(HWND hWnd, LPDIRECTSOUND8 dsObject);

	////////////////////////////////////////////////////////////////////////////
	//	Function		:	"ShutdownWaveManager"
	//
	//	Last Modified	:	4/05/2004
	//
	//	Input			:	NONE
	//
	//	Return			:	true, if successful.
	//
	//	Purpose			:	Release All buffers and pointers.
	////////////////////////////////////////////////////////////////////////////
	bool ShutdownWaveManager(void);

	////////////////////////////////////////////////////////////////////////////
	//	Function		:	"LoadWave"
	//
	//	Last Modified	:	9/21/2006
	//
	//	Input			:	szFilename		-	Name of the wave file to load.
	//
	//	Return			:	ID number for the associated sound file, -1 if failed
	//
	//	Purpose			:	Loads a wave file and stores it in a DS Buffer.
	////////////////////////////////////////////////////////////////////////////
	int LoadWave(char *szFilename);

	////////////////////////////////////////////////////////////////////////////
	//	Function		:	"UnloadWave"
	//
	//	Last Modified	:	9/21/2006
	//
	//	Input			:	nID		-	ID of the wave file to unload.
	//
	//	Return			:	true, if successful.
	//
	//	Purpose			:	Free's allocated information
	////////////////////////////////////////////////////////////////////////////
	bool UnloadWave(int nID);

	////////////////////////////////////////////////////////////////////////////
	//	Function		:	"Play"
	//
	//	Last Modified	:	9/21/2006
	//
	//	Input			:	nID			-	ID of the wave file to play
	//						dwFlags		-	Uses DirectSound play flags. (DSBPLAY_LOOPING, ect..)
	//
	//	Return			:	true, if successful
	//
	//	Purpose			:	Plays a wave file.
	////////////////////////////////////////////////////////////////////////////
	bool Play(int nID, DWORD dwFlags = 0);

	////////////////////////////////////////////////////////////////////////////
	//	Function		:	"Reset"
	//
	//	Last Modified	:	9/21/2006
	//
	//	Input			:	nID		-	ID of the wave file to Reset.
	//
	//	Return			:	true, if successful.
	//
	//	Purpose			:	Set the position of the wave back to the beginning.
	////////////////////////////////////////////////////////////////////////////
	bool Reset(int nID);

	////////////////////////////////////////////////////////////////////////////
	//	Function		:	"Stop"
	//
	//	Last Modified	:	9/21/2006
	//
	//	Input			:	nID		-	ID of the wave file to stop.
	//
	//	Return			:	true, if successful.
	//
	//	Purpose			:	Stop the playing sound.
	//
	//	NOTE:	This will only PAUSE the sound.  If Play() is called on it again
	//			the wave will continue to play from where it was stopped.
	////////////////////////////////////////////////////////////////////////////
	bool Stop(int nID);

	////////////////////////////////////////////////////////////////////////////
	//	Function		:	"SetVolume"
	//
	//	Last Modified	:	4/05/2004
	//
	//	Input			:	nID		- ID of the wave file to set volume for.
	//						nVolume	- The volume to set.
	//
	//	Return			:	true, if successful.
	//
	//	Purpose			:	Sets the volume for the wave file.
	//						Use a range of [0 - 100]
	////////////////////////////////////////////////////////////////////////////
	bool SetVolume(int nID, int nVolume);

	////////////////////////////////////////////////////////////////////////////
	//	Function		:	"SetFrequency"
	//
	//	Last Modified	:	4/05/2004
	//
	//	Input			:	nID		-	ID of the wave file.
	//						nFreq	-	The Frequency to set.
	//
	//	Return			:	true, if successful.
	//
	//	Purpose			:	Sets the frequency for the wave file.
	////////////////////////////////////////////////////////////////////////////
	bool SetFrequency(int nID, int nFreq = 22050);

	////////////////////////////////////////////////////////////////////////////
	//	Function		:	"SetPan"
	//
	//	Last Modified	:	4/05/2004
	//
	//	Input			:	nID		-	ID of the wave file.
	//						nPan	-	The Pan to set
	//
	//	Return			:	true, if successful.
	//
	//	Purpose			:	The pan to set for a wave file.
	//						Use a range of [-100(Left) to 100(Right)] where 0 is both speakers
	////////////////////////////////////////////////////////////////////////////
	bool SetPan(int nID, int nPan = 0);

	////////////////////////////////////////////////////////////////////////////
	//	Function		:	"IsWavePlaying"
	//
	//	Last Modified	:	4/05/2004
	//
	//	Input			:	nID		-	ID of the wave file.
	//
	//	Return			:	true, if the wave file is playing.
	//
	//	Purpose			:	Checks to see if the wave file is currently playing.
	////////////////////////////////////////////////////////////////////////////
	bool IsWavePlaying(int nID);

	////////////////////////////////////////////////////////////////////////////
	//	Function		:	"Update"
	//
	//	Last Modified	:	9/26/2005
	//
	//	Input			:	void
	//
	//	Return			:	void
	//
	//	Purpose			:	Removes finished duplicated sound buffers.
	//
	//  NOTE:	MUST be called once per frame
	////////////////////////////////////////////////////////////////////////////
	void Update(void);
};
