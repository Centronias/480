// ****************************************************************************
// ****************************************************************************
// token.cpp
// ****************************************************************************
// 
// ****************************************************************************
// ****************************************************************************



// ****************************************************************************
// Includes
// ****************************************************************************
#include "common.h"



// ****************************************************************************
// Token()
// ****************************************************************************
Token::Token(Token::Type	type,
			 const char*	spelling)
:	m_type(type)
{
	m_spelling = new char[strlen(spelling + 1)];
	strcpy(m_spelling, spelling);
}



// ****************************************************************************
// ~Token()
// ****************************************************************************
Token::~Token()
{
	delete [] m_spelling;
}