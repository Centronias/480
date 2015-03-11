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
class Production : public ProdEleVec {
  public:
						~Production();

	void				add(Terminal*	t)	{append(new ProdEle(t));}
	void				add(NonTerm*	n)	{append(new ProdEle(n));}
	void				addTScheme(TransScheme*	tScheme)	{m_tSchemes.append(tScheme);}

	void				setDeclarator()	{m_declarator = true;}
	bool				isDeclarator()	{return m_declarator;}

	comString&			printable(comString&	buf);
	bool				matches(Production*	o);

	TSVec&				getTransSchemes()	{return m_tSchemes;}

  private:
	TSVec				m_tSchemes;
	bool				m_declarator;
};



// ****************************************************************************
// Terminal Struct
// ****************************************************************************
class Terminal {
  public:
	Token::Type		m_tType;
	const comString	m_spelling;

						Terminal(Token::Type	type);
						Terminal(Token::Type		type,
								 const comString&	spelling);
	bool				matches(const Token*	token);
};



// ****************************************************************************
// NonTerm Class
// ****************************************************************************
class NonTerm {
  public:
						NonTerm(const comString&	name,
								bool				scopeEdge = false);

	void				addProduction(Production*	production);
	void				addProduction(Terminal*	term);
	void				addProduction(NonTerm*	nTerm);

	const comString&	getName() const	{return m_name;}
	ProdVec&			getProductions()	{return m_productions;}

	bool				isScopeEdge() const	{return m_scopeEdge;}

	static void			dumpGrammar();

  private:
	ProdVec				m_productions;
	const comString		m_name;
	bool				m_scopeEdge;

	static NTermVec		m_nTerms;
};



// ****************************************************************************
// ParseTree Class
// ****************************************************************************
class ParseTree {
  public:
						ParseTree(NonTerm*	nTerm);
						ParseTree(Terminal*	term,
								  Token*	token);
						~ParseTree();

	void				typeCheck();

	void				addChild(ParseTree*	child);
	void				cullChildren();
	ParseTree*			getChild(UINT	i)	{return m_children[i];}

	void				setProduction(Production*	production)	{m_production = production;}
	Production*			getProduction()	{return m_production;}

	void				addVarDef(const comString&	identifier,
								  Translator::Type	type,
								  bool				found = false);
	VarDef*				findVarDef(const comString&	identifier);

	bool				isTerminal()	{return m_prodEle.m_isTerm;}
	Terminal*			getTerminal()	{return m_prodEle.m_term;}
	NonTerm*			getNonTerm()	{return m_prodEle.m_nonTerm;}
	void				print()	{print("tree.out");}
	void				print(const comString&	filename);
	const comString&	getDesc()	{return m_prodEle.toString();}
	TransScheme*		getScheme()	{return m_scheme;}
	Token*				getToken()	{return m_token;}

  private:
	void				print(UINT	level,
							  FILE*	file);
	void				typeCheck(UINT&	lastLine);

	ParseTree*			m_children[PT_MAX_CHILDREN];
	ParseTree*			m_parent;
	ProdEle				m_prodEle;
	UINT				m_numChildren;
	Production*			m_production;
	Token*				m_token;
	Translator::Type	m_type;
	TransScheme*		m_scheme;
	bool				m_scopeEdge;
	VDVec				m_varDefs;
};



// ****************************************************************************
// VarDef Struct
// ****************************************************************************
struct VarDef {
	const comString		m_preName;
	const comString		m_postName;
	Translator::Type	m_type;

						VarDef(const comString&	rName,
							   const comString&	oName,
							   Translator::Type	type);
};



#endif