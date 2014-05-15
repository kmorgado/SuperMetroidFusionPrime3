////////////////////////////////////////////////////////
//  File			:	“CSGD_TextureManager.cpp”
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

#include "CSGD_TextureManager.h"
#include <assert.h>		// code in assertions gets compiled out in Release mode

#pragma warning (disable : 4996)

CSGD_TextureManager CSGD_TextureManager::m_Instance;

///////////////////////////////////////////////////////////////////
//	Function:	"CSGD_TextureManager(Constructor)"
///////////////////////////////////////////////////////////////////
CSGD_TextureManager::CSGD_TextureManager(void)
{
	m_lpDevice = NULL;
	m_lpSprite = NULL;
}

///////////////////////////////////////////////////////////////////
//	Function:	"CSGD_TextureManager(Destructor)"
///////////////////////////////////////////////////////////////////
CSGD_TextureManager::~CSGD_TextureManager(void)
{
}

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
CSGD_TextureManager *CSGD_TextureManager::GetInstance(void)
{
	return &m_Instance;
}

///////////////////////////////////////////////////////////////////
//	Function:	"InitTextureManager"
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
bool CSGD_TextureManager::InitTextureManager(LPDIRECT3DDEVICE9 device, LPD3DXSPRITE sprite)
{
	m_lpDevice = device;
	m_lpSprite = sprite;

	return (m_lpDevice && m_lpSprite) ? true : false;
}

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
void CSGD_TextureManager::ShutdownTextureManager(void)
{
	for (unsigned int i = 0; i < m_Textures.size(); i++)
	{
		// Remove ref.
		m_Textures[i].ref = 0;

		// Release the texture if it's not being used.
		SAFE_RELEASE(m_Textures[i].texture);
		m_Textures[i].filename[0] = '\0';
	}

	// Clear the list of all loaded textures.
	m_Textures.clear();
}

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
int CSGD_TextureManager::LoadTexture(char *szFilename, DWORD colorkey)
{
	//	Make sure the filename is valid.
	if (!szFilename)	return -1;

	// Make sure the texture isn't already loaded.
	for (unsigned int i = 0; i < m_Textures.size(); i++)
	{
		// compare strings without caring about upper or lowercase.
		if (stricmp(szFilename, m_Textures[i].filename) == 0)	// 0 means they are equal.
		{
			m_Textures[i].ref++; // add a reference to this texture.
			return i;	// return the index.
		}
	}

	// Look for an open spot.
	int id = -1;
	for (unsigned int i = 0; i < m_Textures.size(); i++)
	{
		if (m_Textures[i].ref == 0)
		{
			id = i;
			break;
		}
	}

	// if we didn't find an open spot, load it in a new one
	if (id == -1)
	{
		// A temp texture object.
		TEXTURE loaded;

		// Copy the filename of the loaded texture.
		strcpy(loaded.filename, szFilename);

		// Load the texture from the given file.
		HRESULT hr = 0;
		if (FAILED(hr = D3DXCreateTextureFromFileEx(m_lpDevice, szFilename, 0, 0, D3DX_DEFAULT, 0,
			D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT,
			D3DX_DEFAULT, colorkey, 0, 0, &loaded.texture)))
		{
			// Failed.
			MessageBox(0, "Failed to create texture.", "Error", MB_OK);
			return -1;
		}

		// AddRef.
		loaded.ref = 1;

		// Get surface description (to find Width/Height of the texture)
		D3DSURFACE_DESC d3dSurfDesc;
		ZeroMemory(&d3dSurfDesc, sizeof(d3dSurfDesc));

		loaded.texture->GetLevelDesc(0, &d3dSurfDesc);

		// Remember the Width and Height
		loaded.Width	= d3dSurfDesc.Width;
		loaded.Height	= d3dSurfDesc.Height;

		// Put the texture into the list.
		m_Textures.push_back(loaded);

		// Return the id of the texture.
		return (int)m_Textures.size() - 1;
	}
	// we found an open spot
	else
	{
		// Make sure the texture has been released.
		SAFE_RELEASE(m_Textures[id].texture);

		// Copy the filename of the loaded texture.
		strcpy(m_Textures[id].filename, szFilename);

		// Load the texture from the given file.
		HRESULT hr = 0;
		if (FAILED(hr = D3DXCreateTextureFromFileEx(m_lpDevice, szFilename, 0, 0, D3DX_DEFAULT, 0,
			D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT,
			D3DX_DEFAULT, colorkey, 0, 0, &m_Textures[id].texture)))
		{
			// Failed.
			MessageBox(0, "Failed to create texture.", "Error", MB_OK);
			return -1;
		}

		// Get surface description (to find Width/Height of the texture)
		D3DSURFACE_DESC d3dSurfDesc;
		ZeroMemory(&d3dSurfDesc, sizeof(d3dSurfDesc));

		m_Textures[id].texture->GetLevelDesc(0, &d3dSurfDesc);

		// Remember the Width and Height
		m_Textures[id].Width	= d3dSurfDesc.Width;
		m_Textures[id].Height	= d3dSurfDesc.Height;

		// AddRef
		m_Textures[id].ref = 1;

		// Return the id of the texture.
		return id;
	}
}

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
void CSGD_TextureManager::ReleaseTexture(int id)
{
	// Make sure the id is in range.
	assert(id > -1 && id < (int)m_Textures.size() && "id is out of range");
	
	// Remove ref.
	m_Textures[id].ref--;

	// Release the texture if it's not being used.
	if (m_Textures[id].ref <= 0)
	{
		// Do a lazy delete and leave this spot empty
		SAFE_RELEASE(m_Textures[id].texture);
		m_Textures[id].filename[0] = '\0';
		m_Textures[id].ref = 0;
	}
}

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
int CSGD_TextureManager::GetTextureWidth(int id)
{
	// Make sure the id is in range.
	assert(id > -1 && id < (int)m_Textures.size() && "id is out of range");

	return m_Textures[id].Width;
}

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
int CSGD_TextureManager::GetTextureHeight(int id)
{
	// Make sure the id is in range.
	assert(id > -1 && id < (int)m_Textures.size() && "id is out of range");

	return m_Textures[id].Height;
}

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
bool CSGD_TextureManager::Draw(int id, float x, float y, float scalex, float scaley,
							   RECT *section, float rotx, float roty, float rot, DWORD color)
{
	// Make sure the id is in range.
	assert(id > -1 && id < (int)m_Textures.size() && "id is out of range");

	// Make sure the sprite was created and we have a valid texture.
	if (!m_lpSprite)
		return false;

	D3DXMATRIX scale;
	D3DXMATRIX rotation;
	D3DXMATRIX translate;
	D3DXMATRIX combined;

	// Initialize the Combined matrix.
	D3DXMatrixIdentity(&combined);

	// Scale the sprite.
	D3DXMatrixScaling(&scale, scalex, scaley, 1.0f);
	combined *= scale;

	// Rotate the sprite.
	D3DXMatrixTranslation(&translate, -rotx, -roty, 0.0f);
	combined *= translate;
	D3DXMatrixRotationZ(&rotation, rot);
	combined *= rotation;
	D3DXMatrixTranslation(&translate, rotx, roty, 0.0f);
	combined *= translate;

	// Translate the sprite
	D3DXMatrixTranslation(&translate, (float)x, (float)y, 0.0f);
	combined *= translate;

	// Apply the transform.
	m_lpSprite->SetTransform(&combined);

	// Draw the sprite.
	if (FAILED(m_lpSprite->Draw(m_Textures[id].texture, section, NULL, NULL, color)))
		DXERROR("Failed to draw the texture.");

	// Move the world back to identity.
	D3DXMatrixIdentity(&combined);
	m_lpSprite->SetTransform(&combined);

	// success.
	return true;
}

#pragma warning (default : 4996)