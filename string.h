// ****************************************************************************
// ****************************************************************************
// string.h
// ****************************************************************************
// 
// ****************************************************************************
// ****************************************************************************



// ****************************************************************************
// Defines
// ****************************************************************************
#ifndef	H_STRING
#define H_STRING



// ****************************************************************************
// String Class
// ****************************************************************************
class String {
  public:
						String();
						String(const char*	cBuf);
						~String();

						operator const char*() {return m_data;} const;

	String&				operator=(const String&	other);
	String&				operator=(const char*	other);
	bool				operator==(const String&	other) const;
	bool				operator!=(const String&	other) const;
	char&				operator[](const UINT	index) {return m_data[index];}

	UINT				getLength() const	{return strlen(m_data);}


  private:
	char*				m_data;
};



#endif