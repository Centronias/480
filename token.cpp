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
comString	Token::m_examples[];



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
	m_typeNames[Token::PrimType]	= "type";
	m_typeNames[Token::StmtWord]	= "statement";
	m_typeNames[Token::Paren]		= "paren";
	m_typeNames[Token::BoolConst]	= "bool";
	m_typeNames[Token::RealConst]	= "real";
	m_typeNames[Token::IntConst]	= "int";
	m_typeNames[Token::StrConst]	= "string";
	m_typeNames[Token::Identifier]	= "id";

	m_examples[Token::NONE]			= "";
	m_examples[Token::Operator]		= "+";
	m_examples[Token::PrimType]		= "int";
	m_examples[Token::StmtWord]		= "if";
	m_examples[Token::Paren]		= "(";
	m_examples[Token::BoolConst]	= "true";
	m_examples[Token::RealConst]	= "10e9";
	m_examples[Token::IntConst]		= "24";
	m_examples[Token::StrConst]		= "\"I am a robot\"";
	m_examples[Token::Identifier]	= "var";
}