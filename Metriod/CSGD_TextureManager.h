////////////////////////////////////////////////////////
//  File			:	“CSGD_TextureManager.h”
//
//  Author			:	Jensen Rivera (JR)
//
//  Date Created	:	6/26/2006
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
#include <vector>
using std::vector;

// Macros
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)	if (p) { p->Release(); p = NULL; }
#endif

#ifndef DXERROR
#define DXERROR(a)	{ OutputDebugString(a); return false; }
#endif

class CSGD_TextureManager
{
private:
	// All the data we need to contain a texture.
	typedef struct _TEXTURE
	{
		char				filename[_MAX_PATH];	// The filename of this texture.
		int					ref;					// The number of times this texture has be loaded.
		LPDIRECT3DTEXTURE9	texture;				// A pointer to the texture.
		int					Width;					// The width of the texture.
		int					Height;					// The Height of the texture.

		// Default constructor.
		_TEXTURE()
		{
			filename[0] = '\0';
			ref = 0;
			texture = NULL;
			Width = 0;
			Height = 0;
		}
	} TEXTURE, *LPTEXTURE;

private:
	///////////////////////////////////////////////////////////////////
	//	Function:	CSGD_TextureManager(Constructors)
	///////////////////////////////////////////////////////////////////
	CSGD_TextureManager(void);
	CSGD_TextureManager(CSGD_TextureManager &ref);
	CSGD_TextureManager &operator=(CSGD_TextureManager &ref);

private:
	vector<TEXTURE>				m_Textures;			// A list of all the loaded textures.
	LPDIRECT3DDEVICE9			m_lpDevice;			// A pointer to the Direct3D device.
	LPD3DXSPRITE				m_lpSprite;			// A pointer to the sprite interface.
	static CSGD_TextureManager	m_Instance;			// An instance to this class.

public:
	///////////////////////////////////////////////////////////////////
	//	Function:	CSGD_TextureManager(Destructor)
	///////////////////////////////////////////////////////////////////
	virtual ~CSGD_TextureManager(void);

	///////////////////////////////////////////////////////////////////
	//	Function:	"GetInstance"
	//
	//	Last Modified:		6/26/2006
	//
	//	Input:		void
	//
	//	Return:		An instance to this class.
	//
	//	Purpose:	Gets an instance to this class.
	///////////////////////////////////////////////////////////////////
	static CSGD_TextureManager *GetInstance(void);

	///////////////////////////////////////////////////////////////////
	//	Function:	"InitializeTextureManager"
	//
	//	Last Modified:		8/29/2006
	//
	//	Input:		device	-	A pointer to the Direct3D device.
	//				sprite	-	A pointer to the sprite object.
	//
	//	Return:		true, if successful.
	//
	//	Purpose:	Initializes the texture manager.
	///////////////////////////////////////////////////////////////////
	bool InitTextureManager(LPDIRECT3DDEVICE9 device, LPD3DXSPRITE sprite);

	///////////////////////////////////////////////////////////////////
	//	Function:	"ShutdownTextureManager"
	//
	//	Last Modified:		9/21/2006
	//
	//	Input:		void
	//
	//	Return:		void
	//
	//	Purpose:	Unloads all the loaded textures.
	///////////////////////////////////////////////////////////////////
	void ShutdownTextureManager(void);

	///////////////////////////////////////////////////////////////////
	//	Function:	"LoadTexture"
	//
	//	Last Modified:		12/27/2006
	//
	//	Input:		szFilename	-	The file to load.
	//				colorkey	-	The color key to use on the texture (use D3DCOLOR_ARGB() Macro).
	//
	//	Return:		An id to the texture that was loaded.
	//
	//	Purpose:	Loads a specified texture from a file.
	//
	//  NOTE:		Images must be in powers of 2 (i.e. 256x64).
	//
	//				Supports .bmp, .dds, .dib, .hdr, .jpg, .pfm, .png, 
	//				.ppm, and .tga files. 
	///////////////////////////////////////////////////////////////////
	int LoadTexture(char *szFilename, DWORD colorkey = 0);

	///////////////////////////////////////////////////////////////////
	//	Function:	"ReleaseTexture"
	//
	//	Last Modified:		9/21/2006
	//
	//	Input:		id	-	The id to the texture to release.
	//
	//	Return:		void
	//
	//	Purpose:	Releases a reference to a given texture. When the
	//				reference to the texture is zero the texture is
	//				released from memory.
	///////////////////////////////////////////////////////////////////
	void ReleaseTexture(int id);

	///////////////////////////////////////////////////////////////////
	//	Function:	"GetTextureWidth"
	//
	//	Last Modified:		9/21/2006
	//
	//	Input:		id	-	The id to the texture who's width you want.
	//
	//	Return:		The width of the given texture.
	//
	//	Purpose:	Gets the width of a specified texture.
	///////////////////////////////////////////////////////////////////
	int GetTextureWidth(int id);

	///////////////////////////////////////////////////////////////////
	//	Function:	"GetTextureHeight"
	//
	//	Last Modified:		9/21/2006
	//
	//	Input:		id	-	The id to the texture who's height you want.
	//
	//	Return:		The height of the given texture.
	//
	//	Purpose:	Gets the height of a specified texture.
	///////////////////////////////////////////////////////////////////
	int GetTextureHeight(int id);

	///////////////////////////////////////////////////////////////////
	//	Function:	"DrawTexture"
	//
	//	Last Modified:		9/21/2006
	//
	//	Input:		id		-	The id of the texture to draw.
	//				x		-	The x position to draw the texture at.
	//				y		-	The y position to draw the texture at.
	//				scalex	-	How much to scale the texture in the x.
	//				scaley	-	How much to scale the texture in the y.
	//				section	-	The section of the bitmap to draw.
	//				rotx	-	The x center to apply the rotation from.
	//				roty	-	The y center to apply the rotation from.
	//				rot		-	How much to rotate the texture.
	//				color	-	The color to apply to the texture (use D3DCOLOR_ARGB() Macro).
	//
	//	Return:		true if successful.
	//
	//	Purpose:	Draws a texture to the screen.
	///////////////////////////////////////////////////////////////////
	bool Draw(int id, float x, float y, float scalex = 1.0f, float scaley = 1.0f,
		RECT *section = 0, float rotx = 0.0f, float roty = 0.0f, float rot = 0.0f, DWORD color = 0xFFFFFFFF);
};
