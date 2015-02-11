// ****************************************************************************
// ****************************************************************************
// lexer.h
// ****************************************************************************
// 
// ****************************************************************************
// ****************************************************************************



// ****************************************************************************
// Defines
// ****************************************************************************
#ifndef	H_LEXER
#define H_LEXER



// ****************************************************************************
// Lexer Class
// ****************************************************************************
class Lexer {
  public:
	static void			readCmdLine(int		argc,
									char**	argv,
									char*	inFile);
	static void			init();
	static void			run(const comString&	inFile);

	static TokList&		getTokens()	{return m_tokens;}
	static void			printTokens();

	static bool			dumpFSA()	{return m_dumpFSA;}

  private:
  	static Token::Type	checkKeyword(const char*	spelling);

	static void			addCompOps();
	static void			addMultOps();
	static void			addAddOps();
	static void			addUnaryOps();
	static void			addStringConsts();
	static void			addNumericConsts();
	static void			addOthers();
	static State*		addIdentifiers();

	static State*		m_entryState;

	static TokList		m_tokens;

	static const char	m_idStarts[];
	static const char	m_idMids[];
	static const char	m_stringChars[];
	static const char	m_numerals[];

	static bool			m_debug;
	static bool			m_dumpFSA;
};



#endif