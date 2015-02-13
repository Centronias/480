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
ParseTree*	Parser::m_tree;



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
	unOps.append(new Terminal(Token::Operator, "not"));
	unOps.append(new Terminal(Token::Operator, "sin"));
	unOps.append(new Terminal(Token::Operator, "cos"));
	unOps.append(new Terminal(Token::Operator, "tan"));

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
	OPERM->addProduction(RESR);

	// OPER_MINUS -> epsilon
	OPERM->addProduction(epsilon);

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
	prod->add(close);
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

	if (Global::isGenerateTree()) {
		generate();
		Global::succeed();
	}
}



// ****************************************************************************
// run()
// ****************************************************************************
void
Parser::run()
{
	printf("Running parser.\n");

	// Construct our parse tree. The top level non terminal is S
	m_tree = new ParseTree(m_entrySymbol);

	// Prepare to iterate over the tokens.
	TokListIter	iter(Lexer::getTokens());
	UINT		tokensParsed;

	if (parse(iter, m_tree, tokensParsed)) {
		if (!iter.next()) {
			printf("Done running parser.\n");
		} else {
			fprintf(stderr, "Failed to parse all tokens.\n");
//			Global::fail();
		}
	} else {
		fprintf(stderr, "Failed to parse tokens\n");
		Global::fail();
	}
}



// ****************************************************************************
// parse()
// ****************************************************************************
bool
Parser::parse(TokListIter&	iter,
			  ParseTree*	tree,
			  UINT&			tokensParsed)
{
	Token*		token;

	if (tree->isTerminal()) {
		// If this is a terminal node, something went wrong.
		Global::fail();
	} else {
		// This is a non terminal node, so get the productions and loop through
		// them.
		NonTerm*	nTerm = tree->getNonTerm();
		ProdVec&	prods = nTerm->getProductions();

		for (UINT i = 0; i < prods.getNumEntries(); i++) {
			// For each production, loop through the elements and...
			Production*	prod			= prods[i];
			bool		prodSuccess		= true;
			UINT		tokensConsumed	= 0;

			for (UINT j = 0; j < prod->getNumEntries(); j++) {
				ProdEle*	pe = prod->get(j);

				if (pe->m_isTerm) {
					if (pe->m_term->m_tType == Token::NONE) {
						// If the nonterminal is an epsilon, effectively ignore
						// it, consuming no tokens.
						continue;
					}

					if ((token = iter.next()))
						tokensConsumed++;

					if (!token || !pe->m_term->matches(token)) {
						// If the next token does not match the production,
						// this production is not appropriate.
						prodSuccess = false;
						break;
					} else {
						tree->addChild(new ParseTree(pe->m_term, token));
					}
				} else {
					// If the element is not a terminal, create a new tree for
					// this nonterminal and recurse. Also keep track of how
					// many tokens this recursion consumes in case we need to
					// roll back the iterator later.
					ParseTree*	child = new ParseTree(pe->m_nonTerm);
					UINT		recurseParsed = 0;

					if (parse(iter, child, recurseParsed)) {
						// If the recursive parse succeeded, add the number of
						// newly parsed tokens to the tokens consumed count and
						// attach the child tree to the current tree.
						tree->addChild(child);
						tokensConsumed += recurseParsed;
					} else {
						// If the recursive parse failed, this production is
						// not appropriate.
						delete child;
						prodSuccess = false;
						break;
					}
				}
			}

			if (prodSuccess) {
				// If the current production worked the whole way through, set
				// this production as this tree's production, add the number of
				// tokens consumed by the production to the number of tokens
				// parsed and return.
				tree->setProduction(prod);
				tokensParsed += tokensConsumed;
				return true;
			} else {
				// If the current production did not succeed, roll back the
				// iterator based on the number of tokens consumed and remove
				// any child trees from the current tree.
				for (UINT j = 0; j < tokensConsumed; j++)
					iter.prev();
				tree->cullChildren();
			}
		}
	}

	// If we are here, we have gone through all of this nonterminal's
	// productions and gotten a parse out of it.
	return false;
}



// ****************************************************************************
// printTree()
// ****************************************************************************
void
Parser::printTree()
{
	if (m_tree)
		m_tree->print();
	else
		printf("No tree to print");
}



// ****************************************************************************
// generate()
// ****************************************************************************
void
Parser::generate()
{
	FILE*	source = fopen("source.out", "w");
	fprintf(source, "\n\n");
	ParseTree*	tree = generate(new ParseTree(m_entrySymbol), source);
	tree->print("gen.out");
	fclose(source);
}

ParseTree*
Parser::generate(ParseTree*	tree,
				 FILE*		file)
{
	// Choose a random production to use.
	Production*	prod = tree->getNonTerm()->getProductions()[rand() % tree->getNonTerm()->getProductions().getNumEntries()];
	tree->setProduction(prod);

	// For each element in the production...
	for (UINT j = 0; j < prod->getNumEntries(); j++) {
		ProdEle*	pe = prod->get(j);

		// If the element is a terminal...
		if (pe->m_isTerm) {
			if (pe->m_term->m_tType == Token::NONE) {
				// If the element is an epsilon,  do nothing.
				continue;
			}

			// Add the terminal to the tree.
			const comString&	spelling = (pe->m_term->m_spelling != "") ? pe->m_term->m_spelling : Token::getExample(pe->m_term->m_tType);

			tree->addChild(new ParseTree(pe->m_term, new Token(pe->m_term->m_tType, spelling, 0)));

			fprintf(file, "%s ", (const char*) spelling);
		} else {
			// If the element is not a terminal, create a new tree for
			// this nonterminal and recurse.
			tree->addChild(generate(new ParseTree(pe->m_nonTerm), file));
		}
	}

	return tree;
}