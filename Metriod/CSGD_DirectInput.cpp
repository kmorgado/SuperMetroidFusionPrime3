////////////////////////////////////////////////////////
//	File			:	"CSGD_DirectInput.cpp"
//
//	Author			:	Jensen Rivera (JR)
//
//	Creation Date	:	8/31/2004
//
//	Purpose			:	Initialized Input Manager and
//						allows use of Keyboard, Mouse
//						and joystick functionality.
////////////////////////////////////////////////////////

/* 
Disclaimer:
This source code was developed for and is the property of
Full Sail Real World Education Game Design Curriculum
2000, 2001, 2002, 2003, 2004, 2005, 2006, 2007
Full Sail students may not redistribute this code, 
but may use it in any project for educational purposes.
*/

#include "CSGD_DirectInput.h"
#include <stdio.h>

//	Disable "signed/unsigned mismatch" warning.
#pragma warning (disable : 4018)

//	Initialize the static variable member.
CSGD_DirectInput CSGD_DirectInput::m_Instance;

//	Default Constructor.
CSGD_DirectInput::CSGD_DirectInput()
{
	m_lpDIObject		= NULL;
	m_lpDIKey			= NULL;
	m_lpDIMouse			= NULL;
	m_bJStick			= false;
	m_bExclusive		= false;
	m_dwKeyElements		= 0;
	m_dwMouseElements	= 0;
	memset(&m_didodMouse, 0, sizeof(m_didodMouse));
	memset(&m_didodKey, 0, sizeof(m_didodKey));
}

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
CSGD_DirectInput *CSGD_DirectInput::GetInstance(void)
{
	//	Return the address of the static instance.
	return &m_Instance;
}

///////////////////////////////////////////////////////////////////
//	Function:	"EnumJoysticksCallback"
//
//	Last Modified:		8/31/2004
//
//	Input	:	*lpddi		-	Pointer to the DirectInput Device Instance.
//				*pVoid		-	Pointer to something you want to use for this function.
//
//	Return	:	Returns after the first Joystick is found.
//
//	Purpose	:	Checks to see if there is a Joystick presently attached to the computer.
///////////////////////////////////////////////////////////////////
BOOL CALLBACK EnumJoysticksCallback(const DIDEVICEINSTANCE *lpddi, VOID *pVoid)
{
	CSGD_DirectInput *pInput = CSGD_DirectInput::GetInstance();

	JoystickData tempJoystickData;
	tempJoystickData.szJoyname[0]	= 0;
	tempJoystickData.bJoyButtons[0] = 0;
	tempJoystickData.bUnplugged		= false;

	//	Get the Joystick Name and store it.
	strcpy(tempJoystickData.szJoyname, (char*)lpddi->tszProductName);

	//	Try and make a Joystick Device.
	if (FAILED(pInput->m_lpDIObject->CreateDevice(lpddi->guidInstance, &tempJoystickData.lpdiJoy, NULL)))
	{
		//	No more Joysticks are found.
		return DIENUM_STOP;
	}

	pInput->m_joystickList.push_back(tempJoystickData);

	//	Continue looking for Joysticks.
	return DIENUM_CONTINUE;
}

///////////////////////////////////////////////////////////////////
//	Function:	"InitDevices"
//
//	Last Modified:		12/27/2006
//
//	Input	:	hWnd			-	The Main window handle.				
//				hMainInstance	-	The Main application Instance.
//				bExclusive		-	Whether or not you want exclusive control of the device.
//
//	Return	:	true if it initialized the Keyboard, the Mouse, and the Joystick.
//
//	Purpose	:	Initialize all of the Devices for DirectInput.
///////////////////////////////////////////////////////////////////
bool CSGD_DirectInput::InitDevices(HWND hWnd, HINSTANCE hMainInstance, bool bExclusive)
{
	//	Make sure DirectInput hasn't already been initialized.
	if (m_lpDIObject)	DIERRBOX("DirectInput has already been initialized");

	m_hWnd		 = hWnd;			//	Store Main Window Handle.
	m_hInstance  = hMainInstance;	//	Store the main application instance.
	m_bExclusive = bExclusive;		//	Store whether or not we want exclusive access of the device.

	if (!InitKeyboard(hWnd, hMainInstance, bExclusive))
		DIERRBOX("Failed to Initialize the Keyboard");

	if (!InitMouse(hWnd, hMainInstance, bExclusive))
		DIERRBOX("Failed to Initialize the Mouse");

	if (!InitJoysticks(hWnd, hMainInstance, bExclusive))
		DIERRBOX("Failed to Initialize the Joystick");

	//	Return success.
	return true;
}

