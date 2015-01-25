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
	static void			init();

	static void			run();

  private:
	static void			addLogicalOps(State*	genIdentifier);
	static void			addCompOps();
	static void			addMultOps();
	static void			addAddOps();
	static void			addUnaryOps(State*	genIdentifier);
	static void			addTypes(State*	genIdentifier);
	static void			addConditionals(State*	genIdentifier);
	static void			addOthers(State*	genIdentifier);

	static State*		addIdentifiers();

	static State*		m_entryState;

	static const char	m_idStarts[];
	static const char	m_idMids[];
};



#endif