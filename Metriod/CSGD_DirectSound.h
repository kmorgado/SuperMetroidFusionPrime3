////////////////////////////////////////////////////////
//	File			:	"CSGD_DirectSound.h"
//
//	Author			:	Jensen Rivera (JR)
//
//	Creation Date	:	4/05/2004
//
//	Purpose			:	Wrapper class for DirectSound.
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
#pragma comment (lib, "dsound.lib")
#pragma comment (lib, "dxguid.lib")

//	Macro to safely Release COM Objects.
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)		if (p) { p->Release(); p = NULL; }
#endif

#ifndef DSERRBOX
#define DSERRBOX(a)	{ MessageBox(m_hWnd, a, "CSGD_DirectSound Error", MB_OK | MB_ICONEXCLAMATION); return false; }
#endif

//	Primary Buffer quality constants
enum { PB_LOQUAL = 1, PB_MEDQUAL, PB_HIQUAL, };

class CSGD_DirectSound
{
private:

	LPDIRECTSOUND8			 m_dsObject;				//	The Main DirectSound Object.
	LPDIRECTSOUNDBUFFER		 m_dsPrimaryBuffer;			//	The Primary Buffer (if Necessary).

	HWND					 m_hWnd;					//	The Main Window Handle.

	static CSGD_DirectSound m_Instance;		//	The Instance of this class.

	//	CSGD_DirectSound : Constructor for DS setup
	CSGD_DirectSound();
	//	Copy Constructor.
	CSGD_DirectSound(const CSGD_DirectSound&);
	//	Assignment Operator.
	CSGD_DirectSound &operator = (const CSGD_DirectSound&);

public:

	//	~CSGD_DirectSound: Destructor
	virtual ~CSGD_DirectSound();

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
	static CSGD_DirectSound *GetInstance(void);

	///////////////////////////////////////////////////////////////////////////////
	//	Function		:	"GetDSObject"
	//
	//	Last Modified	:	4/05/2004
	//
	//	Input			:	void
	//
	//	Return			:	The Main DirectSound Object.
	//
	//	Purpose			:	To retrieve the main DirectSound Object.
	///////////////////////////////////////////////////////////////////////////////
	inline LPDIRECTSOUND8 GetDSObject(void) { return m_dsObject; }

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
	bool InitDirectSound(HWND hWnd, int nPrimaryBufferFormat = PB_LOQUAL);

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
	bool ShutdownDirectSound(void);
};