///////////////////////////////////////////////////////////////////
//	Function:	"InitKeyboard"
//
//	Last Modified:		12/27/2006
//
//	Input	:	hWnd			-	The Main window handle.				
//				hMainInstance	-	The Main application Instance.
//				bExclusive		-	Whether or not you want exclusive control of the device.
//
//	Return	:	true if the keyboard was initialized.
//
//	Purpose	:	Initialize the keyboard for use with DirectInput.
///////////////////////////////////////////////////////////////////
bool CSGD_DirectInput::InitKeyboard(HWND hWnd, HINSTANCE hMainInstance, bool bExclusive)
{
	//	Make sure Keyboard hasn't already been initialized.
	if (m_lpDIKey)	DIERRBOX("DirectInput Keyboard has already been initialized");

	m_hWnd		 = hWnd;			//	Store Main Window Handle.
	m_hInstance  = hMainInstance;	//	Store the main application instance.
	m_bExclusive = bExclusive;		//	Store whether or not we want exclusive access of the device.

	//	Only create the Main DirectInput object if it hasn't been created yet.
	if (!m_lpDIObject)
	{
		//	Initialize the Direct Input COM object.
		if (FAILED(DirectInput8Create(hMainInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void **)&m_lpDIObject, NULL)))
			DIERRBOX("Failed to Create Main DirectInput Object");
	}

	//	Create the Keyboard Device.
	if (FAILED(m_lpDIObject->CreateDevice(GUID_SysKeyboard, &m_lpDIKey, NULL)))
		DIERRBOX("No Keyboard Found.");

	//	Set the Cooperative level for the keyboard.
	if (m_bExclusive)
	{
		if (FAILED(m_lpDIKey->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE)))
			DIERRBOX("Failed to SetCooperativeLevel of Keyboard for Exclusive Mode.");
	}
	else
	{
		if (FAILED(m_lpDIKey->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE)))
			DIERRBOX("Failed to SetCooperativeLevel of Keyboard.");
	}

	//	Set the Data Format for the keyboard.
	if (FAILED(m_lpDIKey->SetDataFormat(&c_dfDIKeyboard)))
		DIERRBOX("Failed to SetDataFormat for the Keyboard.");

	//	For Use With Buffered Input.
	DIPROPDWORD dipdw;

	memset(&dipdw, 0, sizeof(dipdw));
	dipdw.diph.dwSize       = sizeof(DIPROPDWORD);
	dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	dipdw.diph.dwObj        = 0;
	dipdw.diph.dwHow        = DIPH_DEVICE;
	dipdw.dwData            = BUFFER_SIZE;	//	Arbitrary buffer size

	if (FAILED(m_lpDIKey->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph)))
		DIERRBOX("Could not Set the Properties for Buffered Input for Keyboard.");

	//	Acquire the Keyboard.
	if (FAILED(m_lpDIKey->Acquire()))
		DIERRBOX("Failed to Acquire Keyboard.");

	//	Gets the Keyboard Layout.
	m_keyLayout = GetKeyboardLayout(0);

	//	Return success.
	return true;
}

///////////////////////////////////////////////////////////////////
//	Function:	"InitMouse"
//
//	Last Modified:		12/27/2006
//
//	Input	:	hWnd			-	The Main window handle.				
//				hMainInstance	-	The Main application Instance.
//				bExclusive		-	Whether or not you want exclusive control of the device.
//
//	Return	:	true if the mouse was initialized.
//
//	Purpose	:	Initialize the mouse for use with DirectInput.
///////////////////////////////////////////////////////////////////		
bool CSGD_DirectInput::InitMouse(HWND hWnd, HINSTANCE hMainInstance, bool bExclusive)
{
	//	Make sure the Mouse hasn't already been initialized.
	if (m_lpDIMouse)	DIERRBOX("DirectInput Mouse has already been Initialized.");

	m_hWnd		 = hWnd;			//	Store Main Window Handle.
	m_hInstance  = hMainInstance;	//	Store the main application instance.
	m_bExclusive = bExclusive;		//	Store whether or not we want exclusive access of the device.

	//	Only create the Main DirectInput object if it hasn't been created yet.
	if (!m_lpDIObject)
	{
		//	Initialize the Direct Input COM object.
		if (FAILED(DirectInput8Create(hMainInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void **)&m_lpDIObject, NULL)))
			DIERRBOX("Failed to Create the DirectInput Object.");
	}

	//	Create the Mouse Device.
	if (FAILED(m_lpDIObject->CreateDevice(GUID_SysMouse, &m_lpDIMouse, NULL)))
		DIERRBOX("Mouse not Found.");

	//	Set the Cooperative Level for the Mouse.
	if (m_bExclusive)
	{
		if (FAILED(m_lpDIMouse->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE)))
			DIERRBOX("Failed to SetCooperativeLevel for the Mouse in Exclusive Mode.");
	}
	else
	{
		if (FAILED(m_lpDIMouse->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE)))
			DIERRBOX("Failed to SetCooperativeLevel for the Mouse.");
	}

	//	Set the Data format for the mouse.
	if (FAILED(m_lpDIMouse->SetDataFormat(&c_dfDIMouse2)))
		DIERRBOX("Failed to SetDataFormat for the Mouse.");

	//	For Use With Buffered Input.
	DIPROPDWORD dipdw;

	memset(&dipdw, 0, sizeof(dipdw));
	dipdw.diph.dwSize       = sizeof(DIPROPDWORD);
	dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	dipdw.diph.dwObj        = 0;
	dipdw.diph.dwHow        = DIPH_DEVICE;
	dipdw.dwData            = BUFFER_SIZE;	//	Arbitrary buffer size

	if (FAILED(m_lpDIMouse->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph)))
		DIERRBOX("Could not Set the Properties for Buffered Input for Mouse.");

	//	Acquire the mouse.
	if (FAILED(m_lpDIMouse->Acquire()))
		DIERRBOX("Failed to Acquire the Mouse.");

	//	Return success.
	return true;
}

