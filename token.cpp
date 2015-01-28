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
// init()
// ****************************************************************************
void
Token::init()
{
	m_typeNames[Token::NONE]		= "INVALID TOKEN: NONE";
	m_typeNames[Token::LogicalOp]	= "LogOp";
	m_typeNames[Token::CompOp]		= "CmpOp";
	m_typeNames[Token::ExpoOp]		= "ExpOp";
	m_typeNames[Token::MultOp]		= "MulOp";
	m_typeNames[Token::AddOp]		= "AddOp";
	m_typeNames[Token::UnaryOp]		= "UnOp";
	m_typeNames[Token::AssgnOp]		= "AsgOp";
	m_typeNames[Token::PrimType]	= "Prim";
	m_typeNames[Token::Declarator]	= "Decl";
	m_typeNames[Token::Conditional]	= "Cond";
	m_typeNames[Token::Paren]		= "Paren";
	m_typeNames[Token::BoolConst]	= "Bool";
	m_typeNames[Token::RealConst]	= "Real";
	m_typeNames[Token::IntConst]	= "Int";
	m_typeNames[Token::StrConst]	= "String";
	m_typeNames[Token::Identifier]	= "Id";
}