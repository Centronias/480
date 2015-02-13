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
#ifndef	H_GLOBAL
#define H_GLOBAL



// ****************************************************************************
// Global Class
// ****************************************************************************
class Global {
  public:
  	static void			run(int		argc,
  							char**	argv);
	static void			readCmdLine(int		argc,
									char**	argv,
									char*	inFile);

	static void			fail();
	static void			succeed();

	static bool			isDebug()	{return m_debug;}
	static bool			isDumpFSA()	{return m_dumpFSA;}
	static bool			isDumpGrammar()	{return m_dumpGrammar;}
	static bool			isGenerateTree()	{return m_generateTree;}

  private:
	static bool			m_debug;
	static bool			m_dumpFSA;
	static bool			m_dumpGrammar;
	static bool			m_generateTree;
};



// ****************************************************************************
// Entry Point Declaration
// ****************************************************************************
int						main(int	argc,
							 char**	argv);



#endif