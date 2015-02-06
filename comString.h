// ****************************************************************************
// ****************************************************************************
// comString.h
// ****************************************************************************
// 
// ****************************************************************************
// ****************************************************************************



// ****************************************************************************
// Defines
// ****************************************************************************
#ifndef	H_COM_STRING
#define H_COM_STRING



// ****************************************************************************
// comString Class
// ****************************************************************************
class comString {
  public:
						comString();
						comString(const comString&	other);
						comString(const char*	cBuf);
						~comString();

						operator const char*() const	{return m_data;};

	comString&			operator=(const comString&	other);
	comString&			operator=(const char*	other);
	bool				operator==(const comString&	other) const;
	bool				operator==(const char*	other) const;
	bool				operator!=(const comString&	other) const;
	char&				operator[](const UINT	index) {return m_data[index];}

	UINT				getLength() const	{return strlen(m_data);}


  private:
	char*				m_data;

friend class Token;
};



#endif