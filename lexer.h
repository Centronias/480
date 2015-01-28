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

	static const char	m_idStarts[];
	static const char	m_idMids[];
	static const char	m_stringChars[];
	static const char	m_numerals[];

	static bool			m_debug;
};



#endif