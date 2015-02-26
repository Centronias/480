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
Parser::init(const comString&	file)
{
	printf("Initializing parser.\n");

	if (file != "")
		buildGrammar(file);
	else
		builtinGrammar();

	// If we are running in a special parser mode, do that and then exit.
	if (Global::isDumpGrammar()) {
		NonTerm::dumpGrammar();
		Global::succeed();
	} else if (Global::isGenerateTree()) {
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
			Global::fail();
		}
	} else {
		fprintf(stderr, "Failed to parse tokens\n");
		Global::fail();
	}


	printf("Running type check.\n");
	m_tree->typeCheck();
	printf("Passed type check.\n");
}



// ****************************************************************************
// parse()
// ****************************************************************************
bool
Parser::parse(TokListIter&	iter,
			  ParseTree*	tree,
			  UINT&			tokensParsed)
{
	// This is a non terminal node, so get the productions and loop through
	// them.
	NonTerm*	nTerm = tree->getNonTerm();
	ProdVec&	prods = nTerm->getProductions();
	Token*		token;

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

	token = iter.next();
	printf("Unexpected token (%s:\"%s\") on line %d\n", (const char*) Token::getTypeName(token->getType()), (const char*) token->getSpelling(), token->getLine());
	iter.prev();


	// If we are here, we have gone through all of this nonterminal's
	// productions and not gotten a parse out of it.
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
	FILE*	source = fopen("generated.ibtl", "w");
	ParseTree*	tree = generate(new ParseTree(m_entrySymbol), source, Translator::None);
	tree->print("gen.out");
	fclose(source);
}

ParseTree*
Parser::generate(ParseTree*			tree,
				 FILE*				file,
				 Translator::Type	tType)
{
	// Choose a random production to use which includes a translation scheme
	// which has the proper return type. We assume that we will eventually get
	// a useful production.
	Production*		prod	= NULL;
	TransScheme*	tScheme	= NULL;

	while (true) {
		prod = tree->getNonTerm()->getProductions()[rand() % tree->getNonTerm()->getProductions().getNumEntries()];
		tree->setProduction(prod);

		// Make sure this production doesn't use variables.
		bool	variables = false;
		for (UINT i = 0; i < prod->getNumEntries(); i++) {
			if (prod->get(i)->m_isTerm && prod->get(i)->m_term->m_tType == Token::Identifier) {
				variables = true;
				break;
			}
		}

		if (variables)
			continue;

		// Choose a random translation scheme to use whose return type is the same
		// as the type we are supposed to use. Make sure before trying to select
		// one that one exists.
		if (tType != Translator::None && tType != Translator::Any) {
			bool	valid = false;
			for (UINT i = 0; i < prod->getTransSchemes().getNumEntries(); i++) {
				if (prod->getTransSchemes()[i]->returns() == tType) {
					valid = true;
					break;
				}
			}

			if (!valid)
				continue;
		}

		do {
			tScheme = prod->getTransSchemes()[rand() % prod->getTransSchemes().getNumEntries()];
		} while (tType != Translator::None && tType != Translator::Any && tScheme->returns() != tType);

		break;
	}

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
			tree->addChild(generate(new ParseTree(pe->m_nonTerm), file, tScheme->getPreType(j)));
		}
	}

	return tree;
}



