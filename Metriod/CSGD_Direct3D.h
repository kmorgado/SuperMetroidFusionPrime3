////////////////////////////////////////////////////////
//  File			:	“CSGD_Direct3D.h”
//
//  Author			:	Jensen Rivera (JR)
//
//  Date Created	:	5/25/2006
//
//  Purpose			:	Wrapper class for Direct3D.
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

// The include files for Direct3D9
#include <d3d9.h>
#include <d3dx9.h>

// The library files for Direct3D9
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment (lib, "dxguid.lib")

// Macros
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)	if (p) { p->Release(); p = NULL; }
#endif

#ifndef DXERROR
#define DXERROR(a)	{ OutputDebugString(a); return false; }
#endif

class CSGD_Direct3D
{
	private:
		// The Direct3D object.
		LPDIRECT3D9					m_lpDirect3DObject;

		// The Direct3D Device.
		LPDIRECT3DDEVICE9			m_lpDirect3DDevice;

		// The Direct3DX Sprite Interface.
		LPD3DXSPRITE				m_lpSprite;

		// The Direct3DX Font Interface.
		LPD3DXFONT					m_lpFont;

		// The Direct3DX Line Interface
		LPD3DXLINE					m_lpLine;

		// The presentation parameters of the device.
		D3DPRESENT_PARAMETERS		m_PresentParams;

		// The handle to the window Direct3D is initialized in.
		HWND						m_hWnd;

		// The single static object of this class.
		static CSGD_Direct3D		m_Instance;

	private:
		///////////////////////////////////////////////////////////////////
		//	Function:	"CSGD_Direct3D(Constructor)"
		///////////////////////////////////////////////////////////////////
		CSGD_Direct3D(void);
		CSGD_Direct3D(CSGD_Direct3D &ref);
		CSGD_Direct3D &operator=(CSGD_Direct3D &ref);

	public:
		///////////////////////////////////////////////////////////////////
		//	Function:	"~CSGD_Direct3D(Destructor)"
		///////////////////////////////////////////////////////////////////		
		virtual ~CSGD_Direct3D(void);

		///////////////////////////////////////////////////////////////////
		//	Function:	"GetInstance"
		//
		//	Last Modified:		5/25/2006
		//
		//	Input:		void
		//
		//	Return:		An instance to this class.
		//
		//	Purpose:	Returns an instance to this class.
		///////////////////////////////////////////////////////////////////
		static CSGD_Direct3D *GetInstance(void);

		///////////////////////////////////////////////////////////////////
		//	Function:	"CSGD_Direct3D(Accessors)"
		//
		//	Last Modified:		5/25/2006
		//
		//	Input:		void
		//
		//	Return:		A pointer to a data member in this class.
		//
		//	Purpose:	Accesses data members in this class.
		///////////////////////////////////////////////////////////////////
		LPDIRECT3D9					GetDirect3DObject(void);
		LPDIRECT3DDEVICE9			GetDirect3DDevice(void);
		LPD3DXSPRITE				GetSprite(void);
		LPD3DXLINE					GetLine(void);
		const D3DPRESENT_PARAMETERS	*GetPresentParams(void);

		///////////////////////////////////////////////////////////////////
		//	Function:	"InitDirect3D"
		//
		//	Last Modified:		5/25/2006
		//
		//	Input:		hWnd		A handle to the window to initialize
		//							Direct3D in.
		//				nWidth		The width to initialize the device into.
		//				nHeight		The height to initialize the device into.
		//				bWindowed	The screen mode to initialize the device into.
		//				bVsync		true to put the device into vsynced display mode.
		//
		//	Return:		true if successful.
		//
		//	Purpose:	Initializes Direct3D9.
		///////////////////////////////////////////////////////////////////
		bool InitDirect3D(HWND hWnd, int nWidth, int nHeight, bool bWindowed = false, bool bVsync = true);

		///////////////////////////////////////////////////////////////////
		//	Function:	"ShutdownDirect3D"
		//
		//	Last Modified:		5/25/2006
		//
		//	Input:		void
		//
		//	Return:		void
		//
		//	Purpose:	Shuts down Direct3D9.
		///////////////////////////////////////////////////////////////////
		void ShutdownDirect3D(void);

		///////////////////////////////////////////////////////////////////
		//	Function:	"Clear"
		//
		//	Last Modified:		5/25/2006
		//
		//	Input:		cRed		The amount of red to clear the screen with (0-255).
		//				cGreen		The amount of green to clear the screen with (0-255).
		//				cBlue		The amount of blue to clear the screen with (0-255).
		//
		//	Return:		void
		//
		//	Purpose:	Clears the screen to a given color.
		///////////////////////////////////////////////////////////////////
		void Clear(unsigned char cRed, unsigned char cGreen, unsigned char cBlue);

		///////////////////////////////////////////////////////////////////
		//	Function:	"DeviceBegin"
		//
		//	Last Modified:		5/25/2006
		//
		//	Input:		void
		//
		//	Return:		true if successful.
		//
		//	Purpose:	Begins the device for rendering.
		///////////////////////////////////////////////////////////////////
		bool DeviceBegin(void);

