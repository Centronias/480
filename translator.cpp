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
comString	Translator::m_typeNames[];



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
	printf("Running translator.\n");
	FILE*	output = fopen("translated.forth", "w");
	run(Parser::m_tree, output);
	fclose(output);
	printf("Done running translator.\n");
}

void
Translator::run(ParseTree*	tree,
				FILE*		output)
{

	// If this node does not have a scheme...
	if (!tree->getScheme()) {
		// ... and it's a terminal, just print it.
		if (tree->isTerminal()) {
			fprintf(output, "%s ", (const char*) tree->getToken()->getSpelling());
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