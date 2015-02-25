// ****************************************************************************
// ****************************************************************************
// translator.h
// ****************************************************************************
// 
// ****************************************************************************
// ****************************************************************************



// ****************************************************************************
// Defines
// ****************************************************************************
#ifndef	H_TRANSLATOR
#define H_TRANSLATOR



// ****************************************************************************
// Translator Class
// ****************************************************************************
class Translator {
  public:
	enum Type {
		Any,
		Int,
		Float,
		Str,
		Bool,
		None,
		END
	};

	static void				init();
	static void				run();

	static Type				getType(const comString&	spelling);
	static const comString&	typeToString(Type	type)	{return m_typeNames[(UINT) type];}

  private:
	static void				run(ParseTree*	tree,
								FILE*		output);
	static comString		m_typeNames[Type::END];
};



#endif