// ****************************************************************************
// buildGrammar()
// ****************************************************************************
void
Parser::buildGrammar(const comString&	filename)
{
	FILE*	input = fopen((const char*) filename, "r");
	char	line[128];

	if (!input) {
		fprintf(stderr, "Grammar file \"%s\" failed to open.\n", (const char*) filename);
		Global::fail();
	}

	comVector<NonTerm*>		nTerms;
	comVector<Terminal*>	terms;
	comVector<Production*>	prods;
	bool					unique;

	// Loop through each line of the file, looking for nonterminal
	// declarations.
	while (fgets(line, 128, input)) {
		// Get rid of icky line breaks.
		for (char*	loc = line; *loc; loc++) {
			if (*loc == '\n' || *loc =='\r')
				*loc = '\0';
		}

		// If the first character on the line is not a tab, the line is a
		// nonterminal declaration.
		if (*line != '\t')
			nTerms.append(new NonTerm(line));
	}

	rewind(input);
	NonTerm*	current = NULL;
	Production*	pCurr	= NULL;

	// Loop through each line in the file, looking for productions.
	while(fgets(line, 128, input)) {
		// Get rid of icky line breaks.
		for (char*	loc = line; *loc; loc++) {
			if (*loc == '\n' || *loc =='\r')
				*loc = '\0';
		}

		if (*line == '\t') {
			// If the first character on the line is a tab, it is a production
			// for the current nonterminal.

			// If the second character is also a tab, this is a translation
			// scheme for the current production.
			if (*(line + 1) == '\t') {
				if (!pCurr) {
					fprintf(stderr, "Tried to parse a translation without a production. \"%s\"", line);
					Global::fail();
				}

				// If the last parsed production was not unique, do not pay
				// attention to its translation schemes.
				if (!unique)
					continue;

				TransScheme*	tScheme = new TransScheme;
				pCurr->addTScheme(tScheme);
				char			buf[16];
				char*			bLoc	= buf;
				bool			pre		= true;
				UINT			prIndex	= 0;

				// Read out translation scheme elements seperated by single
				// spaces.
				for (char* loc = line + 2; *(loc - 1) != '\0'; loc++) {
					// If we've reached the end of the current tScheme element,
					// add it to the proper scheme half and then continue.
					if (*loc == '\t' || *loc == ' ' || *loc == '\0') {
						*bLoc = '\0';

						// If the buffer is empty, just continue;
						if (bLoc == buf)
							continue;

						// If the spelling was '->', just ignore it.
						if (strcmp(buf, "->") == 0) {
							bLoc = buf;
							continue;
						}

						// If the spelling was ':', unset the pre flag.
						if (strcmp(buf, ":") == 0) {
							pre = false;
							bLoc = buf;
							continue;
						}

						// Do the actual adding now that we've pulled out
						// indicator stuff.
						if (pre) {
							// Get the type and add a pretranslation unit if it
							// is valid.
							Translator::Type	type = Translator::getType(buf);
							if (type != (Translator::Type) -1) {
								tScheme->addPre(type, prIndex);
								bLoc = buf;
							}

							prIndex++;
						} else {
							if (Global::isNumeric(buf)) {
								tScheme->addPost((UINT) atoi(buf));
								bLoc = buf;
							} else {
								tScheme->addPost(buf);
								bLoc = buf;
							}
						}

						bLoc = buf;
					} else {
						// Copy the current character on the line into the buffer.
						*(bLoc++) = *loc;
					}
				}
			} else {
				if (!current) {
					fprintf(stderr, "Tried to parse a production without a nonterminal. \"%s\"", line);
					Global::fail();
				}

				Production*	prod	= new Production;
				char		buf[16];
				char*		bLoc	= buf;

				// Read out production elements seperated by single spaces and add
				// them to the current production.
				for (char* loc = line + 1; *(loc - 1) != '\0'; loc++) {
					if (*loc == ' ' || *loc == '\0') {
						// We've reached the end of the current production
						// element's spelling. Look for it in the list of nonterms
						// and then the list of terminals.
						*bLoc = '\0';
						bool found = false;

						// If the read in word is '->', we need to swap to the next
						// loop to read in the translation scheme.
						if (strcmp(buf, "->") == 0)
							break;

						for (UINT i = 0; i < nTerms.getNumEntries(); i++) {
							if (nTerms[i]->getName() == buf) {
								prod->add(nTerms[i]);
								bLoc = buf;
								found = true;
								break;
							}
						}

						if (found)
							continue;

						for (UINT i = 0; i < terms.getNumEntries(); i++) {
							if (termMatch(terms[i], buf)) {
								prod->add(terms[i]);
								bLoc = buf;
								found = true;
								break;
							}
						}

						if (found)
							continue;

						// If we're here, this production element does not yet exist.
						// Add it as a terminal.
						Terminal*	term = makeTerminal(buf);
						terms.append(term);
						prod->add(term);
						bLoc = buf;
					} else {
						// Copy the current character on the line into the buffer.
						*(bLoc++) = *loc;
					}
				}

				// Look for duplicate productions. If one exists, replace this
				// one with it.
				unique = true;

				for (UINT i = 0; i < prods.getNumEntries(); i++) {
					if (prods[i]->matches(prod)) {
						delete prod;
						prod = prods[i];
						unique = false;
						break;
					}
				}

				if (unique)
					prods.append(prod);

				current->addProduction(prod);
				pCurr	= prod;
			}
		} else {
			// The current line is a nonterm. Find it in the list of prepared
			// nonterms and make it the current production.
			line[strlen(line)] = '\0';
			for (UINT i = 0; i < nTerms.getNumEntries(); i++) {
				if (nTerms[i]->getName() == line) {
					current = nTerms[i];
					break;
				}
			}
		}
	}

	// Check that all of the nonterminals have productions.
	for (UINT i = 0; i < nTerms.getNumEntries(); i++) {
		if (nTerms[i]->getProductions().getNumEntries() == 0) {
			fprintf(stderr, "Nonterminal symbol \"%s\" has no productions.\n", (const char*) nTerms[i]->getName());
			Global::fail();
		}
	}

	// Check that all of the productions have translation schemes if we are
	// actually going to translate.
	for (UINT i = 0; i < prods.getNumEntries(); i++) {
		if (prods[i]->getTransSchemes().getNumEntries() == 0) {
			comString	printable;
			fprintf(stderr, "Production \"%s\" has no translation schemes.\n", (const char*) prods[i]->printable(printable));
			Global::fail();	
		}
	}

	// Assume the first symbol in the list is the entry symbol.
	m_entrySymbol = nTerms[0];

	fclose(input);
}



