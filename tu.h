// ****************************************************************************
// ****************************************************************************
// tu.h
// ****************************************************************************
// 
// ****************************************************************************
// ****************************************************************************



// ****************************************************************************
// Defines
// ****************************************************************************
#ifndef	H_TRANS_UNIT
#define H_TRANS_UNIT



// ****************************************************************************
// PrUnit Class
// ****************************************************************************
struct PrUnit {
							PrUnit(Translator::Type	type,
								   UINT				index);

	const Translator::Type	m_type;
	const UINT				m_index;
};



// ****************************************************************************
// PoUnit Class
// ****************************************************************************
struct PoUnit {
							PoUnit(const comString&	keyword);
							PoUnit(UINT	index);

	
	const comString		    m_keyword;
	const UINT			    m_index;

	const bool				m_isKeyword;
};



// ****************************************************************************
// TransScheme Class
// ****************************************************************************
class TransScheme {
  public:
	void					addPre(Translator::Type	type,
								   UINT				index)	{m_preVec.append(new PrUnit(type, index));}
	void					addPost(const comString&	keyword)	{m_postVec.append(new PoUnit(keyword));}
	void					addPost(UINT	index)	{m_postVec.append(new PoUnit(index));}

	PrVec&					getPreVec()	{return m_preVec;}
	PoVec&					getPostVec()	{return m_postVec;}

	Translator::Type		getPreType(UINT	i);
	Translator::Type		returns();

	comString&				printable(comString&	buf);

  private:
	PrVec					m_preVec;
	PoVec					m_postVec;
};



#endif