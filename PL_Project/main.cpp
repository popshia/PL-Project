// include libraries
# include <cctype>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <math.h>
# include <cctype>
# include <iostream>
# include <sstream>
# include <iomanip>
# include <string>
# include <vector>
# include <exception>

// definition
# define NOT !

// namespace
using namespace std;

// enum declaration
enum TokenType {
  LEFT_PAREN, RIGHT_PAREN, INT, STRING, DOT, FLOAT, NIL, T, QUOTE, SYMBOL
}; // token types

enum ErrorType {
  NOT_S_EXP, NO_CLOSING_QUOTE, UNEXPECTED_TOKEN_ATOM_LEFT_PAREN, UNEXPECTED_RIGHT_PAREN, NO_MORE_INPUT,
  INCORRECT_NUM_ARGUMENTS, INCORRECT_ARGUMENT_TYPE, DEFINE_UNBOUND, APPLY_NON_FUNCTION, NO_RETURN_VALUE,
  DIVISION_BY_ZERO, NON_LIST, DEFINE_FORMAT, LEVEL_OF_ERROR, COND_FORMAT
}; // error types

enum PrimitiveType {
  CONSTRUCTOR, QUOTE_BYPASSING, DEFINE_BINDING, PART_ACCESSOR, PRIMITIVE_PREDICATE, OPERATOR, EQUIVALENCE,
  BEGIN, CONDITIONAL, CLEAN_ENVIRONMENT, EXIT
}; // primitive types

// define structures
struct TokenStruct {
  string content;
  TokenType tokenType;
  bool isPrimitive;
  PrimitiveType primitiveType;
  bool isQuoteResult;
}; // TokenStruct

struct TreeStruct {
  TokenStruct *leftToken;
  TokenStruct *rightToken;
  TreeStruct *previousNode;
  TreeStruct *leftNode;
  TreeStruct *rightNode;
  bool isQuoteResult;
}; // TreeStruct

struct ErrorStruct {
  ErrorType errorType;
  string errorMessage;
}; // ErrorStruct

// define global variable to track the cursor
int g_CursorLine = 1;
int g_CursorColumn = 0;
int g_uTestNum = 0;

class Project1Class {
private:
  vector<TokenStruct *> m_OriginalTokens;
  vector<TokenStruct *> m_BuildTreeTokens;
  vector<TokenStruct *> m_BuildTreeTokensBackUp;
  TreeStruct *m_Root;
  TreeStruct *m_CurrentTreeLocation;
  ErrorStruct m_Error;

public:
  vector<TokenStruct *> GetSExp() {
    return m_OriginalTokens;
  } // GetSExp()
  
  TreeStruct *GetRoot() {
    return m_Root;
  } // GetRoot()
  
  bool CheckExit() {
    string exit = "\0";
    int tokenCount = 0;
    
    for ( int i = 0 ; i < m_OriginalTokens.size() ; i++ ) {
      if ( IsAtom( m_OriginalTokens[i] ) ||
           m_OriginalTokens[i]->tokenType == LEFT_PAREN ||
           m_OriginalTokens[i]->tokenType == RIGHT_PAREN ) {
        if ( m_OriginalTokens[i]->tokenType == NIL ) {
          if ( i > 1 ) {
            if ( m_OriginalTokens[i - 1]->tokenType != DOT && m_OriginalTokens[i + 1]->tokenType != DOT ) {
              exit += m_OriginalTokens[i]->content;
              tokenCount++;
            } // if: nil with no dot
          } // if: i > 1
        } // if: nil
        
        else {
          exit += m_OriginalTokens[i]->content;
          tokenCount++;
        } // else: not nil
      } // if: attach the symbol
    } // for: go through the tokens
    
    if ( ( exit == "(exit)" && tokenCount == 3 ) ||
         m_Error.errorType == NO_MORE_INPUT ) {
      return true;
    } // if: exit case
    
    else {
      return false;
    } // else: not exit case
  } // CheckExit()
  
  bool IsFloat( TokenStruct *newToken ) {
    for ( int i = 0 ; i < newToken->content.length() ; i++ ) {
      if ( newToken->content[i] == '.' ) {
        return true;
      } // if: if there's a dot in the inputTerm
    } // for: go through the inputTerm
    
    return false;
  } // IsFloat()
  
  bool IsAtom( TokenStruct *currentToken ) {
    if ( currentToken->tokenType == SYMBOL ||
         currentToken->tokenType == INT ||
         currentToken->tokenType == FLOAT ||
         currentToken->tokenType == STRING ||
         currentToken->tokenType == NIL ||
         currentToken->tokenType == T ) {
      return true;
    } // if: atom
    
    else {
      return false;
    } // else: not atom
  } // IsAtom()
  
  bool IsPrimitive( TokenStruct *currentToken ) {
    if ( currentToken->content == "cons" || currentToken->content == "list" ) {
      currentToken->isPrimitive = true;
      currentToken->primitiveType = CONSTRUCTOR;
    } // if: constructor
    
    else if ( currentToken->content == "quote" || currentToken->content == "'" ) {
      currentToken->isPrimitive = true;
      currentToken->primitiveType = QUOTE_BYPASSING;
    } // else if: quote bypassing
    
    else if ( currentToken->content == "define" ) {
      currentToken->isPrimitive = true;
      currentToken->primitiveType = DEFINE_BINDING;
    } // else if: difine binding
    
    else if ( currentToken->content == "car" || currentToken->content == "cdr" ) {
      currentToken->isPrimitive = true;
      currentToken->primitiveType = PART_ACCESSOR;
    } // else if: part accessors
    
    else if ( currentToken->content == "atom?" || currentToken->content == "pair?" ||
              currentToken->content == "list?" || currentToken->content == "null?" ||
              currentToken->content == "integer?" || currentToken->content == "real?" ||
              currentToken->content == "number?" || currentToken->content == "string?" ||
              currentToken->content == "boolean?" || currentToken->content == "symbol?" ) {
      currentToken->isPrimitive = true;
      currentToken->primitiveType = PRIMITIVE_PREDICATE;
    } // else if: primitive predicate
    
    else if ( currentToken->content == "+" || currentToken->content == "-" ||
              currentToken->content == "*" || currentToken->content == "/" ||
              currentToken->content == "not" || currentToken->content == "and" ||
              currentToken->content == "or" || currentToken->content == ">" ||
              currentToken->content == ">=" || currentToken->content == "<" ||
              currentToken->content == "<=" || currentToken->content == "=" ||
              currentToken->content == "string-append" || currentToken->content == "string>?" ||
              currentToken->content == "string<?" || currentToken->content == "string=?" ) {
      currentToken->isPrimitive = true;
      currentToken->primitiveType = OPERATOR;
    } // else if: opertor
    
    else if ( currentToken->content == "eqv?" || currentToken->content == "equal?" ) {
      currentToken->isPrimitive = true;
      currentToken->primitiveType = EQUIVALENCE;
    } // else if: equivalence
    
    else if ( currentToken->content == "begin" ) {
      currentToken->isPrimitive = true;
      currentToken->primitiveType = BEGIN;
    } // else if: begin
    
    else if ( currentToken->content == "if" || currentToken->content == "cond" ) {
      currentToken->isPrimitive = true;
      currentToken->primitiveType = CONDITIONAL;
    } // else if: conditional
    
    else if ( currentToken->content == "clean-environment" ) {
      currentToken->isPrimitive = true;
      currentToken->primitiveType = CLEAN_ENVIRONMENT;
    } // else if: clean environment
    
    else {
      currentToken->isPrimitive = false;
    } // else: not primitive
    
    return currentToken->isPrimitive;
  } // IsPrimitive()
  
  bool ReadSExp() {
    m_OriginalTokens.clear();
    m_BuildTreeTokens.clear();
    m_BuildTreeTokensBackUp.clear();
    m_Root = NULL;
    m_Error.errorType = NOT_S_EXP;
    m_Error.errorMessage = "\0";
    
    if ( HasNextToken() == true ) {
      if ( CheckSExp() == false ) {
        return false;
      } // if: handle the error
      
      else {
        InitializeRoot();
        bool goRight = false;
        m_BuildTreeTokensBackUp = m_BuildTreeTokens;
        BuildTree( m_Root, goRight );
        return true;
      } // else: print <S-exp>
    } // if: check the first input
    
    else {
      return false;
    } // else: first input error
  } // ReadSExp()
  
  char PeekCharAndGetRidOfComment() {
    char peekChar = cin.peek();
    
    while ( peekChar == ' ' || peekChar == '\n' || peekChar == '\r' ||
            peekChar == '\t' || peekChar == ';' || peekChar == '\0' ) {
      if ( peekChar == ';' ) {
        while ( peekChar != '\n' && peekChar != '\r' && peekChar != EOF ) {
          peekChar = cin.get();
          g_CursorColumn++;
        } // while: get the current line
        
        if ( peekChar == EOF ) {
          return EOF;
        } // if: eof
        
        g_CursorLine++;
        peekChar = cin.peek();
        g_CursorColumn = 0;
      } // if: check if there's any comment
      
      else {
        if ( peekChar == '\n' ) {
          g_CursorLine++;
          g_CursorColumn = 0;
        } // if: next line, modifiy cursor position
        
        else {
          g_CursorColumn++;
        } // else: cursor move right
        
        cin.get();
        peekChar = cin.peek();
      } // else: get next char
    } // while: get the first non-whitespace
    
    return peekChar;
  } // PeekCharAndGetRidOfComment()
  
  string GetString() {
    char currentChar = cin.get();
    
    if ( currentChar == '\n' ) {
      SetError( NO_CLOSING_QUOTE );
      return "\0";
    } // if: check closure error
    
    else if ( currentChar == EOF ) {
      SetError( NO_MORE_INPUT );
      return "\0";
    } // else if: eof
    
    g_CursorColumn++;
    string currentString = "\0";
    
    while ( currentChar != '\"' ) {
      char peekChar = cin.peek();
      
      if ( currentChar == '\\' && peekChar == '\"' ) {
        currentString.push_back( currentChar );
        currentString.push_back( cin.get() );
        g_CursorColumn++;
      } // if: "\"" case
      
      else {
        currentString.push_back( currentChar );
      } // else: normal string
      
      currentChar = cin.get();
      
      if ( currentChar == '\n' ) {
        SetError( NO_CLOSING_QUOTE );
        return "\0";
      } // if: check closure error
      
      else if ( currentChar == EOF ) {
        SetError( NO_MORE_INPUT );
        return "\0";
      } // if: eof
      
      g_CursorColumn++;
    } // while: get the string
    
    currentString.push_back( currentChar );
    // push the last " into the string
    return currentString;
  } // GetString()
  
  bool HasNextToken() {
    char peekChar = PeekCharAndGetRidOfComment();
    
    if ( peekChar == EOF ) {
      SetError( NO_MORE_INPUT );
      return false;
    } // if gets nothing
    
    TokenStruct *newToken = new TokenStruct;
    newToken->isQuoteResult = false;
    newToken->isPrimitive = false;
    newToken->content.push_back( cin.get() );
    g_CursorColumn++;
    // initialize new token
    
    if ( peekChar == '(' ) {
      if ( PeekCharAndGetRidOfComment() == ')' ) {
        newToken->content.push_back( cin.get() );
        g_CursorColumn++;
        newToken->tokenType = NIL;
      } // if: () case
      
      else {
        newToken->tokenType = LEFT_PAREN;
      } // else: left parenthesis
    } // if: left parenthesis
    
    else if ( peekChar == ')' ) {
      newToken->tokenType = RIGHT_PAREN;
    } // else if: right parenthesis
    
    else if ( peekChar == '"' ) {
      newToken->content.append( GetString() );
      
      if ( newToken->content == "\"" ) {
        return false;
      } // if: no closing quote error
      
      newToken->tokenType = STRING;
    } // else if: string
    
    else if ( peekChar == '\'' ) {
      newToken->tokenType = QUOTE;
    } // else if: quote
    
    else {
      while ( cin.peek() != '\n' && cin.peek() != ' ' &&
              cin.peek() != '\t' && cin.peek() != '\'' &&
              cin.peek() != '"' && cin.peek() != '(' &&
              cin.peek() != ')' && cin.peek() != ';' &&
              cin.peek() != '\r' && cin.peek() != '\0' &&
              cin.peek() != EOF ) {
        newToken->content.push_back( cin.get() );
        g_CursorColumn++;
      } // while: get the rest of the token
      
      newToken->tokenType = CheckTokenType( newToken );
    } // else: other types
    
    m_OriginalTokens.push_back( newToken ); // push the newToken to the vector
    m_BuildTreeTokens.push_back( newToken );
    return true;
  } // HasNextToken()
  
