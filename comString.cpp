// ****************************************************************************
// ****************************************************************************
// comString.cpp
// ****************************************************************************
// 
// ****************************************************************************
// ****************************************************************************



// ****************************************************************************
// Includes
// ****************************************************************************
#include "common.h"



// ****************************************************************************
// comString()
// ****************************************************************************
comString::comString()
{
	m_data	= new char[1];
	*m_data	= '\0';
}

comString::comString(const comString&	other)
{
	m_data = new char[other.getLength() + 1];
	strcpy(m_data, other.m_data);
}

comString::comString(const char*	cBuf)
{
	m_data = new char[strlen(cBuf) + 1];
	strcpy(m_data, cBuf);
}



// ****************************************************************************
// ~comString()
// ****************************************************************************
comString::~comString()
{
	delete [] m_data;
}



// ****************************************************************************
// operator=()
// ****************************************************************************
comString&
comString::operator=(const comString&	other)
{
	if (this != &other) {
		UINT	len = other.getLength();
		delete [] m_data;

		m_data = new char[len + 1];

		strcpy(m_data, other.m_data);
	}

	return *this;
}



// ****************************************************************************
// operator=()
// ****************************************************************************
comString&
comString::operator=(const char*	other)
{
	UINT	len = strlen(other);
	delete [] m_data;

	m_data = new char[len + 1];

	strcpy(m_data, other);

	return *this;
}



// ****************************************************************************
// operator==()
// ****************************************************************************
bool
comString::operator==(const comString&	other) const
{
	return strcmp(m_data, other.m_data) == 0;
}

bool
comString::operator==(const char*	other) const
{
	return strcmp(m_data, other) == 0;
}



// ****************************************************************************
// operator!=()
// ****************************************************************************
bool
comString::operator!=(const comString&	other) const
{
	return strcmp(m_data, other.m_data) != 0;
}

bool
comString::operator!=(const char*	other) const
{
	return strcmp(m_data, other) != 0;
}