///////////////////////////////////////////////////////////////////
//	Function:	"InitJoysticks"
//
//	Last Modified:		12/27/2006
//
//	Input	:	hWnd			-	The Main window handle.				
//				hMainInstance	-	The Main application Instance.
//				bExclusive		-	Whether or not you want exclusive control of the device.
//
//	Return	:	true if joysticks were initialized.
//
//	Purpose	:	Initialize all joysticks for use with DirectInput.
///////////////////////////////////////////////////////////////////	
bool CSGD_DirectInput::InitJoysticks(HWND hWnd, HINSTANCE hMainInstance, bool bExclusive)
{
	//	Make sure the joysticks haven't already been initialized.
	if (m_joystickList.size() >= 1)		DIERRBOX("DirectInput joysicks have already been initialized");

	m_hWnd		 = hWnd;			//	Store Main Window Handle.
	m_hInstance  = hMainInstance;	//	Store the main application instance.
	m_bExclusive = bExclusive;		//	Store whether or not we want exclusive access of the device.

	//	Only create the Main DirectInput object if it hasn't been created yet.
	if (!m_lpDIObject)
	{
		//	Initialize the Direct Input COM object.
		if (FAILED(DirectInput8Create(hMainInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void **)&m_lpDIObject, NULL)))
			DIERRBOX("Failed to Create the DirectInput Object.");
	}

	//	Look for a simple joystick we can use for this sample program.
	if(FAILED(m_lpDIObject->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumJoysticksCallback, NULL, DIEDFL_ATTACHEDONLY)))
		return false;

	if (m_joystickList.size())
		m_bJStick = true;

	//	Setup the joysticks.
	for (int i = 0; i < m_joystickList.size(); i++)
	{
		if (FAILED(m_joystickList[i].lpdiJoy->SetDataFormat(&c_dfDIJoystick2)))
		{
			sprintf(m_szBuffer, "Could not SetDataFormat for %s joypad.", m_joystickList[i].szJoyname);
			DIERRBOX(m_szBuffer);
		}

		if (m_bExclusive)
		{
			if (FAILED(m_joystickList[i].lpdiJoy->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE)))
			{
				sprintf(m_szBuffer, "Could not SetCooperativeLevel for %s joypad in Exclusive Mode", m_joystickList[i].szJoyname);
				DIERRBOX(m_szBuffer);
			}
		}
		else
		{
			if (FAILED(m_joystickList[i].lpdiJoy->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE)))
			{
				sprintf(m_szBuffer, "Could not SetCooperativeLevel for %s joypad.", m_joystickList[i].szJoyname);
				DIERRBOX(m_szBuffer);
			}
		}

		//	Set the Properties for the Joystick Axes.
		DIPROPRANGE dipr;
		dipr.lMin = -1024;
		dipr.lMax = +1024;
		dipr.diph.dwSize		= sizeof(DIPROPRANGE);
		dipr.diph.dwHeaderSize	= sizeof(DIPROPHEADER);
		dipr.diph.dwObj			= DIJOFS_X;					//	Change the X-Axis.
		dipr.diph.dwHow			= DIPH_BYOFFSET;

		m_joystickList[i].lpdiJoy->SetProperty(DIPROP_RANGE, &dipr.diph);

		//	Change the Y-Axis.
		dipr.diph.dwObj			= DIJOFS_Y;
		m_joystickList[i].lpdiJoy->SetProperty(DIPROP_RANGE, &dipr.diph);

		//	Setup a Dead Zone for the axises.
		DIPROPDWORD deadZone;
		deadZone.diph.dwSize	   = sizeof (deadZone);
		deadZone.diph.dwHeaderSize = sizeof (deadZone.diph);
		deadZone.diph.dwObj		   = DIJOFS_X;
		deadZone.diph.dwHow		   = DIPH_BYOFFSET;
		deadZone.dwData			   = 1000;

		//	Setup the X-Axis Dead Zone.
		m_joystickList[i].lpdiJoy->SetProperty(DIPROP_DEADZONE, &deadZone.diph);

		//	Setup the Y-Axis Dead Zone.
		deadZone.diph.dwObj		   = DIJOFS_Y;
		m_joystickList[i].lpdiJoy->SetProperty(DIPROP_DEADZONE, &deadZone.diph);

		if (FAILED(m_joystickList[i].lpdiJoy->Acquire()))
		{
			sprintf(m_szBuffer, "Could not Acquire for %s joypad.", m_joystickList[i].szJoyname);
			DIERRBOX(m_szBuffer);
		}
	}

	//	Return success.
	return true;
}

