/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	File	:	CSGD_Flags
//
//	Author	:	Jensen Rivera
//
//	Purpose	:	To allow 32 possible Flags or Bools for use in applications.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

//	Bits Pre-Defined for you.  You should make an enum 
//	in your programs with these pre-defined values.
#define BIT_ONE			0x00000001
#define BIT_TWO			0x00000002
#define BIT_THREE		0x00000004
#define BIT_FOUR		0x00000008
#define BIT_FIVE		0x00000010
#define BIT_SIX			0x00000020
#define BIT_SEVEN		0x00000040
#define BIT_EIGHT		0x00000080
#define BIT_NINE		0x00000100
#define BIT_TEN			0x00000200
#define BIT_ELEVEN		0x00000400
#define BIT_TWELVE		0x00000800
#define BIT_THIRTEEN	0x00001000
#define BIT_FOURTEEN	0x00002000
#define BIT_FIFTEEN		0x00004000
#define BIT_SIXTEEN		0x00008000
#define BIT_SEVENTEEN	0x00010000
#define BIT_EIGHTEEN	0x00020000
#define BIT_NINETEEN	0x00040000
#define BIT_TWENTY		0x00080000
#define BIT_TWENTYONE	0x00100000
#define BIT_TWENTYTWO	0x00200000
#define BIT_TWENTYTHREE	0x00400000
#define BIT_TWENTYFOUR	0x00800000
#define BIT_TWENTYFIVE	0x01000000
#define BIT_TWENTYSIX	0x02000000
#define BIT_TWENTYSEVEN	0x04000000
#define BIT_TWENTYEIGHT 0x08000000
#define BIT_TWENTYNINE	0x10000000
#define BIT_THIRTY		0x20000000
#define BIT_THIRTYONE	0x40000000
#define BIT_THIRTYTWO	0x80000000

class CSGD_Flags
{
	private:
		unsigned int	m_unFlags;						//	32 bits = 32 flags

	public:
		//	Constructor.
		CSGD_Flags() { m_unFlags = 0; }
		//	Destructor.
		~CSGD_Flags() { }

		////////////////////////////////////////////////////////////////////////////////////
		//	Function	:	GetFlags
		//
		//	Purpose		:	Return the Flags variable for use outside of this class scope.
		////////////////////////////////////////////////////////////////////////////////////
		inline unsigned int GetFlags(void)	{ return m_unFlags;	}

		////////////////////////////////////////////////////////////////////////////////////
		//	Function	:	Clear
		//
		//	Purpose		:	Zero out all the bits (flags)
		////////////////////////////////////////////////////////////////////////////////////
		void Clear(void) { m_unFlags &= ~0xFFFFFFFF; }

		////////////////////////////////////////////////////////////////////////////////////
		//	Function	:	TurnOn
		//
		//	Purpose		:	Turns on the desired Bit (Flag).
		////////////////////////////////////////////////////////////////////////////////////
		void TurnOn(unsigned int unBit)		{ m_unFlags |= unBit; }

		////////////////////////////////////////////////////////////////////////////////////
		//	Function	:	TurnOff
		//
		//	Purpose		:	Turns off the desired bit (Flag).
		////////////////////////////////////////////////////////////////////////////////////
		void TurnOff(unsigned int unBit)	{ m_unFlags &= ~unBit; }

		////////////////////////////////////////////////////////////////////////////////////
		//	Function	:	Toggle
		//
		//	Purpose		:	Toggles this bit (From OFF to ON or ON to OFF).
		////////////////////////////////////////////////////////////////////////////////////
		void Toggle(unsigned int unBit)		{ m_unFlags ^= unBit; }

		////////////////////////////////////////////////////////////////////////////////////
		//	Function	:	CheckFlag
		//	
		//	Purpose		:	Checks to see if a Bit (Flag) is turned on.
		////////////////////////////////////////////////////////////////////////////////////
		int CheckFlag(unsigned int unBit)	{ return (m_unFlags & unBit); }
};