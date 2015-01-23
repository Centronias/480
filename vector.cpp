// ****************************************************************************
// ****************************************************************************
// vector.cpp
// ****************************************************************************
// 
// ****************************************************************************
// ****************************************************************************



// ****************************************************************************
// Includes
// ****************************************************************************
#include "common.h"



// ****************************************************************************
// Vector<T>()
// ****************************************************************************
Vector<T>::Vector<T>()
{
	m_data = new BYTE[sizeof(T)];
	m_numEntries = 0;
	m_size = 1;
}



// ****************************************************************************
// ~Vector<T>()
// ****************************************************************************
Vector<T>::~Vector<T>()
{
	delete [] m_data;
}



// ****************************************************************************
// append()
// ****************************************************************************
void
Vector<T>::append(T&	element)
{
	if (m_numEntries == m_size) {
		m_size++;
		BYTE*	buf = new BYTE[sizeof(T) * m_size];
		memcpy(buf, m_data, sizeof(T) * m_numEntries);
	}

	m_data[m_numEntries] = element;
}



// ****************************************************************************
// get()
// ****************************************************************************
T&
Vector<T>::operator[](UINT	index)
{
	return m_data[index];
}

const T&
Vector<T>::operator[](UINT	index) const
{
	return m_data[index];
}