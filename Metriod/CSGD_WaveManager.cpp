/////////////////////////////////////////////////////////////////
//	File			:	CSGD_WaveManager.cpp
//
//	Author			:	Jensen Rivera (JR)
//
//	Creation Date	:	4/05/2004
//
//	Purpose			:	Function definitions for CSGD_WaveManager.
//////////////////////////////////////////////////////////////////

/* 
Disclaimer:
This source code was developed for and is the property of
Full Sail Real World Education Game Design Curriculum
2000, 2001, 2002, 2003, 2004, 2005, 2006, 2007
Full Sail students may not redistribute this code, 
but may use it in any project for educational purposes.
*/

#include "CSGD_WaveManager.h"
#include <assert.h>

#pragma warning (disable : 4996)

CSGD_WaveManager CSGD_WaveManager::m_Instance;

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
CSGD_WaveManager *CSGD_WaveManager::GetInstance(void)
{
	//	Return the address of the instance.
	return &m_Instance;
}

//	CSGD_WaveManager : Constructor for WaveManager setup
CSGD_WaveManager::CSGD_WaveManager()
{
	m_dsObject		= 0;
	m_hWnd			= 0;
	debugdealloc	= 0;
	debugalloc		= 0;
}

//	~CSGD_WaveManager : Destructor
CSGD_WaveManager::~CSGD_WaveManager()
{
	ShutdownWaveManager();
}

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
bool CSGD_WaveManager::InitWaveManager(HWND hWnd, LPDIRECTSOUND8 dsObject)
{
	if (!hWnd && !dsObject)	return false;

	//	Store the DirectSound Object and Add a Reference.
	m_dsObject = dsObject;
	m_dsObject->AddRef();

	//	Store the main application handle.
	m_hWnd = hWnd;

	//	Return success.
	return true;
}