///////////////////////////////////////////////////////////////////
//	Function:	"ShutdownDirectInput"
//
//	Last Modified:		9/21/2006
//
//	Input	:	void
//
//	Return	:	void
//
//	Purpose	:	Shuts down all DirectInput Components.
///////////////////////////////////////////////////////////////////	
void CSGD_DirectInput::ShutdownDirectInput(void)
{
	// if we have joysticks
	if (GetJstick())
	{
		//	Release the Joystick
		for (int i = 0; i < m_joystickList.size(); i++)
		{
			m_joystickList[i].lpdiJoy->Unacquire();
			SAFE_RELEASE(m_joystickList[i].lpdiJoy);
		}

		//	Already released joystick devices.
		//	Now empty the list.
		m_joystickList.clear();
	}

	//	Release the Mouse.
	if (m_lpDIMouse)
	{
		m_lpDIMouse->Unacquire();
		SAFE_RELEASE(m_lpDIMouse);
	}

	//	Release the Keyboard
	if (m_lpDIKey)
	{
		m_lpDIKey->Unacquire();
		SAFE_RELEASE(m_lpDIKey);
	}

	//	Release the Direct Input Device.
	if (m_lpDIObject)
	{
		SAFE_RELEASE(m_lpDIObject);
	}
}

///////////////////////////////////////////////////////////////////
//	Function:	"ReadDevices"
//
//	Last Modified:		8/31/2004
//
//	Input	:	void
//
//	Return	:	true if all the devices were read.
//
//	Purpose	:	Gets the state of all the acquired devices.
///////////////////////////////////////////////////////////////////	
bool CSGD_DirectInput::ReadDevices(void)
{
	if (!ReadKeyboard())
		return false;

	if (!ReadBufferedKeyboard())
		return false;

	if (!ReadMouse())
		return false;

	if (!ReadBufferedMouse())
		return false;

	if (!ReadJoystick())
		return false;

	return true;
}

///////////////////////////////////////////////////////////////////
//	Function:	"ReadKeyboard"
//
//	Last Modified:		8/31/2004
//
//	Input	:	void
//
//	Return	:	true if it can get the Device State, false if it cannot acquire the Keyboard.
//
//	Purpose	:	Gets the Keyboard State every Frame.
///////////////////////////////////////////////////////////////////
bool CSGD_DirectInput::ReadKeyboard(void)
{
	HRESULT result;	//	For Error Tracking.

	//	Make sure the keyboard device was made.
	if (!m_lpDIKey)		return false;

	memset(&m_ucKeyState, 0, sizeof(m_ucKeyState));

	m_lpDIKey->Acquire();

	//	Keep attempting to acquire the Keyboard.
	if (result = m_lpDIKey->GetDeviceState(sizeof(m_ucKeyState), (LPVOID)m_ucKeyState) == DIERR_INPUTLOST)	   
	{
		//	Try to re-acquire the device.
		if (FAILED(result = m_lpDIKey->Acquire()))
		{
			//	Could not re-acquire the Keyboard.
			return false;
		}
	}

	//	Return success.
	return true;
}

///////////////////////////////////////////////////////////////////
//	Function:	"ReadBufferedKeyboard"
//
//	Last Modified:		8/31/2004
//
//	Input	:	void
//
//	Return	:	true if it can get the Data, false if it cannot acquire the Keyboard.
//
//	Purpose	:	Gets the Buffered Keyboard Data every frame.
//
//	NOTE	:	Not for use with GetBufferedKeyEx (use ReadKeyboard() for that).
///////////////////////////////////////////////////////////////////
bool CSGD_DirectInput::ReadBufferedKeyboard(void)
{
	HRESULT result;	//	For Error Tracking.

	//	Make sure the keyboard device was made.
	if (!m_lpDIKey)		return false;

	m_lpDIKey->Acquire();

	m_dwKeyElements = BUFFER_SIZE;
	memset(&m_didodKey, 0, sizeof(m_didodKey));
	//	Keep attempting to acquire the Keyboard.
	if (result = m_lpDIKey->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), m_didodKey, &m_dwKeyElements, 0) == DIERR_INPUTLOST)
	{
		//	Could not re-acquire the Keyboard.
		if (FAILED(result = m_lpDIKey->Acquire()))
			return false;
	}

	//	Return success.
	return true;
}

