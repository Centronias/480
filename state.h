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
// State Class
// ****************************************************************************
class State {
	struct Transition {
		char			m_char;
		State*			m_state;
	};

  public:
						State();
						State(Token::Type);

	const State*		transition(char	chr) const;

	void				addTransition(char		chr,
									  State*	state);
	void				setFinalization(Token::Type	type)	{m_finalization = type;}
	bool				hasFinalization() const;

  private:
	Vector<Transition>	m_transitions;

	Token::Type			m_finalization;
};



#endif