// ****************************************************************************
// ****************************************************************************
// state.cpp
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
UINT	State::m_nID = 0;



// ****************************************************************************
// State()
// ****************************************************************************
State::State(const comString&	name)
:	m_finalization(Token::NONE),
	m_num(m_nID++),
	m_name(name)
{
	m_states.append(this);
}

State::State(const comString&	name,
			 Token::Type		type)
:	m_finalization(type),
	m_num(m_nID++),
	m_name(name)
{
	m_states.append(this);
}



// ****************************************************************************
// hasFinalization()
// ****************************************************************************
bool
State::hasFinalization() const
{
	return m_finalization != Token::NONE;
}



// ****************************************************************************
// addTransition()
// ****************************************************************************
void
State::addTransition(char	chr,
					 State*	state)
{
	m_transitions.append(new Transition(chr, state));
}



// ****************************************************************************
// transition()
// ****************************************************************************
const State*
State::transition(char	chr) const
{
	for (UINT i = 0; i < m_transitions.getNumEntries(); i++) {
		if (m_transitions[i]->m_char == chr)
			return m_transitions[i]->m_state;
	}

	return NULL;
}



// ****************************************************************************
// Transition()
// ****************************************************************************
Transition::Transition(char		chr,
					   State*	state)
:	m_char(chr),
	m_state(state)
{
}



// ****************************************************************************
// State::dumpFSA()
//
// This method prints out the states and their transition which compose the FSA
// constructed for the lexer.
// ****************************************************************************
void
State::dumpFSA()
{
	FILE*	file = fopen("FSAdump.out", "w");

	for (UINT i = 0; i < m_transitions.getNumEntries(); i++) {
		State*	state = m_transitions[i];

		fprintf(file, "\"%s\" (%d):\n", (const char*) state->m_name, state->m_num);

		if (state->hasFinalization())
			fprintf(file, "  F: %s\n", state->m_finalization);

		for (UINT j = 0; j < state->m_transitions[j]; j++) {
			Transition*	t = state->m_transitions[j];
			fprintf(file, "\t-'%c'-> %s \"%s\" (%d)", t->m_char, (const char*) t->m_state->m_name, t->m_state->m_num);
		}
	}
}