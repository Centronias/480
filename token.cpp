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
			 const comString&	spelling)
:	m_type(type),
	m_spelling(spelling)
{	
}



// ****************************************************************************
// init()
// ****************************************************************************
void
Token::init()
{
	m_typeNames[Token::NONE]		= "INVALID TOKEN: NONE";
	m_typeNames[Token::LogicalOp]	= "LogicalOp";
	m_typeNames[Token::CompOp]		= "CompOp";
	m_typeNames[Token::ExpoOp]		= "ExpoOp";
	m_typeNames[Token::MultOp]		= "MultOp";
	m_typeNames[Token::AddOp]		= "AddOp";
	m_typeNames[Token::UnaryOp]		= "UnaryOp";
	m_typeNames[Token::AssgnOp]		= "AssgnOp";
	m_typeNames[Token::PrimType]	= "PrimType";
	m_typeNames[Token::Declarator]	= "Declarator";
	m_typeNames[Token::Conditional]	= "Conditional";
	m_typeNames[Token::Paren]		= "Paren";
	m_typeNames[Token::TypeName]	= "TypeName";
}