///////////////////////////////////////////////////////////////////
//	Function:	"GetKey"
//
//	Last Modified:		8/31/2004
//
//	Input	:	ucKey	-	The Direct Input Keyboard Constant such as DIK_ESCAPE.
//
//	Return	:	true if the Key was pressed.
//
//	Purpose	:	Gets the current (immediate) state of a Key.
///////////////////////////////////////////////////////////////////
bool CSGD_DirectInput::GetKey(UCHAR ucKey)
{
	//	Check to see if the key was pressed.
	if (m_ucKeyState[ucKey] & 0x80)
		return true;

	//	The Key was not Pressed.
	return false;
}

//	Disable the "not all control paths return a value" warning.
#pragma warning (disable : 4715)

///////////////////////////////////////////////////////////////////
//	Function:	"CheckKeys"
//
//	Last Modified:		8/31/2004
//
//	Input	:	void
//
//	Return	:	The key that the user pressed.
//
//	Purpose	:	Checks to see what exact button the User pressed.
///////////////////////////////////////////////////////////////////
char CSGD_DirectInput::CheckKeys(void)
{	
	//	Get the Current Keyboard State.
	if (!(GetKeyboardState(m_ucAsciiVals)))
		DIERRBOX("Could not GetKeyboardState.");

	unsigned short num = 0;
	UINT vkcode = 0;

	//	Go through each keyboard state.
	for (int i = 0; i < 256; i++)
	{
		if (m_ucKeyState[i] & 0x80)
		{
			//	Map the Scan Code from Direct Input to a Virtual Key value....
			vkcode = MapVirtualKeyEx(i, 1, m_keyLayout);
			//	...and tranlsate that Virtual Key into an Ascii Value.
			ToAsciiEx(vkcode, i, m_ucAsciiVals, &num, 0, m_keyLayout);
		}
	}

	//	Return the Ascii Value.
	return (char)num;
}

///////////////////////////////////////////////////////////////////
//	Function:	"CheckBufferedKeys"
//
//	Last Modified:		8/31/2004
//
//	Input	:	void
//
//	Return	:	The key that the user pressed this frame.
//
//	Purpose	:	Checks to see what exact button the User pressed, but with Buffered Input.
///////////////////////////////////////////////////////////////////
char CSGD_DirectInput::CheckBufferedKeys(void)
{
	//	Get the Current Keyboard State.
	if (!(GetKeyboardState(m_ucAsciiVals)))
		DIERRBOX("Could not GetKeyboardState.");

	unsigned short num = 0;
	UINT vkcode = 0;

	//	Go through each element in the Buffer.
	for (DWORD i = 0; i < m_dwKeyElements; i++)
	{
		for (unsigned int j = 0; j < 256; j++)
		{
			if ((m_didodKey[i].dwOfs == j) && (m_didodKey[i].dwData & 0x80))
			{
				vkcode = MapVirtualKeyEx(m_didodKey[i].dwOfs, 1, m_keyLayout);
				ToAsciiEx(vkcode, m_didodKey[i].dwOfs, m_ucAsciiVals, &num, 0, m_keyLayout);
			}
		}
	}

	//	Return the Ascii Value.
	return (char)num;	
}

//	Enable the "not all control paths return a value" warning.
#pragma warning (default : 4715)

///////////////////////////////////////////////////////////////////
//	Function:	"GetBufferedKey"
//
//	Last Modified:		8/31/2004
//
//	Input	:	ucKey	-	The Direct Input Keyboard Constant such as DIK_ESCAPE.
//
//	Return	:	true if the Key was pressed this frame.
//
//	Purpose	:	Gets the status of a Key using Buffered Input.
///////////////////////////////////////////////////////////////////
bool CSGD_DirectInput::GetBufferedKey(UCHAR ucKey)
{
	//	Go through each element in the Buffer.
	for (DWORD i = 0; i < m_dwKeyElements; i++)
	{
		//	If the Key is pressed return success.
		if ((m_didodKey[i].dwOfs == ucKey) && (m_didodKey[i].dwData & 0x80))
			return true;
	}

	//	Return Failure.
	return false;
}

