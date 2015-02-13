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
// Static Member Initialization
// ****************************************************************************
comString	Token::m_typeNames[];



// ****************************************************************************
// Token()
// ****************************************************************************
Token::Token(Token::Type		type,
			 const comString&	spelling,
			 UINT				line)
:	m_type(type),
	m_spelling(spelling),
	m_line(line)
{
}



// ****************************************************************************
// printable()
// ****************************************************************************
comString&
Token::printable(comString&	buf)
{
	char	printBuffer[128];

	sprintf(printBuffer, "(%s : \"%s\")", (const char*) getTypeName(m_type), (const char*) m_spelling);

	buf = printBuffer;
	return buf;
}



// ****************************************************************************
// init()
// ****************************************************************************
void
Token::init()
{
	m_typeNames[Token::NONE]		= "NONE";
	m_typeNames[Token::Operator]	= "operator";
	m_typeNames[Token::PrimType]	= "Prim";
	m_typeNames[Token::StmtWord]	= "Statment";
	m_typeNames[Token::Paren]		= "Paren";
	m_typeNames[Token::BoolConst]	= "Bool";
	m_typeNames[Token::RealConst]	= "Real";
	m_typeNames[Token::IntConst]	= "Int";
	m_typeNames[Token::StrConst]	= "String";
	m_typeNames[Token::Identifier]	= "Id";
}