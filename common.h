// ****************************************************************************
// ****************************************************************************
// common.h
// ****************************************************************************
// 
// ****************************************************************************
// ****************************************************************************



// ****************************************************************************
// External Includes
// ****************************************************************************
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>
#include </usr/include/ctype.h>
#include <stdlib.h>
#include <time.h>
#include <stdlib.h>



// ****************************************************************************
// Defines
// ****************************************************************************
#define UINT	unsigned int
#define BYTE	unsigned char

#define PT_MAX_CHILDREN	10



// ****************************************************************************
// Forward Class Declarations
// ****************************************************************************
template <class T> class comVector;
template <class T> class comDList;
template <class T> class comDListLink;
template <class T> class comDListIter;
class comString;
class SymbolTable;
class Token;
class State;
class Lexer;
class NonTerm;
class Parser;
class ParseTree;
class Production;
class Terminal;
class Translator;
class TransScheme;
struct Transition;
struct ProdEle;
struct PrUnit;
struct PoUnit;
struct VarDef;



// ****************************************************************************
// Type Definitions
// ****************************************************************************
typedef comVector<Transition*>	TransVec;
typedef comVector<Token*>		TokVec;
typedef	comVector<Production*>	ProdVec;
typedef comVector<ProdEle*>		ProdEleVec;
typedef comVector<State*>		StateVec;
typedef comVector<NonTerm*>		NTermVec;
typedef comDList<Token*>		TokList;
typedef comDListIter<Token*>	TokListIter;
typedef comVector<PrUnit*>		PrVec;
typedef comVector<PoUnit*>		PoVec;
typedef	comVector<TransScheme*>	TSVec;
typedef comVector<VarDef*>		VDVec;



// ****************************************************************************
// Helper Includes
// ****************************************************************************
#include "global.h"
#include "comVector.h"
#include "comDList.h"
#include "comString.h"



// ****************************************************************************
// Class Includes
// ****************************************************************************
#include "token.h"
#include "translator.h"
#include "symbolTable.h"
#include "state.h"
#include "grammar.h"
#include "lexer.h"
#include "parser.h"
#include "tu.h"