////////////////////////////////////////////////////////////////////////////
//	Function		:	"ShutdownWaveManager"
//
//	Last Modified	:	Aprile 5, 2004
//
//	Input			:	void
//
//	Return			:	true, if successful.
//
//	Purpose			:	Release All buffers and pointers.
////////////////////////////////////////////////////////////////////////////
bool CSGD_WaveManager::ShutdownWaveManager(void)
{
	//	Go through the list and release all textures.
	for (int i = 0; i < (signed)m_waveList.size(); i++)
		SAFE_RELEASE(m_waveList[i].dsBuffer);

	m_waveList.clear();

	list<tSoundChannel>::iterator iter = m_ChannelList.begin();

	while (iter != m_ChannelList.end())
	{
		// Kill the event handle.
		CloseHandle((*iter).event);

		// Kill the sound buffer.
		SAFE_RELEASE((*iter).dsBuffer);

		// Remove the buffer from the list.
		iter = m_ChannelList.erase(iter);
	}

	m_ChannelList.clear();

	SAFE_RELEASE(m_dsObject);

	//	Return success.
	return true;
}

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
int CSGD_WaveManager::LoadWave(char *szFilename)
{
	//	Make sure the filename is valid.
	if (!szFilename)	return -1;

	// Make sure the wave isn't already loaded.
	for (unsigned int i = 0; i < m_waveList.size(); i++)
	{
		// compare strings without caring about upper or lowercase.
		if (stricmp(szFilename, m_waveList[i].filename) == 0)	// 0 means they are equal.
		{
			m_waveList[i].ref++; // add a reference to this wave.
			return i;	// return the index.
		}
	}

	// Look for an open spot.
	int id = -1;
	for (unsigned int i = 0; i < m_waveList.size(); i++)
	{
		if (m_waveList[i].ref == 0)
		{
			id = i;
			break;
		}
	}

	// Load the wave
	tSoundInfo tSound;

	FILE *pFile = 0;			//	Pointer to a file.
	char szBuffer[32] = {0};	//	Store file information.

	//	Variables used for Locking the buffer.
	unsigned char *pAudio1 = NULL;		//	Pointer to first write buffer.
	unsigned char *pAudio2 = NULL;		//	Pointer to second write buffer.
	unsigned long ulAudioLength1 = 0;	//	Length of first pointer.
	unsigned long ulAudioLength2 = 0;	//	Length of second pointer.

	//	Open the file for Reading.
	pFile = fopen(szFilename, "rb");

	//	Make sure the file was loaded.
	if (!pFile)
		WAVEERRBOX("Could not find the file");

	//	Read in the Wave Header information.
	fread(szBuffer, 12, 1, pFile);

	//	Make sure the header information is correct.
	if (memcmp(szBuffer, "RIFF", 4) || memcmp(szBuffer + 8, "WAVE", 4))
	{
		fclose(pFile);
		WAVEERRBOX("Incorrect Header information.");
	}

	tChunkHeader fmtChunk;
	fread(&fmtChunk, sizeof(fmtChunk), 1, pFile);

	//	Make sure this is the format chunk.
	if (memcmp(fmtChunk.cType, "fmt ", 4) || fmtChunk.lSize < 16)
	{
		fclose(pFile);
		WAVEERRBOX("Invalid Format Information");
	}

	//	Read in the wave format.
	fread(&tSound.waveFormatEx.wFormatTag,		sizeof(short), 1, pFile);
	fread(&tSound.waveFormatEx.nChannels,		sizeof(short), 1, pFile);
	fread(&tSound.waveFormatEx.nSamplesPerSec,	sizeof(int),   1, pFile);
	fread(&tSound.waveFormatEx.nAvgBytesPerSec,	sizeof(int),   1, pFile);
	fread(&tSound.waveFormatEx.nBlockAlign,		sizeof(short), 1, pFile);
	fread(&tSound.waveFormatEx.wBitsPerSample,	sizeof(short), 1, pFile);

	//	Make sure the file is encoded in PCM format.
	if (tSound.waveFormatEx.wFormatTag != WAVE_FORMAT_PCM)
	{
		fclose(pFile);
		WAVEERRBOX("Invalid PCM information");
	}

	//	Make sure the bits per sample is valid.
	if ((tSound.waveFormatEx.wBitsPerSample != 8) && 
		(tSound.waveFormatEx.wBitsPerSample != 16))
	{
		fclose(pFile);
		WAVEERRBOX("Invalid Bits per Sample - Must be 8 or 16");
	}

	//	Skip the remaining format bytes.
	for (int i = fmtChunk.lSize - 16; i > 0; i--)
		fread(szBuffer, 1, 1, pFile);

	//	Read in the next chunk.
	tChunkHeader tChunk;
	fread(&tChunk, sizeof(tChunk), 1, pFile);

	//	Skip all chunks until you find the data chunk
	while (memcmp(tChunk.cType, "data", 4))
	{
		//	Skip over chunk data.
		for (int i = 0; i < tChunk.lSize; i++)
			fread(szBuffer, 1, 1, pFile);

		//	Read in the next chunk.
		fread(&tChunk, sizeof(tChunk), 1, pFile);
	}

	//	Make a pointer to store wave information.
	BYTE *pData = new BYTE[tChunk.lSize];

	//	Make sure the memory was set aside.
	if (!pData)
	{
		fclose(pFile);
		WAVEERRBOX("Failed to make memory for wave information");
	}

	//	Read the data from the file.
	fread(pData, sizeof(BYTE), tChunk.lSize, pFile);

	//	Close the file.
	fclose(pFile);

	//	Initialize the DSBUFFERDESC structure before use.
	DSBUFFERDESC dsbd = { sizeof(dsbd) };

	//	Set the control flags for the buffer.
	dsbd.dwFlags = DSBCAPS_CTRLPAN | DSBCAPS_CTRLPOSITIONNOTIFY | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY | DSBCAPS_LOCSOFTWARE;

	//	Set the size of the buffer, in bytes.
	dsbd.dwBufferBytes = tChunk.lSize;

	//	Give the buffer it's format.
	dsbd.lpwfxFormat = &tSound.waveFormatEx;

	//	Create the Sound Buffer.
	if (FAILED(m_dsObject->CreateSoundBuffer(&dsbd, &tSound.dsBuffer, NULL)))
	{
		delete [] pData;
		WAVEERRBOX("Failed to Create the Sound Buffer");
	}

	//	Lock the buffer.
	tSound.dsBuffer->Lock(0, tChunk.lSize, (void **)&pAudio1, &ulAudioLength1, 
		(void **)&pAudio2, &ulAudioLength2, DSBLOCK_FROMWRITECURSOR);

	//	Copy the data into the buffer.
	memcpy(pAudio1, pData, ulAudioLength1);
	memcpy(pAudio2, pData + ulAudioLength1, ulAudioLength2);

	//	Unlock the buffer.
	tSound.dsBuffer->Unlock(pAudio1, ulAudioLength1, pAudio2, ulAudioLength2);

	//	Delete the wave data.
	delete [] pData;

	// if we didn't find an open spot, load it in a new one
	if (id == -1)
	{
		// Copy the filename of the loaded wave.
		strcpy(tSound.filename, szFilename);
	
		// AddRef.
		tSound.ref = 1;

		//	Push the sound into the wave list.	
		m_waveList.push_back(tSound);

		// Return the id of the wave.
		return (int)m_waveList.size() - 1;
	}
	// we found an open spot
	else
	{
		// Make sure the buffer has been released.
		SAFE_RELEASE(m_waveList[id].dsBuffer);

		// Copy the filename of the loaded wave.
		strcpy(m_waveList[id].filename, szFilename);
	
		// AddRef.
		m_waveList[id].ref = 1;

		memcpy(&m_waveList[id].waveFormatEx, &tSound.waveFormatEx, sizeof(WAVEFORMATEX));

		m_waveList[id].dsBuffer = tSound.dsBuffer;

		//	Return the sound's ID.
		return id;
	}

	// Error.
	return -1;
}

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
bool CSGD_WaveManager::UnloadWave(int nID)
{
	// Make sure the id is in range.
	assert(nID > -1 && nID < (int)m_waveList.size() && "id is out of range");

	// Remove ref.
	m_waveList[nID].ref--;

	// Release the wave if it's not being used.
	if (m_waveList[nID].ref <= 0)
	{
		// Do a lazy delete and leave this spot empty
		SAFE_RELEASE(m_waveList[nID].dsBuffer);
		m_waveList[nID].filename[0] = '\0';
		m_waveList[nID].ref = 0;
	}

	//	Return success.
	return true;
}

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
bool CSGD_WaveManager::Play(int nID, DWORD dwFlags)
{
	//	Error check and make sure the system has been initialized
	if (!m_dsObject)	return false;

	// Make sure the id is in range.
	assert(nID > -1 && nID < (int)m_waveList.size() && "id is out of range");

	//	Play sound.
	if (IsWavePlaying(nID))
	{
		tSoundChannel channel;
		m_dsObject->DuplicateSoundBuffer(m_waveList[nID].dsBuffer, &channel.dsBuffer);

		if (!channel.dsBuffer)
			WAVEERRBOX("Failed to allocate duplicate sound buffer.");

		LPDIRECTSOUNDNOTIFY8 lpdsbNotify;
		HRESULT hr = channel.dsBuffer->QueryInterface(IID_IDirectSoundNotify8, (void**)&lpdsbNotify);

		DSBPOSITIONNOTIFY dsbnotify;
		channel.event = CreateEvent(0, 0, 0, 0);
		dsbnotify.dwOffset = DSBPN_OFFSETSTOP;
		dsbnotify.hEventNotify = channel.event;
		lpdsbNotify->SetNotificationPositions(1, &dsbnotify);

		debugalloc++;

		lpdsbNotify->Release();

		channel.dsBuffer->Play(0, 0, dwFlags);
		m_ChannelList.push_back(channel);
	}
	else
	{
		if (FAILED(m_waveList[nID].dsBuffer->Play(0, 0, dwFlags)))
			WAVEERRBOX("Failed to Play the wave file");
	}

	//	Return success.
	return true;
}

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
bool CSGD_WaveManager::Reset(int nID)
{
	//	Error check and make sure the system has been initialized
	if (!m_dsObject)	return false;

	// Make sure the id is in range.
	assert(nID > -1 && nID < (int)m_waveList.size() && "id is out of range");

	//	Stop the Sound first.
	Stop(nID);

	//	Reset the playing position of the sound.
	if (FAILED(m_waveList[nID].dsBuffer->SetCurrentPosition(0)))
		WAVEERRBOX("Failed to Reset the Buffer");

	//	Return success.
	return true;
}

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
bool CSGD_WaveManager::Stop(int nID)
{
	//	Error check and make sure the system has been initialized
	if (!m_dsObject)	return false;

	// Make sure the id is in range.
	assert(nID > -1 && nID < (int)m_waveList.size() && "id is out of range");

	//	Stop the sound from playing.
	if (FAILED(m_waveList[nID].dsBuffer->Stop()))
		WAVEERRBOX("Failed to Stop the buffer");

	//	Return success.
	return true;
}

