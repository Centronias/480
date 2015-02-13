// ****************************************************************************
// ****************************************************************************
// lexer.cpp
// ****************************************************************************
// 
// ****************************************************************************
// ****************************************************************************



// ****************************************************************************
// Includes
// ****************************************************************************
#include "common.h"



// ****************************************************************************
// Static Member Initialization
// ****************************************************************************
const char	Lexer::m_idStarts[]		= "_abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
const char	Lexer::m_idMids[]		= "_abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
const char	Lexer::m_stringChars[]	= "`1234567890-=~!@#$%^&*()_+qwertyuiop[]QWERTYUIOP{}|asdfghjkl;ASDFGHJKL:zxcvbnm,./ZXCVBNM<>?\'\\\t ";
const char	Lexer::m_numerals[]		= "1234567890";
State*		Lexer::m_entryState		= NULL;
TokList		Lexer::m_tokens;



// ****************************************************************************
// init()
//
// This method initialzes the analyzer for use. It pretty much just makes all
// the states and calls the other class's init functions.
// ****************************************************************************
void
Lexer::init()
{
	printf("Initializing lexer.\n");

	Token::init();

	// Create the start state.
	m_entryState = new State("Entry state");

	// Create and link the states of the lexer state machine. All alphanumeric
	// keywords are handled as a special case of identifiers.
	addIdentifiers();
	addCompOps();
	addMultOps();
	addAddOps();
	addOthers();
	addStringConsts();
	addNumericConsts();

	if (Global::isDumpFSA()) {
		State::dumpFSA();
		Global::succeed();
	}
}



// ****************************************************************************
// run()
//
// This method implements the brains of the lexical analyzer. It reads one
// character at a time from stdin and traverses the previously made state
// machine based on those characters. After reading a character, there are
// three possible outcomes:
//	-	We advance to a new state
//	-	We cannot advance and accept the characters we've read so far.
//	-	We cannot advance and reject the characters we've read so far.
// ****************************************************************************
void
Lexer::run(const comString&	input)
{
	printf("Running lexer.\n");

	FILE*	file = fopen(input, "r");

	if (!file) {
		fprintf(stderr, "Input file \"%s\" failed to open.\n", (const char*) input);
		Global::fail();
	}
		

	const State*	curr = m_entryState;
	const State*	next = NULL;
	int				chr;
	UINT			lineNum = 1;

	char	buf[1 << 7];
	char*	bufLoc = buf;

	// Read input until there is no more input to be read.
	while (true) {
		chr = fgetc(file);

		next = curr->transition((char) chr);
		if (next != NULL) {
			// If there is a transition from our current state with our current
			// character, write the current character to the token buffer and
			// advance to the next state.
			*bufLoc++	= (char) chr;
			curr		= next;
		} else {
			// There is no transition from our current state with our current
			// character. Check to see if our current state accepts.
			if (curr->hasFinalization()) {
				// If our current state accepts (and there is no transition for
				// our current character), the previous character was the end
				// of a token. Create the token from the character buffer and
				// then reset the state machine and put the character back into
				// stdin. This is our pushback implementation.
				*bufLoc = '\0';

				Token::Type	type = curr->getFinalization();
				if (type == Token::Identifier) {
					// If this came through the state machine as an identifier,
					// check if it is a keyword.
					type = checkKeyword(buf);
				}

				m_tokens.append(new Token(type, buf, lineNum));


				curr	= m_entryState;
				bufLoc	= buf;
				ungetc(chr, file);
			} else {
				// If the current state does not accept (and there is no
				// transition for our current character), the entire current
				// token fragment is bad. We reset the state machine to the
				// entry state.

				if (bufLoc != buf) {
					// If the current token has characters in the buffer, the
					// current character may be the start of a good token so we
					// put it back into stdin. Before we reset the buffer,
					// though, we print an error stating that the token was not
					// recognized.
					ungetc(chr, file);

					*bufLoc = '\0';
					printf("Failed to match token fragment \"%s\" on line %d.\n", buf, lineNum);

					bufLoc	= buf;
				} else if (chr != EOF) {
					// There are no characters in the buffer and we did not
					// recognize the current character. We throw it out and if
					// it was non-whitespace, we report it as an error.
					switch (chr) {
					  case '\n':
						lineNum++;
					  case ' ':
					  case '\r':
					  case '\t':
						break;
					  default:
						printf("Failed to match start of token with '%c' on line %d.\n", chr, lineNum);
					}
				}

				curr = m_entryState;
			}
		}

		// If the last read character was the end of the input, stop parsing.
		if (chr == EOF)
			break;
	}

	printf("Done running lexer.\n");
}



