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
// main()
// ****************************************************************************
int
main(int			argc,
	 const char*	argv[])
{
	Lexer::init();
	Lexer::run();
}



// ****************************************************************************
// init()
//
// This method initialzes the analyzer for use. It pretty much just makes all
// the states.
// ****************************************************************************
void
Lexer::init()
{
	// Create the start state.
	m_entryState = new State();

	// Create the "special purpose" identifier states which handle pretty much
	// all alphanumeric characters.
	State*	genIdentifier = addIdentifiers()

	// Add all of the other states.
	addLogicalOps(genIdentifier);
	addCompOps();
	addMultOps();
	addAddOps();
	addUnaryOps(genIdentifier);
	addTypes(genIdentifier);
	addConditionals(genIdentifier);
	addOthers(genIdentifier);
}



// ****************************************************************************
// run()
// ****************************************************************************
void
Lexer::run()
{
	State*	curr = m_entryState;
	State*	prev = NULL;
	State*	next = NULL;
	char	chr;

	char	buf[1 << 7];
	char*	bufLoc = buf;

	Vector<Token>	tokens;

	// Read input until there is no more input to be read.
	while (getc(chr, stdin)) {
		if (next = curr->transition(chr)) {
			// If there is a transition from our current state with our current
			// character, write the current character to the token buffer and
			// advance to the next state.
			*bufLoc++ = chr;

			prev = curr;
			curr = next;
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
				token.append(Token(curr->getFinalization(), buf));

				curr = m_entryState;
				prev = NULL;
				bufLoc = buf;

				ungetc(chr, stdin);
			} else {
				// If the current state does not accept (and there is no
				// transition for our current character), the entire current
				// token fragment is bad. We throw out the entire attempt,
				// reset the state machine and put the character back into
				// stdin. I am not sure when this would happen other than in
				// invalid character is read.
				curr = m_entryState;
				prev = NULL;
				bufLoc = buf;

				ungetc(chr, stdin);
			}
		}
	}
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
	State*	startIdentifier	= new State();
	State*	genIdentifier	= new State();

	for (UINT i = 0; i < strlen(m_idStarts); i++)
		m_entryState->addTransition(m_idStarts[i], genIdentifier);

	for (UINT i = 0; i < strlen(m_idMids); i++)
		genIdentifier->addTransition(m_idMids[i], genIdentifier);

	return genIdentifier;
}



// ****************************************************************************
// Lexer::addLogicalOps()
//
// This function adds states for the following logical operator tokens:
//	'and'
//	'or'
// ****************************************************************************
void
Lexer::addLogicalOps(State*	genIdentifier)
{
	// Add 'and' states. Each of these states must also branch off to the
	// generic identifier state.
	State*	and_d = new State(Token::LogicalOp);
	State*	and_n = new State();
	State*	and_a = new State();

	m_entryState->addTransition('a', and_a);
	and_a->addTransition('n', and_n);
	and_n->addTransition('d', and_d);

	for (UINT i = 0; i < strlen(m_idMids); i++)
		and_a->addTransition(m_idMids[i], genIdentifier);
	for (UINT i = 0; i < strlen(m_idMids); i++)
		and_n->addTransition(m_idMids[i], genIdentifier);
	for (UINT i = 0; i < strlen(m_idMids); i++)
		and_d->addTransition(m_idMids[i], genIdentifier);

	// Add 'or' states. Both of these states must also branch off to the
	// generic identifier state.
	State*	or_r = new State(Token::LogicalOp);
	State*	or_o = new State();

	m_entryState->addTransition('o', or_o);
	or_o->addTransition('r', or_r);

	for (UINT i = 0; i < strlen(m_idMids); i++)
		or_o->addTransition(m_idMids[i], genIdentifier);
	for (UINT i = 0; i < strlen(m_idMids); i++)
		or_o->addTransition(m_idMids[i], genIdentifier);
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
	State*	gt	= new State(Token::CompOp);
	State*	gte	= new State(Token::CompOp);
	m_entryState->addTransition('>', gt);
	gt->addTransition('=', gte);

	// Less than '<' and less than or equal to comparison ops.
	State*	lt	= new State(Token::CompOp);
	State*	lte	= new State(Token::CompOp);
	m_entryState->addTransition('<', lt);
	lt->addTransition('=', lte);

	// Equality '=' comparison op.
	State	eq = new State(Token::CompOp);
	m_entryState->addTransition('=', eq);

	// Inequality '!=' comparison op.
	State*	neq_e = new State(Token::CompOp);
	State*	neq_n = new State();
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
	State*	mu = new State(Token::MultOp);
	m_entryState->addTransition('*', mu);

	// Division '/' multiplication op.
	State*	di = new State(Token::MultOp);
	m_entryState->addTransition('/', di);

	// Modulus '%' multiplication op.
	State*	mo = new State(Token::MultOp);
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
	State*	add = new State(Token::AddOp);
	m_entryState->addTransition('+', add);

	// Subtraction '/' addition op.
	State*	sub = new State(Token::AddOp);
	m_entryState->addTransition('-', sub);
}



