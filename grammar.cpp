// ****************************************************************************
// ****************************************************************************
// grammar.cpp
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
NTermVec	NonTerm::m_nTerms;



// ****************************************************************************
// ProdEle()
// ****************************************************************************
ProdEle::ProdEle(Terminal*	term)
:	m_term(term),
	m_isTerm(true)
{
}

ProdEle::ProdEle(NonTerm*	nTerm)
:	m_nonTerm(nTerm),
	m_isTerm(false)
{
}



// ****************************************************************************
// Terminal()
// ****************************************************************************
Terminal::Terminal(Token::Type	type)
:	m_tType(type),
	m_spelling("")
{
}

Terminal::Terminal(Token::Type		type,
		 const comString&	spelling)
:	m_tType(type),
	m_spelling(spelling)
{
}



// ****************************************************************************
// NonTerm::NonTerm()
// ****************************************************************************
NonTerm::NonTerm(const comString&	name)
:	m_name(name)
{
	m_nTerms.append(this);
}



// ****************************************************************************
// NonTerm::addProduction()
// ****************************************************************************
void
NonTerm::addProduction(Production*	prod)
{
	m_productions.append(prod);
}

void
NonTerm::addProduction(Terminal*	term)
{
	Production*	prod = new Production();
	m_productions.append(prod);
	prod->add(term);
}

void
NonTerm::addProduction(NonTerm*	nTerm)
{
	Production*	prod = new Production();
	m_productions.append(prod);
	prod->add(nTerm);
}



// ****************************************************************************
// NonTerm::dumpGrammar()
//
// This method prints out the nonterminals and their productions which compose
// the grammar constructed for the parser.
// ****************************************************************************
void
NonTerm::dumpGrammar()
{
	printf("Dumping grammar to file.");
	FILE*	file = fopen("grammar.out", "w");
	comString	printable("");

	for (UINT i = 0; i < m_nTerms.getNumEntries(); i++) {
		NonTerm*	nTerm = m_nTerms[i];

		fprintf(file, "%s\n", (const char*) nTerm->m_name);

		for (UINT j = 0; j < nTerm->m_productions.getNumEntries(); j++)
			fprintf(file, "\t%s\n", (const char*) nTerm->m_productions[j]->printable(printable));
	}

	fclose(file);
}



// ****************************************************************************
// ProdEle::toString()
// ****************************************************************************
const comString&
ProdEle::toString()
{
	if (m_isTerm)
		return ((m_term->m_spelling != "") ? m_term->m_spelling : Token::getTypeName(m_term->m_tType));
	else
		return m_nonTerm->getName();
}



// ****************************************************************************
// Production::printable()
// ****************************************************************************
comString&
Production::printable(comString&	buf)
{
	char	printBuffer[128];
	char*	loc = printBuffer;

	for (UINT i = 0; i < getNumEntries(); i++)
		loc += sprintf(loc, "%s ", (const char*) m_data[i]->toString());

	*(loc - 1) = '\0';

	buf = printBuffer;
	return buf;
}



// ****************************************************************************
// ParseTree::ParseTree()
// ****************************************************************************
ParseTree::ParseTree(NonTerm*	nTerm)
:	m_prodEle(nTerm),
	m_numChildren(0),
	m_production(NULL),
	m_token(NULL)
{
	for (UINT i = 0; i < PT_MAX_CHILDREN; i++)
		m_children[i] = NULL;
}

ParseTree::ParseTree(Terminal*	term,
					 Token*		token)
:	m_prodEle(term),
	m_numChildren(0),
	m_production(NULL),
	m_token(token)
{
	for (UINT i = 0; i < PT_MAX_CHILDREN; i++)
		m_children[i] = NULL;
}



// ****************************************************************************
// Terminal::matches()
// ****************************************************************************
bool
Terminal::matches(const Token*	token)
{
	if (token->getType() != m_tType)
		return false;

	if (m_spelling != "")
		return m_spelling == token->getSpelling();

	return true;
}



// ****************************************************************************
// ParseTree::addChild()
// ****************************************************************************
void
ParseTree::addChild(ParseTree*	child)
{
	if (m_numChildren == PT_MAX_CHILDREN) {
		fprintf(stderr, "Attempted to add too many children.\n");
		Global::fail();
	}

	m_children[m_numChildren++] = child;
}



// ****************************************************************************
// ParseTree::~ParseTree()
// ****************************************************************************
ParseTree::~ParseTree()
{
	for (UINT i = 0; i < m_numChildren; i++)
		delete m_children[i];
}



// ****************************************************************************
// ParseTree::cullChildren()
// ****************************************************************************
void
ParseTree::cullChildren()
{
	for (UINT i = 0; i < m_numChildren; i++)
		delete m_children[i];

	m_numChildren = 0;
}



// ****************************************************************************
// ParseTree::print()
// ****************************************************************************
void
ParseTree::print(const comString&	filename)
{
	FILE*	file = fopen((const char*) filename, "w");
	this->print(0, file);
	fclose(file);
}

void
ParseTree::print(UINT	level,
				 FILE*	file)
{
	comString	printable("");

	for (UINT i = 0; i < level; i++)
		fprintf(file, "\t");

	fprintf(file, "%s :", (const char*) m_prodEle.toString());

	if (m_production)
		fprintf(file, " %s\n", (const char*) m_production->printable(printable));
	else if (m_token)
		fprintf(file, " %s\n", (const char*) m_token->printable(printable));
	else
		fprintf(file, "\n");

	for (UINT i = 0; i < m_numChildren; i++)
		m_children[i]->print(level + 1, file);
}