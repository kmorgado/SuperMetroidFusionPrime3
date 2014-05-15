///////////////////////////////////////////////////////////////////////
//	File	:	"CBase.cpp"
//
//	Author	:	Keith Morgado
//
//	Purpose	:	Our Main Base class for our application.
///////////////////////////////////////////////////////////////////////
#include "CBase.h"
#include "CSGD_TextureManager.h"

CBase::CBase()
{
	m_fX = m_fY = 0;
	m_fVelX = m_fVelY = 0;
	m_nImageID = -1;
}

CBase::~CBase()
{

}


bool CBase::Update(double dElapsedTime)
{
	//	Update the object's position.
	m_fX += float(m_fVelX * dElapsedTime);
	m_fY += float(m_fVelY * dElapsedTime);

	//	Success.
	return true;
}

bool CBase::Render(void)
{
	//	Draw the object.
	CSGD_TextureManager::GetInstance()->Draw(GetImageID(), GetX(), GetY());

	//	Success.
	return true;
}

RECT CBase::GetRect( void )
{
		RECT temp;
		temp.top = (long)this->GetY();
		temp.left = (long)this->GetX();
		temp.right = (long)(temp.left + this->GetWidth());
		temp.bottom = (long)(temp.top + this->GetHeight());

		return temp;
}

bool CBase::CheckCollision( CBase * pBase )
{
	RECT pTemp;
	pTemp.bottom = 0;
	pTemp.left = 0;
	pTemp.right = 0;
	pTemp.top = 0;
	if(IntersectRect(&pTemp, &this->GetRect(), &pBase->GetRect()))
	{
		return true;
	}
	else
		return false;
}