// ****************************************************************************
// Lexer::addUnaryOps()
//
// This function adds states for the following unary operator tokens:
//	'not'
//	'sin'
//	'cos'
//	'tan'
//	'stdout'
// ****************************************************************************
void
Lexer::addUnaryOps(State*	genIdentifier)
{
	// Add 'not' states. Each of these states must also branch off to the
	// generic identifier state.
	State*	not_t	= new State(Token::UnaryOp);
	State*	not_o	= new State();
	State*	not_n	= new State();

	m_entryState->addTransition('n', not_n);
	not_n->addTransition('o', not_o);
	not_o->addTransition('t', not_t);

	for (UINT i = 0; i < strlen(m_idMids); i++)
		not_n->addTransition(m_idMids[i], genIdentifier);
	for (UINT i = 0; i < strlen(m_idMids); i++)
		not_o->addTransition(m_idMids[i], genIdentifier);
	for (UINT i = 0; i < strlen(m_idMids); i++)
		not_t->addTransition(m_idMids[i], genIdentifier);

	// Add 'sin' states. Each of these states must also branch off to the
	// generic identifier state.
	State*	sin_n	= new State(Token::UnaryOp);
	State*	sin_i	= new State();
	State*	sin_s	= new State();

	m_entryState->addTransition('s', sin_s);
	not_s->addTransition('i', not_i);
	not_i->addTransition('n', not_n);

	for (UINT i = 0; i < strlen(m_idMids); i++)
		sin_s->addTransition(m_idMids[i], genIdentifier);
	for (UINT i = 0; i < strlen(m_idMids); i++)
		sin_i->addTransition(m_idMids[i], genIdentifier);
	for (UINT i = 0; i < strlen(m_idMids); i++)
		sin_n->addTransition(m_idMids[i], genIdentifier);

	// Add 'cos' states. Each of these states must also branch off to the
	// generic identifier state.
	State*	cos_s = new State(Token::UnaryOp);
	State*	cos_o = new State();
	State*	cos_c = new State();

	m_entryState->addTransition('c', cos_c);
	not_c->addTransition('o', cos_o);
	not_o->addTransition('t', cos_s);

	for (UINT i = 0; i < strlen(m_idMids); i++)
		cos_c->addTransition(m_idMids[i], genIdentifier);
	for (UINT i = 0; i < strlen(m_idMids); i++)
		cos_o->addTransition(m_idMids[i], genIdentifier);
	for (UINT i = 0; i < strlen(m_idMids); i++)
		cos_s->addTransition(m_idMids[i], genIdentifier);

	// Add 'tan' states. Each of these states must also branch off to the
	// generic identifier state.
	State*	tan_n = new State(Token::UnaryOp);
	State*	tan_a = new State();
	State*	tan_t = new State();

	m_entryState->addTransition('t', tan_t);
	tan_t->addTransition('a', tan_a);
	tan_a->addTransition('n', tan_n);

	for (UINT i = 0; i < strlen(m_idMids); i++)
		tan_t->addTransition(m_idMids[i], genIdentifier);
	for (UINT i = 0; i < strlen(m_idMids); i++)
		tan_a->addTransition(m_idMids[i], genIdentifier);
	for (UINT i = 0; i < strlen(m_idMids); i++)
		tan_n->addTransition(m_idMids[i], genIdentifier);

	// Add 'stdout' states. Each of these states must also branch off to the
	// generic identifier state.
	State*	stdout_t = new State(Token::UnaryOp);
	State*	stdout_u = new State();
	State*	stdout_o = new State();
	State*	stdout_d = new State();
	State*	stdout_t = new State();
	State*	stdout_s = new State();

	m_entryState->addTransition('s', stdout_s);
	stdout_s->addTransition('t', stdout_t);
	stdout_t->addTransition('d', stdout_d);
	stdout_d->addTransition('o', stdout_o);
	stdout_o->addTransition('u', stdout_u);
	stdout_u->addTransition('t', stdout_t);

	for (UINT i = 0; i < strlen(m_idMids); i++)
		stdout_s->addTransition(m_idMids[i], genIdentifier);
	for (UINT i = 0; i < strlen(m_idMids); i++)
		stdout_t->addTransition(m_idMids[i], genIdentifier);
	for (UINT i = 0; i < strlen(m_idMids); i++)
		stdout_d->addTransition(m_idMids[i], genIdentifier);
	for (UINT i = 0; i < strlen(m_idMids); i++)
		stdout_o->addTransition(m_idMids[i], genIdentifier);
	for (UINT i = 0; i < strlen(m_idMids); i++)
		stdout_u->addTransition(m_idMids[i], genIdentifier);
	for (UINT i = 0; i < strlen(m_idMids); i++)
		stdout_t->addTransition(m_idMids[i], genIdentifier);
}



