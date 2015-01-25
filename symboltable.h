// ****************************************************************************
// ****************************************************************************
// symbolTable.h
// ****************************************************************************
// 
// ****************************************************************************
// ****************************************************************************



// ****************************************************************************
// Defines
// ****************************************************************************
#ifndef	H_SYMBOL_TABLE
#define H_SYMBOL_TABLE



// ****************************************************************************
// SymbolTable Class
// ****************************************************************************
class SymbolTable {
	struct Bucket {
		const comString	m_key;
		Token			m_value;
		Bucket*			m_next;

						Bucket(const comString&	key,
							   const Token&		value,
							   Bucket*			next);
	};
  public:
						SymbolTable();
						~SymbolTable();

	Token*				find(const comString&	key);
	void				insert(const comString&	key,
							   const Token&		value);
	void				remove(const comString&	key);


  private:
	static UINT			hash(const comString&	key);

	SymbolTable*		m_parent;
	Bucket**			m_bucketList;

	const static UINT	m_maxSize = 1 << 6;
};



#endif