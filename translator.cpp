// ****************************************************************************
// ****************************************************************************
// translator.cpp
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
comString					Translator::m_typeNames[];



// ****************************************************************************
// Translator::init()
// ****************************************************************************
void
Translator::init()
{
	m_typeNames[Translator::Int]	= "int";
	m_typeNames[Translator::Str]	= "string";
	m_typeNames[Translator::Bool]	= "bool";
	m_typeNames[Translator::Float]	= "real";
	m_typeNames[Translator::Any]	= "any";
	m_typeNames[Translator::None]	= "none";
}



// ****************************************************************************
// Translator::getType()
// ****************************************************************************
Translator::Type
Translator::getType(const comString&	spelling)
{
	if (spelling == "i" || spelling == "int") {
		return Translator::Int;
	} else if (spelling == "r" || spelling == "real") {
		return Translator::Float;
	} else if (spelling == "b" || spelling == "bool") {
		return Translator::Bool;
	} else if (spelling == "s" || spelling == "string") {
		return Translator::Str;
	} else if (spelling == "@" || spelling == "any") {
		return Translator::Any;
	} else if (spelling == "~" || spelling == "none") {
		return Translator::None;
	} else {
		return (Translator::Type) -1;
	}
}



// ****************************************************************************
// Translator::run()
// ****************************************************************************
void
Translator::run()
{
	// Run the type check before we translate. We do this here because the
	// parser can successfully parse without getting types matched.
	Parser::typeCheck();

	printf("Running translator.\n");
	FILE*	output = fopen("translated.forth", "w");
	VarDef::printVarDefHeader(output);
	FuncDef::printFuncDefHeader(output);
	fprintf(output, ": main\n");
	run(Parser::m_tree, output);
	fprintf(output, "\n; main");
	fclose(output);
	printf("Done running translator.\n");
}

void
Translator::run(ParseTree*	tree,
				FILE*		output)
{
	// If this node is a function invocation, we need to assign parameters.
	if (!tree->isTerminal() && tree->m_production->isFuncInvocation()) {
		translateFuncInvocation(tree, output);
		return;
	}

	// If this node does not have a scheme...
	if (!tree->getScheme()) {
		// ... and it's a terminal, just print it.
		if (tree->isTerminal()) {
			comString	buf;
			fprintf(output, "%s ", (const char*) translateSpelling(tree, buf));
			return;
		}

		// If there's no token, then this is an epsilon branch.
		if (!tree->getToken())
			return;

		// Otherwise error out.
		comString	buf;
		fprintf(stderr, "Nonliteral tree node without scheme \"%s\"\n", (const char*) tree->getProduction()->printable(buf));
		Global::fail();
	}

	// Loop through the translation scheme's elements.
	PoVec&	post = tree->getScheme()->getPostVec();
	for (UINT i = 0; i < post.getNumEntries(); i++) {
		PoUnit*	p = post[i];

		if (p->m_isKeyword) {
			// If the unit is a keyword, just print it.
			fprintf(output, "%s ", (const char*) p->m_keyword);
		} else {
			// If the unit is an index, recurse into that child of the tree.
			run(tree->getChild(p->m_index), output);
		}
	}
}



// ****************************************************************************
// Translator::translateSpelling()
// ****************************************************************************
comString&
Translator::translateSpelling(ParseTree*	tree,
							  comString&	buf)
{
	Token*	token = tree->getToken();

	switch (token->getType()) {
	  case Token::BoolConst:
		if (token->getSpelling() == "true")
			return buf = "-1";
		else
			return buf = "0";
		break;
	  case Token::StrConst:
		{
			char	cBuf[64];
			sprintf(cBuf, "s\" %s", ((const char*) token->getSpelling()) + 1);

			return buf = cBuf;
		}
		break;
	  case Token::Identifier:
		{
			VarDef*	vd = tree->findVarDef(token->getSpelling());
			if (vd) {
				return buf = vd->m_postName;
			} else {
				fprintf(stderr, "Failed to find variable definition for identifier \"%s\" on line %d.\n", (const char*) token->getSpelling(), token->getLine());
				Global::fail();
			}
		}
	  default:
		break;
	}

	return buf = token->getSpelling();
}



// ****************************************************************************
// Translator::getPrimType()
// ****************************************************************************
Translator::Type
Translator::getPrimType(const comString&	spelling)
{
	if (spelling == "int")
		return Translator::Int;
	else if (spelling == "real")
		return Translator::Float;
	else if (spelling == "bool")
		return Translator::Bool;
	else if (spelling == "string")
		return Translator::Str;
	else {
		fprintf(stderr, "WARNING: Tried to parse \"%s\" into primitive type.\n", (const char*) spelling);
		return Translator::None;
	}
}