  TokenType CheckTokenType( TokenStruct *newToken ) {
    bool isNumber = false;
    bool isSymbol = false;
    int plusSignBitCount = 0;
    int minusSignBitCount = 0;
    int decimalPointCount = 0;
    
    for ( int i = 0 ; i < newToken->content.length() ; i++ ) {
      if ( newToken->content[i] == '+' ) {
        plusSignBitCount++;
      } // if: check the number of plus sign bit
      
      else if ( newToken->content[i] == '-' ) {
        minusSignBitCount++;
      } // else if: check the number of minus sign bit
      
      else if ( newToken->content[i] == '.' ) {
        decimalPointCount++;
      } // else if: check the number of decimal point count
      
      else if ( isdigit( newToken->content[i] ) ) {
        isNumber = true;
      } // else if: there are digits in the token
    } // for: go through the token
    
    if ( ( plusSignBitCount > 0 && minusSignBitCount > 0 ) ||
         plusSignBitCount > 1 || minusSignBitCount > 1 ||
         decimalPointCount > 1 ) {
      isNumber = false;
    } // if: more than one sign bit
    
    for ( int i = 0 ; i < newToken->content.length() ; i++ ) {
      if ( isupper( newToken->content[i] ) || islower( newToken->content[i] ) ) {
        isSymbol = true;
      } // if: there are alphabetic characters in the token
    } // for: go through the token
    
    if ( isNumber && NOT isSymbol ) {
      if ( IsFloat( newToken ) ) {
        return FLOAT;
      } // if: float
      
      else {
        return INT;
      } // else: int
    } // if: check if the number is float
    
    else {
      if ( newToken->content == "#f" || newToken->content == "nil" || newToken->content == "()" ) {
        return NIL;
      } // if: nil
      
      else if ( newToken->content == "t" || newToken->content == "#t" ) {
        return T;
      } // else if: #t
      
      else if ( newToken->content == "." ) {
        return DOT;
      } // else if: dot
      
      else if ( newToken->content == "quote" ) {
        return QUOTE;
      } // else if: quote
      
      else {
        return SYMBOL;
      } // else: symbols
    } // else: check t, nil or dot
  } // CheckTokenType()
  
  bool CheckSExp() {
    if ( IsAtom( m_OriginalTokens.back() ) ) {
      return true;
    } // if: <ATOM>
    
    else if ( m_OriginalTokens.back()->tokenType == LEFT_PAREN ) {
      if ( HasNextToken() == false ) {
        return false;
      } // if: check if there is any token left
      
      // LEFT-PAREN <S-exp>
      if ( CheckSExp() == true ) {
        if ( HasNextToken() == false ) {
          return false;
        } // if: check if there is any token left
        
        // LEFT-PAREN <S-exp> { <S-exp> }
        while ( CheckSExp() == true ) {
          if ( HasNextToken() == false ) {
            return false;
          } // if: check if there is any token left
        } // while: { <S-exp> }
        
        if ( m_Error.errorType != NOT_S_EXP ) {
          return false;
        } // if: there's a syntax error in the while loop
        
        // LEFT-PAREN <S-exp> { <S-exp> } [ DOT ]
        if ( m_OriginalTokens.back()->tokenType == DOT ) {
          if ( HasNextToken() == false ) {
            return false;
          } // if: check if there is any token left
          
          // LEFT-PAREN <S-exp> { <S-exp> } [ DOT <S-exp> ]
          if ( CheckSExp() == true ) {
            if ( HasNextToken() == false ) {
              return false;
            } // if: check if there is any token left
          } // if: <S-exp>
          
          else {
            if ( m_Error.errorType == NOT_S_EXP ) {
              SetError( UNEXPECTED_TOKEN_ATOM_LEFT_PAREN );
            } // if: overwrite error
            
            return false;
          } // else: syntax error
        } // if: [ DOT <S-exp> ]
        
        // LEFT-PAREN <S-exp> { <S-exp> } [ DOT <S-exp> ] RIGHT-PAREN
        if ( m_OriginalTokens.back()->tokenType == RIGHT_PAREN ) {
          return true;
        } // if: RIGHT-PAREN
        
        else {
          SetError( UNEXPECTED_RIGHT_PAREN );
          return false;
        } // else: syntax error
      } // if: <S-exp>
      
      else {
        if ( m_Error.errorType == NOT_S_EXP ) {
          SetError( UNEXPECTED_TOKEN_ATOM_LEFT_PAREN );
        } // if: overwrite error
        
        return false;
      } // else: syntax error
    } // else if: LEFT-PAREN
    
    else if ( m_OriginalTokens.back()->tokenType == QUOTE ) {
      bool quoteSymbolCase = false;
      
      if ( m_OriginalTokens.back()->content == "'" ) {
        m_BuildTreeTokens.pop_back();
        quoteSymbolCase = true;
        TokenStruct *quoteLeftParen = new TokenStruct;
        quoteLeftParen->tokenType = LEFT_PAREN;
        quoteLeftParen->content = "(";
        TokenStruct *quoteWord = new TokenStruct;
        quoteWord->tokenType = QUOTE;
        quoteWord->content = "quote";
        m_BuildTreeTokens.push_back( quoteLeftParen );
        m_BuildTreeTokens.push_back( quoteWord );
      } // if: ' case
      
      if ( HasNextToken() == false ) {
        return false;
      } // if: check if there is any token left
      
      // QUOTE <S-exp>
      if ( CheckSExp() == true ) {
        if ( quoteSymbolCase ) {
          TokenStruct *quoteRightParen = new TokenStruct;
          quoteRightParen->tokenType = RIGHT_PAREN;
          quoteRightParen->content = ")";
          m_BuildTreeTokens.push_back( quoteRightParen );
        } // if: ' case
        
        return true;
      } // if: <S-exp>
      
      else {
        SetError( UNEXPECTED_TOKEN_ATOM_LEFT_PAREN );
        return false;
      } // else: syntax error
    } // else if: QUOTE
    
    SetError( NOT_S_EXP );
    return false;
  } // CheckSExp()
  
  void BuildTree( TreeStruct *root, bool &goRight ) {
    if ( m_BuildTreeTokens.front()->tokenType == LEFT_PAREN ) {
      if ( goRight ) {
        goRight = false;
        root->rightNode = NewNode();
        root->rightNode->previousNode = root;
        m_BuildTreeTokens.erase( m_BuildTreeTokens.begin() );
        BuildTree( root->rightNode, goRight );
        root = root->rightNode;
      } // if: goRight
      
      else {
        root->leftNode = NewNode();
        root->leftNode->previousNode = root;
        m_BuildTreeTokens.erase( m_BuildTreeTokens.begin() );
        BuildTree( root->leftNode, goRight );
        root = root->leftNode;
      } // else: leftCreate
      
      while ( m_BuildTreeTokens.front()->tokenType == LEFT_PAREN || IsAtom( m_BuildTreeTokens.front() ) ||
              m_BuildTreeTokens.front()->content == "quote" ) {
        root->rightNode = NewNode();
        root->rightNode->previousNode = root;
        BuildTree( root->rightNode, goRight );
        root = root->rightNode;
      } // while: s-exp
      
      if ( m_BuildTreeTokens.front()->tokenType == DOT ) {
        m_BuildTreeTokens.erase( m_BuildTreeTokens.begin() );
        goRight = true;
        BuildTree( root, goRight );
      } // if: dot
      
      if ( m_BuildTreeTokens.front()->tokenType == RIGHT_PAREN ) {
        m_BuildTreeTokens.erase( m_BuildTreeTokens.begin() );
        return;
      } // if: not dot
    } // if: left-paren
    
    else if ( IsAtom( m_BuildTreeTokens.front() ) || m_BuildTreeTokens.front()->content == "quote" ) {
      if ( goRight ) {
        goRight = false;
        root->rightToken = m_BuildTreeTokens.front();
      } // if: goRight
      
      else {
        root->leftToken = m_BuildTreeTokens.front();
      } // else: leftToken
      
      m_BuildTreeTokens.erase( m_BuildTreeTokens.begin() );
    } // else if: atom
  } // BuildTree()
  
  void InitializeRoot() {
    m_Root = new TreeStruct;
    m_Root->leftNode = NULL;
    m_Root->rightNode = NULL;
    m_Root->previousNode = NULL;
    m_Root->leftToken = NULL;
    m_Root->rightToken = NULL;
    m_Root->isQuoteResult = false;
  } // InitializeRoot()
  
  TreeStruct *NewNode() {
    TreeStruct *newNode;
    newNode = new TreeStruct;
    newNode->leftNode = NULL;
    newNode->rightNode = NULL;
    newNode->previousNode = NULL;
    newNode->leftToken = NULL;
    newNode->rightToken = NULL;
    newNode->isQuoteResult = false;
    return newNode;
  } // NewNode()
  
  bool NeedToPrint( vector<TokenStruct *> printVectorint, int i, int leftParenCount ) {
    if ( printVectorint[i]->tokenType == DOT ) {
      if ( printVectorint[i + 1]->tokenType == LEFT_PAREN ) {
        return false;
      } // if: dot-leftParen case
      
      else if ( printVectorint[i + 1]->tokenType == NIL ) {
        return false;
      } // else if: dot-nil
    } // if: current token is DOT
    
    else if ( printVectorint[i]->tokenType == LEFT_PAREN ) {
      if ( i >= 1 ) {
        if ( printVectorint[i - 1]->tokenType == DOT ) {
          return false;
        } // if: dot-leftParen case
      } // if: check index validility
    } // if: current token is left-paren
    
    else if ( printVectorint[i]->tokenType == RIGHT_PAREN ) {
      if ( IsAtom( printVectorint[i - 1] ) ) {
        return true;
      } // if: printing after an atom
      
      else {
        if ( leftParenCount > 1 ) {
          return true;
        } // if: still have spaces for continious rightParen
        
        return false;
      } // else if: continious rightParen
    } // else if: currrent token is right-paren
    
    else if ( printVectorint[i]->tokenType == NIL ) {
      if ( i >= 1 ) {
        if ( printVectorint[i - 1]->tokenType == DOT &&
             printVectorint[i + 1]->tokenType == RIGHT_PAREN ) {
          return false;
        } // if: nil-rightParen case
      } // if: check index validility
    } // else if: current token is NIL
    
    else if ( printVectorint[i]->content == "'" ) {
      return false;
    } // else if: ' quote case
    
    return true;
  } // NeedToPrint()
  
  void PrintIndentation( int leftParenCount ) {
    int spaces = leftParenCount*2;
    
    while ( spaces > 0 ) {
      cout << " ";
      spaces--;
    } // while: print spaces
  } // PrintIndentation()
  
  void PrintSExp( string originalOrBuild ) {
    int leftParenCount = 0;
    bool hasLineReturn = false;
    vector<TokenStruct *> result;
    
    if ( originalOrBuild == "original" ) {
      result = m_OriginalTokens;
    } // if: originalTokens
    
    else {
      result = m_BuildTreeTokensBackUp;
    } // else: buildTreeTokens
    
    for ( int i = 0 ; i < result.size() ; i++ ) {
      if ( NeedToPrint( result, i, leftParenCount ) ) {
        if ( result[i]->tokenType == RIGHT_PAREN ) {
          leftParenCount--;
        } // if: print right parenthesis, minus leftParenCount by one
        
        if ( hasLineReturn ) {
          PrintIndentation( leftParenCount );
        } // if: has a line return
        
        if ( IsAtom( result[i] ) || result[i]->content == "quote" ) {
          if ( result[i]->tokenType == INT ) {
            cout << atoi( result[i]->content.c_str() ) << endl;
          } // if: int case
          
          else if ( result[i]->tokenType == FLOAT ) {
            cout << fixed << setprecision( 3 )
                 << round( atof( result[i]->content.c_str() )*1000 )/1000
                 << endl;
          } // else if: float case with precision and round
          
          else if ( result[i]->tokenType == NIL ) {
            cout << "nil" << endl;
          } // else if: nil
          
          else if ( result[i]->tokenType == T ) {
            cout << "#t" << endl;
          } // else if: #t
          
          else if ( result[i]->tokenType == STRING ) {
            PrintString( result[i]->content );
          } // else if: string
          
          else if ( result[i]->tokenType == SYMBOL || result[i]->content == "quote" ) {
            cout << result[i]->content << endl;
          } // else if: symbol
          
          hasLineReturn = true;
        } // if: current token is an atom
        
        else if ( result[i]->tokenType == LEFT_PAREN ) {
          leftParenCount++;
          hasLineReturn = false;
          cout << "( ";
        } // else if: left paren
        
        else if ( result[i]->tokenType == RIGHT_PAREN ) {
          cout << ")" << endl;
          hasLineReturn = true;
        } // else if: right paren
        
        else if ( result[i]->tokenType == DOT ) {
          cout << "." << endl;
          hasLineReturn = true;
        } // else if: dot
      } // if: token need to be printed
      
      if ( leftParenCount == 0 && result[i]->tokenType != QUOTE ) {
        return;
      } // if: check print complete
      
      else if ( i == result.size() - 1 && leftParenCount > 0 ) {
        while ( leftParenCount > 0 ) {
          leftParenCount--;
          PrintIndentation( leftParenCount );
          cout << ")" << endl;
        } // while: loop print right-paren
      } // if: for loop is end but leftParenCount still greater than zero
    } // for: go through the vector
  } // PrintSExp()
  
