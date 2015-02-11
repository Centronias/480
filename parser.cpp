// ****************************************************************************
// ****************************************************************************
// parser.cpp
// ****************************************************************************
// 
// ****************************************************************************
// ****************************************************************************



// ****************************************************************************
// Includes
// ****************************************************************************
#include "common.h"



// ****************************************************************************
// Initialize Static Members
// ****************************************************************************
NonTerm*	Parser::m_entrySymbol;



// ****************************************************************************
// init()
// ****************************************************************************
void
Parser::init()
{
	printf("Initializing parser.\n");

	// Create the terminal symbols
	Terminal*	oParen	= new Terminal(Token::Paren, "(");
	Terminal*	cParen	= new Terminal(Token::Paren, ")");
	Terminal*	assignment	= new Terminal(Token::Operator, ":=");
	Terminal*	identifier	= new Terminal(Token::Identifier);

	Terminal*	add	= new Terminal(Token::Operator, "+");
	Terminal*	sub	= new Terminal(Token::Operator, "-");
	Terminal*	mul	= new Terminal(Token::Operator, "*");
	Terminal*	div	= new Terminal(Token::Operator, "/");
	Terminal*	mod	= new Terminal(Token::Operator, "%");
	Terminal*	exp	= new Terminal(Token::Operator, "^");
	Terminal*	equ	= new Terminal(Token::Operator, "=");
	Terminal*	grt	= new Terminal(Token::Operator, ">");
	Terminal*	gte	= new Terminal(Token::Operator, ">=");
	Terminal*	lst	= new Terminal(Token::Operator, "<");
	Terminal*	lte	= new Terminal(Token::Operator, "<=");
	Terminal*	nte	= new Terminal(Token::Operator, "!=");
	Terminal*	boo	= new Terminal(Token::Operator, "or");
	Terminal*	boa	= new Terminal(Token::Operator, "and");
	Terminal*	bon	= new Terminal(Token::Operator, "not");
	Terminal*	sin	= new Terminal(Token::Operator, "sin");
	Terminal*	cos	= new Terminal(Token::Operator, "cos");
	Terminal*	tan	= new Terminal(Token::Operator, "tan");

	Terminal*	strConst	= new Terminal(Token::StrConst);
	Terminal*	intConst	= new Terminal(Token::IntConst);
	Terminal*	realConst	= new Terminal(Token::RealConst);
	Terminal*	boolConst	= new Terminal(Token::BoolConst);

	Terminal*	printStmt	= new Terminal(Token::StmtWord, "stdout");
	Terminal*	ifStmt		= new Terminal(Token::StmtWord, "if");
	Terminal*	whileStmt	= new Terminal(Token::StmtWord, "while");
	Terminal*	letStmt		= new Terminal(Token::Operator, "let");

	Terminal*	primType	= new Terminal(Token::PrimType);


	// Create the nonterminal symbols
	m_entrySymbol = new NonTerm("S");

	NonTerm*	expr 		= new NonTerm("EXPR");
	NonTerm*	oper 		= new NonTerm("OPER");
	NonTerm*	binOps 		= new NonTerm("BINOPS");
	NonTerm*	unOps 		= new NonTerm("UNOPS");
	NonTerm*	consts 		= new NonTerm("CONSTANTS");
	NonTerm*	stmts 		= new NonTerm("STMTS");
	NonTerm*	printStmts 	= new NonTerm("PRINTSTMTS");
	NonTerm*	ifStmts 	= new NonTerm("IFSTMTS");
	NonTerm*	whileStmts 	= new NonTerm("WHILESTMTS");
	NonTerm*	letStmts 	= new NonTerm("LETSTMTS");
	NonTerm*	exprList 	= new NonTerm("EXPRLISTS");
	NonTerm*	varLists 	= new NonTerm("VARLISTS");

	// ----------------------------------------------------
	// S Productions
	// ----------------------------------------------------

	{
		// S -> ()
		Production*	prod = new Production;
		prod->add(oParen);
		prod->add(cParen);
		m_entrySymbol->addProduction(prod);
	} {
		// S -> (S)
		Production*	prod = new Production;
		prod->add(oParen);
		prod->add(m_entrySymbol);
		prod->add(cParen);
		m_entrySymbol->addProduction(prod);
	} {
		// S -> EXPR
		m_entrySymbol->addProduction(expr);
	} {
		// S -> ()S
		Production*	prod = new Production;
		prod->add(oParen);
		prod->add(cParen);
		prod->add(m_entrySymbol);
		m_entrySymbol->addProduction(prod);
	} {
		// S -> (S)S
		Production*	prod = new Production;
		prod->add(oParen);
		prod->add(m_entrySymbol);
		prod->add(cParen);
		prod->add(m_entrySymbol);
		m_entrySymbol->addProduction(prod);
	} {
		// S -> EXPR S
		Production*	prod = new Production;
		prod->add(expr);
		prod->add(m_entrySymbol);
		m_entrySymbol->addProduction(prod);
	}

	// ----------------------------------------------------
	// EXPR Productions
	// ----------------------------------------------------
		expr->addProduction(oper);
		expr->addProduction(stmts);

	// ----------------------------------------------------
	// OPER Productions
	// ----------------------------------------------------

	{
		// OPER -> (:= identifier OPER)
		Production*	prod = new Production;
		prod->add(oParen);
		prod->add(assignment);
		prod->add(identifier);
		prod->add(oper);
		prod->add(cParen);
		oper->addProduction(prod);
	} {
		// OPER -> (BINOPS OPER OPER)
		Production*	prod = new Production;
		prod->add(oParen);
		prod->add(binOps);
		prod->add(oper);
		prod->add(oper);
		prod->add(cParen);
		oper->addProduction(prod);
	} {
		// OPER -> (UNOPS OPER)
		Production*	prod = new Production;
		prod->add(oParen);
		prod->add(unOps);
		prod->add(oper);
		prod->add(cParen);
		oper->addProduction(prod);
	} {
		// OPER -> CONSTANTS
		Production*	prod = new Production;
		prod->add(consts);
		oper->addProduction(prod);
	} {
		// OPER -> identifier
		Production*	prod = new Production;
		prod->add(identifier);
		oper->addProduction(prod);
	}

	// ----------------------------------------------------
	// BINOPS Productions
	// ----------------------------------------------------
	binOps->addProduction(add);
	binOps->addProduction(sub);
	binOps->addProduction(mul);
	binOps->addProduction(div);
	binOps->addProduction(mod);
	binOps->addProduction(exp);
	binOps->addProduction(equ);
	binOps->addProduction(grt);
	binOps->addProduction(gte);
	binOps->addProduction(lst);
	binOps->addProduction(lte);
	binOps->addProduction(nte);
	binOps->addProduction(boo);
	binOps->addProduction(boa);

	// ----------------------------------------------------
	// UNOPS Productions
	// ----------------------------------------------------
	unOps->addProduction(sub);
	unOps->addProduction(bon);
	unOps->addProduction(sin);
	unOps->addProduction(cos);
	unOps->addProduction(tan);

	// ----------------------------------------------------
	// CONSTANTS Productions
	// ----------------------------------------------------
	consts->addProduction(strConst);
	consts->addProduction(intConst);
	consts->addProduction(realConst);
	consts->addProduction(boolConst);

	// ----------------------------------------------------
	// STMTS Productions
	// ----------------------------------------------------
	stmts->addProduction(ifStmts);
	stmts->addProduction(whileStmts);
	stmts->addProduction(letStmts);
	stmts->addProduction(printStmts);

	// ----------------------------------------------------
	// STMTS Productions
	// ----------------------------------------------------

	{
		// PRINTSTMTS -> (stdout OPER)
		Production*	prod = new Production;
		prod->add(oParen);
		prod->add(printStmt);
		prod->add(oper);
		prod->add(cParen);
		printStmts->addProduction(prod);
	} {
		// IFSTMTS -> (if EXPR EXPR EXPR)
		Production*	prod = new Production;
		prod->add(oParen);
		prod->add(ifStmt);
		prod->add(expr);
		prod->add(expr);
		prod->add(expr);
		prod->add(cParen);
		ifStmts->addProduction(prod);
	} {
		// IFSTMTS -> (if EXPR EXPR)
		Production*	prod = new Production;
		prod->add(oParen);
		prod->add(ifStmt);
		prod->add(expr);
		prod->add(expr);
		prod->add(cParen);
		ifStmts->addProduction(prod);
	} {
		// WHILESTMTS -> (if EXPR EXPRLIST)
		Production*	prod = new Production;
		prod->add(oParen);
		prod->add(whileStmt);
		prod->add(expr);
		prod->add(exprList);
		prod->add(cParen);
		whileStmts->addProduction(prod);
	} {
		// LETSTMTS -> (LET (VARLIST))
		Production*	prod = new Production;
		prod->add(oParen);
		prod->add(letStmt);
		prod->add(oParen);
		prod->add(varLists);
		prod->add(cParen);
		prod->add(cParen);
		letStmts->addProduction(prod);
	}

	// ----------------------------------------------------
	// LIST Productions
	// ----------------------------------------------------

	{
		// EXPRLIST -> EXPR
		exprList->addProduction(expr);
	} {
		// EXPRLIST -> EXPR EXPRLIST
		Production*	prod = new Production;
		prod->add(expr);
		prod->add(exprList);
		exprList->addProduction(prod);
	} {
		// VARLIST -> (identifier type)
		Production*	prod = new Production;
		prod->add(oParen);
		prod->add(identifier);
		prod->add(primType);
		prod->add(cParen);
		varLists->addProduction(prod);
	} {
		// VARLIST -> (identifier type) VARLIST
		Production*	prod = new Production;
		prod->add(oParen);
		prod->add(identifier);
		prod->add(primType);
		prod->add(cParen);
		prod->add(varLists);
		varLists->addProduction(prod);
	}

	if (Global::isDumpGrammar()) {
		NonTerm::dumpGrammar();
		Global::succeed();
	}
}



// ****************************************************************************
// run()
// ****************************************************************************
void
Parser::run()
{
	TokList&	tokens = Lexer::getTokens();
	TokListIter	iter(tokens);
}