// ****************************************************************************
// Translator::buildFunction()
// ****************************************************************************
void
Translator::buildFunction(ParseTree*	tree,
						  UINT&			lastLine)
{	
	// Admittedly a hack.
	tree->forceScopeEdge();

	// We know that this is a parse tree using the production
	//	let ( ( id FUNLIST ) ( type FUNTYPE ) ) EXPR EXPRLIST
	// Immediately create a function definition.
	const comString&	fId		= tree->getChild(3)->getToken()->getSpelling();
	Translator::Type	type	= Translator::getPrimType(tree->getChild(7)->getToken()->getSpelling());
	char				prefix[64];
	sprintf(prefix, "f%p_%s", tree, (const char*) fId);
	FuncDef*	def = new FuncDef(fId, prefix, type, tree);
	tree->addFuncDef(def);

	// Look for parameters.
	const comString*	identifier;
	VarDef*				vDef = NULL;
	ParseTree*			iRec = tree->getChild(4);
	ParseTree*			tRec = tree->getChild(8);
	bool				foundIdentifier	= false;
	bool				foundType		= false;
	char				postName[64];

	while (true) {
		foundIdentifier	= false;
		foundType		= false;

		// If this node has children, this node has a parameter id. Its
		// production looks like
		// 	id FUNLIST
		if (iRec->getNumChildren()) {
			identifier = &iRec->getChild(0)->getToken()->getSpelling();
			iRec = iRec->getChild(1);
			foundIdentifier = true;
		}

		// If this node has children, this node has a parameter type. Its
		// production looks like
		// 	type FUNTYPE
		if (tRec->getNumChildren()) {
			type = Translator::getPrimType(tRec->getChild(0)->getToken()->getSpelling());
			tRec = tRec->getChild(1);
			foundType = true;	
		}

		if (foundType && foundIdentifier) {
			// We got a type and an identifier. Make them into a parameter.
			sprintf(postName, "p%s_%s", prefix, (const char*) *identifier);
			vDef = new VarDef(*identifier, postName, type);
			tree->forceVarDef(vDef);
			def->m_params.append(vDef);
		} else if (foundType || foundIdentifier) {
			// We got one or the other but not both. Error out.
			fprintf(stderr, "Mismatched number of parameter types and identifiers for function \"%s\"\n", (const char*) fId);
			Global::fail();
		} else {
			// Otherwise we didn't get either and will stop looking for parameters.
			break;
		}
	}

	// Now that we've made the declaration. Let's typecheck the definition
	// itself.
	tree->getChild(11)->typeCheck(lastLine);
	tree->getChild(12)->typeCheck(lastLine);
}



// ****************************************************************************
// Translator::typeCheckFunction()
// ****************************************************************************
void
Translator::typeCheckFunction(ParseTree*	tree,
							  UINT&			lastLine)
{
	// Find the function definition. This node looks like
	//	id PARAMLIST
	// So the first child will be the function's identifier token.
	FuncDef*	def = tree->findFuncDef(tree->getChild(0)->getToken()->getSpelling());
	tree->m_type = def->m_type;

	// Now that we have the definition, make sure that all of the parameters
	// are supplied.
	if (def) {
		VDVec&		params = def->m_params;
		ParseTree*	pRec = tree->getChild(1);
		UINT		pThis = 0;
		UINT		pNext = 1;

		// The param list nodes look like
		// <param> PARAMLIST
		// Use the automatic typechecker for the parameter and then loop on the
		// PARAMLIST
		for (UINT i = 0; i < params.getNumEntries(); i++) {
			if (!pRec) {
				fprintf(stderr, "Call to function \"%s\" on line %d has too few parameters.\n",
						(const char*) tree->getChild(0)->getToken()->getSpelling(),
						tree->getChild(0)->getToken()->getLine());
				Global::fail();
			}
			
			// It is possible that the first token is ( in the production
			//	( OPER ) PARAMLIST
			// If this is the case, just typecheck on the next token.
			if (pRec->getChild(0)->getToken()->getType() == Token::Paren) {
				pThis = 1;
				pNext = 3;
			}

			pRec->getChild(pThis)->typeCheck(lastLine);
			
			if (pRec->getChild(pThis)->m_type != params[i]->m_type) {
				fprintf(stderr, "Parameter \"%s\" (%d) for call to function \"%s\" on line %d failed typecheck. Got %s, expected %s\n",
						(const char*) params[i]->m_preName,
						i,
						(const char*) tree->getChild(pThis)->getToken()->getSpelling(),
						tree->getChild(pThis)->getToken()->getLine(),
						(const char*) typeToString(pRec->getChild(pThis)->m_type),
						(const char*) typeToString(params[i]->m_type));
				Global::fail();
			}

			pRec = pRec->getChild(pNext);
		}
	} else {
		fprintf(stderr, "Attempted to call undeclared function \"%s\" on line %d.\n",
				(const char*) tree->getChild(0)->getToken()->getSpelling(),
				tree->getChild(0)->getToken()->getLine());
		Global::fail();
	}
}



// ****************************************************************************
// Translator::translateFuncInvocation()
// ****************************************************************************
void
Translator::translateFuncInvocation(ParseTree*	tree,
									FILE*		output)
{
	FuncDef*	def = tree->findFuncDef(tree->getChild(0)->getToken()->getSpelling());
	VDVec&		params = def->m_params;
	ParseTree*	pRec = tree->getChild(1);
	UINT		pThis = 0;
	UINT		pNext = 1;

	// The param list nodes look like
	// <param> PARAMLIST
	// Use the automatic translator to translate the first param, then print
	// the post-translation identifier and then finally print the assignment
	// operator appropriate for the type.
	for (UINT i = 0; i < params.getNumEntries(); i++) {
		// It is possible that the first token is ( in the production
		//	( OPER ) PARAMLIST
		// If this is the case, just typecheck on the next token.
		if (pRec->getChild(0)->getToken()->getType() == Token::Paren) {
			pThis = 1;
			pNext = 3;
		}

		run(pRec->getChild(pThis), output);
		fprintf(output, "%s ", (const char*) params[i]->m_postName);
		switch (params[i]->m_type) {
		  case Translator::Bool:
		  case Translator::Int:
			fprintf(output, "! ");
			break;
		  case Translator::Str:
			fprintf(output, "2! ");
			break;
		  case Translator::Float:
			fprintf(output, "f! ");
			break;
		}

		pRec = pRec->getChild(pNext);
	}

	fprintf(output, "%s ", (const char*) def->m_postName);
}