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
	Terminal*	open	= new Terminal(Token::Paren, "(");
	Terminal*	close	= new Terminal(Token::Paren, ")");
	Terminal*	assign	= new Terminal(Token::Operator, ":=");
	Terminal*	iden	= new Terminal(Token::Identifier);

	Terminal*	sub	= new Terminal(Token::Operator, "-");

	comVector<Terminal*>	binOps;
	binOps.append(new Terminal(Token::Operator, "+"));
	binOps.append(new Terminal(Token::Operator, "*"));
	binOps.append(new Terminal(Token::Operator, "/"));
	binOps.append(new Terminal(Token::Operator, "%"));
	binOps.append(new Terminal(Token::Operator, "^"));
	binOps.append(new Terminal(Token::Operator, "="));
	binOps.append(new Terminal(Token::Operator, ">"));
	binOps.append(new Terminal(Token::Operator, ">="));
	binOps.append(new Terminal(Token::Operator, "<"));
	binOps.append(new Terminal(Token::Operator, "<="));
	binOps.append(new Terminal(Token::Operator, "!="));
	binOps.append(new Terminal(Token::Operator, "or"));
	binOps.append(new Terminal(Token::Operator, "and"));

	comVector<Terminal*>	unOps;
	unOps(new Terminal(Token::Operator, "not"));
	unOps(new Terminal(Token::Operator, "sin"));
	unOps(new Terminal(Token::Operator, "cos"));
	unOps(new Terminal(Token::Operator, "tan"));

	Terminal*	constStr	= new Terminal(Token::StrConst);
	Terminal*	constInt	= new Terminal(Token::IntConst);
	Terminal*	constReal	= new Terminal(Token::RealConst);
	Terminal*	constBool	= new Terminal(Token::BoolConst);

	Terminal*	printKey	= new Terminal(Token::StmtWord, "stdout");
	Terminal*	ifKey		= new Terminal(Token::StmtWord, "if");
	Terminal*	whileKey	= new Terminal(Token::StmtWord, "while");
	Terminal*	letKey		= new Terminal(Token::Operator, "let");

	Terminal*	typeName	= new Terminal(Token::PrimType);

	Terminal*	epsilon		= new Terminal(Token::NONE);


	// Create the nonterminal symbols
	m_entrySymbol = new NonTerm("S");

	NonTerm*	S		= m_entrySymbol;
	NonTerm*	SPAREN	= new NonTerm("SPAREN");
	NonTerm*	SP 		= new NonTerm("SP");
	NonTerm*	EXPR	= new NonTerm("EXPR");
	NonTerm*	RESR	= new NonTerm("RESR");
	NonTerm*	OPER	= new NonTerm("OPER");
	NonTerm*	OPERM	= new NonTerm("OPER_MINUS");
	NonTerm*	STMTS	= new NonTerm("STMTS");
	NonTerm*	ELSE	= new NonTerm("ELSE");
	NonTerm*	ELIST	= new NonTerm("EXPRLIST");
	NonTerm*	ELISTC	= new NonTerm("EXPRLIST_CAP");
	NonTerm*	VLIST	= new NonTerm("VARLIST");
	NonTerm*	VLISTC	= new NonTerm("VARLIST_CAP");

	Production*	prod = NULL;

	// ----------------------------------------------------
	// S Productions
	// ----------------------------------------------------
	// S -> ( SPAREN ) SP
	prod = new Production;
	prod->add(open);
	prod->add(SPAREN);
	prod->add(close);
	prod->add(SP);
	S->addProduction(prod);

	// S -> EXPR SP
	prod = new Production;
	prod->add(EXPR);
	prod->add(SP);
	S->addProduction(prod);

	// ----------------------------------------------------
	// SPAREN Productions
	// ----------------------------------------------------
	// SPAREN -> S
	SPAREN->addProduction(S);

	// SPAREN -> epsilon
	SPAREN->addProduction(epsilon);

	// ----------------------------------------------------
	// SP Productions
	// ----------------------------------------------------
	// SP -> S SP
	prod = new Production;
	prod->add(S);
	prod->add(SP);
	SP->addProduction(prod);

	// SP -> epsilon
	prod = new Production;
	prod->add(epsilon);
	SP->addProduction(prod);

	// ----------------------------------------------------
	// EXPR Productions
	// ----------------------------------------------------
	// EXPR -> RESR
	EXPR->addProduction(RESR);

	// EXPR -> ( STMTS )
	prod = new Production;
	prod->add(open);
	prod->add(STMTS);
	prod->add(close);
	EXPR->addProduction(prod);

	// ----------------------------------------------------
	// RESR Productions
	// ----------------------------------------------------
	// RESR -> ( OPER )
	prod = new Production;
	prod->add(open);
	prod->add(OPER);
	prod->add(close);
	RESR->addProduction(prod);

	// RESR -> strings | ints | reals | bools | identifiers
	RESR->addProduction(constStr);
	RESR->addProduction(constInt);
	RESR->addProduction(constReal);
	RESR->addProduction(constBool);
	RESR->addProduction(iden);

	// ----------------------------------------------------
	// OPER Productions
	// ----------------------------------------------------
	// OPER -> := identifiers RESR
	prod = new Production;
	prod->add(assign);
	prod->add(iden);
	prod->add(RESR);
	OPER->addProduction(prod);

	// OPER -> - RESR OPER_MINUS
	prod = new Production;
	prod->add(sub);
	prod->add(RESR);
	prod->add(OPERM);
	OPER->addProduction(prod);

	// <binary operator> RESR RESR
	for (UINT i = 0; i < binOps.getNumEntries(); i++) {
		prod = new Production;
		prod->add(binOps[i]);
		prod->add(RESR);
		prod->add(RESR);
		OPER->addProduction(prod);
	}

	// <unary operator> RESR
	for (UINT i = 0; i < unOps.getNumEntries(); i++) {
		prod = new Production;
		prod->add(unOps[i]);
		prod->add(RESR);
		OPER->addProduction(prod);
	}

	// ----------------------------------------------------
	// OPER_MINUS Productions
	// ----------------------------------------------------
	// OPER_MINUS -> RESR
	OPER_MINUS->addProduction(RESR);

	// OPER_MINUS -> epsilon
	OPER_MINUS->addProduction(epsilon);

	// ----------------------------------------------------
	// STMTS Productions
	// ----------------------------------------------------
	// STMTS -> if EXPR EXPR ELSE
	prod = new Production;
	prod->add(ifKey);
	prod->add(EXPR);
	prod->add(EXPR);
	prod->add(ELSE);
	STMTS->addProduction(prod);

	// STMTS -> while EXPR EXPRLIST
	prod = new Production;
	prod->add(whileKey);
	prod->add(EXPR);
	prod->add(ELIST);
	STMTS->addProduction(prod);

	// STMTS -> let ( VARLIST )
	prod = new Production;
	prod->add(letKey);
	prod->add(open);
	prod->add(VLIST);
	prod->add(close);
	STMTS->addProduction(prod);

	// STMTS -> stdout RESR
	prod = new Production;
	prod->add(printKey);
	prod->add(RESR);
	STMTS->addProduction(prod);

	// ----------------------------------------------------
	// ELSE Productions
	// ----------------------------------------------------
	// ELSE -> EXPR
	ELSE->addProduction(EXPR);

	// ELSE -> epsilon
	ELSE->addProduction(epsilon);

	// ----------------------------------------------------
	// EXPRLIST and EXPRLIST_CAP Productions
	// ----------------------------------------------------
	// EXPRLIST -> EXPR EXPRLIST_CAP
	prod = new Production;
	prod->add(EXPR);
	prod->add(ELISTC);
	ELIST->addProduction(prod);

	// EXPRLIST_CAP -> EXPRLIST
	ELISTC->addProduction(ELIST);

	// EXPRLIST_CAP -> epsilon
	ELISTC->addProduction(epsilon);

	// ----------------------------------------------------
	// VARLIST and VARLIST_CAP Productions
	// ----------------------------------------------------
	// VARLIST -> (identifier type ) VARLIST_CAP
	prod = new Production;
	prod->add(open);
	prod->add(iden);
	prod->add(typeName);
	prod->add(close;
	prod->add(VLISTC);
	VLIST->addProduction(prod);

	// VARLIST_CAP -> VARLIST
	VLISTC->addProduction(VLIST);

	// VARLIST_CAP -> epsilon
	VLISTC->addProduction(epsilon);


	// If we are running in grammar dump mode, dump the grammar and exit.
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
	// Construct our parse tree. The top level non terminal is S
	ParseTree*	tree = new ParseTree(m_entrySymbol);

	// Prepare to iterate over the tokens.
	TokList&	tokens = Lexer::getTokens();
	TokListIter	iter(tokens);
}



