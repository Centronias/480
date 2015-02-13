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
bool	Global::m_generateTree	= false;



// ****************************************************************************
// main()
// ****************************************************************************
int
main(int	argc,
	 char**	argv)
{
	srand(time(NULL));

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
	printf("\n////////////////////////////////////////////////////////////\n");
	printf("\tRunning compiler\n");
	printf("////////////////////////////////////////////////////////////\n\n");

	char	input[PATH_MAX];

	Global::readCmdLine(argc, argv, input);
	Lexer::init();
	Parser::init();

	Lexer::run(comString(input));
	Parser::run();
	Parser::printTree();
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
	while ( (c = getopt(argc, argv, "dfgt")) != -1 ) {
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
		  case 't':
			m_generateTree = true;
			printf("Tree generation on\n");
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

	if (!m_dumpFSA && !m_dumpGrammar && !m_generateTree && (argv[optind] == NULL)) {
		fprintf(stderr, "Usage:\n\tcompiler <input file>\n\tcompiler -f\t\t(Dumps the FSA)\n\tcompiler -g\t\t(Dumps the grammar)\n");
		exit(EXIT_FAILURE);
	}

	if (!(m_dumpFSA || m_dumpGrammar || m_generateTree))
		strncpy(inFile, argv[optind], PATH_MAX);
}



// ************************************************************************************************
// fail()
// ************************************************************************************************
void
Global::fail()
{
	printf("\n\n////////////////////////////////////////////////////////////\n");
	printf("\tEncountered error, exiting.\n");
	printf("////////////////////////////////////////////////////////////\n");
	exit(EXIT_FAILURE);
}



// ************************************************************************************************
// exit()
// ************************************************************************************************
void
Global::succeed()
{
	printf("\n\n////////////////////////////////////////////////////////////\n");
	printf("\tExiting compiler\n");
	printf("////////////////////////////////////////////////////////////\n");
	exit(EXIT_SUCCESS);
}