///////////////////////////////////////////////////////////////////
//	Function:	"GetBufferedKeyEx"
//
//	Last Modified:		8/31/2004
//
//	Input	:	keytype	-	The Direct Input Keyboard Constant such as DIK_ESCAPE.
//
//	Return	:	true if Key was pressed this frame.
//
//	Purpose	:	Gets a Buffered Key without using DirectInput's Buffered Input.
///////////////////////////////////////////////////////////////////
bool CSGD_DirectInput::GetBufferedKeyEx(UCHAR ucKey)
{
	if (GetKey(ucKey))
	{
		if (!m_bKeysPressed[ucKey])
		{
			m_bKeysPressed[ucKey] = true;
			return true;
		}
	}
	else if (!GetKey(ucKey))
		m_bKeysPressed[ucKey] = false;

	//	The Key was not pressed.
	return false;
}

///////////////////////////////////////////////////////////////////
//	Function:	"ReadMouse"
//
//	Last Modified:		8/31/2004
//
//	Input	:	void
//
//	Return	:	true if able to get Device State.
//
//	Purpose	:	Gets the Mouse State every frame.
///////////////////////////////////////////////////////////////////
bool CSGD_DirectInput::ReadMouse(void)
{
	HRESULT result;	//	For Error Tracking.

	//	Make sure the Mouse Device was made.
	if (!m_lpDIMouse)	return false;

	if (FAILED(m_lpDIMouse->Poll()))
		m_lpDIMouse->Acquire();

	memset(&m_diMouseState, 0, sizeof(m_diMouseState));
	//	Keep attempting to acquire the Mouse.
	if (result = m_lpDIMouse->GetDeviceState(sizeof(m_diMouseState), &m_diMouseState) == DIERR_INPUTLOST)
	{
		//	Try to re-acquire the device.
		if (FAILED(result = m_lpDIMouse->Acquire()))
			//	Serious error - Could not re-acquire the Mouse.
			return false;
	}

	//	Return success.
	return true;
}

///////////////////////////////////////////////////////////////////
//	Function:	"ReadBufferedMouse"
//
//	Last Modified:		8/31/2004
//
//	Input	:	void
//
//	Return	:	true if it can get the Data, false if it cannot acquire the Mouse.
//
//	Purpose	:	Gets the Buffered Mouse Data every frame.
///////////////////////////////////////////////////////////////////
bool CSGD_DirectInput::ReadBufferedMouse(void)
{
	HRESULT result;	//	For Error Tracking.

	//	Make sure the Mouse Device was made.
	if (!m_lpDIMouse)	return false;

	if (FAILED(m_lpDIMouse->Poll()))
		m_lpDIMouse->Acquire();

	m_dwMouseElements = BUFFER_SIZE;
	memset(&m_didodMouse, 0, sizeof(m_didodMouse));
	//	Keep attempting to acquire the Mouse.
	if (result = m_lpDIMouse->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), m_didodMouse, &m_dwMouseElements, 0) == DIERR_INPUTLOST)
	{
		//	Could not re-acquire the Mouse.
		if (FAILED(result = m_lpDIMouse->Acquire()))
			return false;
	}

	if (m_dwMouseElements > 0)
		return true;

	//	Return success.
	return true;
}

///////////////////////////////////////////////////////////////////
//	Function:	"OnMouseButtonRelease"
//
//	Last Modified:		8/31/2004
//
//	Input	:	mButton	-	The Mouse button you wish to check for.
//
//	Return	:	true if Mouse Button was released.
//
//	Purpose	:	Check to see if the Mouse Button specified was released.
///////////////////////////////////////////////////////////////////
bool CSGD_DirectInput::OnMouseButtonRelease(BYTE mButton)
{
	//	Check to see if the button was pressed and set a flag.
	if (GetMouseButton(mButton))
	{
		if (!m_bMouseButtons[mButton])
			m_bMouseButtons[mButton] = true;
	}
	//	Checks to see if the button isn't pressed and if the flag is true then
	//	go ahead and change the flag and return true.
	else if (!GetMouseButton(mButton))
	{
		if (m_bMouseButtons[mButton])
		{
			m_bMouseButtons[mButton] = false;
			//	The Mouse Button was release.
			return true;
		}
	}

	//	Mouse button was not released.
	return false;
}

///////////////////////////////////////////////////////////////////
//	Function:	"GetMouseButton"
//
//	Last Modified:		8/31/2004
//
//	Input	:	mButton	-	The Mouse button you wish to check for.
//
//	Return	:	true if the Mouse Button was pressed.
//
//	Purpose	:	Gets the current (immediate) state of a Mouse Button.
///////////////////////////////////////////////////////////////////
bool CSGD_DirectInput::GetMouseButton(BYTE mButton)
{
	//	Check to see if the Mouse Button was pressed.
	if (m_diMouseState.rgbButtons[mButton] & 0x80)
		return true;

	//	Button Not Pressed.
	return false;
}

