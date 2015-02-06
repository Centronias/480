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
	FILE*	file = fopen("grammardump.out", "w");

	for (UINT i = 0; i < m_nTerms.getNumEntries(); i++) {
		NonTerm*	nTerm = m_nTerms[i];

		fprintf(file, "%s\n", (const char*) nTerm->m_name);

		for (UINT j = 0; j < nTerm->m_productions.getNumEntries(); j++) {
			Production*	prod = nTerm->m_productions[j];

			fprintf(file, "\t");
			for (UINT k = 0; k < prod->m_elements.getNumEntries(); k++) {
				ProdEle*	e = prod->m_elements[k];

				if (e->m_isTerm)
					fprintf(file, "%s ", (const char*) ((e->m_term->m_spelling != "") ? e->m_term->m_spelling : Token::getTypeName(e->m_term->m_tType)));
				else
					fprintf(file, "%s ", (const char*) e->m_nonTerm->m_name);
			}
			fprintf(file, "\n");
		}
	}
}