///////////////////////////////////////////////////////////////////////
//	File	:	"CBase.h"
//
//	Author	:	Keith Morgado
//
//	Purpose	:	Our Main Base class for our application.
///////////////////////////////////////////////////////////////////////
#pragma once

#include "IBaseInterface.h"
#include "Stats.h"
#include <string>
using namespace std;

class CBase : public IBaseInterface
{
	protected:
		float m_fX, m_fY;				//	X and Y Coordinates.
		float m_fVelX, m_fVelY;		//	X and Y Velocities.
		float m_fWidth, m_fHeight;    // Width and Height
		int m_nImageID;				//	Bitmap to use.
		int m_nType;				//Type of Object it is

	public:
		CBase();
		virtual ~CBase();

		////////////////////////////////////////////////////////////////////
		//	Function	:	Accessors.
		//
		//	Return		:	To return the specified type.
		////////////////////////////////////////////////////////////////////
		inline float GetX(void)				{ return m_fX;			}
		inline float GetY(void)				{ return m_fY;			}
		inline float GetWidth(void)			{ return m_fWidth;		}
		inline float GetHeight(void)			{ return m_fHeight;		}
		inline float GetVelX(void)			{ return m_fVelX;		}
		inline float GetVelY(void)			{ return m_fVelY;		}
		inline int GetImageID(void)			{ return m_nImageID;	}
		inline int GetType(void)			{ return m_nType;			}
		RECT GetRect(void);

		////////////////////////////////////////////////////////////////////
		//	Function	:	Modifiers.
		//
		//	Return		:	To modify the specified type.
		////////////////////////////////////////////////////////////////////
		inline void SetX(float fX)			{ m_fX = fX;			}
		inline void SetY(float fY)			{ m_fY = fY;			}
		inline void SetVelX(float fVelX)		{ m_fVelX = fVelX;		}
		inline void SetVelY(float fVelY)		{ m_fVelY = fVelY;		}
		inline void	SetWidth(float fWidth)			{ m_fWidth = fWidth;		}
		inline void	SetHeight(float fHeight)			{ m_fHeight = fHeight;		}
		inline void SetImageID(int nID)		{ m_nImageID = nID;		}
		inline void SetType(int	nType)		{ m_nType = nType;		}

		////////////////////////////////////////////////////////////////////
		//	Function	:	AddRef
		//
		//	Return		:	Increment the Reference Counter.
		////////////////////////////////////////////////////////////////////
		void AddRef(void) { m_unRefCount++; }
	  
		////////////////////////////////////////////////////////////////////
		//	Function	:	Release
		//
		//	Return		:	Decrement the Reference Counter and Delete the memory
		//					when the reference reaches zero.
		////////////////////////////////////////////////////////////////////
		void Release(void)
		{
			m_unRefCount--;
			if (m_unRefCount == 0)
				delete this;
		}


		////////////////////////////////////////////////////////////////////
		//	Function	:	CheckCollision
		//
		//	Return		:	Checks for collision between objects
		////////////////////////////////////////////////////////////////////
		bool CheckCollision(CBase * pBase);

		////////////////////////////////////////////////////////////////////
		//	Function	:	Update
		//
		//	Return		:	Update this object.
		////////////////////////////////////////////////////////////////////
		bool Update(double dElapsedTime);

		////////////////////////////////////////////////////////////////////
		//	Function	:	Render
		//
		//	Return		:	Render this object.
		////////////////////////////////////////////////////////////////////
		bool Render(void);

};