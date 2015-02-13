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
		Operator,
		PrimType,
		StmtWord,
		Paren,
		Identifier,
		BoolConst,
		RealConst,
		IntConst,
		StrConst,
		END
	};

							Token(Token::Type		type,
								  const comString&	spelling,
								  UINT				line);

	const Type				getType() const	{return m_type;}
	const comString&		getSpelling() const	{return m_spelling;}
	UINT					getLine() const	{return m_line;}
	comString&				printable(comString&	buf);

	static void				init();
	static const comString&	getTypeName(Type	type) {return m_typeNames[type];}
	static const comString&	getExample(Type	type) {return m_examples[type];}

  private:
	Type					m_type;
	comString				m_spelling;
	UINT					m_line;

	static comString		m_typeNames[Token::END];
	static comString		m_examples[Token::END];
};



#endif