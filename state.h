// ****************************************************************************
// ****************************************************************************
// state.h
// ****************************************************************************
// 
// ****************************************************************************
// ****************************************************************************



// ****************************************************************************
// Defines
// ****************************************************************************
#ifndef	H_STATE
#define H_STATE



// ****************************************************************************
// Transition Struct
// ****************************************************************************
struct Transition {
	char					m_char;
	State*					m_state;

							Transition(char		chr,
									   State*	state);
};



// ****************************************************************************
// State Class
// ****************************************************************************
class State {
  public:
							State();
							State(Token::Type);

	const State*			transition(char	chr) const;

	void					addTransition(char		chr,
										  State*	state);

	Token::Type				getFinalization() const	{return m_finalization;}
	void					setFinalization(Token::Type	type)	{m_finalization = type;}
	bool					hasFinalization() const;

  private:
	TransVec				m_transitions;

	Token::Type				m_finalization;
};



#endif