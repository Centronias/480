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
		END
	}

						Token(Token::Type	type,
							  const String	spelling);


	static void				init();
	static const String&	getTypeName(Type	type) {return m_typeNames[type];}

  private:
	const String		m_spelling;
	const Type			m_type;

	static String		m_typeNames[Type::END];
};



#endif