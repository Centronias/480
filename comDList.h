// ****************************************************************************
// ****************************************************************************
// comDList.h
// ****************************************************************************
// 
// ****************************************************************************
// ****************************************************************************



// ****************************************************************************
// Defines
// ****************************************************************************
#ifndef	H_COM_D_LIST
#define H_COM_D_LIST



// ****************************************************************************
// comDListLink<T> Class
// ****************************************************************************
template <class T>
class comDListLink {
  public:
						comDListLink<T>(const T&	data);

  private:
	comDListLink<T>*	m_next;
	comDListLink<T>*	m_prev;

	T					m_data;

friend class comDList<T>;
friend class comDListIter<T>;
};



// ****************************************************************************
// comDList<T> Class
// ****************************************************************************
template <class T>
class comDList {
  public:
						comDList<T>();
						~comDList<T>();

	void				append(const T&	element);

  private:
	comDListLink<T>*	m_first;
	comDListLink<T>*	m_last;
	UINT				m_numEntries;

friend class comDListIter<T>;
};



// ****************************************************************************
// comDListIter<T> Class
// ****************************************************************************
template <class T>
class comDListIter {
  public:
						comDListIter<T>(const comDList<T>&	list);

	T					next();
	T					prev();

  private:
	comDListLink<T>*	m_next;
	comDListLink<T>*	m_prev;
};



// ****************************************************************************
// comDListIter<T>()
// ****************************************************************************
template <class T>
comDListIter<T>::comDListIter(const comDList<T>& list)
:	m_next(list.m_first),
	m_prev(list.m_last)
{
}



// ****************************************************************************
// comDListLink<T>()
// ****************************************************************************
template <class T>
comDListLink<T>::comDListLink(const T&	data)
:	m_next(NULL),
	m_prev(NULL),
	m_data(data)
{
}



// ****************************************************************************
// comDList<T>()
// ****************************************************************************
template <class T>
comDList<T>::comDList()
:	m_first(NULL),
	m_last(NULL),
	m_numEntries(0)
{
}



// ****************************************************************************
// ~comDList<T>()
// ****************************************************************************
template <class T>
comDList<T>::~comDList()
{
	comDListLink<T>*	link = m_first;
	comDListLink<T>*	next = m_first;

	while (link) {
		next = link->m_next;
		delete link;
		link = next;
	}
}



// ****************************************************************************
// append()
// ****************************************************************************
template <class T>
void
comDList<T>::append(const T&	element)
{
	comDListLink<T>*	next = new comDListLink<T>(element);
	if (m_last) {
		m_last->m_next = next;
		m_last = next;
	} else {
		m_first = next;
		m_last = next;
	}

	m_numEntries++;
}



// ****************************************************************************
// next()
// prev()
// ****************************************************************************
template <class T>
T
comDListIter<T>::next()
{
	if (!m_next)
		return NULL;

	comDListLink<T>*	ret	= m_next;
		m_prev	= ret;
		m_next	= ret->m_next;
	return ret->m_data;
}

template <class T>
T
comDListIter<T>::prev()
{
	if (!m_prev)
		return NULL;

	comDListLink<T>*	ret	= m_prev;
	if (ret) {
		m_next	= ret;
		m_prev	= ret->m_prev;
	}
	return ret->m_data;
}



#endif