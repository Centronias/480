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
		const String	m_key;
		Token*			m_value;
		Bucket*			m_next;
	};
  public:
						SymbolTable();
						~SymbolTable();

	Token*				find(const String&	key);
	void				insert(const String&	key,
							   const Token*		value);
	void				remove(const String&	key);


  private:
	static UINT			hash(const String&	key);

	SymbolTable*		m_parent;
	Bucket**			m_bucketList;

	const static UINT	m_maxSize = 1 << 6;
};



#endif