  void PrintString( string stringContent ) {
    for ( int index = 0 ; index < stringContent.length() ; index++ ) {
      if ( stringContent[index] == '\\' ) {
        if ( stringContent[index + 1] == 'n' ) {
          cout << endl;
          index++;
        } // if: '\n'
        
        else if ( stringContent[index + 1] == 't' ) {
          cout << '\t';
          index++;
        } // else if: '\t'
        
        else if ( stringContent[index + 1] == '"' ) {
          cout << '"';
          index++;
        } // else if: '"'
        
        else if ( stringContent[index + 1] == '\\' ) {
          cout << '\\';
          index++;
        } // else if: '\\'
        
        else {
          cout << stringContent[index];
        } // else: normal '\\'
      } // if: escape
      
      else {
        cout << stringContent[index];
      } // else: normal character
    } // for: go through the string
    
    cout << endl;
  } // PrintString()
  
  void SetError( ErrorType errorType ) {
    stringstream errorStream;
    
    if ( errorType == NO_CLOSING_QUOTE ) {
      errorStream << "ERROR (no closing quote) : "
                  << "END-OF-LINE encountered at "
                  << "Line " << g_CursorLine << " Column " << g_CursorColumn + 1;
      m_Error.errorType = NO_CLOSING_QUOTE;
      m_Error.errorMessage = errorStream.str();
    } // if: no closing quote
    
    else if ( errorType == UNEXPECTED_TOKEN_ATOM_LEFT_PAREN ) {
      errorStream << "ERROR (unexpected token) : "
                  << "atom or '(' expected when token at "
                  << "Line " << g_CursorLine
                  << " Column " << g_CursorColumn - ( m_OriginalTokens.back()->content.size() - 1 )
                  << " is >>" << m_OriginalTokens.back()->content << "<<";
      m_Error.errorType = UNEXPECTED_TOKEN_ATOM_LEFT_PAREN;
      m_Error.errorMessage = errorStream.str();
    } // else if: unexpected atom or left paren
    
    else if ( errorType == UNEXPECTED_RIGHT_PAREN ) {
      errorStream << "ERROR (unexpected token) : "
                  << "')' expected when token at "
                  << "Line " << g_CursorLine
                  << " Column " << g_CursorColumn - ( m_OriginalTokens.back()->content.size() - 1 )
                  << " is >>" << m_OriginalTokens.back()->content << "<<";
      m_Error.errorType = UNEXPECTED_RIGHT_PAREN;
      m_Error.errorMessage = errorStream.str();
    } // else if: unexpected right paren
    
    else if ( errorType == NO_MORE_INPUT ) {
      m_Error.errorType = NO_MORE_INPUT;
      m_Error.errorMessage = "ERROR (no more input) : END-OF-FILE encountered";
    } // else if: no more input
    
    else if ( errorType == NOT_S_EXP ) {
      errorStream << "ERROR (unexpected token) : "
                  << "atom or '(' expected when token at "
                  << "Line " << g_CursorLine
                  << " Column " << g_CursorColumn - ( m_OriginalTokens.back()->content.size() - 1 )
                  << " is >>" << m_OriginalTokens.back()->content << "<<";
      m_Error.errorType = NOT_S_EXP;
      m_Error.errorMessage = errorStream.str();
    } // else if: none above
  } // SetError()
  
  void ErrorHandling() {
    if ( m_Error.errorType == NO_MORE_INPUT ) {
      cout << m_Error.errorMessage;
    } // if: eof
    
    else {
      cout << m_Error.errorMessage << endl;
      
      if ( m_Error.errorType != NO_CLOSING_QUOTE ) {
        char peekChar = cin.peek();
        
        while ( peekChar != '\n' ) {
          if ( cin.peek() == EOF ) {
            return;
          } // if: eof
          
          cin.get();
          peekChar = cin.peek();
        } // while: get the left overs
        
        cin.get();
      } // if: not no closing quote, get the leftovers
    } // else: not eof
  } // ErrorHandling()
  
  void ClearTheLine() {
    g_CursorLine = 1;
    g_CursorColumn = 0;
    
    char peekChar = cin.peek();
    
    while ( peekChar == ' ' ) {
      cin.get();
      g_CursorColumn++;
      peekChar = cin.peek();
    } // while: get space
    
    if ( peekChar == '\n' ) {
      cin.get();
      g_CursorLine = 1;
      g_CursorColumn = 0;
    } // if: get the useless line return
    
    else if ( peekChar == ';' ) {
      cin.get();
      peekChar = cin.peek();
      
      while ( peekChar != '\n' ) {
        if ( cin.peek() == EOF ) {
          return;
        } // if: eof
        
        cin.get();
        peekChar = cin.peek();
      } // while: get the rest off the comment
      
      cin.get();
      g_CursorLine = 1;
      g_CursorColumn = 0;
    } // else if: comment
    
    else {
      return;
    } // else: get valid character
  } // ClearTheLine()
}; // Project1Class

class Project2Class {
  Project1Class m_project1;
  vector<TreeStruct *> m_DefineBindingList;
  TreeStruct *m_Root;

public:
  bool IsPrimitive( TokenStruct *currentToken ) {
    if ( currentToken->content == "cons" || currentToken->content == "list" ) {
      currentToken->isPrimitive = true;
      currentToken->primitiveType = CONSTRUCTOR;
    } // if: constructor
    
    else if ( currentToken->content == "quote" || currentToken->content == "'" ) {
      currentToken->isPrimitive = true;
      currentToken->primitiveType = QUOTE_BYPASSING;
    } // else if: quote bypassing
    
    else if ( currentToken->content == "define" ) {
      currentToken->isPrimitive = true;
      currentToken->primitiveType = DEFINE_BINDING;
    } // else if: difine binding
    
    else if ( currentToken->content == "car" || currentToken->content == "cdr" ) {
      currentToken->isPrimitive = true;
      currentToken->primitiveType = PART_ACCESSOR;
    } // else if: part accessors
    
    else if ( currentToken->content == "atom?" || currentToken->content == "pair?" ||
              currentToken->content == "list?" || currentToken->content == "null?" ||
              currentToken->content == "integer?" || currentToken->content == "real?" ||
              currentToken->content == "number?" || currentToken->content == "string?" ||
              currentToken->content == "boolean?" || currentToken->content == "symbol?" ) {
      currentToken->isPrimitive = true;
      currentToken->primitiveType = PRIMITIVE_PREDICATE;
    } // else if: primitive predicate
    
    else if ( currentToken->content == "+" || currentToken->content == "-" ||
              currentToken->content == "*" || currentToken->content == "/" ||
              currentToken->content == "not" || currentToken->content == "and" ||
              currentToken->content == "or" || currentToken->content == ">" ||
              currentToken->content == ">=" || currentToken->content == "<" ||
              currentToken->content == "<=" || currentToken->content == "=" ||
              currentToken->content == "string-append" || currentToken->content == "string>?" ||
              currentToken->content == "string<?" || currentToken->content == "string=?" ) {
      currentToken->isPrimitive = true;
      currentToken->primitiveType = OPERATOR;
    } // else if: opertor
    
    else if ( currentToken->content == "eqv?" || currentToken->content == "equal?" ) {
      currentToken->isPrimitive = true;
      currentToken->primitiveType = EQUIVALENCE;
    } // else if: equivalence
    
    else if ( currentToken->content == "begin" ) {
      currentToken->isPrimitive = true;
      currentToken->primitiveType = BEGIN;
    } // else if: begin
    
    else if ( currentToken->content == "if" || currentToken->content == "cond" ) {
      currentToken->isPrimitive = true;
      currentToken->primitiveType = CONDITIONAL;
    } // else if: conditional
    
    else if ( currentToken->content == "clean-environment" ) {
      currentToken->isPrimitive = true;
      currentToken->primitiveType = CLEAN_ENVIRONMENT;
    } // else if: clean environment
    
    else if ( currentToken->content == "exit" ) {
      currentToken->isPrimitive = true;
      currentToken->primitiveType = EXIT;
    } // else if: difine binding
    
    else {
      currentToken->isPrimitive = false;
    } // else: not primitive
    
    return currentToken->isPrimitive;
  } // IsPrimitive()
  
  TreeStruct *NewNode() {
    TreeStruct *newNode = new TreeStruct;
    newNode->leftToken = NULL;
    newNode->leftNode = NULL;
    newNode->rightToken = NULL;
    newNode->rightNode = NULL;
    newNode->previousNode = NULL;
    return newNode;
  } // NewNode()
  
  TokenStruct *NewToken() {
    TokenStruct *newToken = new TokenStruct;
    newToken->content = "\0";
    newToken->isPrimitive = false;
    return newToken;
  } // NewToken()
  
  void ErrorHandling( ErrorType type, string message, TokenStruct *token, TreeStruct *node ) {
    cout << message;
    
    if ( token ) {
      PrintData( token, false );
      return;
    } // if: has token
    
    if ( node ) {
      int layer = 0;
      bool isRightNode = false;
      
      if ( type == NON_LIST || type == DEFINE_FORMAT ||
           type == COND_FORMAT || type == NO_RETURN_VALUE ) {
        PrintTree( node, layer, isRightNode, false );
      } // if: non-list, no need to print #<procedure>
      
      else {
        PrintTree( node, layer, isRightNode, true );
      } // else: print #<procedure>
      
      if ( layer > 0 ) {
        while ( layer > 0 ) {
          layer--;
          cout << string( layer, ' ' ) << ')' << endl;
        } // while: loop print right-paren
      } // if: layer still greater than zero
      
      return;
    } // if: error argument is a node
    
    cout << endl;
  } // ErrorHandling()
  
  void EvalSExp( Project1Class project1Result ) {
    m_project1 = project1Result;
    m_Root = ProcessSExp( m_project1.GetRoot() );
  } // EvalSExp()
  
  TreeStruct *ProcessSExp( TreeStruct *currentNode ) {
    if ( currentNode->leftToken ) {
      if ( !IsPrimitive( currentNode->leftToken ) && currentNode->leftToken->tokenType == SYMBOL &&
           !currentNode->isQuoteResult ) {
        TreeStruct *defined = GetDefineBindings( currentNode->leftToken->content );
        
        if ( defined == NULL ) {
          string errorMessage = "ERROR (unbound symbol) : ";
          ErrorHandling( DEFINE_UNBOUND, errorMessage, currentNode->leftToken, NULL );
          throw "define unbound";
        } // if: define unbound
        
        else if ( defined->leftToken ) {
          currentNode->leftToken = defined->leftToken;
        } // else if: defined as token
        
        else if ( defined->leftNode ) {
          currentNode->leftToken = NULL;
          currentNode->leftNode = defined->leftNode;
          
          if ( defined->isQuoteResult ) {
            currentNode->leftNode->isQuoteResult = true;
          } // if: inherit isQuoteResult
        } // else if: defined as node
      } // if: can be a token, or a node
    } // if: send in a token, then return
    
    else {
      if ( currentNode->leftNode->leftToken == NULL ) {
        currentNode->leftNode = ProcessSExp( currentNode->leftNode );
        
        if ( currentNode->leftNode->rightNode == NULL ) {
          return currentNode->leftNode;
        } // if: no arguments left, return
      } // if: still got nodes
      
      if ( IsList( currentNode->leftNode, false )->leftToken->tokenType == NIL ) {
        ErrorHandling( NON_LIST, "ERROR (non-list) : ", NULL, currentNode->leftNode );
        throw "non-list";
      } // if: non-list
      
      if ( currentNode->leftNode->leftNode ) {
        ErrorHandling( APPLY_NON_FUNCTION, "ERROR (attempt to apply non-function) : ", NULL,
                       currentNode->leftNode->leftNode );
        throw "apply non-function";
      } // if: the left node is still a node
      
      if ( !IsPrimitive( currentNode->leftNode->leftToken ) &&
           currentNode->leftNode->leftToken->tokenType == SYMBOL ) {
        TreeStruct *defined = GetDefineBindings( currentNode->leftNode->leftToken->content );
        
        if ( defined->leftToken ) {
          currentNode->leftNode->leftToken = defined->leftToken;
        } // else if: defined as token
        
        else {
          if ( defined->isQuoteResult ) {
            defined->leftNode->isQuoteResult = true;
          } // if: inherit isQuoteResult
          
          string errorMessage = "ERROR (attempt to apply non-function) : ";
          ErrorHandling( APPLY_NON_FUNCTION, errorMessage, NULL, defined->leftNode );
          throw "apply non function";
        } // else if: defined as node
      } // if: symbol
      
      if ( !IsPrimitive( currentNode->leftNode->leftToken ) ) {
        string errorMessage = "ERROR (attempt to apply non-function) : ";
        ErrorHandling( APPLY_NON_FUNCTION, errorMessage, currentNode->leftNode->leftToken, NULL );
        throw "apply non function";
      } // if: apply non-function
      
      else {
        TreeStruct *subResult = CallCorrespondingFunction( currentNode->leftNode );
        subResult->rightNode = currentNode->rightNode;
        
        if ( currentNode->previousNode && currentNode == currentNode->previousNode->rightNode ) {
          currentNode->previousNode->rightNode = subResult;
        } // if: has previousNode
        
        currentNode = subResult;
      } // else: call the corresponding funciton
    } // else: send in a node, call and process the function
    
    return currentNode;
  } // ProcessSExp()
  
