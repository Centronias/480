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
	char	input[PATH_MAX];
	char	grammar[PATH_MAX];
	*input		= '\0';
	*grammar	= '\0';

	Global::readCmdLine(argc, argv, input, grammar);

	printf("\n////////////////////////////////////////////////////////////\n");
	printf("\tRunning compiler");
	if (*input)
		printf(" on \"%s\"", input);
	if (m_debug)
		printf("\n\tDebugging on");
	if (m_dumpFSA)
		printf("\n\tDumping FSA to fsa.out");
	if (m_generateTree)
		printf("\n\tGenerating tree and writing to gen.out");
	if (m_dumpGrammar)
		printf("\n\tDumping grammar to grammar.out");
	if (*grammar)
		printf("\n\tBuilding grammar from \"%s\"", grammar);
	printf("\n////////////////////////////////////////////////////////////\n\n");

	Lexer::init();
	Parser::init(grammar);

	Lexer::run(input);
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
				   char*	inFile,
				   char*	grammarFile)
{
	opterr = 0;

	char c;
	while ( (c = getopt(argc, argv, "dfgtb:")) != -1 ) {
		switch (c) {
		  case 'd':
			m_debug = true;
			break;
		  case 'f':
			m_dumpFSA = true;
			break;
		  case 'g':
			m_dumpGrammar = true;
			break;
		  case 't':
			m_generateTree = true;
			break;
		  case 'b':
			strncpy(grammarFile, optarg, PATH_MAX);
			break;
		  case '?':
			if (optopt == 'b')
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
		fprintf(stderr, "Usage:\n");
		fprintf(stderr, "\tcompiler [-b <grammar file>] <input file>\n");
		fprintf(stderr, "\tcompiler -f\t\t\t\t\t\t(Dumps the FSA to fsa.out)\n");
		fprintf(stderr, "\tcompiler [-b <grammar file>] -g\t\t\t\t(Dumps the grammar to grammar.out)\n");
		fprintf(stderr, "\tcompiler -t\t\t\t\t\t\t(Generates a random parse tree to gen.out)\n");
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