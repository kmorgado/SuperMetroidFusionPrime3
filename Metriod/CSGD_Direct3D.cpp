////////////////////////////////////////////////////////
//  File			:	“CSGD_Direct3D.cpp”
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

#include "CSGD_Direct3D.h"

// Initialize the static object.
CSGD_Direct3D CSGD_Direct3D::m_Instance;

///////////////////////////////////////////////////////////////////
//	Function:	"CSGD_Direct3D(Constructor)"
///////////////////////////////////////////////////////////////////
CSGD_Direct3D::CSGD_Direct3D(void)
{
	m_lpDirect3DObject	= NULL;
	m_lpDirect3DDevice	= NULL;
	m_lpSprite			= NULL;
	m_lpFont			= NULL;
	m_lpLine			= NULL;
	memset(&m_PresentParams, 0, sizeof(D3DPRESENT_PARAMETERS));
}

///////////////////////////////////////////////////////////////////
//	Function:	"~CSGD_Direct3D(Destructor)"
///////////////////////////////////////////////////////////////////		
CSGD_Direct3D::~CSGD_Direct3D(void)
{
}

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
CSGD_Direct3D *CSGD_Direct3D::GetInstance(void)
{
	return &m_Instance;
}

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
LPDIRECT3D9					CSGD_Direct3D::GetDirect3DObject(void)	{ return m_lpDirect3DObject; }
LPDIRECT3DDEVICE9			CSGD_Direct3D::GetDirect3DDevice(void)	{ return m_lpDirect3DDevice; }
LPD3DXSPRITE				CSGD_Direct3D::GetSprite(void)			{ return m_lpSprite;		 }
LPD3DXLINE					CSGD_Direct3D::GetLine(void)				{ return m_lpLine;			 }
const D3DPRESENT_PARAMETERS	*CSGD_Direct3D::GetPresentParams(void)	{ return &m_PresentParams;	 }

///////////////////////////////////////////////////////////////////
//	Function:	"InitDirect3D"
//
//	Last Modified:		5/25/2006
//
//	Input:		hWnd		Handle to the window to initialize
//							Direct3D to render to.
//				nWidth		The backbuffer width of the device.
//				nHeight		The backbuffer height of the device.
//				bWindowed	The screen mode to set the device to (Windowed or Full screen).
//				bVsync		true to turn on vsync (locks game to approx 60 FPS).
//
//	Return:		true if successful.
//
//	Purpose:	Initializes Direct3D9.
///////////////////////////////////////////////////////////////////
bool CSGD_Direct3D::InitDirect3D(HWND hWnd, int nWidth, int nHeight, bool bWindowed, bool bVsync)
{
	// Make sure the hWnd is valid.
	if (!hWnd) return false;

	// Set the handle to the window.
	m_hWnd = hWnd;

	// Create the Direct3D9 Object.
	m_lpDirect3DObject = Direct3DCreate9(D3D_SDK_VERSION);

	// Make sure the object is valid.
	if (!m_lpDirect3DObject) DXERROR("Failed to Create D3D Object");

	// Setup the parameters for using Direct3D.
	m_PresentParams.BackBufferWidth				= nWidth;
	m_PresentParams.BackBufferHeight			= nHeight;
	m_PresentParams.BackBufferFormat			= (bWindowed) ? D3DFMT_UNKNOWN : D3DFMT_R5G6B5;
	m_PresentParams.BackBufferCount				= 1;
	m_PresentParams.MultiSampleType				= D3DMULTISAMPLE_NONE;
	m_PresentParams.MultiSampleQuality			= 0;
	m_PresentParams.SwapEffect					= D3DSWAPEFFECT_COPY;
	m_PresentParams.hDeviceWindow				= hWnd;
	m_PresentParams.Windowed					= bWindowed;
	m_PresentParams.EnableAutoDepthStencil		= false;
	m_PresentParams.Flags						= D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;
	m_PresentParams.FullScreen_RefreshRateInHz	= D3DPRESENT_RATE_DEFAULT;
	m_PresentParams.PresentationInterval		= (bVsync) ? D3DPRESENT_INTERVAL_DEFAULT : D3DPRESENT_INTERVAL_IMMEDIATE;

	// Create the Direct3D Device.
	if (FAILED(m_lpDirect3DObject->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&m_PresentParams, &m_lpDirect3DDevice)))
		DXERROR("Failed to Create D3D Device");

	// Create Sprite Object.
	if (FAILED(D3DXCreateSprite(m_lpDirect3DDevice, &m_lpSprite)))
		DXERROR("Failed to Create the Sprite object");

	// Create the Line Object.
	if (FAILED(D3DXCreateLine(m_lpDirect3DDevice, &m_lpLine)))
		DXERROR("Failed to Create the Line Object");

	// Setup Line parameters.
	m_lpLine->SetAntialias(TRUE);
	m_lpLine->SetWidth(3.0f);

	//SetGammaEx(0);

	//	Return success.
	return true;
}