////////////////////////////////////////////////////////////////////////////
//	Function		:	"SetVolume"
//
//	Last Modified	:	9/21/2006
//
//	Input			:	nID		- ID of the wave file to set volume for.
//						nVolume	- The volume to set.
//
//	Return			:	true, if successful.
//
//	Purpose			:	Sets the volume for the wave file.
//						Use a range of [0 - 100]
////////////////////////////////////////////////////////////////////////////
bool CSGD_WaveManager::SetVolume(int nID, int nVolume)
{
	//	Error check and make sure the system has been initialized
	if (!m_dsObject)	return false;

	// Make sure the id is in range.
	assert(nID > -1 && nID < (int)m_waveList.size() && "id is out of range");

	//	Limit the bounds of our volume.
	if(nVolume < 0)    { nVolume = 0;   }
	if(nVolume > 100)  { nVolume = 100; }

	//	Change the volume to a logarithmic value.
	nVolume = -30 * (100 - nVolume);

	//	Set the volume for the sound.
	if (FAILED(m_waveList[nID].dsBuffer->SetVolume(nVolume)))
		WAVEERRBOX("Failed to Set the Volume for the Buffer");

	//	Return success.
	return true;
}

////////////////////////////////////////////////////////////////////////////
//	Function		:	"SetFrequency"
//
//	Last Modified	:	9/21/2006
//
//	Input			:	nID		-	ID of the wave file.
//						nFreq	-	The Frequency to set.
//
//	Return			:	true, if successful.
//
//	Purpose			:	Sets the frequency for the wave file.
////////////////////////////////////////////////////////////////////////////
bool CSGD_WaveManager::SetFrequency(int nID, int nFreq)
{
	//	Error check and make sure the system has been initialized
	if (!m_dsObject)	return false;

	// Make sure the id is in range.
	assert(nID > -1 && nID < (int)m_waveList.size() && "id is out of range");

	//	Alter the frequency.
	if (FAILED(m_waveList[nID].dsBuffer->SetFrequency(nFreq)))
		WAVEERRBOX("Failed to Set the Frequency for the buffer.");

	//	Return success.
	return true;
}

