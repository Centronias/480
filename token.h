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
								  const comString&	spelling,
								  UINT				line);
							~Token()	{printf("Destroying token: \"%s\"", (const char*) m_spelling);}

	const Type				getType() const	{return m_type;}
	const comString&		getSpelling() const	{return m_spelling;}
	UINT					getLine() const	{return m_line;}

	static void				init();
	static const comString&	getTypeName(Type	type) {return m_typeNames[type];}

  private:
	Type					m_type;
	comString				m_spelling;
	UINT					m_line;

	static comString		m_typeNames[Token::END];
};



#endif