  TreeStruct *GetDefineBindings( string currentWord ) {
    for ( int i = 0 ; i < m_DefineBindingList.size() ; i++ ) {
      if ( currentWord == m_DefineBindingList[i]->leftToken->content ) {
        return m_DefineBindingList[i]->rightNode;
      } // if: find
    } // for: go through m_DefineBindingList
    
    string errorMessage = "ERROR (unbound symbol) : " + currentWord;
    ErrorHandling( DEFINE_UNBOUND, errorMessage.c_str(), NULL, NULL );
    throw "define unbound";
  } // GetDefineBindings()
  
  void CheckArgumentNumber( TreeStruct *functionNode, int argumentNumber, bool greaterThan ) {
    int count = 0;
    string functionName = functionNode->leftToken->content;
    
    while ( functionNode->rightNode ) {
      count++;
      functionNode = functionNode->rightNode;
    } // while: go through the arguments
    
    if ( ( NOT greaterThan && count != argumentNumber ) || ( greaterThan && count < argumentNumber ) ) {
      if ( functionName == "if" && count == 3 ) {
        return;
      } // if: if can take 3 arguments
      
      else if ( functionName == "define" ) {
        string errorMessage = "ERROR (DEFINE format) : ";
        ErrorHandling( DEFINE_FORMAT, errorMessage, NULL, m_project1.GetRoot()->leftNode );
        throw "define format";
      } // if: define format
      
      else if ( functionName == "cond" ) {
        string errorMessage = "ERROR (COND format) : ";
        ErrorHandling( COND_FORMAT, errorMessage, NULL, m_project1.GetRoot()->leftNode );
        throw "cond format";
      } // else if: cond format
      
      else {
        string errorMessage = "ERROR (incorrect number of arguments) : " + functionName;
        ErrorHandling( INCORRECT_NUM_ARGUMENTS, errorMessage, NULL, NULL );
        throw "incorrect numbers of argument";
      } // else: incorrect num of arguments
    } // if: count != argumentNumber
  } // CheckArgumentNumber()
  
  bool CompareTwoTrees( TreeStruct *firstTree, TreeStruct *secondTree ) {
    if ( firstTree == NULL && secondTree == NULL ) {
      return true;
    } // if: both empty
    
    if ( firstTree != NULL && secondTree != NULL ) {
      bool sameLeftContent = false;
      bool sameRightContent = false;
      
      if ( firstTree->leftToken && secondTree->leftToken ) {
        if ( firstTree->leftToken->content == secondTree->leftToken->content ) {
          sameLeftContent = true;
        } // if: same content
      } // if: both have leftToken
      
      if ( firstTree->rightToken && secondTree->rightToken ) {
        if ( firstTree->rightToken->content == secondTree->rightToken->content ) {
          sameRightContent = true;
        } // if: same content
      } // if: both have rightToken
      
      if ( firstTree->leftToken || secondTree->leftToken ) {
        if ( firstTree->rightToken || secondTree->rightToken ) {
          return ( sameLeftContent && sameRightContent );
        } // if: have both token
        
        else {
          return ( sameLeftContent &&
                   CompareTwoTrees( firstTree->rightNode, secondTree->rightNode ) );
        } // else: only leftToken
      } // if: have any leftToken
      
      else if ( firstTree->rightToken || secondTree->rightToken ) {
        if ( firstTree->leftToken || secondTree->leftToken ) {
          return ( sameLeftContent && sameRightContent );
        } // if: have both token
        
        else {
          return ( sameRightContent &&
                   CompareTwoTrees( firstTree->leftNode, secondTree->leftNode ) );
        } // else: only rightToken
      } // if; have any rightToken
      
      else {
        return ( CompareTwoTrees( firstTree->leftNode, secondTree->leftNode ) &&
                 CompareTwoTrees( firstTree->rightNode, secondTree->rightNode ) );
      } // else: has no token to compare
    } // if: both not empty, compare two trees
    
    return false;
  } // CompareTwoTrees()
  
  void SetQuoteResult( TreeStruct *resultNode ) {
    if ( resultNode->leftToken ) {
      resultNode->leftToken->isQuoteResult = true;
    } // if: has leftToken
    
    if ( resultNode->rightToken ) {
      resultNode->rightToken->isQuoteResult = true;
    } // if: has rightToken
    
    if ( resultNode->leftNode ) {
      SetQuoteResult( resultNode->leftNode );
    } // if: has leftNode
    
    if ( resultNode->rightNode ) {
      SetQuoteResult( resultNode->rightNode );
    } // if: has rightNode
  } // SetQuoteResult()
  
  TreeStruct *CallCorrespondingFunction( TreeStruct *functionNode ) {
    TokenStruct *function = functionNode->leftToken;
    
    if ( function->primitiveType == CONSTRUCTOR ) {
      if ( function->content == "cons" ) {
        return Cons( functionNode );
      } // if: cons
      
      else {
        return List( functionNode );
      } // else if: list
    } // if: constructor
    
    else if ( function->primitiveType == QUOTE_BYPASSING ) {
      return Quote( functionNode );
    } // else if: quote
    
    else if ( function->primitiveType == DEFINE_BINDING ) {
      return Define( functionNode );
    } // else if: define
    
    else if ( function->primitiveType == PART_ACCESSOR ) {
      if ( function->content == "car" ) {
        return Car( functionNode );
      } // if: car
      
      else {
        return Cdr( functionNode );
      } // else: cdr
    } // else if: part accessor
    
    else if ( function->primitiveType == PRIMITIVE_PREDICATE ) {
      if ( function->content == "atom?" ) {
        return IsAtom( functionNode );
      } // if: atom?
      
      else if ( function->content == "pair?" ) {
        return IsPair( functionNode );
      } // else if: pair?
      
      else if ( function->content == "list?" ) {
        return IsList( functionNode, true );
      } // else if: list?
      
      else if ( function->content == "null?" ) {
        return IsNull( functionNode );
      } // else if: null?
      
      else if ( function->content == "integer?" ) {
        return IsInteger( functionNode );
      } // else if: integer?
      
      else if ( function->content == "real?" || function->content == "number?" ) {
        return IsRealOrNumber( functionNode );
      } // else if: real? or number?
      
      else if ( function->content == "string?" ) {
        return IsString( functionNode );
      } // else if: string?
      
      else if ( function->content == "boolean?" ) {
        return IsBoolean( functionNode );
      } // else if: boolean?
      
      else if ( function->content == "symbol?" ) {
        return IsSymbol( functionNode );
      } // else if: symbol?
    } // else if: primitive predicate
    
    else if ( function->primitiveType == OPERATOR ) {
      if ( function->content == "+" ) {
        return Addition( functionNode );
      } // if: +
      
      else if ( function->content == "-" ) {
        return Subtraction( functionNode );
      } // else if: -
      
      else if ( function->content == "*" ) {
        return Mutiplication( functionNode );
      } // else if: *
      
      else if ( function->content == "/" ) {
        return Division( functionNode );
      } // else if: /
      
      else if ( function->content == "not" ) {
        return Not( functionNode );
      } // else if: not
      
      else if ( function->content == "and" ) {
        return And( functionNode );
      } // else if: and
      
      else if ( function->content == "or" ) {
        return Or( functionNode );
      } // else if: or
      
      else if ( function->content == ">" ) {
        return GreaterThan( functionNode );
      } // else if: >
      
      else if ( function->content == ">=" ) {
        return GreaterThanOrEqualTo( functionNode );
      } // else if: >=
      
      else if ( function->content == "<" ) {
        return LessThan( functionNode );
      } // else if: <
      
      else if ( function->content == "<=" ) {
        return LessThanOrEqualTo( functionNode );
      } // else if: <=
      
      else if ( function->content == "=" ) {
        return EqualTo( functionNode );
      } // else if: =
      
      else if ( function->content == "string-append" ) {
        return StringAppend( functionNode );
      } // else if: string-append
      
      else if ( function->content == "string>?" ) {
        return StringGreaterThan( functionNode );
      } // else if: string>?
      
      else if ( function->content == "string<?" ) {
        return StringLessThan( functionNode );
      } // else if: string<?
      
      else if ( function->content == "string=?" ) {
        return StringEqualTo( functionNode );
      } // else if: string=?
    } // else if; operator
    
    else if ( function->primitiveType == EQUIVALENCE ) {
      if ( function->content == "eqv?" ) {
        return Eqv( functionNode );
      } // if: eqv?
      
      else if ( function->content == "equal?" ) {
        return Equal( functionNode );
      } // else if: equal?
    } // else if: equivalence
    
    else if ( function->primitiveType == BEGIN ) {
      return Begin( functionNode );
    } // else if: begin
    
    else if ( function->primitiveType == CONDITIONAL ) {
      if ( function->content == "if" ) {
        return If( functionNode );
      } // if: if
      
      else if ( function->content == "cond" ) {
        return Cond( functionNode );
      } // else if: cond
    } // else if: conditional
    
    else if ( function->primitiveType == CLEAN_ENVIRONMENT ) {
      return CleanEnvironment( functionNode );
    } // else if: clean-environment
    
    else if ( function->primitiveType == EXIT ) {
      return Exit( functionNode );
    } // else if: exit
    
    return NULL;
  } // CallCorrespondingFunction()
  
  TreeStruct *Cons( TreeStruct *functionNode ) {
    CheckArgumentNumber( functionNode, 2, false );
    TreeStruct *resultRoot = NewNode();
    resultRoot->leftNode = NewNode();
    TreeStruct *result = resultRoot->leftNode;
    TreeStruct *resultLeft = ProcessSExp( functionNode->rightNode );
    TreeStruct *resultRight = ProcessSExp( functionNode->rightNode->rightNode );
    
    if ( resultLeft->leftToken ) {
      result->leftToken = resultLeft->leftToken;
    } // if: left is token
    
    else {
      result->leftNode = resultLeft->leftNode;
    } // else: left is node
    
    if ( resultRight->leftToken && resultRight->leftToken->tokenType != NIL ) {
      result->rightToken = resultRight->leftToken;
    } // if: right is token
    
    else {
      result->rightNode = resultRight->leftNode;
    } // else: right is node
    
    return resultRoot;
  } // Cons()
  
  TreeStruct *List( TreeStruct *functionNode ) {
    CheckArgumentNumber( functionNode, 0, true );
    TreeStruct *resultRoot = NewNode();
    
    if ( functionNode->rightNode == NULL ) {
      resultRoot->leftToken = NewToken();
      resultRoot->leftToken->tokenType = NIL;
      resultRoot->leftToken->content = "nil";
      return resultRoot;
    } // if: only the list function, no arguments
    
    resultRoot->leftNode = NewNode();
    TreeStruct *resultWalk = resultRoot->leftNode;
    TreeStruct *argumentWalk = functionNode->rightNode;
    
    while ( argumentWalk ) {
      TreeStruct *argumentNode = ProcessSExp( argumentWalk );
      
      if ( argumentNode->leftToken ) {
        resultWalk->leftToken = argumentNode->leftToken;
      } // if: argument is a token
      
      else {
        resultWalk->leftNode = argumentNode->leftNode;
      } // else: argument is a node
      
      argumentWalk = argumentWalk->rightNode;
      
      if ( argumentWalk ) {
        resultWalk->rightNode = NewNode();
        resultWalk = resultWalk->rightNode;
      } // if: has next argument
    } // while: go through the arguments
    
    return resultRoot;
  } // List()
  
  TreeStruct *Quote( TreeStruct *functionNode ) {
    functionNode->rightNode->isQuoteResult = true;
    SetQuoteResult( functionNode->rightNode );
    return functionNode->rightNode;
  } // Quote()
  