////////////////////////////////////////////////////////////////////////////
//	Function		:	"SetPan"
//
//	Last Modified	:	9/21/2006
//
//	Input			:	nID		-	ID of the wave file.
//						nPan	-	The Pan to set
//
//	Return			:	true, if successful.
//
//	Purpose			:	The pan to set for a wave file.
//						Use a range of [-100(Left) to 100(Right)] where 0 is both speakers
////////////////////////////////////////////////////////////////////////////
bool CSGD_WaveManager::SetPan(int nID, int nPan)
{
	//	Error check and make sure the system has been initialized
	if (!m_dsObject)	return false;

	// Make sure the id is in range.
	assert(nID > -1 && nID < (int)m_waveList.size() && "id is out of range");

	//	Limit the bounds of our Panning.
	if (nPan < -100) nPan = -100;
	if (nPan >  100) nPan =  100;

	//	Convert to dB of (-10,000, 10,000)
	nPan *= 100;

	//	Alter the panning.
	if (FAILED(m_waveList[nID].dsBuffer->SetPan(nPan)))
		WAVEERRBOX("Failed to Set the Panning for the buffer.");

	//	Return success.
	return true;
}

////////////////////////////////////////////////////////////////////////////
//	Function		:	"IsWavePlaying"
//
//	Last Modified	:	9/21/2006
//
//	Input			:	nID		-	ID of the wave file.
//
//	Return			:	true, if the wave file is playing.
//
//	Purpose			:	Checks to see if the wave file is currently playing.
////////////////////////////////////////////////////////////////////////////
bool CSGD_WaveManager::IsWavePlaying(int nID)
{
	//	Error check and make sure the system has been initialized
	if (!m_dsObject)	return false;
	
	// Make sure the id is in range.
	assert(nID > -1 && nID < (int)m_waveList.size() && "id is out of range");

	DWORD dwStatus = 0;
	if (FAILED(m_waveList[nID].dsBuffer->GetStatus(&dwStatus)))
		WAVEERRBOX("Failed to Get the Status of the Buffer.");

	//	Check to see if we are playing the sound.
	if (dwStatus & DSBSTATUS_PLAYING || dwStatus & DSBSTATUS_LOOPING)
		return true;

	//	Return Failure.
	return false;
}

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
void CSGD_WaveManager::Update()
{
	// Go though all the open channels.
	list<tSoundChannel>::iterator iter = m_ChannelList.begin();
	while (iter != m_ChannelList.end())
	{
		// Get the status of the wait event.
		HRESULT hr = WaitForSingleObject((*iter).event, 0);

		// If the event has fired.
		if (hr == WAIT_OBJECT_0)
		{
			// Kill the event handle.
			BOOL err = CloseHandle((*iter).event);

			// Make sure the buffer has stoped playing.
			(*iter).dsBuffer->Stop();

			// Kill the sound buffer.
			SAFE_RELEASE((*iter).dsBuffer);

			debugdealloc++;

			// Remove the buffer from the list.
			iter = m_ChannelList.erase(iter);
			continue;
		}

		iter++;
	}
}

#pragma warning (default : 4996)