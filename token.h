// ****************************************************************************
// ****************************************************************************
// token.h
// ****************************************************************************
// 
// ****************************************************************************
// ****************************************************************************



// ****************************************************************************
// Defines
// ****************************************************************************
#ifndef	H_TOKEN
#define H_TOKEN



// ****************************************************************************
// State Class
// ****************************************************************************
class Token {
	enum Type {
		None,
		LogicalOp,
		CompOp,
		ExpoOp,
		MultOp,
		AddOp,
		UnaryOp,
		AssgnOp,
		PrimType,
		Declarator,
		Conditional,
		Paren
	}

  public:
						Token(Token::Type	type,
							  const String	spelling);

  private:
	const String		m_spelling;
	const Type			m_type;
};



#endif