// ****************************************************************************
// Lexer::addTypes()
//
// This function adds states for the following primitive type tokens:
//	'int'
//	'real'
//	'bool'
//	'string'
// ****************************************************************************
void
Lexer::addTypes(State*	genIdentifier)
{
	// Add 'int' states. Each of these states must also branch off to the
	// generic identifier state.
	State*	int_t = new State(Token::Type);
	State*	int_n = new State();
	State*	int_i = new State();

	m_entryState->addTransition('i', int_i);
	int_i->addTransition('n', int_n);
	int_n->addTransition('t', int_t);

	for (UINT i = 0; i < strlen(m_idMids); i++)
		int_i->addTransition(m_idMids[i], genIdentifier);
	for (UINT i = 0; i < strlen(m_idMids); i++)
		int_n->addTransition(m_idMids[i], genIdentifier);
	for (UINT i = 0; i < strlen(m_idMids); i++)
		int_t->addTransition(m_idMids[i], genIdentifier);

	// Add 'bool' states. Each of these states must also branch off to the
	// generic identifier state.
	State*	bool_l = new State(Token::Type);
	State*	bool_2 = new State();
	State*	bool_o = new State();
	State*	bool_b = new State();

	m_entryState->addTransition('b', bool_b);
	bool_b->addTransition('o', bool_o);
	bool_o->addTransition('o', bool_2);
	bool_2->addTransition('l', bool_l);

	for (UINT i = 0; i < strlen(m_idMids); i++)
		bool_b->addTransition(m_idMids[i], genIdentifier);
	for (UINT i = 0; i < strlen(m_idMids); i++)
		bool_o->addTransition(m_idMids[i], genIdentifier);
	for (UINT i = 0; i < strlen(m_idMids); i++)
		bool_2->addTransition(m_idMids[i], genIdentifier);
	for (UINT i = 0; i < strlen(m_idMids); i++)
		bool_l->addTransition(m_idMids[i], genIdentifier);

	// Add 'real' states. Each of these states must also branch off to the
	// generic identifier state.
	State*	real_l = new State(Token::Type);
	State*	real_a = new State();
	State*	real_e = new State();
	State*	real_r = new State();

	m_entryState->addTransition('r', real_r);
	real_r->addTransition('e', real_e);
	real_e->addTransition('a', real_a);
	real_a->addTransition('l', real_l);

	for (UINT i = 0; i < strlen(m_idMids); i++)
		real_r->addTransition(m_idMids[i], genIdentifier);
	for (UINT i = 0; i < strlen(m_idMids); i++)
		real_e->addTransition(m_idMids[i], genIdentifier);
	for (UINT i = 0; i < strlen(m_idMids); i++)
		real_a->addTransition(m_idMids[i], genIdentifier);
	for (UINT i = 0; i < strlen(m_idMids); i++)
		real_l->addTransition(m_idMids[i], genIdentifier);

	// Add 'string' states. Each of these states must also branch off to the
	// generic identifier state.
	State*	string_g = new State(Token::Type);
	State*	string_n = new State();
	State*	string_i = new State();
	State*	string_r = new State();
	State*	string_t = new State();
	State*	string_s = new State();

	m_entryState->addTransition('s', string_s);
	string_s->addTransition('t', string_t);
	string_t->addTransition('r', string_r);
	string_r->addTransition('i', string_i);
	string_i->addTransition('n', string_n);
	string_n->addTransition('g', string_g);

	for (UINT i = 0; i < strlen(m_idMids); i++)
		string_s->addTransition(m_idMids[i], genIdentifier);
	for (UINT i = 0; i < strlen(m_idMids); i++)
		string_t->addTransition(m_idMids[i], genIdentifier);
	for (UINT i = 0; i < strlen(m_idMids); i++)
		string_r->addTransition(m_idMids[i], genIdentifier);
	for (UINT i = 0; i < strlen(m_idMids); i++)
		string_i->addTransition(m_idMids[i], genIdentifier);
	for (UINT i = 0; i < strlen(m_idMids); i++)
		string_n->addTransition(m_idMids[i], genIdentifier);
	for (UINT i = 0; i < strlen(m_idMids); i++)
		string_g->addTransition(m_idMids[i], genIdentifier);
}