  TreeStruct *Define( TreeStruct *functionNode ) {
    if ( functionNode->previousNode->previousNode ) {
      
      ErrorHandling( LEVEL_OF_ERROR, "ERROR (level of DEFINE)", NULL, NULL );
      throw "level of define";
    } // if: level of define
    
    CheckArgumentNumber( functionNode, 2, false );
    
    TreeStruct *definedFrom = functionNode->rightNode;
    TreeStruct *definedTo = NewNode();
    TreeStruct *result = NewNode();
    
    if ( ( definedFrom->leftNode ) ||
         ( definedFrom->leftToken && definedFrom->leftToken->tokenType != SYMBOL ) ||
         ( IsPrimitive( definedFrom->leftToken ) ) ) {
      ErrorHandling( DEFINE_FORMAT, "ERROR (DEFINE format) : ", NULL, m_project1.GetRoot()->leftNode );
      throw "define format";
    } // if: define format error
    
    if ( functionNode->rightNode->rightNode->leftNode ) {
      definedTo = ProcessSExp( functionNode->rightNode->rightNode );
    } // if: defineTo is a node
    
    else {
      definedTo = functionNode->rightNode->rightNode;
    } // else: defineTo is a token
    
    result->leftToken = NewToken();
    result->leftToken->content = definedFrom->leftToken->content + " defined";
    result->leftToken->tokenType = STRING;
    TreeStruct *defineRecord = NewNode();
    defineRecord->leftToken = definedFrom->leftToken;
    defineRecord->rightNode = definedTo;
    
    for ( int i = 0 ; i < m_DefineBindingList.size() ; i++ ) {
      if ( defineRecord->leftToken->content == m_DefineBindingList[i]->leftToken->content ) {
        m_DefineBindingList.erase( m_DefineBindingList.begin() + i );
      } // if: find pre-defined , delete it
      
      if ( NOT m_DefineBindingList.empty() ) {
        if ( definedTo->leftToken ) {
          if ( definedTo->leftToken->content == m_DefineBindingList[i]->leftToken->content ) {
            defineRecord->rightNode = m_DefineBindingList[i]->rightNode;
          } // if: double define
        } // if: the defined as is a token, not a node
      } // if: define list is not empty
    } // for: find any pre-defined
    
    m_DefineBindingList.push_back( defineRecord );
    return result;
  } // Define()
  
  TreeStruct *Car( TreeStruct *functionNode ) {
    CheckArgumentNumber( functionNode, 1, false );
    TreeStruct *result = NewNode();
    TreeStruct *argument = ProcessSExp( functionNode->rightNode );
    
    if ( argument->leftToken ) {
      string errorMessage = "ERROR (car with incorrect argument type) : ";
      ErrorHandling( INCORRECT_ARGUMENT_TYPE, errorMessage, argument->leftToken, NULL );
      throw "incorrect argument type";
    } // if: incorrect argument type
    
    else {
      if ( argument->leftNode->leftToken ) {
        result->leftToken = argument->leftNode->leftToken;
      } // if: has leftToken
      
      else if ( argument->leftNode->leftNode ) {
        result->leftNode = argument->leftNode->leftNode;
      } // else if: has leftNode
      
      else {
        result->leftToken = NewToken();
        result->leftToken->tokenType = NIL;
        result->leftToken->content = "nil";
      } // else: nothing
    } // else: argument is a node
    
    return result;
  } // Car()
  
  TreeStruct *Cdr( TreeStruct *functionNode ) {
    CheckArgumentNumber( functionNode, 1, false );
    TreeStruct *result = NewNode();
    TreeStruct *argument = ProcessSExp( functionNode->rightNode );
    
    if ( argument->leftToken ) {
      string errorMessage = "ERROR (cdr with incorrect argument type) : ";
      ErrorHandling( INCORRECT_ARGUMENT_TYPE, errorMessage, argument->leftToken, NULL );
      throw "incorrect argument type";
    } // if: incorrect argument type
    
    else {
      if ( argument->leftNode->rightToken ) {
        result->leftToken = argument->leftNode->rightToken;
      } // if: has rightToken
      
      else if ( argument->leftNode->rightNode ) {
        result->leftNode = argument->leftNode->rightNode;
      } // else if: has rightNode
      
      else {
        result->leftToken = NewToken();
        result->leftToken->tokenType = NIL;
        result->leftToken->content = "nil";
      } // else: nothing
    } // else: argument is a node
    
    return result;
  } // Cdr()
  
  TreeStruct *IsAtom( TreeStruct *functionNode ) {
    CheckArgumentNumber( functionNode, 1, false );
    TreeStruct *result = NewNode();
    result->leftToken = NewToken();
    TreeStruct *argument = ProcessSExp( functionNode->rightNode );
    
    if ( argument->leftToken ) {
      result->leftToken->tokenType = T;
      result->leftToken->content = "#t";
    } // if: leftToken
    
    else {
      result->leftToken->tokenType = NIL;
      result->leftToken->content = "nil";
    } // else: leftNode
    
    return result;
  } // IsAtom()
  
  TreeStruct *IsPair( TreeStruct *functionNode ) {
    CheckArgumentNumber( functionNode, 1, false );
    TreeStruct *result = NewNode();
    result->leftToken = NewToken();
    TreeStruct *argument = ProcessSExp( functionNode->rightNode );
    
    if ( argument->leftToken ) {
      result->leftToken->tokenType = NIL;
      result->leftToken->content = "nil";
    } // if: token
    
    else {
      result->leftToken->tokenType = T;
      result->leftToken->content = "#t";
    } // else: node
    
    return result;
  } // IsPair()
  
  TreeStruct *IsList( TreeStruct *functionNode, bool checkingArgument ) {
    TreeStruct *result = NewNode();
    TreeStruct *walk = NewNode();
    result->leftToken = NewToken();
    
    if ( checkingArgument ) {
      CheckArgumentNumber( functionNode, 1, false );
      walk = ProcessSExp( functionNode->rightNode );
      
      if ( walk->leftToken ) {
        result->leftToken->content = "nil";
        result->leftToken->tokenType = NIL;
        return result;
      } // if: the argument is only a token
      
      else {
        walk = walk->leftNode;
      } // else: the argument is a node, keep checking
    } // if: checking argument
    
    else {
      walk = functionNode;
    } // else: checking the whole tree
    
    while ( walk->rightNode ) {
      walk = walk->rightNode;
    } // while: go to the rightmost node
    
    if ( walk->rightToken && walk->rightToken->tokenType != NIL ) {
      result->leftToken->content = "nil";
      result->leftToken->tokenType = NIL;
    } // if: has a rightToken, not list
    
    else {
      result->leftToken->content = "#t";
      result->leftToken->tokenType = T;
    } // else: has no rightToken, is a list
    
    return result;
  } // IsList()
  
  TreeStruct *IsNull( TreeStruct *functionNode ) {
    CheckArgumentNumber( functionNode, 1, false );
    TreeStruct *result = NewNode();
    result->leftToken = NewToken();
    TreeStruct *argument = ProcessSExp( functionNode->rightNode );
    
    if ( argument->leftToken && argument->leftToken->tokenType == NIL ) {
      result->leftToken->tokenType = T;
      result->leftToken->content = "#t";
    } // if: token and nil
    
    else {
      result->leftToken->tokenType = NIL;
      result->leftToken->content = "nil";
    } // else: not nil
    
    return result;
  } // IsNull()
  
  TreeStruct *IsInteger( TreeStruct *functionNode ) {
    CheckArgumentNumber( functionNode, 1, false );
    TreeStruct *result = NewNode();
    result->leftToken = NewToken();
    TreeStruct *argument = ProcessSExp( functionNode->rightNode );
    
    if ( argument->leftToken && argument->leftToken->tokenType == INT ) {
      result->leftToken->tokenType = T;
      result->leftToken->content = "#t";
    } // if: token and int
    
    else {
      result->leftToken->tokenType = NIL;
      result->leftToken->content = "nil";
    } // else: not int
    
    return result;
  } // IsInteger()
  
  TreeStruct *IsRealOrNumber( TreeStruct *functionNode ) {
    CheckArgumentNumber( functionNode, 1, false );
    TreeStruct *result = NewNode();
    result->leftToken = NewToken();
    TreeStruct *argument = ProcessSExp( functionNode->rightNode );
    
    if ( argument->leftToken &&
         ( argument->leftToken->tokenType == INT || argument->leftToken->tokenType == FLOAT ) ) {
      result->leftToken->tokenType = T;
      result->leftToken->content = "#t";
    } // if: token and int
    
    else {
      result->leftToken->tokenType = NIL;
      result->leftToken->content = "nil";
    } // else: not int
    
    return result;
  } // IsRealOrNumber()
  
  TreeStruct *IsString( TreeStruct *functionNode ) {
    CheckArgumentNumber( functionNode, 1, false );
    TreeStruct *result = NewNode();
    result->leftToken = NewToken();
    TreeStruct *argument = ProcessSExp( functionNode->rightNode );
    
    if ( argument->leftToken && argument->leftToken->tokenType == STRING ) {
      result->leftToken->tokenType = T;
      result->leftToken->content = "#t";
    } // if: token and int
    
    else {
      result->leftToken->tokenType = NIL;
      result->leftToken->content = "nil";
    } // else: not int
    
    return result;
  } // IsString()
  
  TreeStruct *IsBoolean( TreeStruct *functionNode ) {
    CheckArgumentNumber( functionNode, 1, false );
    TreeStruct *result = NewNode();
    result->leftToken = NewToken();
    TreeStruct *argument = ProcessSExp( functionNode->rightNode );
    
    if ( argument->leftToken &&
         ( argument->leftToken->tokenType == T || argument->leftToken->tokenType == NIL ) ) {
      result->leftToken->tokenType = T;
      result->leftToken->content = "#t";
    } // if: token and int
    
    else {
      result->leftToken->tokenType = NIL;
      result->leftToken->content = "nil";
    } // else: not int
    
    return result;
  } // IsBoolean()
  
  TreeStruct *IsSymbol( TreeStruct *functionNode ) {
    CheckArgumentNumber( functionNode, 1, false );
    TreeStruct *result = NewNode();
    result->leftToken = NewToken();
    TreeStruct *argument = ProcessSExp( functionNode->rightNode );
    
    if ( argument->leftToken && argument->leftToken->tokenType == SYMBOL ) {
      result->leftToken->tokenType = T;
      result->leftToken->content = "#t";
    } // if: token and int
    
    else {
      result->leftToken->tokenType = NIL;
      result->leftToken->content = "nil";
    } // else: not int
    
    return result;
  } // IsSymbol()
  
  TreeStruct *Addition( TreeStruct *functionNode ) {
    CheckArgumentNumber( functionNode, 2, true );
    TreeStruct *result = NewNode();
    result->leftToken = NewToken();
    result->leftToken->tokenType = INT;
    float answer = 0.0;
    TreeStruct *walk = functionNode->rightNode;
    
    while ( walk ) {
      TreeStruct *argument = ProcessSExp( walk );
      
      if ( ( argument->leftToken ) &&
           ( argument->leftToken->tokenType == FLOAT || argument->leftToken->tokenType == INT ) ) {
        if ( argument->leftToken->tokenType == FLOAT ) {
          result->leftToken->tokenType = FLOAT;
        } // if: has any float
        
        answer += atof( argument->leftToken->content.c_str() );
      } // if: do addition
      
      else {
        string errorMessage = "ERROR (+ with incorrect argument type) : ";
        
        if ( argument->leftToken ) {
          ErrorHandling( INCORRECT_ARGUMENT_TYPE, errorMessage, argument->leftToken, NULL );
        } // if: error is a token
        
        else {
          ErrorHandling( INCORRECT_ARGUMENT_TYPE, errorMessage, NULL, argument->leftNode );
        } // else: error is a node
        
        throw "incorrect argument type";
      } // else: error
      
      walk = walk->rightNode;
    } // while: go through the arguments
    
    stringstream toString;
    
    if ( result->leftToken->tokenType == INT ) {
      int intAnswer = ( int ) answer;
      toString << intAnswer;
      result->leftToken->content = toString.str();
    } // if: result is int
    
    else {
      toString << answer;
      result->leftToken->content = toString.str();
    } // else: result is float
    
    return result;
  } // Addition()
  
  TreeStruct *Subtraction( TreeStruct *functionNode ) {
    CheckArgumentNumber( functionNode, 2, true );
    TreeStruct *result = NewNode();
    result->leftToken = NewToken();
    result->leftToken->tokenType = INT;
    float answer = 0.0;
    TreeStruct *walk = functionNode->rightNode;
    bool isFirstNum = true;
    
    while ( walk ) {
      TreeStruct *argument = ProcessSExp( walk );
      
      if ( ( argument->leftToken ) &&
           ( argument->leftToken->tokenType == FLOAT || argument->leftToken->tokenType == INT ) ) {
        if ( argument->leftToken->tokenType == FLOAT ) {
          result->leftToken->tokenType = FLOAT;
        } // if: has any float
        
        if ( isFirstNum ) {
          answer = atof( argument->leftToken->content.c_str() );
          isFirstNum = false;
        } // if: isFirstNum
        
        else {
          answer -= atof( argument->leftToken->content.c_str() );
        } // else: not firstNum
      } // if: do subtraction
      
      else {
        string errorMessage = "ERROR (- with incorrect argument type) : ";
        
        if ( argument->leftToken ) {
          ErrorHandling( INCORRECT_ARGUMENT_TYPE, errorMessage, argument->leftToken, NULL );
        } // if: error is a token
        
        else {
          ErrorHandling( INCORRECT_ARGUMENT_TYPE, errorMessage, NULL, argument->leftNode );
        } // else: error is a node
        
        throw "incorrect argument type";
      } // else: error
      
      walk = walk->rightNode;
    } // while: go through the arguments
    
    stringstream toString;
    
    if ( result->leftToken->tokenType == INT ) {
      int intAnswer = ( int ) answer;
      toString << intAnswer;
      result->leftToken->content = toString.str();
    } // if: result is int
    
    else {
      toString << answer;
      result->leftToken->content = toString.str();
    } // else: result is float
    
    return result;
  } // Subtraction()
  
