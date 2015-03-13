// ****************************************************************************
// ****************************************************************************
// comVector.h
// ****************************************************************************
// 
// ****************************************************************************
// ****************************************************************************



// ****************************************************************************
// Defines
// ****************************************************************************
#ifndef	H_COM_VECTOR
#define H_COM_VECTOR



// ****************************************************************************
// comVector<T> Class
// ****************************************************************************
template <class T>
class comVector {
  public:
						comVector<T>();
						~comVector<T>();

	void				append(const T&	element);
	T&					operator[](const UINT	index);
	const T&			operator[](const UINT	index) const;
	T&					get(const UINT	index);
	UINT				getNumEntries()	const {return m_numEntries;}

	void				forceZero()	{m_numEntries = 0;}

  protected:
	UINT				m_numEntries;
	UINT				m_size;
	T*					m_data;
};



// ****************************************************************************
// comVector<T>()
// ****************************************************************************
template <class T>
comVector<T>::comVector()
{
	m_data = (T*) new BYTE[sizeof(T)];
	m_numEntries = 0;
	m_size = 1;
}



// ****************************************************************************
// ~comVector<T>()
// ****************************************************************************
template <class T>
comVector<T>::~comVector()
{
	delete [] m_data;
}



// ****************************************************************************
// append()
// ****************************************************************************
template <class T>
void
comVector<T>::append(const T&	element)
{
	if (m_numEntries == m_size) {
		m_size++;
		BYTE*	buf = new BYTE[sizeof(T) * m_size];
		memcpy(buf, m_data, sizeof(T) * m_numEntries);
		delete [] m_data;
		m_data = (T*) buf;
	}

	m_data[m_numEntries++] = element;
}



// ****************************************************************************
// operator[]()
// get()
// ****************************************************************************
template <class T>
T&
comVector<T>::operator[](const UINT	index)
{
	return m_data[index];
}

template <class T>
const T&
comVector<T>::operator[](const UINT	index) const
{
	return m_data[index];
}

template <class T>
T&
comVector<T>::get(const UINT	index)
{
	return m_data[index];
}



#endif