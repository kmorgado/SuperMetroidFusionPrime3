////////////////////////////////////////////////////////
//	File			:	"CSGD_DirectInput.h"
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

#pragma once

//	The version of DirectInput to use.
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <vector>
using std::vector;

#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")
#pragma warning (disable : 4996)

//	Macro to safely Release COM Objects.
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)		if (p) { p->Release(); p = NULL; }
#endif

//	MessageBox for Errors that occur within DirectInput.
#ifndef DIERRBOX
#define DIERRBOX(a)	{ MessageBox(m_hWnd, a, "CSGD_DirectInput Error", MB_OK | MB_ICONEXCLAMATION); return false; }
#endif

typedef unsigned char UCHAR;
typedef unsigned char BYTE;

//	Defines for the Mouse.
enum MouseAxises  { XAXIS = 1, YAXIS, ZAXIS, };
enum MouseButtons { M_BUTTON_LEFT = 0, M_BUTTON_RIGHT, M_BUTTON_MIDDLE, M_BUTTON_BACK, };

//	Defines for the Joystick.
enum JoyButtons	  {	JOYSTICK_X = 0, JOYSTICK_B, JOYSTICK_A, JOYSTICK_Y, JOYSTICK_L, JOYSTICK_R, JOYSTICK_L2,
					JOYSTICK_R2, JOYSTICK_SELECT, JOYSTICK_START, JOYSTICK_RIGHT, JOYSTICK_LEFT, JOYSTICK_UP,
					JOYSTICK_DOWN, };

//	Defines for setting up Axises using Action Mapping.
enum ActionAxis  { ACTIONXLEFT = 500, ACTIONXRIGHT, ACTIONYUP, ACTIONYDOWN, ACTIONZUP, ACTIONZDOWN };

#define BUFFER_SIZE 10										//	Arbitrary number of buffer element for Buffered Input.

struct JoystickData
{
	char szJoyname[_MAX_PATH];								//	Hold the name of the Joystick.
	bool bJoyButtons[128];									//	Used for Buffered Joystick Input.
	bool bUnplugged;										//	Is the Joystick currently plugged in?
	DIJOYSTATE2			 diJoystate;						//	Get's Joystick Data.
	LPDIRECTINPUTDEVICE8 lpdiJoy;							//	Joystick Device.
};

class CSGD_DirectInput
{
private:
	LPDIRECTINPUT8		 m_lpDIObject;					//	Direct Input COM Object.
	LPDIRECTINPUTDEVICE8 m_lpDIKey;						//	Direct Input Device for the Keyboard.
	LPDIRECTINPUTDEVICE8 m_lpDIMouse;					//	Direct Input Device for the Mouse.
	DIDEVICEOBJECTDATA   m_didodKey[BUFFER_SIZE];		//	Receives buffered data for the Keyboard.
	DIDEVICEOBJECTDATA	 m_didodMouse[BUFFER_SIZE];		//	Receives buffered data for the Mouse.

	bool	  m_bExclusive;								//	Determines the cooperation level for the devices.
	bool	  m_bJStick;								//	Determine wether or not there is a Joystick.
	char	  m_szBuffer[128];							//	For Error Tracking.
	HKL	 	  m_keyLayout;								//	The Keyboard Layout.
	HWND	  m_hWnd;									//	Main Window Handle.
	HINSTANCE m_hInstance;								//	Main Application Instance.

	UCHAR		  m_ucKeyState[256];					//	Holds Keyboard Data.
	bool		  m_bKeysPressed[256];					//	Used for BufferedKeyEx.
	BYTE		  m_ucAsciiVals[256];					//	Gonna hold the DAMN ascii value of a given key.
	DIMOUSESTATE2 m_diMouseState;						//	Holds Mouse	Data.
	bool		  m_bMouseButtons[8];					//	Used for OnMouseButtonRelease Method.
	DWORD		  m_dwKeyElements;						//	Number of Elements in the Buffer for the Keyboard.
	DWORD		  m_dwMouseElements;					//	Number of Elements in the Buffer for the Mouse.

	vector<JoystickData> m_joystickList;				//	Holds the joysticks that have been made.

	static CSGD_DirectInput m_Instance;					//	The instance of this class.

	//	Constructor.
	CSGD_DirectInput();
	//	Copy Constructor.
	CSGD_DirectInput(const CSGD_DirectInput&);
	//	Assignment Operator.
	CSGD_DirectInput &operator = (const CSGD_DirectInput&);

public:

	//	Destructor.
	virtual ~CSGD_DirectInput() { ShutdownDirectInput(); }

	///////////////////////////////////////////////////////////////////
	//	Function:	"GetInstance"
	//
	//	Last Modified:		8/04/2006
	//
	//	Input	:	void
	//
	//	Return	:	An instance to this class.
	//
	//	Purpose	:	Gets an instance to this class.
	///////////////////////////////////////////////////////////////////
	static CSGD_DirectInput *GetInstance(void);

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
	friend BOOL CALLBACK EnumJoysticksCallback(const DIDEVICEINSTANCE *lpddi, VOID *pVoid);

	///////////////////////////////////////////////////////////////////
	//	Function:	"GetJstick"
	//
	//	Last Modified:		8/31/2004
	//
	//	Input	:	void
	//
	//	Return	:	true if there is a Joystick, false otherwise.
	//
	//	Purpose	:	Returns the State of jstick bool.
	///////////////////////////////////////////////////////////////////
	inline bool GetJstick(void) { return m_bJStick; }

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
	bool InitDevices(HWND hWnd, HINSTANCE hMainInstance, bool bExclusive = false);

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
	bool InitKeyboard(HWND hWnd, HINSTANCE hMainInstance, bool bExclusive = false);

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
	bool InitMouse(HWND hWnd, HINSTANCE hMainInstance, bool bExclusive = false);

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
	bool InitJoysticks(HWND hWnd, HINSTANCE hMainInstance, bool bExclusive = false);

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
	void ShutdownDirectInput(void);

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
	bool ReadDevices(void);

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
	bool ReadKeyboard(void);

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
	bool ReadBufferedKeyboard(void);

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
	bool GetKey(UCHAR ucKey);

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
	char CheckKeys(void);

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
	char CheckBufferedKeys(void);

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
	bool GetBufferedKey(UCHAR ucKey);

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
	bool GetBufferedKeyEx(UCHAR ucKey);

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
	bool ReadMouse(void);

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
	bool ReadBufferedMouse(void);

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
	bool OnMouseButtonRelease(BYTE mButton);

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
	bool GetMouseButton(BYTE mButton);

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
	bool GetBufferedMouseButton(BYTE mButton);

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
	LONG GetMouseAxis(LONG lAxis);

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
	bool ReadJoystick(void);

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
	bool GetJoystickButton(BYTE jButton, int nNum = 0);

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
	bool GetBufferedJoyButton(BYTE jButton, int nNum = 0);

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
	bool GetJoystickDir(int nDir, int nNum = 0);

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
	int ReturnJoystickDir(int nDir, int nNum = 0);

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
	char *GetJoystickName(int nNum = 0);
};

