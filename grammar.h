// ****************************************************************************
// ****************************************************************************
// grammar.h
// ****************************************************************************
// 
// ****************************************************************************
// ****************************************************************************



// ****************************************************************************
// Defines
// ****************************************************************************
#ifndef	H_GRAMMAR
#define H_GRAMMAR



// ****************************************************************************
// ProdEle Struct
// ****************************************************************************
struct ProdEle {
	union {
		Terminal*	m_term;
		NonTerm*	m_nonTerm;
	};

	const bool	m_isTerm;

						ProdEle(Terminal*	term);
						ProdEle(NonTerm*	nTerm);

	const comString&	toString();
};



// ****************************************************************************
// Production Struct
// ****************************************************************************
class Production : ProdEleVec {
  public:
	void				add(Terminal*	t)	{append(new ProdEle(t));}
	void				add(NonTerm*	n)	{append(new ProdEle(n));}

	comString&			printable(comString&	buf);
};



// ****************************************************************************
// Terminal Struct
// ****************************************************************************
struct Terminal {
	Token::Type		m_tType;
	const comString	m_spelling;

						Terminal(Token::Type	type);
						Terminal(Token::Type		type,
								 const comString&	spelling);
};



// ****************************************************************************
// NonTerm Class
// ****************************************************************************
class NonTerm {
  public:
						NonTerm(const comString&	name);

	void				addProduction(Production*	production);
	void				addProduction(Terminal*	term);
	void				addProduction(NonTerm*	nTerm);

	const comString&	getName() const	{return m_name;}
	ProdVec&			getProductions()	{return m_productions;}

	static void			dumpGrammar();

  private:
	ProdVec				m_productions;
	const comString		m_name;

	static NTermVec		m_nTerms;
};



// ****************************************************************************
// ParseTree Class
// ****************************************************************************
class ParseTree {
  public:
						ParseTree(NonTerm*	nTerm);
						ParseTree(Terminal*	term);

	bool				isTerminal()	{return m_prodEle.m_isTerm;}
	const Terminal*		getTerminal()	{return m_prodEle.m_term;}
	const NonTerm*		getNonTerm()	{return m_prodEle.m_nonTerm;}

  private:
	ParseTree*			m_children[6];
	ProdEle				m_prodEle;
	UINT				m_numChildren;
	Production*			m_production;
};



#endif