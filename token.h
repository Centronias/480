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
  public:
	enum Type {
		NONE,
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
		Paren,
		TypeName,
		END
	};

							Token(Token::Type		type,
								  const comString&	spelling);


	static void				init();
	static const comString&	getTypeName(Type	type) {return m_typeNames[type];}

  private:
	Type					m_type;
	comString				m_spelling;

	static comString		m_typeNames[Token::END];
};



#endif