  TreeStruct *Mutiplication( TreeStruct *functionNode ) {
    CheckArgumentNumber( functionNode, 2, true );
    TreeStruct *result = NewNode();
    result->leftToken = NewToken();
    result->leftToken->tokenType = INT;
    float answer = 1;
    TreeStruct *walk = functionNode->rightNode;
    
    while ( walk ) {
      TreeStruct *argument = ProcessSExp( walk );
      
      if ( ( argument->leftToken ) &&
           ( argument->leftToken->tokenType == FLOAT || argument->leftToken->tokenType == INT ) ) {
        if ( argument->leftToken->tokenType == FLOAT ) {
          result->leftToken->tokenType = FLOAT;
        } // if: has any float
        
        answer *= atof( argument->leftToken->content.c_str() );
      } // if: do mutiplication
      
      else {
        string errorMessage = "ERROR (* with incorrect argument type) : ";
        
        if ( argument->leftToken ) {
          ErrorHandling( INCORRECT_ARGUMENT_TYPE, errorMessage, argument->leftToken, NULL );
        } // if: error is a token
        
        else {
          ErrorHandling( INCORRECT_ARGUMENT_TYPE, errorMessage, NULL, argument->leftNode );
        } // else: error is a node
        
        throw "incorrect argument type";
      } // else: error
      
      walk = walk->rightNode;
    } // while: go through the arguments
    
    stringstream toString;
    
    if ( result->leftToken->tokenType == INT ) {
      int intAnswer = ( int ) answer;
      toString << intAnswer;
      result->leftToken->content = toString.str();
    } // if: result is int
    
    else {
      toString << answer;
      result->leftToken->content = toString.str();
    } // else: result is float
    
    return result;
  } // Mutiplication()
  
  TreeStruct *Division( TreeStruct *functionNode ) {
    CheckArgumentNumber( functionNode, 2, true );
    TreeStruct *result = NewNode();
    result->leftToken = NewToken();
    result->leftToken->tokenType = INT;
    float answer = 0.0;
    TreeStruct *walk = functionNode->rightNode;
    bool isFirstNum = true;
    
    while ( walk ) {
      TreeStruct *argument = ProcessSExp( walk );
      
      if ( ( argument->leftToken ) &&
           ( argument->leftToken->tokenType == FLOAT || argument->leftToken->tokenType == INT ) ) {
        if ( argument->leftToken->tokenType == FLOAT ) {
          result->leftToken->tokenType = FLOAT;
        } // if: has any float
        
        if ( isFirstNum ) {
          answer = atof( argument->leftToken->content.c_str() );
          isFirstNum = false;
        } // if: isFirstNum
        
        else {
          if ( atof( argument->leftToken->content.c_str() ) == 0 ) {
            ErrorHandling( DIVISION_BY_ZERO, "ERROR (division by zero) : /", NULL, NULL );
            throw "division by zero";
          } // if: division by zero
          
          answer /= atof( argument->leftToken->content.c_str() );
        } // else: not firstNum
      } // if: do division
      
      else {
        string errorMessage = "ERROR (/ with incorrect argument type) : ";
        
        if ( argument->leftToken ) {
          ErrorHandling( INCORRECT_ARGUMENT_TYPE, errorMessage, argument->leftToken, NULL );
        } // if: error is a token
        
        else {
          ErrorHandling( INCORRECT_ARGUMENT_TYPE, errorMessage, NULL, argument->leftNode );
        } // else: error is a node
        
        throw "incorrect argument type";
      } // else: error
      
      walk = walk->rightNode;
    } // while: go through the arguments
    
    stringstream toString;
    
    if ( result->leftToken->tokenType == INT ) {
      int intAnswer = ( int ) answer;
      toString << intAnswer;
      result->leftToken->content = toString.str();
    } // if: result is int
    
    else {
      toString << answer;
      result->leftToken->content = toString.str();
    } // else: result is float
    
    return result;
  } // Division()
  
  TreeStruct *Not( TreeStruct *functionNode ) {
    return IsNull( functionNode );
  } // Not()
  
  TreeStruct *And( TreeStruct *functionNode ) {
    CheckArgumentNumber( functionNode, 2, true );
    TreeStruct *result = NewNode();
    TreeStruct *walk = functionNode->rightNode;
    
    while ( walk ) {
      TreeStruct *argument = ProcessSExp( walk );
      
      if ( argument->leftToken ) {
        if ( result->leftNode ) {
          result->leftNode = NULL;
        } // if: clear previous result
        
        result->leftToken = argument->leftToken;
        
        if ( argument->leftToken->tokenType == NIL ) {
          return result;
        } // if: nil
      } // if: argument is a token
      
      else {
        if ( result->leftToken ) {
          result->leftToken = NULL;
        } // if: clear previous result
        
        result->leftNode = argument->leftNode;
      } // else: argument is a node
      
      walk = walk->rightNode;
    } // while: go through the arguments
    
    return result;
  } // And()
  
  TreeStruct *Or( TreeStruct *functionNode ) {
    CheckArgumentNumber( functionNode, 2, true );
    TreeStruct *result = NewNode();
    TreeStruct *walk = functionNode->rightNode;
    
    while ( walk ) {
      TreeStruct *argument = ProcessSExp( walk );
      
      if ( argument->leftToken ) {
        if ( result->leftNode ) {
          result->leftNode = NULL;
        } // if: clear previous result
        
        result->leftToken = argument->leftToken;
        
        if ( argument->leftToken->tokenType != NIL ) {
          return result;
        } // if: not nil
      } // if: argument is a token
      
      else {
        if ( result->leftToken ) {
          result->leftToken = NULL;
        } // if: clear previous result
        
        result->leftNode = argument->leftNode;
      } // else: argument is a node
      
      walk = walk->rightNode;
    } // while: go through the arguments
    
    return result;
  } // Or()
  
  TreeStruct *GreaterThan( TreeStruct *functionNode ) {
    CheckArgumentNumber( functionNode, 2, true );
    TreeStruct *result = NewNode();
    result->leftToken = NewToken();
    result->leftToken->tokenType = T;
    result->leftToken->content = "#t";
    float previousNumber = 0.0;
    TreeStruct *walk = functionNode->rightNode;
    bool isFirstNum = true;
    
    while ( walk ) {
      TreeStruct *argument = ProcessSExp( walk );
      
      if ( ( !argument->isQuoteResult ) && ( argument->leftToken ) &&
           ( argument->leftToken->tokenType == FLOAT || argument->leftToken->tokenType == INT ) ) {
        if ( isFirstNum ) {
          previousNumber = atof( argument->leftToken->content.c_str() );
          isFirstNum = false;
        } // if: firstNum
        
        else {
          if ( previousNumber > atof( argument->leftToken->content.c_str() ) ) {
            previousNumber = atof( argument->leftToken->content.c_str() );
          } // if: if greater than
          
          else {
            result->leftToken->tokenType = NIL;
            result->leftToken->content = "nil";
            previousNumber = atof( argument->leftToken->content.c_str() );
          } // else: false
        } // else: not firstNum
      } // if: argument is token
      
      else {
        string errorMessage = "ERROR (> with incorrect argument type) : ";
        
        if ( argument->leftToken ) {
          ErrorHandling( INCORRECT_ARGUMENT_TYPE, errorMessage, argument->leftToken, NULL );
        } // if: error is a token
        
        else {
          ErrorHandling( INCORRECT_ARGUMENT_TYPE, errorMessage, NULL, argument->leftNode );
        } // else: error is a node
        
        throw "incorrect argument type";
      } // else if: argument is node
      
      walk = walk->rightNode;
    } // while: add up the arguments
    
    return result;
  } // GreaterThan()
  
  TreeStruct *GreaterThanOrEqualTo( TreeStruct *functionNode ) {
    CheckArgumentNumber( functionNode, 2, true );
    TreeStruct *result = NewNode();
    result->leftToken = NewToken();
    result->leftToken->tokenType = T;
    result->leftToken->content = "#t";
    float previousNumber = 0.0;
    TreeStruct *walk = functionNode->rightNode;
    bool isFirstNum = true;
    
    while ( walk ) {
      TreeStruct *argument = ProcessSExp( walk );
      
      if ( ( !argument->isQuoteResult ) && ( argument->leftToken ) &&
           ( argument->leftToken->tokenType == FLOAT || argument->leftToken->tokenType == INT ) ) {
        if ( isFirstNum ) {
          previousNumber = atof( argument->leftToken->content.c_str() );
          isFirstNum = false;
        } // if: firstNum
        
        else {
          if ( previousNumber >= atof( argument->leftToken->content.c_str() ) ) {
            previousNumber = atof( argument->leftToken->content.c_str() );
          } // if: if greater than
          
          else {
            result->leftToken->tokenType = NIL;
            result->leftToken->content = "nil";
            previousNumber = atof( argument->leftToken->content.c_str() );
          } // else: false
        } // else: not firstNum
      } // if: argument is token
      
      else {
        string errorMessage = "ERROR (>= with incorrect argument type) : ";
        
        if ( argument->leftToken ) {
          ErrorHandling( INCORRECT_ARGUMENT_TYPE, errorMessage, argument->leftToken, NULL );
        } // if: error is a token
        
        else {
          ErrorHandling( INCORRECT_ARGUMENT_TYPE, errorMessage, NULL, argument->leftNode );
        } // else: error is a node
        
        throw "incorrect argument type";
      } // else if: argument is node
      
      walk = walk->rightNode;
    } // while: add up the arguments
    
    return result;
  } // GreaterThanOrEqualTo()
  
  TreeStruct *LessThan( TreeStruct *functionNode ) {
    CheckArgumentNumber( functionNode, 2, true );
    TreeStruct *result = NewNode();
    result->leftToken = NewToken();
    result->leftToken->tokenType = T;
    result->leftToken->content = "#t";
    float previousNumber = 0.0;
    TreeStruct *walk = functionNode->rightNode;
    bool isFirstNum = true;
    
    while ( walk ) {
      TreeStruct *argument = ProcessSExp( walk );
      
      if ( ( !argument->isQuoteResult ) && ( argument->leftToken ) &&
           ( argument->leftToken->tokenType == FLOAT || argument->leftToken->tokenType == INT ) ) {
        if ( isFirstNum ) {
          previousNumber = atof( argument->leftToken->content.c_str() );
          isFirstNum = false;
        } // if: firstNum
        
        else {
          if ( previousNumber < atof( argument->leftToken->content.c_str() ) ) {
            previousNumber = atof( argument->leftToken->content.c_str() );
          } // if: if greater than
          
          else {
            result->leftToken->tokenType = NIL;
            result->leftToken->content = "nil";
            previousNumber = atof( argument->leftToken->content.c_str() );
          } // else: false
        } // else: not firstNum
      } // if: argument is token
      
      else {
        string errorMessage = "ERROR (< with incorrect argument type) : ";
        
        if ( argument->leftToken ) {
          ErrorHandling( INCORRECT_ARGUMENT_TYPE, errorMessage, argument->leftToken, NULL );
        } // if: error is a token
        
        else {
          ErrorHandling( INCORRECT_ARGUMENT_TYPE, errorMessage, NULL, argument->leftNode );
        } // else: error is a node
        
        throw "incorrect argument type";
      } // else if: argument is node
      
      walk = walk->rightNode;
    } // while: add up the arguments
    
    return result;
  } // LessThan()
  
  TreeStruct *LessThanOrEqualTo( TreeStruct *functionNode ) {
    CheckArgumentNumber( functionNode, 2, true );
    TreeStruct *result = NewNode();
    result->leftToken = NewToken();
    result->leftToken->tokenType = T;
    result->leftToken->content = "#t";
    float previousNumber = 0.0;
    TreeStruct *walk = functionNode->rightNode;
    bool isFirstNum = true;
    
    while ( walk ) {
      TreeStruct *argument = ProcessSExp( walk );
      
      if ( ( !argument->isQuoteResult ) && ( argument->leftToken ) &&
           ( argument->leftToken->tokenType == FLOAT || argument->leftToken->tokenType == INT ) ) {
        if ( isFirstNum ) {
          previousNumber = atof( argument->leftToken->content.c_str() );
          isFirstNum = false;
        } // if: firstNum
        
        else {
          if ( previousNumber <= atof( argument->leftToken->content.c_str() ) ) {
            previousNumber = atof( argument->leftToken->content.c_str() );
          } // if: if greater than
          
          else {
            result->leftToken->tokenType = NIL;
            result->leftToken->content = "nil";
            previousNumber = atof( argument->leftToken->content.c_str() );
          } // else: false
        } // else: not firstNum
      } // if: argument is token
      
      else {
        string errorMessage = "ERROR (<= with incorrect argument type) : ";
        
        if ( argument->leftToken ) {
          ErrorHandling( INCORRECT_ARGUMENT_TYPE, errorMessage, argument->leftToken, NULL );
        } // if: error is a token
        
        else {
          ErrorHandling( INCORRECT_ARGUMENT_TYPE, errorMessage, NULL, argument->leftNode );
        } // else: error is a node
        
        throw "incorrect argument type";
      } // else if: argument is node
      
      walk = walk->rightNode;
    } // while: add up the arguments
    
    return result;
  } // LessThanOrEqualTo()
  
