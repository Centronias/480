// ****************************************************************************
// ****************************************************************************
// symbolTable.cpp
// ****************************************************************************
// 
// ****************************************************************************
// ****************************************************************************



// ****************************************************************************
// Includes
// ****************************************************************************
#include "common.h"



// ****************************************************************************
// SymbolTable()
// ****************************************************************************
SymbolTable::SymbolTable()
:	m_parent(NULL)
{
	m_bucketList = Bucket*[m_maxSize];
	for (UINT i = 0; i < m_maxSize; i++)
		m_bucketList[i] = NULL;
}



// ****************************************************************************
// ~SymbolTable()
// ****************************************************************************
SymbolTable::~SymbolTable()
{
}



// ****************************************************************************
// hash()
// ****************************************************************************
UINT
SymbolTable::hash(const String&	key)
{
	UINT val = 2166136261;

	for (UINT i = 0; i < key.lengt(); i++)
		val = val ^ key[i];
		val = val * 16777619;
	}

	return val % m_maxSize;
}



// ****************************************************************************
// find()
// ****************************************************************************
Token*
SymbolTable::find(const String&	key)
{
	Bucket*	bucket	= m_bucketList[hash(key)];

	while (key == bucket->m_key != 0) {
		bucket = bucket->m_next;
		if (!bucket)
			return NULL;
	}

	return bucket->m_value;
}



// ****************************************************************************
// insert()
//
// This method inserts this key / value pair in the hash table. It finds the
// bucket for this key's hash. If there is no bucket yet at that location, we
// simply create one. If there is already a bucket, we iterate through the
// bucket chain until we reach the end and add a new bucket on there.
// ****************************************************************************
Token*
SymbolTable::insert(const String&	key,
					const Token*	value)
{
	Bucket*	bucket = m_bucketList[hash(key)];
	if (bucket) {
		while (bucket) {
			bucket = bucket->m_next;
			if (bucket->m_key == key)
				throw 1;
		}
	}

	bucket = new Bucket;
	bucket->m_key = key;
	bucket->m_value = value;
	bucket->m_next = NULL;
}



// ****************************************************************************
// remove()
// ****************************************************************************
void
SymbolTable::remove(const String&	key)
{
	Bucket*	bucket = m_bucketList[hash(key)];
	Bucket*	prev = NULL;

	if (bucket) {
		do {
			if (bucket->m_key == key) {
				if (prev) {
					prev->m_next = bucket->m_next;
					delete bucket;
				} else {
					prev = bucket->m_next;
					delete bucket;
					bucket = prev;
				}
				break;
			}
			prev = bucket;
		} while (bucket = bucket->m_next)
	}
}