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
};



// ****************************************************************************
// Production Struct
// ****************************************************************************
struct Production {
	ProdEleVec			m_elements;

	void				add(ProdEle*	ele)	{m_elements.append(ele);}
	void				add(Terminal*	t)	{m_elements.append(new ProdEle(t));}
	void				add(NonTerm*	n)	{m_elements.append(new ProdEle(n));}
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

	static void			dumpGrammar();

  private:
	ProdVec				m_productions;
	const comString		m_name;

	static NTermVec		m_nTerms;
};



#endif