// ****************************************************************************
// Lexer::addConditionals()
//
// This function adds states for the following conditional tokens:
//	'if'
//	'while'
// ****************************************************************************
void
Lexer::addConditionals(State*	genIdentifier)
{
	// Add 'if' states. Each of these states must also branch off to the
	// generic identifier state.
	State*	if_f = new State(Token::Conditional);
	State*	if_i = new State();

	m_entryState->addTransition('i', if_i);
	if_i->addTransition('f', if_f);

	for (UINT i = 0; i < strlen(m_idMids); i++)
		if_i->addTransition(m_idMids[i], genIdentifier);
	for (UINT i = 0; i < strlen(m_idMids); i++)
		if_f->addTransition(m_idMids[i], genIdentifier);

	// Add 'while' states. Each of these states must also branch off to the
	// generic identifier state.
	State*	while_e = new State(Token::Conditional);
	State*	while_l = new State();
	State*	while_i = new State();
	State*	while_h = new State();
	State*	while_w = new State();

	m_entryState->addTransition('w', while_w);
	while_w->addTransition('h', while_h);
	while_h->addTransition('i', while_i);
	while_i->addTransition('l', while_l);
	while_l->addTransition('e', while_e);

	for (UINT i = 0; i < strlen(m_idMids); i++)
		while_w->addTransition(m_idMids[i], genIdentifier);
	for (UINT i = 0; i < strlen(m_idMids); i++)
		while_h->addTransition(m_idMids[i], genIdentifier);
	for (UINT i = 0; i < strlen(m_idMids); i++)
		while_i->addTransition(m_idMids[i], genIdentifier);
	for (UINT i = 0; i < strlen(m_idMids); i++)
		while_l->addTransition(m_idMids[i], genIdentifier);
	for (UINT i = 0; i < strlen(m_idMids); i++)
		while_e->addTransition(m_idMids[i], genIdentifier);
}



// ****************************************************************************
// Lexer::addOthers()
//
// This function adds all of the other states for the token types not covered
// in other methods.
// Exponentiation operator:	'^'
// Declarator:				'let'
// Parentheses:				'('
//							')'
// ****************************************************************************
void
Lexer::addOthers(State*	genIdentifier)
{
	// Add exponentiation '^' exponentiation op.
	State*	ex = new State(Token::ExpoOp);
	m_entryState->addTransition('^', ex);

	// Add assignment ':=' op.
	State*	asg_e = new State(Token::AssgnOp);
	State*	asg_c = new State();

	m_entryState->addTransition(':', asg_c);
	asg_c->addTransition('=', asg_e);

	// Add 'let' declarator states. Each of these states must also branch off
	// to the generic identifier state.
	State*	let_t = new State(Token::Declarator);
	State*	let_e = new State();
	State*	let_l = new State();

	m_entryState->addTransition('l', let_l);
	let_l->addTransition('e', let_e);
	let_e->addTransition('t', let_t);

	for (UINT i = 0; i < strlen(m_idMids); i++)
		let_l->addTransition(m_idMids[i], genIdentifier);
	for (UINT i = 0; i < strlen(m_idMids); i++)
		let_e->addTransition(m_idMids[i], genIdentifier);
	for (UINT i = 0; i < strlen(m_idMids); i++)
		let_t->addTransition(m_idMids[i], genIdentifier);

	// Add parentheses '(' and ')'.
	State*	op = new State(Token::Paren);
	State*	cp = new State(Token::Paren);
	m_entryState->addTransition('(', op);
	m_entryState->addTransition(')', cp);
}