///////////////////////////////////////////////////////////////////
//	Function:	"GetBufferedMouseButton"
//
//	Last Modified:		8/31/2004
//
//	Input	:	mButton	-	The Mouse button you wish to check for.
//
//	Return	:	true if the Mouse Button was pressed this frame.
//
//	Purpose	:	Gets the status of a Mouse Button using Buffered Input.
///////////////////////////////////////////////////////////////////
bool CSGD_DirectInput::GetBufferedMouseButton(BYTE mButton)
{
	//	Go through each element in the Buffer.
	for (DWORD i = 0; i < m_dwMouseElements; i++)
	{
		//	If the Key is pressed return success.  Add 12 because DirectInput uses 
		//	DIMOFS_BUTTON0 and so forth for the check on this.
		if ((m_didodMouse[i].dwOfs == (DWORD)(mButton + 12)) && (m_didodMouse[i].dwData & 0x80))
			return true;
	}

	//	Return Failure.
	return false;
}

///////////////////////////////////////////////////////////////////
//	Function:	"GetMouseAxis"
//
//	Last Modified:		8/31/2004
//
//	Input	:	lAxis	-	The Mouse Axis you wish to check for.
//
//	Return	:	The Movement of the Mouse along the axis.
//
//	Purpose	:	Gets the Status of a Mouse Axis.
//
//  NOTE: Reports the relative movement of the mouse this frame 
//  (i.e. the mouse moved 5 pixels in this direction)
///////////////////////////////////////////////////////////////////
LONG CSGD_DirectInput::GetMouseAxis(LONG lAxis)
{
	//	Check the Axis and return its state.
	switch (lAxis)
	{
	case XAXIS:
		return m_diMouseState.lX;
		break;

	case YAXIS:
		return m_diMouseState.lY;
		break;

	case ZAXIS:
		return m_diMouseState.lZ;
		break;
	};

	return 0;
}

///////////////////////////////////////////////////////////////////
//	Function:	"ReadJoystick"
//
//	Last Modified:		9/21/2006
//
//	Input	:	void
//
//	Return	:	true if able to get the Device State.
//
//	Purpose	:	Reads the Joystick State every frame.
///////////////////////////////////////////////////////////////////
bool CSGD_DirectInput::ReadJoystick(void)
{
	//	If there is no joystick then don't bother reading for it.
	if (!GetJstick())		return false;

	//	Poll the device to read the current state.
	HRESULT result;

	for (int i = 0; i < m_joystickList.size(); i++)
	{
		//	Make sure the joystick was created.
		if (!m_joystickList[i].lpdiJoy)	continue;

		result = m_joystickList[i].lpdiJoy->Poll(); 
		memset(&m_joystickList[i].diJoystate, 0, sizeof(m_joystickList[i].diJoystate));

		if (FAILED(result))
		{
			//	Try to Acquire the Joystick.
			result = m_joystickList[i].lpdiJoy->Acquire();

			//	Make sure the joystick is plugged in.
			if (!m_joystickList[i].bUnplugged)
				//	Ooops the joystick was unplugged.
				if (result == DIERR_UNPLUGGED)
				{
					//MessageBox(main_hwnd, "You have unplugged your joystick!", "Error", MB_OK);
					m_joystickList[i].bUnplugged = true;
				}

				//	Try to re-acquire the joystick if it was unplugged.
				if (m_joystickList[i].bUnplugged)
					//	If you succeed in getting the joystick the set the flag
					//	back to being plugged in.
					if (m_joystickList[i].lpdiJoy->Acquire() == DI_OK)
						m_joystickList[i].bUnplugged = false;

				//	If no input from joystick re-acquire.
				if (result == DIERR_INPUTLOST)
					result = m_joystickList[i].lpdiJoy->Acquire();

				//	Return success.
				return true;
		}

		//	Get the input's device state
		if(FAILED(result = m_joystickList[i].lpdiJoy->GetDeviceState(sizeof(m_joystickList[i].diJoystate), 
			&m_joystickList[i].diJoystate)))
			return false; // The device should have been acquired during the Poll()
	}

	//	Return success.
	return true;
}

///////////////////////////////////////////////////////////////////
//	Function:	"GetJoystickButton"
//
//	Last Modified:		9/21/2006
//
//	Input	:	jButton	-	The Joystick Button you wish to check for.
//				nNum	-	The number of the Joystick you wish to check 
//							that the button was pressed on (i.e. which player: 0, 1, etc.).
//
//	Return	:	true if the button is currently pressed.
//
//	Purpose	:	Gets the current (immediate) state of a Joystick's Button.
///////////////////////////////////////////////////////////////////
bool CSGD_DirectInput::GetJoystickButton(BYTE jButton, int nNum)
{
	//	The Data is valid.
	if (!GetJstick())
		return false;

	//	Make sure the number is valid.
	if (nNum < 0 || nNum >= m_joystickList.size())
		return false;

	if (m_joystickList[nNum].diJoystate.rgbButtons[jButton] & 0x80)
		return true;

	//	Return Failure.
	return false;
}

