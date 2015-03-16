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

	static Type				getPrimType(const comString&	spelling);

	static Type				getType(const comString&	spelling);
	static const comString&	typeToString(Type	type)	{return m_typeNames[(UINT)type];}

	static void				buildFunction(ParseTree*	tree,
										  UINT&			lastLine);
	static void             typeCheckFunction(ParseTree*	tree,
											  UINT&			lastLine);
	static void				translateFunction();
	static void				translateFuncInvocation(ParseTree*	tree,
													FILE*		output);

  private:
	static void				run(ParseTree*	tree,
								FILE*		output);
	static comString&		translateSpelling(ParseTree*	tree,
											  comString&	buf);
	static comString		m_typeNames[Type::END];

friend class FuncDef;
};



#endif