///////////////////////////////////////////////////////////////////
//	Function:	"Shutdown"
//
//	Last Modified:		5/25/2006
//
//	Input:		void
//
//	Return:		void
//
//	Purpose:	Shuts down Direct3D9.
///////////////////////////////////////////////////////////////////
void CSGD_Direct3D::ShutdownDirect3D(void)
{
	SAFE_RELEASE(m_lpFont);
	SAFE_RELEASE(m_lpLine);
	SAFE_RELEASE(m_lpSprite);
	SAFE_RELEASE(m_lpDirect3DDevice);
	SAFE_RELEASE(m_lpDirect3DObject);
}

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
void CSGD_Direct3D::Clear(unsigned char cRed, unsigned char cGreen, unsigned char cBlue)
{
	m_lpDirect3DDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(cRed, cGreen, cBlue), 1.0f, 0);

	// Check for ALT+TAB.
	if (m_lpDirect3DDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
	{
		m_lpLine->OnLostDevice();
		m_lpSprite->OnLostDevice();
		m_lpDirect3DDevice->Reset(&m_PresentParams);
		m_lpSprite->OnResetDevice();
		m_lpLine->OnResetDevice();
	}
}

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
bool CSGD_Direct3D::DeviceBegin(void)
{
	if (FAILED(m_lpDirect3DDevice->BeginScene()))
		DXERROR("Failed to begin device scene.")

		return true;
}

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
bool CSGD_Direct3D::SpriteBegin(void)
{
	if (FAILED(m_lpSprite->Begin(D3DXSPRITE_ALPHABLEND)))
		DXERROR("Failed to begin sprite scene.")

		return true;
}

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
bool CSGD_Direct3D::LineBegin(void)
{
	if (FAILED(m_lpLine->Begin()))
		DXERROR("Failed to begin line scene.")

		return true;
}

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
bool CSGD_Direct3D::DeviceEnd(void)
{
	if (FAILED(m_lpDirect3DDevice->EndScene()))
		DXERROR("Failed to end device scene.")

		return true;
}

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
bool CSGD_Direct3D::SpriteEnd(void)
{
	if (FAILED(m_lpSprite->End()))
		DXERROR("Failed to end sprite scene.")

		return true;
}

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
bool CSGD_Direct3D::LineEnd(void)
{
	if (FAILED(m_lpLine->End()))
		DXERROR("Failed to end line scene.")

		return true;
}

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
void CSGD_Direct3D::Present(void)
{
	m_lpDirect3DDevice->Present(NULL, NULL, NULL, NULL);
}

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
void CSGD_Direct3D::ChangeDisplayParam(int nWidth, int nHeight, bool bWindowed)
{
	// Set the new Presentation Parameters.
	m_PresentParams.BackBufferFormat	= (bWindowed) ? D3DFMT_UNKNOWN : D3DFMT_R5G6B5;
	m_PresentParams.Windowed			= bWindowed;
	m_PresentParams.BackBufferWidth		= nWidth;
	m_PresentParams.BackBufferHeight	= nHeight;

	// Set the window to an overlapped window.
	if (bWindowed)
		SetWindowLong(m_hWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);
	// Change the window to a popup window.
	else
		SetWindowLong(m_hWnd, GWL_STYLE, WS_POPUP);

	// Let windows know the window has changed.
	SetWindowPos(m_hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_SHOWWINDOW | SWP_NOMOVE | SWP_NOSIZE);

	// Reset the device.
	m_lpLine->OnLostDevice();
	m_lpSprite->OnLostDevice();
	m_lpDirect3DDevice->Reset(&m_PresentParams);
	m_lpSprite->OnResetDevice();
	m_lpLine->OnResetDevice();

	//	Set the window to the middle of the screen.
	if (bWindowed)
	{
		SetWindowPos(m_hWnd, HWND_TOP,	(GetSystemMetrics(SM_CXSCREEN)>>1) - (nWidth>>1),
										(GetSystemMetrics(SM_CYSCREEN)>>1) - (nHeight>>1),
										nWidth, nHeight, SWP_SHOWWINDOW);
	}
}

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
void CSGD_Direct3D::DrawRect(RECT rRt, unsigned char cRed, unsigned char cGreen, unsigned char cBlue)
{
	D3DRECT d3dRect;
	d3dRect.x1 = rRt.left;
	d3dRect.y1 = rRt.top;
	d3dRect.x2 = rRt.right;
	d3dRect.y2 = rRt.bottom;

	m_lpDirect3DDevice->Clear(1, &d3dRect, D3DCLEAR_TARGET, D3DCOLOR_XRGB(cRed, cGreen, cBlue), 1.0f, 0);
}

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
void CSGD_Direct3D::DrawLine(int nX1, int nY1, int nX2, int nY2, unsigned char cRed, unsigned char cGreen, unsigned char cBlue)
{
	D3DXVECTOR2 verts[2];

	verts[0].x = (float)nX1;
	verts[0].y = (float)nY1;
	verts[1].x = (float)nX2;
	verts[1].y = (float)nY2;

	m_lpLine->Draw(verts, 2, D3DCOLOR_XRGB(cRed, cGreen, cBlue));
}

///////////////////////////////////////////////////////////////////
//	Function:	"DrawText"
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
void CSGD_Direct3D::DrawText(char *lpzText, int nX, int nY, unsigned char cRed, unsigned char cGreen, unsigned char cBlue)
{
	// Pointer to the Back Buffer.
	LPDIRECT3DSURFACE9 d3dBackBuffer = 0;

	// Get the Back Buffer from the Device.
	m_lpDirect3DDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &d3dBackBuffer);

	// Get a Device Context.
	HDC hdc = 0;
	d3dBackBuffer->GetDC(&hdc);

	// Print the string out to the screen.
	SetTextColor(hdc, RGB(cRed, cGreen, cBlue));
	SetBkMode(hdc, TRANSPARENT);
	TextOut(hdc, nX, nY, lpzText, (int)strlen(lpzText));

	// Release the Device Context.
	d3dBackBuffer->ReleaseDC(hdc);
	SAFE_RELEASE(d3dBackBuffer);
}