// ****************************************************************************
// Lexer::addIdentifiers()
//
// This function adds states for handling identifiers. We do this with two
// states, one raches from the start state and is either a letter or
// underscore, and then a followup "generic" state which can be a letter,
// underscore, or number. The generic state is also reachable from other
// alphabetical states below.
// ****************************************************************************
State*
Lexer::addIdentifiers()
{
	State*	genIdentifier	= new State("genIdentifier", Token::Identifier);

	for (UINT i = 0; i < strlen(m_idStarts); i++)
		m_entryState->addTransition(m_idStarts[i], genIdentifier);

	for (UINT i = 0; i < strlen(m_idMids); i++)
		genIdentifier->addTransition(m_idMids[i], genIdentifier);

	return genIdentifier;
}



// ****************************************************************************
// Lexer::addCompOps()
//
// This function adds states for the following comparison operator tokens:
//	'>'
//	'>='
//	'<'
//	'<='
//	'='
//	'!='
// ****************************************************************************
void
Lexer::addCompOps()
{
	// Greater than '>' and greater than or equal to comparison ops.
	State*	gt	= new State(">", Token::Operator);
	State*	gte	= new State(">=", Token::Operator);
	m_entryState->addTransition('>', gt);
	gt->addTransition('=', gte);

	// Less than '<' and less than or equal to comparison ops.
	State*	lt	= new State("<", Token::Operator);
	State*	lte	= new State("<=", Token::Operator);
	m_entryState->addTransition('<', lt);
	lt->addTransition('=', lte);

	// Equality '=' comparison op.
	State*	eq = new State("=", Token::Operator);
	m_entryState->addTransition('=', eq);

	// Inequality '!=' comparison op.
	State*	neq_e = new State("!=", Token::Operator);
	State*	neq_n = new State("!=_!");
	m_entryState->addTransition('!', neq_n);
	neq_n->addTransition('=', neq_e);
}



// ****************************************************************************
// Lexer::addMultiplicationOps()
//
// This function adds states for the following multiplication operator tokens:
//	'*'
//	'/'
//	'%'
// ****************************************************************************
void
Lexer::addMultOps()
{
	// Multiplication '*' multiplication op.
	State*	mu = new State("*", Token::Operator);
	m_entryState->addTransition('*', mu);

	// Division '/' multiplication op.
	State*	di = new State("/", Token::Operator);
	m_entryState->addTransition('/', di);

	// Modulus '%' multiplication op.
	State*	mo = new State("%", Token::Operator);
	m_entryState->addTransition('%', mo);
}



// ****************************************************************************
// Lexer::addAdditionOps()
//
// This function adds states for the following addition operator tokens:
//	'+'
//	'-'
// ****************************************************************************
void
Lexer::addAddOps()
{	
	// Addition '*' addition op.
	State*	add = new State("+", Token::Operator);
	m_entryState->addTransition('+', add);

	// Subtraction '/' addition op.
	State*	sub = new State("-", Token::Operator);
	m_entryState->addTransition('-', sub);
}



// ****************************************************************************
// Lexer::addStringConsts()
//
// This function adds all of the states required to handle string literal
// constants. Note that the format of a string is essentially "[anything]"
// ****************************************************************************
void
Lexer::addStringConsts()
{
	// Add state to capture the first doublequote.
	State*	start = new State("String start");
	m_entryState->addTransition('\"', start);

	// Add state to capture the ending doublequote.
	State*	end = new State("String end", Token::StrConst);
	start->addTransition('\"', end);

	// Add a state for every character which can be inside a string and link it
	// to the stard state.
	State*	mid = new State("String mid");
	mid->addTransition('\"', end);
	
	for (UINT i = 0; i < strlen(m_stringChars); i++) {
		start->addTransition(m_stringChars[i], mid);
		mid->addTransition(m_stringChars[i], mid);
	}
}