// ****************************************************************************
// parse()
// ****************************************************************************
bool
Parser::parse(TokListIter&	iter,
			  ParseTree*	tree,
			  UINT&			tokensParsed)
{
/*	Token*	token = NULL;

	if (tree->isTerminal()) {
		// If this is a terminal node, something went wrong.
		fprintf(stderr, "Parser::parse() encountered terminal node.\n");
		Global::fail();
	} else {
		// This is a non terminal node, so get the productions and loop through
		// them.
		NonTerm*	nTerm = tree->getNonTerm();
		ProdVec&	prods = nTerm->getProductions();

		for (UINT i = 0; i < prods.getNumEntries(); i++) {
			// For each production, loop through the elements and...
			Production*	prod		= prods[i];
			bool		prodSuccess	= false;

			for (UINT j = 0; j < prod.getNumEntries(); j++) {
				ProdEle*	pe = prod[j];

				if (pe->m_isTerm) {
					// If the element is a terminal, check it against the next
					// token from the iterator.
					// TODO:
				} else {
					// If the element is not a terminal, create a new tree
					// branch for this nonterminal and recurse.
					// TODO:
				}

				if (j == prod.getNumEntries() - 1) {
					// If we're here, we have successfully parsed the entire
					// production and it is a good match.
					prodSuccess = true;
				}
			}
		}
	}*/
}