  TreeStruct *EqualTo( TreeStruct *functionNode ) {
    CheckArgumentNumber( functionNode, 2, true );
    TreeStruct *result = NewNode();
    result->leftToken = NewToken();
    result->leftToken->tokenType = T;
    result->leftToken->content = "#t";
    float previousNumber = 0.0;
    TreeStruct *walk = functionNode->rightNode;
    bool isFirstNum = true;
    
    while ( walk ) {
      TreeStruct *argument = ProcessSExp( walk );
      
      if ( ( !argument->isQuoteResult ) && ( argument->leftToken ) &&
           ( argument->leftToken->tokenType == FLOAT || argument->leftToken->tokenType == INT ) ) {
        if ( isFirstNum ) {
          previousNumber = atof( argument->leftToken->content.c_str() );
          isFirstNum = false;
        } // if: firstNum
        
        else {
          if ( previousNumber == atof( argument->leftToken->content.c_str() ) ) {
            previousNumber = atof( argument->leftToken->content.c_str() );
          } // if: if greater than
          
          else {
            result->leftToken->tokenType = NIL;
            result->leftToken->content = "nil";
            previousNumber = atof( argument->leftToken->content.c_str() );
          } // else: false
        } // else: not firstNum
      } // if: argument is token
      
      else {
        string errorMessage = "ERROR (= with incorrect argument type) : ";
        
        if ( argument->leftToken ) {
          ErrorHandling( INCORRECT_ARGUMENT_TYPE, errorMessage, argument->leftToken, NULL );
        } // if: error is a token
        
        else {
          ErrorHandling( INCORRECT_ARGUMENT_TYPE, errorMessage, NULL, argument->leftNode );
        } // else: error is a node
        
        throw "incorrect argument type";
      } // else if: argument is node
      
      walk = walk->rightNode;
    } // while: add up the arguments
    
    return result;
  } // EqualTo()
  
  TreeStruct *StringAppend( TreeStruct *functionNode ) {
    CheckArgumentNumber( functionNode, 2, true );
    TreeStruct *result = NewNode();
    result->leftToken = NewToken();
    result->leftToken->tokenType = STRING;
    result->leftToken->content = "\0";
    TreeStruct *walk = functionNode->rightNode;
    bool isFirstString = true;
    
    while ( walk ) {
      TreeStruct *argument = ProcessSExp( walk );
      
      if ( ( argument->leftToken ) &&
           ( argument->leftToken->tokenType == STRING ) ) {
        if ( isFirstString ) {
          argument->leftToken->content.erase( argument->leftToken->content.end() - 1 );
          isFirstString = false;
        } // if: firstString
        
        else if ( walk->rightNode == NULL ) {
          argument->leftToken->content.erase( argument->leftToken->content.begin() );
        } // else if: lastString
        
        else {
          argument->leftToken->content.erase( argument->leftToken->content.begin() );
          argument->leftToken->content.erase( argument->leftToken->content.end() - 1 );
        } // else: not firstNum
        
        result->leftToken->content += argument->leftToken->content;
      } // if: argument is token
      
      else {
        string errorMessage = "ERROR (string-append with incorrect argument type) : ";
        
        if ( argument->leftToken ) {
          ErrorHandling( INCORRECT_ARGUMENT_TYPE, errorMessage, argument->leftToken, NULL );
        } // if: error is a token
        
        else {
          ErrorHandling( INCORRECT_ARGUMENT_TYPE, errorMessage, NULL, argument->leftNode );
        } // else: error is a node
        
        throw "incorrect argument type";
      } // else if: argument is node
      
      walk = walk->rightNode;
    } // while: add up the arguments
    
    return result;
  } // StringAppend()
  
  TreeStruct *StringGreaterThan( TreeStruct *functionNode ) {
    CheckArgumentNumber( functionNode, 2, true );
    TreeStruct *result = NewNode();
    result->leftToken = NewToken();
    result->leftToken->tokenType = T;
    result->leftToken->content = "#t";
    string previousString = "\0";
    TreeStruct *walk = functionNode->rightNode;
    bool isFirstString = true;
    
    while ( walk ) {
      TreeStruct *argument = ProcessSExp( walk );
      
      if ( ( argument->leftToken ) &&
           ( argument->leftToken->tokenType == STRING ) ) {
        if ( isFirstString ) {
          previousString = argument->leftToken->content;
          isFirstString = false;
        } // if: firstNum
        
        else {
          if ( previousString > argument->leftToken->content ) {
            previousString = argument->leftToken->content;
          } // if: if greater than
          
          else {
            result->leftToken->tokenType = NIL;
            result->leftToken->content = "nil";
            previousString = argument->leftToken->content;
          } // else: false
        } // else: not firstNum
      } // if: argument is token
      
      else {
        string errorMessage = "ERROR (string>? with incorrect argument type) : ";
        
        if ( argument->leftToken ) {
          ErrorHandling( INCORRECT_ARGUMENT_TYPE, errorMessage, argument->leftToken, NULL );
        } // if: error is a token
        
        else {
          ErrorHandling( INCORRECT_ARGUMENT_TYPE, errorMessage, NULL, argument->leftNode );
        } // else: error is a node
        
        throw "incorrect argument type";
      } // else if: argument is node
      
      walk = walk->rightNode;
    } // while: add up the arguments
    
    return result;
  } // StringGreaterThan()
  
  TreeStruct *StringLessThan( TreeStruct *functionNode ) {
    CheckArgumentNumber( functionNode, 2, true );
    TreeStruct *result = NewNode();
    result->leftToken = NewToken();
    result->leftToken->tokenType = T;
    result->leftToken->content = "#t";
    string previousString = "\0";
    TreeStruct *walk = functionNode->rightNode;
    bool isFirstString = true;
    
    while ( walk ) {
      TreeStruct *argument = ProcessSExp( walk );
      
      if ( ( argument->leftToken ) &&
           ( argument->leftToken->tokenType == STRING ) ) {
        if ( isFirstString ) {
          previousString = argument->leftToken->content;
          isFirstString = false;
        } // if: firstNum
        
        else {
          if ( previousString < argument->leftToken->content ) {
            previousString = argument->leftToken->content;
          } // if: if greater than
          
          else {
            result->leftToken->tokenType = NIL;
            result->leftToken->content = "nil";
            previousString = argument->leftToken->content;
          } // else: false
        } // else: not firstNum
      } // if: argument is token
      
      else {
        string errorMessage = "ERROR (string<? with incorrect argument type) : ";
        
        if ( argument->leftToken ) {
          ErrorHandling( INCORRECT_ARGUMENT_TYPE, errorMessage, argument->leftToken, NULL );
        } // if: error is a token
        
        else {
          ErrorHandling( INCORRECT_ARGUMENT_TYPE, errorMessage, NULL, argument->leftNode );
        } // else: error is a node
        
        throw "incorrect argument type";
      } // else if: argument is node
      
      walk = walk->rightNode;
    } // while: add up the arguments
    
    return result;
  } // StringLessThan()
  
  TreeStruct *StringEqualTo( TreeStruct *functionNode ) {
    CheckArgumentNumber( functionNode, 2, true );
    TreeStruct *result = NewNode();
    result->leftToken = NewToken();
    result->leftToken->tokenType = T;
    result->leftToken->content = "#t";
    string previousString = "\0";
    TreeStruct *walk = functionNode->rightNode;
    bool isFirstString = true;
    
    while ( walk ) {
      TreeStruct *argument = ProcessSExp( walk );
      
      if ( ( argument->leftToken ) &&
           ( argument->leftToken->tokenType == STRING ) ) {
        if ( isFirstString ) {
          previousString = argument->leftToken->content;
          isFirstString = false;
        } // if: firstNum
        
        else {
          if ( previousString == argument->leftToken->content ) {
            previousString = argument->leftToken->content;
          } // if: if greater than
          
          else {
            result->leftToken->tokenType = NIL;
            result->leftToken->content = "nil";
            previousString = argument->leftToken->content;
          } // else: false
        } // else: not firstNum
      } // if: argument is token
      
      else {
        string errorMessage = "ERROR (string=? with incorrect argument type) : ";
        
        if ( argument->leftToken ) {
          ErrorHandling( INCORRECT_ARGUMENT_TYPE, errorMessage, argument->leftToken, NULL );
        } // if: error is a token
        
        else {
          ErrorHandling( INCORRECT_ARGUMENT_TYPE, errorMessage, NULL, argument->leftNode );
        } // else: error is a node
        
        throw "incorrect argument type";
      } // else if: argument is node
      
      walk = walk->rightNode;
    } // while: add up the arguments
    
    return result;
  } // StringEqualTo()
  
  TreeStruct *Eqv( TreeStruct *functionNode ) {
    CheckArgumentNumber( functionNode, 2, false );
    TreeStruct *result = NewNode();
    result->leftToken = NewToken();
    TreeStruct *firstArgument = ProcessSExp( functionNode->rightNode );
    TreeStruct *secondArgument = ProcessSExp( functionNode->rightNode->rightNode );
    
    if ( firstArgument->leftToken ) {
      if ( secondArgument->leftToken ) {
        if ( ( firstArgument->isQuoteResult || secondArgument->isQuoteResult ) ) {
          result->leftToken->tokenType = NIL;
        } // if: any of one is a quote result
        
        else if ( firstArgument->leftToken->tokenType == STRING ||
                  secondArgument->leftToken->tokenType == STRING ) {
          result->leftToken->tokenType = NIL;
        } // if: quote result
        
        else {
          result->leftToken->tokenType = T;
        } // else: both atom
      } // if: second argument is also a token
      
      else {
        result->leftToken->tokenType = NIL;
      } // else: second argument is a node
    } // if: first argument is a token
    
    else {
      if ( secondArgument->leftToken ) {
        result->leftToken->tokenType = NIL;
      } // if: second argument is a token
      
      else {
        if ( firstArgument->leftNode == secondArgument->leftNode ) {
          result->leftToken->tokenType = T;
        } // if: node address is the same
        
        else {
          result->leftToken->tokenType = NIL;
        } // else: node address not the same
      } // else: second argument is also a node
    } // else: first argument is a node
    
    if ( result->leftToken->tokenType == T ) {
      result->leftToken->content = "#t";
    } // if: the result is true, set the content
    
    else {
      result->leftToken->content = "nil";
    } // else: the result is nil, set the content
    
    return result;
  } // Eqv()
  
  TreeStruct *Equal( TreeStruct *functionNode ) {
    CheckArgumentNumber( functionNode, 2, false );
    TreeStruct *result = NewNode();
    result->leftToken = NewToken();
    TreeStruct *firstArgument = ProcessSExp( functionNode->rightNode );
    TreeStruct *secondArgument = ProcessSExp( functionNode->rightNode->rightNode );
    
    if ( firstArgument->leftToken ) {
      if ( secondArgument->leftToken ) {
        if ( firstArgument->isQuoteResult || secondArgument->isQuoteResult ) {
          if ( ( firstArgument->isQuoteResult && secondArgument->isQuoteResult ) &&
               ( firstArgument->leftToken->content == secondArgument->leftToken->content ) ) {
            result->leftToken->tokenType = T;
          } // if: both quote result, and content is the same
          
          result->leftToken->tokenType = NIL;
        } // if: any of one is a quote result
        
        else if ( firstArgument->leftToken->content == secondArgument->leftToken->content ) {
          result->leftToken->tokenType = T;
        } // if: quote result
        
        else {
          result->leftToken->tokenType = NIL;
        } // else: both atom
      } // if: second argument is also a token
      
      else {
        result->leftToken->tokenType = NIL;
      } // else: second argument is a node
    } // if: first argument is a token
    
    else {
      if ( secondArgument->leftToken ) {
        result->leftToken->tokenType = NIL;
      } // if: second argument is a token
      
      else {
        if ( CompareTwoTrees( firstArgument->leftNode, secondArgument->leftNode ) ) {
          result->leftToken->tokenType = T;
        } // if: node address is the same
        
        else {
          result->leftToken->tokenType = NIL;
        } // else: node address not the same
      } // else: second argument is also a node
    } // else: first argument is a node
    
    if ( result->leftToken->tokenType == T ) {
      result->leftToken->content = "#t";
    } // if: the result is true, set the content
    
    else {
      result->leftToken->content = "nil";
    } // else: the result is nil, set the content
    
    return result;
  } // Equal()
  
  TreeStruct *Begin( TreeStruct *functionNode ) {
    CheckArgumentNumber( functionNode, 1, true );
    TreeStruct *result = NewNode();
    TreeStruct *walk = functionNode->rightNode;
    TreeStruct *argument = NewNode();
    
    while ( walk ) {
      argument = ProcessSExp( walk );
      walk = walk->rightNode;
    } // while: go to the last argument
    
    if ( argument->leftToken ) {
      result->leftToken = argument->leftToken;
    } // if: the lastArgument is a token
    
    else {
      result->leftNode = argument->leftNode;
    } // else: the lastArgument is a node
    
    return result;
  } // Begin()
  
