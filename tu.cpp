// ****************************************************************************
// ****************************************************************************
// tu.cpp
// ****************************************************************************
// 
// ****************************************************************************
// ****************************************************************************



// ****************************************************************************
// Includes
// ****************************************************************************
#include "common.h"



// ****************************************************************************
// PrUnit::PrUnit()
// ****************************************************************************
PrUnit::PrUnit(Translator::Type	type,
			   UINT				index)
:	m_type(type),
	m_index(index)
{
}



// ****************************************************************************
// PoUnit::PoUnit()
// ****************************************************************************
PoUnit::PoUnit(const comString&	keyword)
:	m_keyword(keyword),
	m_isKeyword(true),
    m_index(-1)
{
}

PoUnit::PoUnit(UINT	index)
:	m_index(index),
	m_isKeyword(false)
{
}



// ****************************************************************************
// TransScheme::printable()
// ****************************************************************************
comString&
TransScheme::printable(comString&	buf)
{
	char	printBuffer[128];
	char*	loc = printBuffer;

	UINT	pIndex = 0;

	for (UINT i = 0; i < m_preVec.getNumEntries(); i++) {
		PrUnit*	u = m_preVec[i];

		while (pIndex++ < u->m_index)
			loc += sprintf(loc, "x ");

		if (i == m_preVec.getNumEntries() - 1)
			loc += sprintf(loc, "-> ");

		loc += sprintf(loc, "%s ", (const char*) Translator::typeToString(u->m_type));
	}

	loc += sprintf(loc, ": ");

	for (UINT i = 0; i < m_postVec.getNumEntries(); i++) {
		PoUnit*	u = m_postVec[i];

		if (u->m_isKeyword)
			loc += sprintf(loc, "%s ", (const char*) u->m_keyword);
		else
			loc += sprintf(loc, "%d ", u->m_index);
	}

	*(loc - 1) = '\0';
	buf = printBuffer;
	return buf;
}



// ****************************************************************************
// TransScheme::returns()
// ****************************************************************************
Translator::Type
TransScheme::returns()
{
	if (m_preVec.getNumEntries())
		return m_preVec[m_preVec.getNumEntries() - 1]->m_type;

	return Translator::None;
}



// ****************************************************************************
// TransScheme::getPreType()
// ****************************************************************************
Translator::Type
TransScheme::getPreType(UINT	i)
{
	for (UINT j = 0; j < m_preVec.getNumEntries(); j++) {
		if (m_preVec[j]->m_index == i)
			return m_preVec[j]->m_type;
	}

	return Translator::None;
}