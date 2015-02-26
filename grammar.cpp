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
NTermVec				NonTerm::m_nTerms;



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

		for (UINT j = 0; j < nTerm->m_productions.getNumEntries(); j++) {
			Production*	prod = nTerm->m_productions[j];
			fprintf(file, "\t%s\n", (const char*) prod->printable(printable));

			for (UINT k = 0; k < prod->getTransSchemes().getNumEntries(); k++)
				fprintf(file, "\t\t%s\n", (const char*) prod->getTransSchemes()[k]->printable(printable));
		}
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
	m_token(NULL),
	m_type(Translator::None)
{
	for (UINT i = 0; i < PT_MAX_CHILDREN; i++)
		m_children[i] = NULL;
}

ParseTree::ParseTree(Terminal*	term,
					 Token*		token)
:	m_prodEle(term),
	m_numChildren(0),
	m_production(NULL),
	m_token(token),
	m_type(Translator::None)
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

	fprintf(file, "%s (%s):", (const char*) m_prodEle.toString(), (const char*) Translator::typeToString(m_type));

	if (m_production)
		fprintf(file, " %s\n", (const char*) m_production->printable(printable));
	else if (m_token)
		fprintf(file, " %s\n", (const char*) m_token->printable(printable));
	else
		fprintf(file, "\n");

	for (UINT i = 0; i < m_numChildren; i++)
		m_children[i]->print(level + 1, file);
}



// ****************************************************************************
// ParseTree::typeCheck()
// ****************************************************************************
void
ParseTree::typeCheck()
{
	UINT	line = -1;
	typeCheck(line);
}

void
ParseTree::typeCheck(UINT&	lastLine)
{
	if (m_numChildren) {
		// If this node has children, recurse into them.
		for (UINT i = 0; i < m_numChildren; i++)
			m_children[i]->typeCheck(lastLine);

		for (UINT i = 0; i < m_production->getTransSchemes().getNumEntries(); i++) {
			PrVec&	pre		= m_production->getTransSchemes()[i]->getPreVec();
			bool	match	= true;

			for (UINT j = 0; pre.getNumEntries() && j < pre.getNumEntries() - 1; j++) {
				if (pre[j]->m_type != Translator::Any && pre[j]->m_type != m_children[pre[j]->m_index]->m_type) {
					match = false;
					break;
				}
			}

			if (match) {
				m_scheme = m_production->getTransSchemes()[i];
				break;
			}
		}

		if (!m_scheme) {
			comString	buf;
			fprintf(stderr, "Type error in production \"%s\" originating from line %d.\n", (const char*) m_production->printable(buf), lastLine);
			fprintf(stderr, "Could not match >");
			for (UINT j = 0; j < m_numChildren; j++)
				fprintf(stderr, "%s ", (const char*) Translator::typeToString(m_children[j]->m_type));
			printf("< to schemes\n");
			for (UINT j = 0; j < m_production->getTransSchemes().getNumEntries(); j++)
				fprintf(stderr, "\t\"%s\"\n", (const char*) m_production->getTransSchemes()[j]->printable(buf));
			Global::fail();
		}

		PrVec&	pre	= m_scheme->getPreVec();
		if (pre.getNumEntries())
			m_type = pre[pre.getNumEntries() - 1]->m_type;
		else
			m_type = Translator::None;
	} else {
        if (!m_token)
            m_type = Translator::None;
        else {
        	switch(m_token->getType()) {
			  case Token::BoolConst:
				m_type = Translator::Bool;
				break;
			  case Token::RealConst:
				m_type = Translator::Float;
				break;
			  case Token::IntConst:
				m_type = Translator::Int;
				break;
			  case Token::StrConst:
				m_type = Translator::Str;
				break;
			  case Token::Identifier:
				fprintf(stderr, "Identifiers are not currently supported.\n");
				Global::fail();
			  default:
				m_type = Translator::None;
				break;
			}
			
			lastLine = m_token->getLine();
		}
	}
}



// ****************************************************************************
// Production::~Production()
// ****************************************************************************
Production::~Production()
{
	for (UINT i = 0; i < getNumEntries(); i++)
		delete m_data[i];
}



// ****************************************************************************
// Production::matches()
// ****************************************************************************
bool
Production::matches(Production*	o)
{
	if (o->getNumEntries() != getNumEntries())
		return false;

	for (UINT i = 0; i < getNumEntries(); i++) {
		if (o->get(i)->m_isTerm != this->get(i)->m_isTerm)
			return false;

		if (this->get(i)->m_isTerm) {
			if (this->get(i)->m_term != o->get(i)->m_term)
				return false;
		} else {
			if (this->get(i)->m_nonTerm != o->get(i)->m_nonTerm)
				return false;
		}
	}

	return true;
}