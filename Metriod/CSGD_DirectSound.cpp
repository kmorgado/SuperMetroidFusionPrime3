/////////////////////////////////////////////////////////////////
//	File			:	"CSGD_DirectSound.cpp"
//
//	Author			:	Jensen Rivera (JR)
//
//	Creation Date	:	4/05/2004
//
//	Purpose			:	Function definitions for CSGD_DirectSound.
//////////////////////////////////////////////////////////////////

/* 
Disclaimer:
This source code was developed for and is the property of
Full Sail Real World Education Game Design Curriculum
2000, 2001, 2002, 2003, 2004, 2005, 2006, 2007
Full Sail students may not redistribute this code, 
but may use it in any project for educational purposes.
*/

#include "CSGD_DirectSound.h"

CSGD_DirectSound CSGD_DirectSound::m_Instance;

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
CSGD_DirectSound *CSGD_DirectSound::GetInstance(void)
{
	//	Return the address of the static instance.
	return &m_Instance;
}

//	CSGD_DirectSound : constructor for SoundDevice manager class
CSGD_DirectSound::CSGD_DirectSound() 
{
	m_dsObject			= NULL;
	m_dsPrimaryBuffer	= NULL;
	m_hWnd				= 0;
}

//	~CSGD_DirectSound : destructor for SoundDevice manager class
CSGD_DirectSound::~CSGD_DirectSound() 
{
}

///////////////////////////////////////////////////////////////////////////////
//	Function		:	"InitDirectSound"
//
//	Last Modified	:	4/05/2004
//
//	Input			:	hWnd				 -	Main Window Handle.
//						nPrimaryBufferFormat -	PB_LOQUAL for Normal, 11khz (default)
//												PB_MEDQUAL for 22khz
//												PB_HIQUAL for 44khz
//
//	Return			:	true, if successful.
//
//	Purpose			:	Initialize DirectSound and create a Primar Buffer, if needed.
///////////////////////////////////////////////////////////////////////////////
bool CSGD_DirectSound::InitDirectSound(HWND hWnd, int nPrimaryBufferFormat)
{
	//	Make sure hWnd is valid.
	if (!hWnd)	return false;

	//	Store the Main Window Handle.
	m_hWnd = hWnd;

	//	Create the main DirectSound Object.
	if (FAILED(DirectSoundCreate8(NULL, &m_dsObject, NULL))) 
		DSERRBOX("Failed to Create DirectSound Object");

	//	Set the Cooperative Level for DirectSound.
	if (nPrimaryBufferFormat != PB_MEDQUAL && nPrimaryBufferFormat != PB_HIQUAL)
	{
		//	The Default cooperative level.
		if (FAILED(m_dsObject->SetCooperativeLevel(m_hWnd, DSSCL_NORMAL)))
			DSERRBOX("Failed to SetCooperativeLevel - NORMAL");
	}
	else
	{
		//	Attempt to set the primary buffer format
		//	we're doing this because DirectSound likes to convert our samples to its
		//	default low qual format, causing lots of white noise on our samples
		if (FAILED(m_dsObject->SetCooperativeLevel(m_hWnd, DSSCL_PRIORITY)))
			DSERRBOX("Failed to SetCooperativeLevel - PRIORITY");

		//	Make a wave format structure for our primary sound buffer.
		static WAVEFORMATEX waveFormatEx;
		memset(&waveFormatEx, 0, sizeof(waveFormatEx));
		waveFormatEx.wFormatTag = WAVE_FORMAT_PCM;
		waveFormatEx.nChannels	= 2;
		//	Give us 22050 or 44100
		(nPrimaryBufferFormat == PB_MEDQUAL) ? waveFormatEx.nSamplesPerSec = 22050 : waveFormatEx.nSamplesPerSec = 44100;
		waveFormatEx.nBlockAlign	 = 4;
		waveFormatEx.nAvgBytesPerSec = waveFormatEx.nSamplesPerSec * waveFormatEx.nBlockAlign;
		waveFormatEx.wBitsPerSample	 = 16;

		//	Setup the DSBUFFERDESC struct.
		DSBUFFERDESC dsbd;
		memset(&dsbd, 0, sizeof(dsbd));
		dsbd.dwSize = sizeof(dsbd);

		//	Will be making a Primary Buffer.
		dsbd.dwFlags	   = DSBCAPS_PRIMARYBUFFER;
		dsbd.dwBufferBytes = 0;
		dsbd.lpwfxFormat   = NULL; // Must be NULL for primary buffers.

		//	Make the primary sound buffer
		if (FAILED(m_dsObject->CreateSoundBuffer(&dsbd, &m_dsPrimaryBuffer, NULL)))
			DSERRBOX("Couldn't Create the Primary Sound Buffer");

		//	Set the desired format for the buffer.
		if (FAILED(m_dsPrimaryBuffer->SetFormat(&waveFormatEx)))
			DSERRBOX("Couldn't Set the Primary Sound Buffer Format");
	}

	//	Return success.
	return true;
}

///////////////////////////////////////////////////////////////////////////////
//	Function		:	"ShutdownDirectSound"
//
//	Last Modified	:	4/05/2004
//
//	Input			:	void
//
//	Return			:	true, if successful.
//
//	Purpose			:	Release all DirectSound pointers
///////////////////////////////////////////////////////////////////////////////
bool CSGD_DirectSound::ShutdownDirectSound(void) 
{
	SAFE_RELEASE(m_dsPrimaryBuffer);
	SAFE_RELEASE(m_dsObject);

	//	Return success.
	return true;
}
