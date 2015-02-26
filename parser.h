// ****************************************************************************
// ****************************************************************************
// Parser.h
// ****************************************************************************
// 
// ****************************************************************************
// ****************************************************************************



// ****************************************************************************
// Defines
// ****************************************************************************
#ifndef	H_PARSER
#define H_PARSER



// ****************************************************************************
// Parser Class
// ****************************************************************************
class Parser {
  public:
	static void			init(const comString&	grammarFile);
	static void			run();
	static void			buildGrammar(const comString&	filename);
	static void			builtinGrammar();
	static bool			parse(TokListIter&	tIter,
							  ParseTree*	tree,
							  UINT&			tokensParsed);
	static void			generate();

	static void			printTree();

  private:
  	static ParseTree*	generate(ParseTree*			tree,
  								 FILE*				file,
  								 Translator::Type	tType);
  	static Terminal*	makeTerminal(const comString&	spelling);
  	static bool			termMatch(Terminal*	term,
  								  const comString&	spelling);

	static NonTerm*		m_entrySymbol;
	static ParseTree*	m_tree;

friend class Translator;
};



#endif