  TreeStruct *If( TreeStruct *functionNode ) { // TODO: need to backup condition
    CheckArgumentNumber( functionNode, 2, false );
    TreeStruct *condition = ProcessSExp( functionNode->rightNode );
    TreeStruct *result = NewNode();
    
    if ( ( condition->leftToken && condition->leftToken->tokenType != NIL ) || condition->leftNode ) {
      TreeStruct *trueResult = ProcessSExp( functionNode->rightNode->rightNode );
      
      if ( trueResult->leftToken ) {
        result->leftToken = trueResult->leftToken;
      } // if: result is a token
      
      else {
        result->leftNode = trueResult->leftNode;
      } // else: result is a node
    } // if: condition as true
    
    else {
      if ( functionNode->rightNode->rightNode->rightNode == NULL ) {
        ErrorHandling( NO_RETURN_VALUE, "ERROR (no return value) : ", NULL, functionNode );
        throw "no return value";
      } // if: no return value
      
      else {
        TreeStruct *falseResult = ProcessSExp( functionNode->rightNode->rightNode->rightNode );
        
        if ( falseResult->leftToken ) {
          result->leftToken = falseResult->leftToken;
        } // if: result is a token
        
        else {
          result->leftNode = falseResult->leftNode;
        } // else: result is a node
      } // else: has a return value
    } // else: condition as false
    
    return result;
  } // If()
  
  TreeStruct *Cond( TreeStruct *functionNode ) {
    CheckArgumentNumber( functionNode, 1, true );
    TreeStruct *conditionWalk = NewNode();
    TreeStruct *result = NewNode();
    conditionWalk = functionNode->rightNode;
    
    while ( conditionWalk ) {
      if ( conditionWalk->leftNode == NULL || conditionWalk->leftNode->rightNode == NULL ||
           ( IsList( conditionWalk->leftNode, false )->leftToken->tokenType == NIL ) ) {
        ErrorHandling( COND_FORMAT, "ERROR (COND format) : ", NULL, m_project1.GetRoot()->leftNode );
        throw "cond format";
      } // if: condition is not in left-paren
      
      conditionWalk = conditionWalk->rightNode;
    } // while: check cond format
    
    conditionWalk = functionNode->rightNode;
    
    while ( conditionWalk ) {
      if ( conditionWalk->rightNode == NULL &&
           ( conditionWalk->leftNode && conditionWalk->leftNode->leftToken &&
             conditionWalk->leftNode->leftToken->content == "else" ) ) {
        TreeStruct *elseResultWalk = conditionWalk->leftNode->rightNode;
        TreeStruct *elseResult = NewNode();
        
        if ( elseResultWalk == NULL ) {
          ErrorHandling( COND_FORMAT, "ERROR (COND format) : ", NULL, m_project1.GetRoot()->leftNode );
          throw "cond format";
        } // if: condition is not in left-paren
        
        elseResultWalk = conditionWalk->leftNode->rightNode;
        
        while ( elseResultWalk ) {
          elseResult = ProcessSExp( elseResultWalk );
          elseResultWalk = elseResultWalk->rightNode;
        } // while: go to the last else result
        
        if ( elseResult->leftToken ) {
          result->leftToken = elseResult->leftToken;
        } // if: the result is a token
        
        else {
          result->leftNode = elseResult->leftNode;
        } // else the result is a node
        
        return result;
      } // if: else case
      
      else {
        TreeStruct *condition = NewNode();
        condition = ProcessSExp( conditionWalk->leftNode );
        TreeStruct *conditionResultWalk = conditionWalk->leftNode->rightNode;
        TreeStruct *conditionResult = NewNode();
        
        if ( conditionResultWalk == NULL ) {
          ErrorHandling( COND_FORMAT, "ERROR (COND format) : ", NULL, m_project1.GetRoot()->leftNode );
          throw "cond format";
        } // if: no true result
        
        if ( ( condition->leftToken && condition->leftToken->tokenType != NIL ) || condition->leftNode ) {
          while ( conditionResultWalk ) {
            conditionResult = ProcessSExp( conditionResultWalk );
            conditionResultWalk = conditionResultWalk->rightNode;
          } // while: go to the last condition result
          
          if ( conditionResult->leftToken ) {
            result->leftToken = conditionResult->leftToken;
          } // if: the result is a token
          
          else {
            result->leftNode = conditionResult->leftNode;
          } // else: the result is a node
          
          return result;
        } // if: condition is true, go through the results
        
        conditionWalk = conditionWalk->rightNode;
      } // else: not else case
    } // while: go through the conditions
    
    ErrorHandling( NO_RETURN_VALUE, "ERROR (no return value) : ", NULL, m_project1.GetRoot()->leftNode );
    throw "no return value";
  } // Cond()
  
  TreeStruct *CleanEnvironment( TreeStruct *functionNode ) {
    if ( functionNode->previousNode->previousNode ) {
      
      ErrorHandling( LEVEL_OF_ERROR, "ERROR (level of CLEAN-ENVIRONMENT)", NULL, NULL );
      throw "level of define";
    } // if: level of define
    
    CheckArgumentNumber( functionNode, 0, false );
    m_DefineBindingList.clear();
    TreeStruct *result = NewNode();
    result->leftToken = NewToken();
    result->leftToken->tokenType = STRING;
    result->leftToken->content = "environment cleaned";
    return result;
  } // CleanEnvironment()
  
  TreeStruct *Exit( TreeStruct *functionNode ) {
    if ( functionNode->previousNode->previousNode ) {
      
      ErrorHandling( LEVEL_OF_ERROR, "ERROR (level of EXIT)", NULL, NULL );
      throw "level of define";
    } // if: level of define
    
    CheckArgumentNumber( functionNode, 0, false );
    return NULL;
  } // Exit()
  
  void PrintResult() {
    if ( m_Root->leftToken == NULL && m_Root->leftNode == NULL ) {
      return;
    } // if: no results
    
    else if ( m_Root->leftToken && !m_Root->rightToken && !m_Root->rightNode ) {
      if ( m_Root->leftToken->tokenType == STRING ) {
        PrintString( m_Root->leftToken->content );
      } // if: string form result
      
      else if ( m_Root->leftToken->tokenType == SYMBOL ) {
        if ( IsPrimitive( m_Root->leftToken ) ) {
          cout << "#<procedure " << m_Root->leftToken->content << ">" << endl;
        } // if: is primitive
        
        else {
          cout << m_Root->leftToken->content << endl;
        } // else: not primitive
      } // else if: symbol
      
      else if ( m_Root->leftToken->tokenType == T ) {
        cout << "#t" << endl;
      } // if: boolean result is true, #t
      
      else if ( m_Root->leftToken->tokenType == NIL ) {
        cout << "nil" << endl;
      } // else if: boolean result is false, nil
      
      else if ( m_Root->leftToken->tokenType == FLOAT ) {
        cout << fixed << setprecision( 3 )
             << round( atof( m_Root->leftToken->content.c_str() )*1000 )/1000
             << endl;
      } // else if: float form result
      
      else if ( m_Root->leftToken->tokenType == INT ) {
        cout << atoi( m_Root->leftToken->content.c_str() ) << endl;
      } // else if: int form result
    } // if: the result is a token
    
    else {
      int layer = 0;
      bool isRightNode = false;
      PrintTree( m_Root->leftNode, layer, isRightNode, true );
      
      if ( layer > 0 ) {
        while ( layer > 0 ) {
          layer--;
          cout << string( layer, ' ' ) << ')' << endl;
        } // while: loop print right-paren
      } // if: layer still greater than zero
    } // else if: tree structre form result
  } // PrintResult()
  
  void PrintTree( TreeStruct *currentRoot, int &layer, bool isRightNode, bool printProcedure ) {
    static bool hasLineReturn = false;
    
    if ( isRightNode == false ) {
      if ( hasLineReturn ) {
        cout << string( layer*2, ' ' );
      } // if: has lineReturn
      
      cout << "( ";
      hasLineReturn = false;
      layer++;
    } // if: currentNode is the leftNode of the previousNode
    
    if ( currentRoot->leftNode ) {
      isRightNode = false;
      PrintTree( currentRoot->leftNode, layer, isRightNode, printProcedure );
    } // if: has leftNode
    
    if ( currentRoot->leftToken ) {
      if ( hasLineReturn ) {
        cout << string( layer*2, ' ' );
      } // if: has line return
      
      PrintData( currentRoot->leftToken, printProcedure );
      isRightNode = false;
      hasLineReturn = true;
      
      if ( currentRoot->rightToken && currentRoot->rightToken->tokenType != NIL ) {
        cout << string( layer*2, ' ' ) << '.' << endl << string( layer*2, ' ' );
        PrintData( currentRoot->rightToken, printProcedure );
      } // if: has data in rightToken
    } // if: has data in leftToken
    
    else if ( currentRoot->rightToken ) {
      cout << string( layer*2, ' ' ) << '.' << endl << string( layer*2, ' ' );
      PrintData( currentRoot->rightToken, printProcedure );
    } // else if: has rightToken
    
    if ( currentRoot->rightNode ) {
      isRightNode = true;
      PrintTree( currentRoot->rightNode, layer, isRightNode, printProcedure );
    } // if: has isRightNode
    
    if ( layer > 1 && ( ( currentRoot->rightNode == NULL ) ||
                        ( ( currentRoot->leftToken ) && ( currentRoot->rightToken ) ) ) ) {
      layer--;
      cout << string( layer*2, ' ' ) << ')' << endl;
    } // if: still space print right paren
  } // PrintTree()
  
  void PrintData( TokenStruct *currentToken, bool printProcedure ) {
    if ( currentToken->tokenType == INT ) {
      cout << atoi( currentToken->content.c_str() ) << endl;
    } // if: int case
    
    else if ( currentToken->tokenType == FLOAT ) {
      cout << fixed << setprecision( 3 )
           << round( atof( currentToken->content.c_str() )*1000 )/1000
           << endl;
    } // else if: float case with precision and round
    
    else if ( currentToken->tokenType == NIL ) {
      cout << "nil" << endl;
    } // else if: nil
    
    else if ( currentToken->tokenType == T ) {
      cout << "#t" << endl;
    } // else if: #t
    
    else if ( currentToken->tokenType == STRING ) {
      PrintString( currentToken->content );
    } // else if: string
    
    else if ( currentToken->tokenType == SYMBOL ) {
      if ( IsPrimitive( currentToken ) && printProcedure && !currentToken->isQuoteResult ) {
        cout << "#<procedure " << currentToken->content << ">" << endl;
      } // if: is primitive
      
      else {
        cout << currentToken->content << endl;
      } // else: not primitive
    } // else if: symbol
    
    else if ( currentToken->content == "quote" ) {
      cout << currentToken->content << endl;
    } // else if: quote
  } // PrintData()
  
  void PrintString( string stringContent ) {
    for ( int index = 0 ; index < stringContent.length() ; index++ ) {
      if ( stringContent[index] == '\\' ) {
        if ( stringContent[index + 1] == 'n' ) {
          cout << endl;
          index++;
        } // if: '\n'
        
        else if ( stringContent[index + 1] == 't' ) {
          cout << '\t';
          index++;
        } // else if: '\t'
        
        else if ( stringContent[index + 1] == '"' ) {
          cout << '"';
          index++;
        } // else if: '"'
        
        else if ( stringContent[index + 1] == '\\' ) {
          cout << '\\';
          index++;
        } // else if: '\\'
        
        else {
          cout << stringContent[index];
        } // else: normal '\\'
      } // if: escape
      
      else {
        cout << stringContent[index];
      } // else: normal character
    } // for: go through the string
    
    cout << endl;
  } // PrintString()
}; // Project2Class

int main() {
  cin >> g_uTestNum;
  bool end = false;
  cout << "Welcome to OurScheme!" << endl << endl;
  
  if ( g_uTestNum == 5 )
    cout << "hehe";
  
  else {
    Project1Class project1;
    Project2Class project2;
    
    do {
      cout << "> ";
      
      if ( project1.ReadSExp() == true ) {
        if ( project1.CheckExit() == true ) {
          end = true;
        } // if: check exit
        
        else {
          try {
            project2.EvalSExp( project1 );
            project2.PrintResult();
          } // try: evaluate
          
          catch ( const char *errorType ) {
          } // catch: error
          
          project1.ClearTheLine();
        } // else: not exit case
      } // if: valid syntax
      
      else {
        project1.ErrorHandling();
        g_CursorLine = 1;
        g_CursorColumn = 0;
        
        if ( project1.CheckExit() == true ) {
          end = true;
        } // if: check exit
      } // else: syntax error
      
      cout << endl;
    } while ( NOT end );
  } // else
  
  cout << "Thanks for using OurScheme!" << endl << endl;
} // main(): main function