///////////////////////////////////////////////////////////////////
//	Function:	"GetBufferedJoyButton"
//
//	Last Modified:		9/21/2006
//
//	Input	:	jButton	-	The Joystick Button you wish to check for.
//				nNum	-	The number of the Joystick you wish to check 
//							that the button was pressed on (i.e. which player: 0, 1, etc.).
//
//	Return	:	true if the button was pressed this frame.
//
//	Purpose	:	Gets a Buffered Joystick Button without using DirectInput's Buffered Input.
///////////////////////////////////////////////////////////////////
bool CSGD_DirectInput::GetBufferedJoyButton(BYTE jButton, int nNum)
{
	//	If there are no Joysticks then
	//	Don't try to do anything.
	if (!GetJstick())		return false;

	//	Make sure the number is valid.
	if (nNum < 0 || nNum >= m_joystickList.size())
		return false;

	if (GetJoystickButton(jButton, nNum))
	{
		if (!m_joystickList[nNum].bJoyButtons[jButton])
		{
			m_joystickList[nNum].bJoyButtons[jButton] = true;
			return true;
		}
	}
	else if (!GetJoystickButton(jButton, nNum))
		m_joystickList[nNum].bJoyButtons[jButton] = false;

	//	Return Failure.
	return false;
}

///////////////////////////////////////////////////////////////////
//	Function:	"GetJoystickDir"
//
//	Last Modified:		9/21/2006
//
//	Input	:	nDir	-	The Desired direction to check for.
//				nNum	-	The number of the Joystick you wish to check 
//							that the button was pressed on (i.e. which player: 0, 1, etc.).
//
//	Return	:	true if the Joystick Axis is being pressed.
//
//	Purpose	:	Gets the status of a Joystick Axis (Direction).
///////////////////////////////////////////////////////////////////
bool CSGD_DirectInput::GetJoystickDir(int nDir, int nNum)
{
	//	The Data is valid.
	if (!GetJstick())		return false;

	//	Make sure the number is valid.
	if (nNum < 0 || nNum >= m_joystickList.size())
		return false;

	//	Check for the Direction.
	switch (nDir)
	{
	case JOYSTICK_RIGHT:
		if (m_joystickList[nNum].diJoystate.lX > 0)
			return true;
		break;

	case JOYSTICK_LEFT:
		if (m_joystickList[nNum].diJoystate.lX < 0)
			return true;
		break;

	case JOYSTICK_UP:
		if (m_joystickList[nNum].diJoystate.lY < 0)
			return true;
		break;

	case JOYSTICK_DOWN:
		if (m_joystickList[nNum].diJoystate.lY > 0)
			return true;
		break;
	};

	//	Return Failure.
	return false;
}

///////////////////////////////////////////////////////////////////
//	Function:	"ReturnJoystickDir"
//
//	Last Modified:		8/31/2004
//
//	Input	:	nDir	-	The Desired direction to check for.
//				nNum	-	The number of the Joystick you wish to check 
//							that the button was pressed on (i.e. which player: 0, 1, etc.).
//
//	Return	:	The actual number in the range that the axis is currently in.
//
//	Purpose	:	Gets the amount the Joystick Axis is pressed in the Direction.
///////////////////////////////////////////////////////////////////
int CSGD_DirectInput::ReturnJoystickDir(int nDir, int nNum)
{
	//	Make sure the number is valid.
	if (nNum < 0 || nNum >= m_joystickList.size() || !m_joystickList.size())
		return -1;

	//	Check for the Direction.
	switch (nDir)
	{
	case JOYSTICK_RIGHT:
	case JOYSTICK_LEFT:
		return m_joystickList[nNum].diJoystate.lX;
		break;

	case JOYSTICK_UP:
	case JOYSTICK_DOWN:
		return m_joystickList[nNum].diJoystate.lY;
		break;
	};

	//	Failure.
	return -1;
}

///////////////////////////////////////////////////////////////////
//	Function:	"GetJoystickName"
//
//	Last Modified:		9/21/2006
//
//	Input	:	nNum	-	The number of the Joystick you wish to check 
//							that the button was pressed on (i.e. which player: 0, 1, etc.).
//
//	Return	:	The name of the joystick.
//
//	Purpose	:	Returns the name of the joystick (i.e. "Logitech Dual Action Gamepad")
///////////////////////////////////////////////////////////////////
char *CSGD_DirectInput::GetJoystickName(int nNum)
{
	//	The Data is valid.
	if (!GetJstick()) 	return 0;

	//	Make sure the number is valid.
	if (nNum < 0 || nNum >= m_joystickList.size())
		return 0;

	//	Return the name of the joystick.
	return m_joystickList[nNum].szJoyname;
}
