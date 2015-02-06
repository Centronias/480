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



// ****************************************************************************
// Defines
// ****************************************************************************
#define UINT	unsigned int
#define BYTE	unsigned char



// ****************************************************************************
// Forward Class Declarations
// ****************************************************************************
template <class T> class comVector;
class comString;
class SymbolTable;
class Token;
class State;
class Lexer;
class NonTerm;
class Parser;
struct Transition;
struct Production;
struct ProdEle;
struct Terminal;



// ****************************************************************************
// Type Definitions
// ****************************************************************************
typedef comVector<Transition*>	TransVec;
typedef comVector<Token*>		TokVec;
typedef	comVector<Production*>	ProdVec;
typedef comVector<ProdEle*>		ProdEleVec;
typedef comVector<State*>		StateVec;
typedef comVector<NonTerm*>		NTermVec;



// ****************************************************************************
// Helper Includes
// ****************************************************************************
#include "global.h"
#include "comVector.h"
#include "comString.h"



// ****************************************************************************
// Class Includes
// ****************************************************************************
#include "token.h"
#include "symbolTable.h"
#include "state.h"
#include "grammar.h"
#include "lexer.h"
#include "parser.h"