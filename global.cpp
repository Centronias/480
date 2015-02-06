// ****************************************************************************
// ****************************************************************************
// global.cpp
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
bool	Global::m_debug			= false;
bool	Global::m_dumpFSA		= false;
bool	Global::m_dumpGrammar	= false;



// ****************************************************************************
// main()
// ****************************************************************************
int
main(int	argc,
	 char**	argv)
{
	Global::run(argc, argv);
	Global::succeed();
}



// ****************************************************************************
// run()
// ****************************************************************************
void
Global::run(int		argc,
			char**	argv)
{
	printf("Running compiler\n\n");

	char	input[PATH_MAX];

	Global::readCmdLine(argc, argv, input);
	Lexer::init();
	Parser::init();

	Lexer::run(comString(input));
//	Parser::run();
}



// ************************************************************************************************
// readCmdLine()
//
// This method reads the command line arguments to get the input file. If the input file is
// missing, we print the proper usage and then exit.
// ************************************************************************************************
void
Global::readCmdLine(int		argc,
				   char**	argv,
				   char*	inFile)
{
	opterr = 0;

	char c;
	while ( (c = getopt(argc, argv, "dfg")) != -1 ) {
		switch (c) {
		  case 'd':
			m_debug = true;
			break;
		  case 'f':
			m_dumpFSA = true;
			printf("FSA dump on\n");
			break;
		  case 'g':
			m_dumpGrammar = true;
			printf("Grammar dump on\n");
			break;
		  case '?':
			if (optopt == 'c')
				fprintf(stderr, "Option -%c requires an argument.\n", optopt);
			else if (isprint(optopt))
				fprintf(stderr, "Unknown option '-%c'.\n", optopt);
			else
				fprintf(stderr, "Unknown option character '\\x%x'.\n", optopt);
			exit(EXIT_FAILURE);
		  default:
		  	exit(2);
		}
	}

	if (!m_dumpFSA && !m_dumpGrammar && (argv[optind] == NULL)) {
		fprintf(stderr, "Usage:\n\tcompiler <input file>\n\tcompiler -f\t\t(Dumps the FSA)\n\tcompiler -g\t\t(Dumps the grammar)\n");
		exit(EXIT_FAILURE);
	}

	if (!(m_dumpFSA || m_dumpGrammar))
		strncpy(inFile, argv[optind], PATH_MAX);
}



// ************************************************************************************************
// fail()
// ************************************************************************************************
void
Global::fail()
{
	fprintf(stderr, "\n\nEncountered error. Exiting.\n");
	exit(EXIT_FAILURE);
}



// ************************************************************************************************
// exit()
// ************************************************************************************************
void
Global::succeed()
{
	printf("\n\nExiting compiler\n");
	exit(EXIT_SUCCESS);
}