		///////////////////////////////////////////////////////////////////
		//	Function:	"SpriteBegin"
		//
		//	Last Modified:		5/25/2006
		//
		//	Input:		void
		//
		//	Return:		true if successful.
		//
		//	Purpose:	Begins the sprite for rendering.
		///////////////////////////////////////////////////////////////////
		bool SpriteBegin(void);

		///////////////////////////////////////////////////////////////////
		//	Function:	"LineBegin"
		//
		//	Last Modified:		5/25/2006
		//
		//	Input:		void
		//
		//	Return:		true if successful.
		//
		//	Purpose:	Begins the line for rendering.
		///////////////////////////////////////////////////////////////////
		bool LineBegin(void);

		///////////////////////////////////////////////////////////////////
		//	Function:	"DeviceEnd"
		//
		//	Last Modified:		5/25/2006
		//
		//	Input:		void
		//
		//	Return:		true if successful.
		//
		//	Purpose:	Ends the device for rendering.
		///////////////////////////////////////////////////////////////////
		bool DeviceEnd(void);

		///////////////////////////////////////////////////////////////////
		//	Function:	"SpriteEnd"
		//
		//	Last Modified:		5/25/2006
		//
		//	Input:		void
		//
		//	Return:		true if successful.
		//
		//	Purpose:	Ends the sprite for rendering.
		///////////////////////////////////////////////////////////////////
		bool SpriteEnd(void);

		///////////////////////////////////////////////////////////////////
		//	Function:	"LineEnd"
		//
		//	Last Modified:		5/25/2006
		//
		//	Input:		void
		//
		//	Return:		true if successful.
		//
		//	Purpose:	Ends the line for rendering.
		///////////////////////////////////////////////////////////////////
		bool LineEnd(void);

		///////////////////////////////////////////////////////////////////
		//	Function:	"Present"
		//
		//	Last Modified:		5/25/2006
		//
		//	Input:		void
		//
		//	Return:		void
		//
		//	Purpose:	Renders your back buffer to the screen.
		///////////////////////////////////////////////////////////////////
		void Present(void);

		///////////////////////////////////////////////////////////////////
		//	Function:	"ChangeDisplayParam"
		//
		//	Last Modified:		5/25/2006
		//
		//	Input:		nWidth		The width to change the device to.
		//				nHeight		The height to change the device to.
		//				bWindowed	The mode to put the window in.
		//
		//	Return:		void
		//
		//	Purpose:	Changes the display parameters of the device.
		///////////////////////////////////////////////////////////////////
		void ChangeDisplayParam(int nWidth, int nHeight, bool bWindowed);

		///////////////////////////////////////////////////////////////////
		//	Function:	"DrawRect"
		//
		//	Last Modified:		5/25/2006
		//
		//	Input:		rRt		The region of the screen to fill.
		//				cRed	The amount of red to fill that region with (0-255).
		//				cGreen	The amount of green to fill that region with (0-255).
		//				cBlue	The amount of blue to fill that region with (0-255).
		//
		//	Return:		void
		//
		//	Purpose:	Draws a rectangle of a given color to the screen.
		///////////////////////////////////////////////////////////////////
		void DrawRect(RECT rRt, unsigned char cRed, unsigned char cGreen, unsigned char cBlue);

		///////////////////////////////////////////////////////////////////
		//	Function:	"DrawLine"
		//
		//	Last Modified:		5/25/2006
		//
		//	Input:		nX1		The starting x of the line.
		//				nY1		The starting y of the line.
		//				nX2		The ending x of the line.
		//				nY2		The ending y of the line.
		//				cRed	The amount of red to draw the line with (0-255).
		//				cGreen	The amount of green to draw the line with (0-255).
		//				cBlue	The amount of blue to draw the line with (0-255).
		//
		//	Return:		void
		//
		//	Purpose:	Draws a rectangle of a given color to the screen.
		///////////////////////////////////////////////////////////////////
		void DrawLine(int nX1, int nY1, int nX2, int nY2, unsigned char cRed, unsigned char cGreen, unsigned char cBlue);

		///////////////////////////////////////////////////////////////////
		//	Function:			"DrawText"
		//
		//	Last Modified:		5/25/2006
		//
		//	Input:		lpzText		The text to draw to the screen.
		//				nX			The x position to draw the text at.
		//				nY			The y position to draw the text at.
		//				cRed	The amount of red to draw the text with (0-255).
		//				cGreen	The amount of green to draw the text with (0-255).
		//				cBlue	The amount of blue to draw the text with (0-255).
		//
		//	Return:		void
		//
		//	Purpose:	Draws text to the screen with a given color.
		///////////////////////////////////////////////////////////////////
		void DrawText(char *lpzText, int nX, int nY, unsigned char cRed, unsigned char cGreen, unsigned char cBlue);
};