// ****************************************************************************
// Lexer::addNumericConstants()
//
// This function adds all of the states required to handle real and integer 
// constants.
// ****************************************************************************
void
Lexer::addNumericConsts()
{	
	// Add a state to handle numerals from the start state. Note that this is
	// the finalization state for integers and the first part of reals.
	State*	initial = new State("num init", Token::IntConst);

	// Add two states for the '.' in reals, the first is for the one that
	// follows the "initial" state and could define a proper real while the
	// second one is for a real which is started with a point and is not, on
	// its own, a proper real.
	State*	mPoint = new State("mPoint", Token::RealConst);
	State*	iPoint = new State("iPoint");

	// Add a state for the numerals after the point in a real, the [eE] in a
	// real, the sign of the exponent, and finally add a state for the
	// magintude of the exponent.
	State*	pNums = new State("pNums", Token::RealConst);
	State*	e = new State("num e", Token::RealConst);
	State*	s = new State("num s");
	State*	mag = new State("num mag", Token::RealConst);

	// Link the initial state.
	// 	Entry	-[0-9]-> Initial
	// 	Initial	-[0-9]-> Initial
	// 	Initial	--[.]--> mPoint
	// 	Initial	-[eE]--> e
	initial->addTransition('.', mPoint);
	initial->addTransition('e', e);
	initial->addTransition('E', e);
	for (UINT i = 0; i < strlen(m_numerals); i++) {
		m_entryState->addTransition(m_numerals[i], initial);
		initial->addTransition(m_numerals[i], initial);
	}
	
	// Link the point states.
	//	Entry	--[.]--> iPoint
	//	Point	-[0-9]-> pNums
	m_entryState->addTransition('.', iPoint);
	for (UINT i = 0; i < strlen(m_numerals); i++) {
		iPoint->addTransition(m_numerals[i], pNums);
		mPoint->addTransition(m_numerals[i], pNums);
	}

	// Link pNums to itself.
	for (UINT i = 0; i < strlen(m_numerals); i++)
		pNums->addTransition(m_numerals[i], pNums);

	// Link the e state.
	//	pNums	-[eE]--> e
	//	e		-[+-]--> s
	//	e		-[0-9]-> Mag
	pNums->addTransition('e', e);
	pNums->addTransition('E', e);
	e->addTransition('+', s);
	e->addTransition('-', s);
	for (UINT i = 0; i < strlen(m_numerals); i++)
		e->addTransition(m_numerals[i], mag);
	
	// Finally, link the magnitude state.
	//	s	-[0-9]-> mag
	//	mag	-[0-9]-> mag
	for (UINT i = 0; i < strlen(m_numerals); i++) {
		s->addTransition(m_numerals[i], mag);
		mag->addTransition(m_numerals[i], mag);
	}
}



// ****************************************************************************
// Lexer::addOthers()
//
// This function adds all of the other states for the token types not covered
// in other methods.
// Exponentiation operator:	'^'
// Parentheses:				'('
//							')'
// ****************************************************************************
void
Lexer::addOthers()
{	
	// Add exponentiation '^' exponentiation op.
	State*	ex = new State("^", Token::Operator);
	m_entryState->addTransition('^', ex);

	// Add assignment ':=' op.
	State*	asg_e = new State(":=", Token::Operator);
	State*	asg_c = new State(":= :");

	m_entryState->addTransition(':', asg_c);
	asg_c->addTransition('=', asg_e);

	// Add parentheses '(' and ')'.
	State*	op = new State("(", Token::Paren);
	State*	cp = new State(")", Token::Paren);
	m_entryState->addTransition('(', op);
	m_entryState->addTransition(')', cp);
}



// ****************************************************************************
// Lexer::checkKeyword()
//
// Checks if the passed in string is a keyword. If it is, return the token
// type of the keyword. Otherwise return identifier type.
// ****************************************************************************
Token::Type
Lexer::checkKeyword(const char*	spelling)
{
	const comString	tok(spelling);

	if (tok == "true" || tok == "false")
		return Token::BoolConst;

	if (tok == "bool" || tok == "real" || tok == "int" || tok == "string")
		return Token::PrimType;

	if (tok == "not" || tok == "sin" || tok == "cos" || tok == "tan" || tok == "and" || tok == "or")
		return Token::Operator;

	if (tok == "stdout" || tok == "while" || tok == "if" || tok == "let")
		return Token::StmtWord;

	return Token::Identifier;
}



// ****************************************************************************
// Lexer::printTokens()
//
// This method simply prints out the contents of the token list.
// ****************************************************************************
void
Lexer::printTokens()
{
	UINT			printL = 0;
	TokListIter		iter(m_tokens);
	const Token*	token = NULL;

	while ((token = iter.next())) {
		while (printL < token->getLine()) {
			printf("\n");
			printL++;
		}

		printf("(%s : \"%s\")",
			   (const char*) Token::getTypeName(token->getType()),
			   (const char*) token->getSpelling());
	}
}