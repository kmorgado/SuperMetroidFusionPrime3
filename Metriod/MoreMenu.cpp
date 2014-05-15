///////////////////////////////////////////////////////////////////////
//	File	:	"MoreMenu.cpp"
//
//	Author	:	Keith Morgado
//
//	Purpose	:	Contains the information for the More options menu
///////////////////////////////////////////////////////////////////////
#include "MoreMenu.h"

CMoreMenu::CMoreMenu(void)
{
	m_bVisible = false;
	m_bLoaded = false;
	m_bInMenu = false;
	m_nSelected = 0;
	m_nInMenuSelected = 0;
}

CMoreMenu::~CMoreMenu(void)
{
}

int CMoreMenu::GetOKImageID(void)
{
	return m_nOKID;
}
int CMoreMenu::GetMCImageID(void)
{
	return m_nCharacterID;
}
int CMoreMenu::GetMKImageID(void)
{
	return m_nKeyBindID;
}
int CMoreMenu::GetMDImageID(void)
{
	return m_nDiffID;
}
int CMoreMenu::GetMSImageID(void)
{
	return m_nHighScoreID;
}
int CMoreMenu::GetMBImageID(void)
{
	return m_nBackstoryID;
}
int CMoreMenu::GetMHImageID(void)
{
	return m_nHelpID;
}
int CMoreMenu::GetSelected(void)
{
	return m_nSelected;
}
int CMoreMenu::GetInMenuSelected(void)
{
	return m_nInMenuSelected;
}
bool CMoreMenu::GetVisible(void)
{
	return m_bVisible;
}
bool CMoreMenu::GetLoaded(void)
{
	return m_bLoaded;
}
bool CMoreMenu::GetInMenu(void)
{
	return m_bInMenu;
}
bool CMoreMenu::GetChanges(void)
{
	return m_bChanges;
}


void CMoreMenu::SetOKImageID(int nID)
{
	m_nOKID = nID;
}
void CMoreMenu::SetMCImageID(int nID)
{
	m_nCharacterID = nID;
}
void CMoreMenu::SetMKImageID(int nID)
{
	m_nKeyBindID = nID;
}
void CMoreMenu::SetMDImageID(int nID)
{
	m_nDiffID = nID;
}
void CMoreMenu::SetMSImageID(int nID)
{
	m_nHighScoreID = nID;
}
void CMoreMenu::SetMBImageID(int nID)
{
	m_nBackstoryID = nID;
}
void CMoreMenu::SetMHImageID(int nID)
{
	m_nHelpID = nID;
}
void CMoreMenu::SetSelected(int nSelected)
{
	m_nSelected = nSelected;
}
void CMoreMenu::SetInMenuSelected(int nSelected)
{
	m_nInMenuSelected = nSelected;
}
void CMoreMenu::SetVisible(bool bVisible)
{
	m_bVisible = bVisible;
}
void CMoreMenu::SetLoaded(bool bLoaded)
{
	m_bLoaded = bLoaded;
}
void CMoreMenu::SetInMenu(bool bInMenu)
{
	m_bInMenu = bInMenu;
}
void CMoreMenu::SetChanges(bool bChange)
{
	m_bChanges = bChange;
}


void CMoreMenu::Load( void )
{
	CSGD_TextureManager *mTM = CSGD_TextureManager::GetInstance();

	SetOKImageID(mTM->LoadTexture("Resources/Graphics/KeM_OK.bmp",D3DCOLOR_XRGB(0, 0, 0)));
	SetMCImageID(mTM->LoadTexture("Resources/Graphics/KeM_Character.bmp"));
	SetMKImageID(mTM->LoadTexture("Resources/Graphics/KeM_Bind.bmp"));
	SetMDImageID(mTM->LoadTexture("Resources/Graphics/KeM_Diff.bmp"));
	SetMSImageID(mTM->LoadTexture("Resources/Graphics/KeM_High.bmp"));
	SetMBImageID(mTM->LoadTexture("Resources/Graphics/KeM_Back.bmp"));
	SetMHImageID(mTM->LoadTexture("Resources/Graphics/KeM_Help.bmp"));	
}