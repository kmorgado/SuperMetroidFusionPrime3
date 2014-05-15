///////////////////////////////////////////////////////////////////////
//	File	:	"Stats.cpp"
//
//	Author	:	Keith Morgado
//
//	Purpose	:	Contains all of the Statistical information
///////////////////////////////////////////////////////////////////////
#include "Stats.h"

CStats *CStats::m_pInstance = 0;

CStats::~CStats(void)
{
}

CStats *CStats::GetInstance(void)
{
	if (!m_pInstance)
		m_pInstance = new CStats;

	return m_pInstance;
}
void CStats::DeleteInstance(void)
{
	if (m_pInstance)			
	{
		delete m_pInstance;		
		m_pInstance = 0;		
	}
}

int CStats::GetLives(void)
{
	return m_nLives;
}
int CStats::GetHealth(void)
{
	return m_nHealth;
}
int CStats::GetCharacter(void)
{
	return m_nCharacer;
}
int CStats::GetScore(void)
{
	return m_nScore;
}
int CStats::GetDifficulty(void)
{
	return m_nDiff;
}
int CStats::GetBType(void)
{
	return m_nBType;
}
bool CStats::GetSlow(void)
{
	return m_bSlow;
}
bool CStats::GetFast(void)
{
	return m_bFast;
}
int CStats::GetLvlnum()
{
	return m_nlvlnum;
}
int	CStats::GetShootKey(void)
{
	return m_nShoot;
}


void CStats::SetLives(int nLives)
{
	m_nLives = nLives;
}
void CStats::SetHealth(int nHealth)
{
	m_nHealth = nHealth;
}
void CStats::SetCharacter(int nChar)
{
	m_nCharacer = nChar;
}	
void CStats::SetScore(int nScore)
{
	m_nScore = nScore;
}
void CStats::SetDifficulty(int nDiff)
{
	m_nDiff = nDiff;
}
void CStats::SetBType(int nBtype)
{
	m_nBType = nBtype;
}
void CStats::SetSlow(bool bSlow)
{
	m_bSlow = bSlow;
}
void CStats::SetFast(bool bFast)
{
	m_bFast = bFast;
}
void CStats::SetLvlnum( int nLvlnum )
{
	m_nlvlnum = nLvlnum;
}
void CStats::SetShootKey(int nSKey)
{
	m_nShoot = nSKey;
}