// ****************************************************************************
// makeTerminal()
// ****************************************************************************
Terminal*
Parser::makeTerminal(const comString&	spelling)
{
	if (spelling == "id")
		return new Terminal(Token::Identifier);
	if (spelling == "string")
		return new Terminal(Token::StrConst);
	if (spelling == "int")
		return new Terminal(Token::IntConst);
	if (spelling == "real")
		return new Terminal(Token::RealConst);
	if (spelling == "bool")
		return new Terminal(Token::BoolConst);
	if (spelling == "type")
		return new Terminal(Token::PrimType);
	if (spelling == "~" || spelling == "NONE")
		return new Terminal(Token::NONE);

	Token*	token = Lexer::tokenize(spelling);
	if (!token) {
		fprintf(stderr, "Failed to tokenize \"%s\"\n", (const char*) spelling);
		Global::fail();
	}

	Terminal*	ret = new Terminal(token->getType(), token->getSpelling());
	delete token;

	return ret;
}



// ****************************************************************************
// termMatch()
// ****************************************************************************
bool
Parser::termMatch(Terminal*			term,
				  const comString&	spelling)
{
	// If the terminal's spelling in blank, it is a constant with variable
	// spelling and must be checked against the token type.
	if (term->m_spelling == "") {
		switch (term->m_tType) {
		  case Token::StrConst:
			return spelling == "string";
		  case Token::IntConst:
			return spelling == "int";
		  case Token::RealConst:
			return spelling == "real";
		  case Token::BoolConst:
			return spelling == "bool";
		  case Token::Identifier:
			return spelling == "id";
		  case Token::PrimType:
			return spelling == "type";
		  case Token::NONE:
			return spelling == "~" || spelling == "NONE";
		  default:
			fprintf(stderr, "Trying to match against non-literal constant type token with variable spelling: (%d)\n", term->m_tType);
			Global::fail();
		}
	}

	return term->m_spelling == spelling;
}



// ****************************************************************************
// builtinGrammar()
// ****************************************************************************
void
Parser::builtinGrammar()
{

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
}