// main.cpp
// PL_Project

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
  DIVISION_BY_ZERO, NON_LIST, DEFINE_FORMAT
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
  bool leftParenCreate;
  bool quoteResult;
}; // TreeStruct

struct ErrorStruct {
  ErrorType errorType;
  string errorMessage;
}; // ErrorStruct

struct ResultStruct {
  TokenStruct *tokenResult;
  bool hasTokenResult;
  TreeStruct *nodeResult;
  bool hasNodeResult;
}; // ResultStruct

// define global variable to track the cursor
int g_CursorLine = 1;
int g_CursorColumn = 0;
int g_uTestNum = 0;

class Project1Class {
private:
  vector<TokenStruct> m_LineOfTokens;
  TreeStruct *m_Root;
  TreeStruct *m_CurrentTreeLocation;
  ErrorStruct m_Error;

public:
  /*
    ------------------ Class -------------------
    ------------------ Getter ------------------
  */
  
  vector<TokenStruct> GetSExp() {
    return m_LineOfTokens;
  } // GetSExp()
  
  TreeStruct *GetRoot() {
    return m_Root;
  } // GetRoot()
  
  /*
    ------------------ Check -------------------
    ------------------- exit -------------------
  */
  
  bool CheckExit() {
    string exit = "\0";
    int tokenCount = 0;
    
    for ( int i = 0 ; i < m_LineOfTokens.size() ; i++ ) {
      if ( IsAtom( m_LineOfTokens[i] ) ||
           m_LineOfTokens[i].tokenType == LEFT_PAREN ||
           m_LineOfTokens[i].tokenType == RIGHT_PAREN ) {
        if ( m_LineOfTokens[i].tokenType == NIL ) {
          if ( i > 1 ) {
            if ( m_LineOfTokens[i - 1].tokenType != DOT && m_LineOfTokens[i + 1].tokenType != DOT ) {
              exit += m_LineOfTokens[i].content;
              tokenCount++;
            } // if: nil with no dot
          } // if: i > 1
        } // if: nil
        
        else {
          exit += m_LineOfTokens[i].content;
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
  
  /*
    -------------------- Is --------------------
    ----------------- function -----------------
  */
  
  bool IsFloat( TokenStruct newToken ) {
    for ( int i = 0 ; i < newToken.content.length() ; i++ ) {
      if ( newToken.content[i] == '.' ) {
        return true;
      } // if: if there's a dot in the inputTerm
    } // for: go through the inputTerm
    
    return false;
  } // IsFloat()
  
  bool IsAtom( TokenStruct currentToken ) {
    if ( currentToken.tokenType == SYMBOL ||
         currentToken.tokenType == INT ||
         currentToken.tokenType == FLOAT ||
         currentToken.tokenType == STRING ||
         currentToken.tokenType == NIL ||
         currentToken.tokenType == T ) {
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
  
  /*
    ------------------- Token ------------------
    ---------------- Processing ----------------
  */
  
  bool ReadSExp() {
    m_LineOfTokens.clear();
    m_Root = NULL;
    m_Error.errorType = NOT_S_EXP;
    m_Error.errorMessage = "\0";
    
    if ( HasNextToken() == true ) {
      if ( CheckSExp() == false ) {
        return false;
      } // if: handle the error
      
      else {
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
    
    TokenStruct newToken;
    newToken.content.push_back( cin.get() );
    g_CursorColumn++;
    // initialize new token
    
    if ( peekChar == '(' ) {
      if ( PeekCharAndGetRidOfComment() == ')' ) {
        newToken.content.push_back( cin.get() );
        g_CursorColumn++;
        newToken.tokenType = NIL;
      } // if: () case
      
      else {
        newToken.tokenType = LEFT_PAREN;
      } // else: left parenthesis
    } // if: left parenthesis
    
    else if ( peekChar == ')' ) {
      newToken.tokenType = RIGHT_PAREN;
    } // else if: right parenthesis
    
    else if ( peekChar == '"' ) {
      newToken.content.append( GetString() );
      
      if ( newToken.content == "\"" ) {
        return false;
      } // if: no closing quote error
      
      newToken.tokenType = STRING;
    } // else if: string
    
    else if ( peekChar == '\'' ) {
      newToken.tokenType = QUOTE;
    } // else if: quote
    
    else {
      while ( cin.peek() != '\n' && cin.peek() != ' ' &&
              cin.peek() != '\t' && cin.peek() != '\'' &&
              cin.peek() != '"' && cin.peek() != '(' &&
              cin.peek() != ')' && cin.peek() != ';' &&
              cin.peek() != '\r' && cin.peek() != '\0' &&
              cin.peek() != EOF ) {
        newToken.content.push_back( cin.get() );
        g_CursorColumn++;
      } // while: get the rest of the token
      
      newToken.tokenType = CheckTokenType( newToken );
    } // else: other types
    
    m_LineOfTokens.push_back( newToken ); // push the newToken to the vector
    return true;
  } // HasNextToken()
  
  TokenType CheckTokenType( TokenStruct newToken ) {
    bool isNumber = false;
    bool isSymbol = false;
    int plusSignBitCount = 0;
    int minusSignBitCount = 0;
    int decimalPointCount = 0;
    
    for ( int i = 0 ; i < newToken.content.length() ; i++ ) {
      if ( newToken.content[i] == '+' ) {
        plusSignBitCount++;
      } // if: check the number of plus sign bit
      
      else if ( newToken.content[i] == '-' ) {
        minusSignBitCount++;
      } // else if: check the number of minus sign bit
      
      else if ( newToken.content[i] == '.' ) {
        decimalPointCount++;
      } // else if: check the number of decimal point count
      
      else if ( isdigit( newToken.content[i] ) ) {
        isNumber = true;
      } // else if: there are digits in the token
    } // for: go through the token
    
    if ( ( plusSignBitCount > 0 && minusSignBitCount > 0 ) ||
         plusSignBitCount > 1 || minusSignBitCount > 1 ||
         decimalPointCount > 1 ) {
      isNumber = false;
    } // if: more than one sign bit
    
    for ( int i = 0 ; i < newToken.content.length() ; i++ ) {
      if ( isupper( newToken.content[i] ) || islower( newToken.content[i] ) ) {
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
      if ( newToken.content == "#f" || newToken.content == "nil" || newToken.content == "()" ) {
        return NIL;
      } // if: nil
      
      else if ( newToken.content == "t" || newToken.content == "#t" ) {
        return T;
      } // else if: #t
      
      else if ( newToken.content == "." ) {
        return DOT;
      } // else if: dot
      
      else if ( newToken.content == "quote" ) {
        return QUOTE;
      } // else if: quote
      
      else {
        return SYMBOL;
      } // else: symbols
    } // else: check t, nil or dot
  } // CheckTokenType()
  
  /*
    ------------------- Check ------------------
    ------------------- Syntax -----------------
  */
  
  bool CheckSExp() {
    if ( IsAtom( m_LineOfTokens.back() ) ) {
      if ( m_Root != NULL ) {
        if ( m_CurrentTreeLocation->leftToken != NULL ) {
          FindValidNodePosition();
          
          if ( m_LineOfTokens.at( m_LineOfTokens.size() - 2 ).tokenType != DOT ) {
            RightCreateNode();
            LeftInsertToken();
          } // if: without dot case
          
          else {
            RightInsertToken();
          } // else: with case
        } // if: left is empty, insert left
        
        else {
          if ( m_CurrentTreeLocation->leftNode != NULL ) {
            FindValidNodePosition();
            
            if ( m_LineOfTokens.at( m_LineOfTokens.size() - 2 ).tokenType != DOT ) {
              RightCreateNode();
              LeftInsertToken();
            } // if: without dot case
            
            else {
              RightInsertToken();
            } // else: with dot case
          } // if : left node is not NULL
          
          else {
            LeftInsertToken();
          } // else: insert at left
        } // else: insert at the right
      } // if: check if there's any tree
      
      return true;
    } // if: <ATOM>
    
    else if ( m_LineOfTokens.back().tokenType == LEFT_PAREN ) {
      // initialize or create node
      if ( m_Root == NULL ) {
        InitializeRoot();
        m_CurrentTreeLocation->leftParenCreate = true;
      } // if: initialize the root pointer
      
      else {
        if ( m_CurrentTreeLocation->leftToken != NULL ) {
          FindValidNodePosition();
          
          if ( m_CurrentTreeLocation->previousNode && m_CurrentTreeLocation->previousNode->leftToken ) {
            if ( IsPrimitive( m_CurrentTreeLocation->previousNode->leftToken ) &&
                 m_CurrentTreeLocation->previousNode->previousNode != NULL ) {
              m_CurrentTreeLocation = m_CurrentTreeLocation->previousNode;
              FindValidNodePosition();
            } // if: is function
          } // if: previousNode && previousNode->leftToken
          
          RightCreateNode();
          
          if ( m_LineOfTokens.at( m_LineOfTokens.size() - 2 ).tokenType != DOT ) {
            LeftCreateNode();
          } // if: with out dot
        } // if: right create case
        
        else {
          if ( m_CurrentTreeLocation->leftNode != NULL ) {
            FindValidNodePosition();
            
            if ( m_CurrentTreeLocation->previousNode && m_CurrentTreeLocation->previousNode->leftToken ) {
              if ( IsPrimitive( m_CurrentTreeLocation->previousNode->leftToken ) &&
                   m_CurrentTreeLocation->previousNode->previousNode != NULL ) {
                m_CurrentTreeLocation = m_CurrentTreeLocation->previousNode;
                FindValidNodePosition();
              } // if: is function
            } // if: previousNode && previousNode->leftToken
            
            RightCreateNode();
            
            if ( m_LineOfTokens.at( m_LineOfTokens.size() - 2 ).tokenType != DOT ) {
              LeftCreateNode();
            } // if: with out dot
          } // if: insert right
          
          else {
            FindValidNodePosition();
            
            if ( m_CurrentTreeLocation->previousNode && m_CurrentTreeLocation->previousNode->leftToken ) {
              if ( IsPrimitive( m_CurrentTreeLocation->previousNode->leftToken ) &&
                   m_CurrentTreeLocation->previousNode->previousNode != NULL ) {
                m_CurrentTreeLocation = m_CurrentTreeLocation->previousNode;
                FindValidNodePosition();
              } // if: is function
            } // if: previousNode && previousNode->leftToken
            
            LeftCreateNode();
          } // else: insert right
        } // else: right insert node
        
        m_CurrentTreeLocation->leftParenCreate = true;
      } // else: create a node
      
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
        if ( m_LineOfTokens.back().tokenType == DOT ) {
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
        if ( m_LineOfTokens.back().tokenType == RIGHT_PAREN ) {
          m_CurrentTreeLocation = m_CurrentTreeLocation->previousNode;
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
    
    else if ( m_LineOfTokens.back().tokenType == QUOTE ) {
      bool isQuoteWordCase = false;
      
      if ( m_LineOfTokens.back().content == "'" ) {
        m_LineOfTokens.pop_back();
        TokenStruct quoteLeftParen;
        quoteLeftParen.tokenType = LEFT_PAREN;
        quoteLeftParen.content = "(";
        TokenStruct quoteString;
        quoteString.tokenType = QUOTE;
        quoteString.content = "quote";
        m_LineOfTokens.push_back( quoteLeftParen );
        m_LineOfTokens.push_back( quoteString );
        isQuoteWordCase = true;
        
        if ( m_Root == NULL ) {
          InitializeRoot();
          m_CurrentTreeLocation->leftParenCreate = true;
          LeftInsertToken();
        } // if: initialize the root pointer
        
        else {
          if ( m_CurrentTreeLocation->leftToken != NULL ) {
            FindValidNodePosition();
            
            if ( m_CurrentTreeLocation->previousNode && m_CurrentTreeLocation->previousNode->leftToken ) {
              if ( IsPrimitive( m_CurrentTreeLocation->previousNode->leftToken ) &&
                   m_CurrentTreeLocation->previousNode->previousNode != NULL ) {
                m_CurrentTreeLocation = m_CurrentTreeLocation->previousNode;
                FindValidNodePosition();
              } // if: is function
            } // if: previousNode && previousNode->leftToken
            
            RightCreateNode();
            
            if ( m_LineOfTokens.at( m_LineOfTokens.size() - 3 ).tokenType != DOT ) {
              LeftCreateNode();
            } // if: without dot
            
            m_CurrentTreeLocation->leftParenCreate = true;
            LeftInsertToken();
          } // if: already has left token
          
          else {
            if ( m_CurrentTreeLocation->leftNode != NULL ) {
              FindValidNodePosition();
              
              if ( m_CurrentTreeLocation->previousNode && m_CurrentTreeLocation->previousNode->leftToken ) {
                if ( IsPrimitive( m_CurrentTreeLocation->previousNode->leftToken ) &&
                     m_CurrentTreeLocation->previousNode->previousNode != NULL ) {
                  m_CurrentTreeLocation = m_CurrentTreeLocation->previousNode;
                  FindValidNodePosition();
                } // if: is function
              } // if: previousNode && previousNode->leftToken
              
              RightCreateNode();
              
              if ( m_LineOfTokens.at( m_LineOfTokens.size() - 3 ).tokenType != DOT ) {
                LeftCreateNode();
              } // if: without dot
              
              m_CurrentTreeLocation->leftParenCreate = true;
              LeftInsertToken();
            } // if: insert right
            
            else {
              FindValidNodePosition();
              
              if ( m_CurrentTreeLocation->previousNode && m_CurrentTreeLocation->previousNode->leftToken ) {
                if ( IsPrimitive( m_CurrentTreeLocation->previousNode->leftToken ) &&
                     m_CurrentTreeLocation->previousNode->previousNode != NULL ) {
                  m_CurrentTreeLocation = m_CurrentTreeLocation->previousNode;
                  FindValidNodePosition();
                } // if: is function
              } // if: previousNode && previousNode->leftToken
              
              LeftCreateNode();
              m_CurrentTreeLocation->leftParenCreate = true;
              LeftInsertToken();
            } // else: insert right
          } // else: right insert node
        } // else: create a node
      } // if: is quote symbol case
      
      else {
        m_CurrentTreeLocation->leftParenCreate = true;
        LeftInsertToken();
      } // else: quote word case
      
      if ( HasNextToken() == false ) {
        return false;
      } // if: check if there is any token left
      
      // QUOTE <S-exp>
      if ( CheckSExp() == true ) {
        if ( isQuoteWordCase ) {
          TokenStruct quoteRightParen;
          quoteRightParen.tokenType = RIGHT_PAREN;
          quoteRightParen.content = ")";
          m_LineOfTokens.push_back( quoteRightParen );
          m_CurrentTreeLocation = m_CurrentTreeLocation->previousNode;
        } // if: is quote symbol case
        
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
  
  /*
    ------------------- Tree -------------------
    --------------- Constructing ---------------
  */
  
  void InitializeRoot() {
    m_Root = new TreeStruct;
    m_Root->leftNode = NULL;
    m_Root->rightNode = NULL;
    m_Root->previousNode = NULL;
    m_Root->leftToken = NULL;
    m_Root->rightToken = NULL;
    m_CurrentTreeLocation = m_Root;
  } // InitializeRoot()
  
  void LeftCreateNode() {
    m_CurrentTreeLocation->leftNode = new TreeStruct;
    m_CurrentTreeLocation->leftNode->previousNode = m_CurrentTreeLocation;
    m_CurrentTreeLocation = m_CurrentTreeLocation->leftNode;
    m_CurrentTreeLocation->leftParenCreate = false;
    m_CurrentTreeLocation->quoteResult = false;
    m_CurrentTreeLocation->leftNode = NULL;
    m_CurrentTreeLocation->rightNode = NULL;
    m_CurrentTreeLocation->leftToken = NULL;
    m_CurrentTreeLocation->rightToken = NULL;
  } // LeftCreateNode()
  
  void RightCreateNode() {
    m_CurrentTreeLocation->rightNode = new TreeStruct;
    m_CurrentTreeLocation->rightNode->previousNode = m_CurrentTreeLocation;
    m_CurrentTreeLocation = m_CurrentTreeLocation->rightNode;
    m_CurrentTreeLocation->leftParenCreate = false;
    m_CurrentTreeLocation->quoteResult = false;
    m_CurrentTreeLocation->leftNode = NULL;
    m_CurrentTreeLocation->rightNode = NULL;
    m_CurrentTreeLocation->leftToken = NULL;
    m_CurrentTreeLocation->rightToken = NULL;
  } // RightCreateNode()
  
  void FindValidNodePosition() {
    if ( m_CurrentTreeLocation->previousNode ) {
      if ( m_CurrentTreeLocation->previousNode->leftToken ) {
        if ( m_CurrentTreeLocation->previousNode->leftToken->tokenType == QUOTE ) {
          m_CurrentTreeLocation = m_CurrentTreeLocation->previousNode;
        } // if: quote case
      } // if: previous->leftToken exist
    } // if: previousNode exist
    
    while ( m_CurrentTreeLocation->rightNode != NULL ) {
      m_CurrentTreeLocation = m_CurrentTreeLocation->previousNode;
    } // while: move currentTreeLocation to an available position
  } // FindValidNodePosition()
  
  void LeftInsertToken() {
    m_CurrentTreeLocation->leftToken = new TokenStruct;
    m_CurrentTreeLocation->leftToken->content = m_LineOfTokens.back().content;
    m_CurrentTreeLocation->leftToken->tokenType = m_LineOfTokens.back().tokenType;
  } // LeftInsertToken()
  
  void RightInsertToken() {
    m_CurrentTreeLocation->rightToken = new TokenStruct;
    m_CurrentTreeLocation->rightToken->content = m_LineOfTokens.back().content;
    m_CurrentTreeLocation->rightToken->tokenType = m_LineOfTokens.back().tokenType;
  } // RightInsertToken()
  
  /*
    ------------------- Print ------------------
    ------------------ Results -----------------
  */
  
  bool NeedToPrint( int i, int leftParenCount ) {
    if ( m_LineOfTokens[i].tokenType == DOT ) {
      if ( m_LineOfTokens[i + 1].tokenType == LEFT_PAREN ) {
        return false;
      } // if: dot-leftParen case
      
      else if ( m_LineOfTokens[i + 1].tokenType == NIL ) {
        return false;
      } // else if: dot-nil
    } // if: current token is DOT
    
    else if ( m_LineOfTokens[i].tokenType == LEFT_PAREN ) {
      if ( i >= 1 ) {
        if ( m_LineOfTokens[i - 1].tokenType == DOT ) {
          return false;
        } // if: dot-leftParen case
      } // if: check index validility
    } // if: current token is left-paren
    
    else if ( m_LineOfTokens[i].tokenType == RIGHT_PAREN ) {
      if ( IsAtom( m_LineOfTokens[i - 1] ) ) {
        return true;
      } // if: printing after an atom
      
      else {
        if ( leftParenCount > 1 ) {
          return true;
        } // if: still have spaces for continious rightParen
        
        return false;
      } // else if: continious rightParen
    } // else if: currrent token is right-paren
    
    else if ( m_LineOfTokens[i].tokenType == NIL ) {
      if ( i >= 1 ) {
        if ( m_LineOfTokens[i - 1].tokenType == DOT &&
             m_LineOfTokens[i + 1].tokenType == RIGHT_PAREN ) {
          return false;
        } // if: nil-rightParen case
      } // if: check index validility
    } // else if: current token is NIL
    
    else if ( m_LineOfTokens[i].content == "'" ) {
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
  
  void PrintSExp() {
    int leftParenCount = 0;
    bool hasLineReturn = false;
    
    for ( int i = 0 ; i < m_LineOfTokens.size() ; i++ ) {
      if ( NeedToPrint( i, leftParenCount ) ) {
        if ( m_LineOfTokens[i].tokenType == RIGHT_PAREN ) {
          leftParenCount--;
        } // if: print right parenthesis, minus leftParenCount by one
        
        if ( hasLineReturn ) {
          PrintIndentation( leftParenCount );
        } // if: has a line return
        
        if ( IsAtom( m_LineOfTokens[i] ) ) {
          if ( m_LineOfTokens[i].tokenType == INT ) {
            cout << atoi( m_LineOfTokens[i].content.c_str() ) << endl;
          } // if: int case
          
          else if ( m_LineOfTokens[i].tokenType == FLOAT ) {
            cout << fixed << setprecision( 3 )
                 << round( atof( m_LineOfTokens[i].content.c_str() )*1000 )/1000
                 << endl;
          } // else if: float case with precision and round
          
          else if ( m_LineOfTokens[i].tokenType == NIL ) {
            cout << "nil" << endl;
          } // else if: nil
          
          else if ( m_LineOfTokens[i].tokenType == T ) {
            cout << "#t" << endl;
          } // else if: #t
          
          else if ( m_LineOfTokens[i].tokenType == STRING ) {
            PrintString( m_LineOfTokens[i].content );
          } // else if: string
          
          else if ( m_LineOfTokens[i].tokenType == SYMBOL ) {
            cout << m_LineOfTokens[i].content << endl;
          } // else if: symbol
          
          hasLineReturn = true;
        } // if: current token is an atom
        
        else if ( m_LineOfTokens[i].tokenType == LEFT_PAREN ) {
          leftParenCount++;
          hasLineReturn = false;
          cout << "( ";
        } // else if: left paren
        
        else if ( m_LineOfTokens[i].tokenType == RIGHT_PAREN ) {
          cout << ")" << endl;
          hasLineReturn = true;
        } // else if: right paren
        
        else if ( m_LineOfTokens[i].tokenType == DOT ) {
          cout << "." << endl;
          hasLineReturn = true;
        } // else if: dot
      } // if: token need to be printed
      
      if ( leftParenCount == 0 && m_LineOfTokens[i].tokenType != QUOTE ) {
        return;
      } // if: check print complete
      
      else if ( i == m_LineOfTokens.size() - 1 && leftParenCount > 0 ) {
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
  
  /*
    ------------------- Error ------------------
    ------------------ Handling ----------------
  */
  
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
                  << " Column " << g_CursorColumn - ( m_LineOfTokens.back().content.size() - 1 )
                  << " is >>" << m_LineOfTokens.back().content << "<<";
      m_Error.errorType = UNEXPECTED_TOKEN_ATOM_LEFT_PAREN;
      m_Error.errorMessage = errorStream.str();
    } // else if: unexpected atom or left paren
    
    else if ( errorType == UNEXPECTED_RIGHT_PAREN ) {
      errorStream << "ERROR (unexpected token) : "
                  << "')' expected when token at "
                  << "Line " << g_CursorLine
                  << " Column " << g_CursorColumn - ( m_LineOfTokens.back().content.size() - 1 )
                  << " is >>" << m_LineOfTokens.back().content << "<<";
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
                  << " Column " << g_CursorColumn - ( m_LineOfTokens.back().content.size() - 1 )
                  << " is >>" << m_LineOfTokens.back().content << "<<";
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
  
  /*
    ------------------- Clear ------------------
    -------------------- Line ------------------
  */
  
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
  // vector<ResultStruct *> m_ResultList;
  ErrorStruct m_Error;
  TreeStruct *m_Root;

public:
  /*
    --------------------- Is -------------------
    ------------------ Function ----------------
  */
  
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
  
  bool IsPrimitive( TokenStruct currentToken ) {
    if ( currentToken.content == "cons" || currentToken.content == "list" ) {
      currentToken.isPrimitive = true;
      currentToken.primitiveType = CONSTRUCTOR;
    } // if: constructor
    
    else if ( currentToken.content == "quote" || currentToken.content == "'" ) {
      currentToken.isPrimitive = true;
      currentToken.primitiveType = QUOTE_BYPASSING;
    } // else if: quote bypassing
    
    else if ( currentToken.content == "define" ) {
      currentToken.isPrimitive = true;
      currentToken.primitiveType = DEFINE_BINDING;
    } // else if: difine binding
    
    else if ( currentToken.content == "car" || currentToken.content == "cdr" ) {
      currentToken.isPrimitive = true;
      currentToken.primitiveType = PART_ACCESSOR;
    } // else if: part accessors
    
    else if ( currentToken.content == "atom?" || currentToken.content == "pair?" ||
              currentToken.content == "list?" || currentToken.content == "null?" ||
              currentToken.content == "integer?" || currentToken.content == "real?" ||
              currentToken.content == "number?" || currentToken.content == "string?" ||
              currentToken.content == "boolean?" || currentToken.content == "symbol?" ) {
      currentToken.isPrimitive = true;
      currentToken.primitiveType = PRIMITIVE_PREDICATE;
    } // else if: primitive predicate
    
    else if ( currentToken.content == "+" || currentToken.content == "-" ||
              currentToken.content == "*" || currentToken.content == "/" ||
              currentToken.content == "not" || currentToken.content == "and" ||
              currentToken.content == "or" || currentToken.content == ">" ||
              currentToken.content == ">=" || currentToken.content == "<" ||
              currentToken.content == "<=" || currentToken.content == "=" ||
              currentToken.content == "string-append" || currentToken.content == "string>?" ||
              currentToken.content == "string<?" || currentToken.content == "string=?" ) {
      currentToken.isPrimitive = true;
      currentToken.primitiveType = OPERATOR;
    } // else if: opertor
    
    else if ( currentToken.content == "eqv?" || currentToken.content == "equal?" ) {
      currentToken.isPrimitive = true;
      currentToken.primitiveType = EQUIVALENCE;
    } // else if: equivalence
    
    else if ( currentToken.content == "begin" ) {
      currentToken.isPrimitive = true;
      currentToken.primitiveType = BEGIN;
    } // else if: begin
    
    else if ( currentToken.content == "if" || currentToken.content == "cond" ) {
      currentToken.isPrimitive = true;
      currentToken.primitiveType = CONDITIONAL;
    } // else if: conditional
    
    else if ( currentToken.content == "clean-environment" ) {
      currentToken.isPrimitive = true;
      currentToken.primitiveType = CLEAN_ENVIRONMENT;
    } // else if: clean environment
    
    else if ( currentToken.content == "exit" ) {
      currentToken.isPrimitive = true;
      currentToken.primitiveType = EXIT;
    } // else if: difine binding
    
    else {
      currentToken.isPrimitive = false;
    } // else: not primitive
    
    return currentToken.isPrimitive;
  } // IsPrimitive()
  /*
    -------------------- New -------------------
    ----------------- Structures ---------------
  */
  
  ResultStruct *NewResult() {
    ResultStruct *newResult = new ResultStruct;
    newResult->hasNodeResult = false;
    newResult->hasTokenResult = false;
    newResult->nodeResult = new TreeStruct;
    newResult->tokenResult = new TokenStruct;
    // initialization
    newResult->nodeResult->leftNode = NULL;
    newResult->nodeResult->rightNode = NULL;
    newResult->nodeResult->previousNode = NULL;
    newResult->nodeResult->leftToken = NULL;
    newResult->nodeResult->rightToken = NULL;
    return newResult;
  } // NewResult()
  
  /*
    ------------------- Error ------------------
    ------------------ Handling ----------------
  */
  
  void SetError( ErrorType errorType, string errorMessage ) {
    m_Error.errorType = errorType;
    m_Error.errorMessage = errorMessage;
  } // SetError()
  
  void ErrorHandling() {
    cout << m_Error.errorMessage;
    
    if ( m_Error.errorType == NON_LIST || m_Error.errorType == DEFINE_FORMAT ) {
      m_project1.PrintSExp();
      return;
    } // if: non-list error, print the expression
    
    cout << endl;
  } // ErrorHandling()
  /*
    ------------------- Start ------------------
    ---------------- Evaluation ----------------
  */
  
  bool EvalSExp( Project1Class project1Result ) {
    m_project1 = project1Result;
    m_Error.errorType = NOT_S_EXP;
    m_Error.errorMessage = "\0";
    m_Root = m_project1.GetRoot();
    
    bool hasError = false;
    
    do {
      if ( !ProcessSExp( m_Root, hasError ) ) {
        return false;
      } // if: error
    } while ( m_Root->rightNode != NULL );
    
    return true;
  } // EvalSExp()
  
  bool ProcessSExp( TreeStruct *walk, bool &hasError ) {
    static bool underQuote = false;
    
    if ( walk ) {
      if ( walk->leftToken ) {
        if ( walk->leftToken->tokenType == QUOTE ) {
          underQuote = true;
        } // if: under quote function
        
        else if ( IsPrimitive( walk->leftToken ) ) {
          underQuote = false;
        } // if: is Primitive
      } // if: has leftToken
      
      if ( walk->rightNode ) {
        ProcessSExp( walk->rightNode, hasError );
        
        if ( walk->leftToken && walk->leftToken->tokenType != QUOTE ) {
          if ( IsPrimitive( walk->leftToken ) ) {
            underQuote = false;
          } // if: reset underQuote if the token is a primitive
        } // if: not quote
      } // if: has a right node, go right
      
      if ( walk->leftToken && hasError == false ) {
        if ( walk->leftParenCreate ) {
          if ( walk->leftToken->tokenType == SYMBOL && NOT IsPrimitive( walk->leftToken ) ) {
            if ( FoundDefineBindings( walk->leftToken->content ) ) {
              if ( GetDefineBindings( walk->leftToken->content )->leftToken ) {
                walk->leftToken = GetDefineBindings( walk->leftToken->content )->leftToken;
                
                if ( NOT IsPrimitive( GetDefineBindings( walk->leftToken->content )->leftToken ) ) {
                  string errorMessage = "ERROR (attempt to apply non-function) : ";
                  errorMessage += walk->leftToken->content;
                  SetError( APPLY_NON_FUNCTION, errorMessage );
                  hasError = true;
                  return !hasError;
                } // if: find pre-defined symbol, but not a function
              } // if: the defined binding is a token
              
              else {
                TreeStruct *errorNode = GetDefineBindings( walk->leftToken->content )->leftNode;
                stringstream errorMessage;
                errorMessage << "ERROR (attempt to apply non-function) : ";
                int layer = 0;
                AddTreeString( errorNode, layer, false, errorMessage );
                
                if ( layer > 0 ) {
                  while ( layer > 0 ) {
                    layer--;
                    errorMessage << string( layer, ' ' ) << ')' << endl;
                  } // while: loop print right-paren
                } // if: layer still greater than zero
                
                SetError( APPLY_NON_FUNCTION, errorMessage.str() );
                hasError = true;
                return !hasError;
              } // else: the defined binding is a node
            } // if: found binding
            
            else {
              if ( NOT underQuote ) {
                string errorMessage;
                errorMessage = "ERROR (unbound symbol) : " + walk->leftToken->content;
                SetError( DEFINE_UNBOUND, errorMessage );
                hasError = true;
                return !hasError;
              } // if: not under quote, print error
            } // else: not found binding
          } // if: current token is symbol
          
          if ( IsPrimitive( walk->leftToken ) ) {
            vector<TreeStruct *> argumentList;
            hasError = !CheckArgument( walk, argumentList );
            
            if ( NOT hasError ) {
              hasError = CallCorrespondingFunction( walk, argumentList );
            } // if: arguments has no problem
            
            else {
              return !hasError;
            } // else: arguments has error
          } // if: check the parameters
          
          else {
            if ( NOT underQuote ) {
              string errorMessage;
              errorMessage = "ERROR (attempt to apply non-function) : " + walk->leftToken->content;
              SetError( APPLY_NON_FUNCTION, errorMessage );
              hasError = true;
            } // if: not under quote
          } // else: not function
        } // if: this node is created by left-paren and isn't under a quote
      } // if: has a left token
      
      else if ( walk->leftNode ) {
        ProcessSExp( walk->leftNode, hasError );
      } // if: has a left node, go left
    } // if: walk != NULL
    
    else {
      if ( m_project1.GetSExp().front().tokenType == SYMBOL ) {
        m_project1.InitializeRoot();
        m_Root = m_project1.GetRoot();
        
        if ( IsPrimitive( m_project1.GetSExp().front() ) ) {
          ResultStruct *result = NewResult();
          result->hasTokenResult = true;
          result->tokenResult->tokenType = SYMBOL;
          result->tokenResult->content = m_project1.GetSExp().front().content;
          m_Root->leftToken = result->tokenResult;
        } // if: is primitive
        
        else if ( FoundDefineBindings( m_project1.GetSExp().front().content ) ) {
          if ( GetDefineBindings( m_project1.GetSExp().front().content )->leftToken ) {
            TokenStruct *w_Defined = GetDefineBindings( m_project1.GetSExp().front().content )->leftToken;
            m_Root->leftToken = w_Defined;
          } // if: pre-defined is a token
          
          else {
            m_Root->leftNode = GetDefineBindings( m_project1.GetSExp().front().content )->leftNode;
          } // else: pre-defined is a node
        } // if: doesn't find a binding
        
        else {
          string errorMessage = "ERROR (unbound symbol) : " + m_project1.GetSExp().front().content;
          SetError( DEFINE_UNBOUND, errorMessage );
          hasError = true;
        } // else: found a binding
      } // if: single input is a symbol
      
      else {
        m_project1.InitializeRoot();
        m_Root = m_project1.GetRoot();
        ResultStruct *result = NewResult();
        result->hasTokenResult = true;
        result->tokenResult->tokenType = m_project1.GetSExp().front().tokenType;
        result->tokenResult->content = m_project1.GetSExp().front().content;
        m_Root->leftToken = result->tokenResult;
      } // else: print plain form
    } // else: only inputs a word, check m_DefineBindingList
    
    return !hasError;
  } // ProcessSExp(): go through the data tree while using postorder traversal
  
  /*
    ------------------- Define -----------------
    ------------------ Bindings ----------------
  */
  
  bool FoundDefineBindings( string currentWord ) {
    for ( int i = 0 ; i < m_DefineBindingList.size() ; i++ ) {
      if ( currentWord == m_DefineBindingList[i]->leftToken->content ) {
        return true;
      } // if: find
    } // for: go through m_DefineBindingList
    
    return false;
  } // FoundDefineBindings()
  
  TreeStruct *GetDefineBindings( string currentWord ) {
    for ( int i = 0 ; i < m_DefineBindingList.size() ; i++ ) {
      if ( currentWord == m_DefineBindingList[i]->leftToken->content ) {
        return m_DefineBindingList[i]->rightNode;
      } // if: find
    } // for: go through m_DefineBindingList
    
    return m_DefineBindingList.back()->rightNode;
  } // GetDefineBindings()
  
  /*
    ----------------- Parameters ---------------
    ----------------- Processing ---------------
  */
  
  bool CheckArgument( TreeStruct *current, vector<TreeStruct *> &argumentList ) {
    TreeStruct *temp = current;
    
    while ( temp->rightNode ) {
      temp = temp->rightNode;
    } // while: find the rightmost node
    
    if ( temp->rightToken ) {
      string errorMessage = "ERROR (non-list) : ";
      SetError( NON_LIST, errorMessage );
      return false;
    } // if: check non-list
    
    if ( current->leftToken->primitiveType == CONSTRUCTOR ) {
      if ( current->leftToken->content == "cons" ) {
        if ( current->rightNode ) {
          argumentList.push_back( current->rightNode );
          
          if ( current->rightNode->rightNode ) {
            argumentList.push_back( current->rightNode->rightNode );
            
            if ( current->rightNode->rightNode->rightNode != NULL ) {
              string errorMessage = "ERROR (incorrect number of arguments) : cons";
              SetError( INCORRECT_NUM_ARGUMENTS, errorMessage );
              return false;
            } // if: no third argument
          } // if: has two arguments
          
          else {
            string errorMessage = "ERROR (incorrect number of arguments) : cons";
            SetError( INCORRECT_NUM_ARGUMENTS, errorMessage );
            return false;
          } // else: incorrct number of arguments
        } // if: has one argument
        
        else {
          string errorMessage = "ERROR (incorrect number of arguments) : cons";
          SetError( INCORRECT_NUM_ARGUMENTS, errorMessage );
          return false;
        } // else: incorrct number of arguments
      } // if: cons
      
      else if ( current->leftToken->content == "list" ) {
        argumentList.push_back( current->rightNode );
      } // else if: list
      
      TreeStruct *walk = argumentList.front();
      
      while ( walk->rightNode ) {
        if ( walk->leftToken && !walk->leftToken->isQuoteResult ) {
          if ( walk->leftToken->tokenType == SYMBOL && NOT IsPrimitive( walk->leftToken ) ) {
            if ( NOT FoundDefineBindings( walk->leftToken->content ) ) {
              string errorMessage = "ERROR (unbound symbol) : " + walk->leftToken->content;
              SetError( DEFINE_UNBOUND, errorMessage );
              return false;
            } // if: cant find bound symbol
          } // if: tokenType is symbol
        } // if: has a leftToken
        
        walk = walk->rightNode;
        
        if ( walk->rightNode == NULL ) {
          if ( walk->leftToken && !walk->leftToken->isQuoteResult ) {
            if ( walk->leftToken->tokenType == SYMBOL && NOT IsPrimitive( walk->leftToken ) ) {
              if ( NOT FoundDefineBindings( walk->leftToken->content ) ) {
                string errorMessage = "ERROR (unbound symbol) : " + walk->leftToken->content;
                SetError( DEFINE_UNBOUND, errorMessage );
                return false;
              } // if: cant find bound symbol
            } // if: tokenType is symbol
          } // if: has a leftToken
        } // if: last node
      } // while: check the symbols in the arguments
      
      return true;
    } // if: constuctor
    
    else if ( current->leftToken->primitiveType == QUOTE_BYPASSING ) {
      if ( current->rightNode ) {
        argumentList.push_back( current->rightNode );
        
        if ( current->rightNode->rightNode != NULL ) {
          string errorMessage = "ERROR (incorrect number of arguments) : quote";
          SetError( INCORRECT_NUM_ARGUMENTS, errorMessage );
          return false;
        } // if: incorrect number of arguments
      } // if: has one argument
      
      else {
        string errorMessage = "ERROR (incorrect number of arguments) : quote";
        SetError( INCORRECT_NUM_ARGUMENTS, errorMessage );
        return false;
      } // else: incorrect number of arguments
      
      return true;
    } // else if: quote
    
    else if ( current->leftToken->primitiveType == DEFINE_BINDING ) {
      if ( current->rightNode ) {
        if ( current->rightNode->leftToken ) {
          if ( current->rightNode->leftToken->tokenType != SYMBOL ||
               IsPrimitive( current->rightNode->leftToken ) ) {
            string errorMessage = "ERROR (DEFINE format) : ";
            SetError( DEFINE_FORMAT, errorMessage );
            return false;
          } // if: define primitive error
        } // if: the first argument is a token
        
        else {
          string errorMessage = "ERROR (DEFINE format) : ";
          SetError( DEFINE_FORMAT, errorMessage );
          return false;
        } // else: the first argument is a node
        
        argumentList.push_back( current->rightNode );
        
        if ( current->rightNode->rightNode ) {
          if ( current->rightNode->rightNode->leftToken &&
               current->rightNode->rightNode->leftToken->tokenType == SYMBOL ) {
            if ( !current->rightNode->rightNode->leftToken->isQuoteResult ) {
              if ( !FoundDefineBindings( current->rightNode->rightNode->leftToken->content ) ) {
                string errorMessage = "ERROR (unbound symbol) : ";
                errorMessage += current->rightNode->rightNode->leftToken->content;
                SetError( DEFINE_UNBOUND, errorMessage );
                return false;
              } // if: found no bindings
            } // if: not a quote result
          } // if: has a leftToken and the tokenType is a symbol
          
          argumentList.push_back( current->rightNode->rightNode );
          
          if ( current->rightNode->rightNode->rightNode != NULL ) {
            string errorMessage = "ERROR (DEFINE format) : ";
            SetError( DEFINE_FORMAT, errorMessage );
            return false;
          } // if: number error
        } // if: has two arguments
        
        else {
          string errorMessage = "ERROR (DEFINE format) : ";
          SetError( DEFINE_FORMAT, errorMessage );
          return false;
        } // else: number error
      } // if: has one argument
      
      else {
        string errorMessage = "ERROR (DEFINE format) : ";
        SetError( DEFINE_FORMAT, errorMessage );
        return false;
      } // else: number error
      
      return true;
    } // else if: define binding
    
    else if ( current->leftToken->primitiveType == PART_ACCESSOR ) {
      if ( current->rightNode ) {
        argumentList.push_back( current->rightNode );
        
        if ( current->rightNode->rightNode != NULL ) {
          string errorMessage = "ERROR (incorrect number of arguments) : " + current->leftToken->content;
          SetError( INCORRECT_NUM_ARGUMENTS, errorMessage );
          return false;
        } // if: more than one arguments
      } // if: has one arguments
      
      else {
        string errorMessage = "ERROR (incorrect number of arguments) : " + current->leftToken->content;
        SetError( INCORRECT_NUM_ARGUMENTS, errorMessage );
        return false;
      } // else: number error
      
      for ( int i = 0 ; i < argumentList.size() ; i++ ) {
        if ( argumentList[i]->leftToken ) {
          if ( argumentList[i]->leftToken->tokenType == SYMBOL ) {
            if ( NOT FoundDefineBindings( argumentList[i]->leftToken->content ) ) {
              string errorMessage = "ERROR (unbound symbol) : " + argumentList[i]->leftToken->content;
              SetError( DEFINE_UNBOUND, errorMessage );
              return false;
            } // if: cant find bound symbol
          } // if: tokenType is symbol
        } // if: has a leftToken
      } // for: check the symbols in the arguments
      
      return true;
    } // else if: part accessors
    
    else if ( current->leftToken->primitiveType == PRIMITIVE_PREDICATE ) {
      if ( current->rightNode ) {
        argumentList.push_back( current->rightNode );
        
        if ( current->rightNode->rightNode != NULL ) {
          string errorMessage = "ERROR (incorrect number of arguments) : " + current->leftToken->content;
          SetError( INCORRECT_NUM_ARGUMENTS, errorMessage );
          return false;
        } // if: number error
      } // if: has one arguments
      
      else {
        string errorMessage = "ERROR (incorrect number of arguments) : " + current->leftToken->content;
        SetError( INCORRECT_NUM_ARGUMENTS, errorMessage );
        return false;
      } // else: number error
      
      for ( int i = 0 ; i < argumentList.size() ; i++ ) {
        if ( argumentList[i]->leftToken && !argumentList[i]->quoteResult ) {
          if ( argumentList[i]->leftToken->tokenType == SYMBOL &&
               NOT IsPrimitive( argumentList[i]->leftToken ) ) {
            if ( NOT FoundDefineBindings( argumentList[i]->leftToken->content ) ) {
              string errorMessage = "ERROR (unbound symbol) : " + argumentList[i]->leftToken->content;
              SetError( DEFINE_UNBOUND, errorMessage );
              return false;
            } // if: cant find bound symbol
          } // if: tokenType is symbol
        } // if: has a leftToken
      } // for: check the symbols in the arguments
      
      return true;
    } // else if: primitive predicate
    
    else if ( current->leftToken->primitiveType == OPERATOR ) {
      if ( current->leftToken->content == "not" ) {
        if ( current->rightNode ) {
          argumentList.push_back( current->rightNode );
          
          if ( current->rightNode->rightNode != NULL ) {
            string errorMessage = "ERROR (incorrect number of arguments) : not";
            SetError( INCORRECT_NUM_ARGUMENTS, errorMessage );
            return false;
          } // if: number error
        } // if: hos one argument
        
        else {
          string errorMessage = "ERROR (incorrect number of arguments) : not";
          SetError( INCORRECT_NUM_ARGUMENTS, errorMessage );
          return false;
        } // else: number error
      } // if: not
      
      else {
        if ( current->rightNode ) {
          argumentList.push_back( current->rightNode );
          
          if ( current->rightNode->rightNode ) {
            argumentList.push_back( current->rightNode->rightNode );
          } // if: has greater than two arguments
          
          else {
            string errorMessage = "ERROR (incorrect number of arguments) : " + current->leftToken->content;
            SetError( INCORRECT_NUM_ARGUMENTS, errorMessage );
            return false;
          } // else: number error
        } // if: has one arguments
        
        else {
          string errorMessage = "ERROR (incorrect number of arguments) : " + current->leftToken->content;
          SetError( INCORRECT_NUM_ARGUMENTS, errorMessage );
          return false;
        } // else: number error
      } // else: other operators
      
      TreeStruct *walk = argumentList.front();
      
      while ( walk->rightNode ) {
        if ( walk->leftToken ) {
          if ( walk->leftToken->tokenType == SYMBOL ) {
            if ( NOT FoundDefineBindings( walk->leftToken->content ) ) {
              string errorMessage = "ERROR (unbound symbol) : " + walk->leftToken->content;
              SetError( DEFINE_UNBOUND, errorMessage );
              return false;
            } // if: cant find bound symbol
          } // if: tokenType is symbol
        } // if: has a leftToken
        
        walk = walk->rightNode;
        
        if ( walk->rightNode == NULL ) {
          if ( walk->leftToken ) {
            if ( walk->leftToken->tokenType == SYMBOL ) {
              if ( NOT FoundDefineBindings( walk->leftToken->content ) ) {
                string errorMessage = "ERROR (unbound symbol) : " + walk->leftToken->content;
                SetError( DEFINE_UNBOUND, errorMessage );
                return false;
              } // if: cant find bound symbol
            } // if: tokenType is symbol
          } // if: has a leftToken
        } // if: last node
      } // while: check the symbols in the arguments
      
      return true;
    } // else if: operators
    
    else if ( current->leftToken->primitiveType == EQUIVALENCE ) {
      if ( current->rightNode ) {
        argumentList.push_back( current->rightNode );
        
        if ( current->rightNode->rightNode ) {
          argumentList.push_back( current->rightNode->rightNode );
          
          if ( current->rightNode->rightNode->rightNode != NULL ) {
            string errorMessage = "ERROR (incorrect number of arguments) : " + current->leftToken->content;
            SetError( INCORRECT_NUM_ARGUMENTS, errorMessage );
            return false;
          } // if: number error
        } // if: has two arguments
        
        else {
          string errorMessage = "ERROR (incorrect number of arguments) : " + current->leftToken->content;
          SetError( INCORRECT_NUM_ARGUMENTS, errorMessage );
          return false;
        } // else: number error
      } // if: has one argument
      
      else {
        string errorMessage = "ERROR (incorrect number of arguments) : " + current->leftToken->content;
        SetError( INCORRECT_NUM_ARGUMENTS, errorMessage );
        return false;
      } // else: number error
      
      for ( int i = 0 ; i < argumentList.size() ; i++ ) {
        if ( argumentList[i]->leftToken ) {
          if ( argumentList[i]->leftToken->tokenType == SYMBOL ) {
            if ( NOT FoundDefineBindings( argumentList[i]->leftToken->content ) ) {
              string errorMessage = "ERROR (unbound symbol) : " + argumentList[i]->leftToken->content;
              SetError( DEFINE_UNBOUND, errorMessage );
              return false;
            } // if: cant find bound symbol
          } // if: tokenType is symbol
        } // if: has a leftToken
      } // for: check the symbols in the arguments
      
      return true;
    } // else if: equivalence
    
    else if ( current->leftToken->primitiveType == BEGIN ) {
      if ( current->rightNode ) {
        argumentList.push_back( current->rightNode );
      } // if: has more than one arguments
      
      else {
        string errorMessage = "ERROR (incorrect number of arguments) : begin";
        SetError( INCORRECT_NUM_ARGUMENTS, errorMessage );
        return false;
      } // else: number error
      
      for ( int i = 0 ; i < argumentList.size() ; i++ ) {
        if ( argumentList[i]->leftToken ) {
          if ( argumentList[i]->leftToken->tokenType == SYMBOL ) {
            if ( NOT FoundDefineBindings( argumentList[i]->leftToken->content ) ) {
              string errorMessage = "ERROR (unbound symbol) : " + argumentList[i]->leftToken->content;
              SetError( DEFINE_UNBOUND, errorMessage );
              return false;
            } // if: cant find bound symbol
          } // if: tokenType is symbol
        } // if: has a leftToken
      } // for: check the symbols in the arguments
      
      return true;
    } // else if: begin
    
    else if ( current->leftToken->primitiveType == CONDITIONAL ) {
      if ( current->leftToken->content == "if" ) {
        if ( current->rightNode ) {
          argumentList.push_back( current->rightNode );
          
          if ( current->rightNode->rightNode ) {
            argumentList.push_back( current->rightNode->rightNode );
            
            if ( current->rightNode->rightNode->rightNode ) {
              argumentList.push_back( current->rightNode->rightNode->rightNode );
              
              if ( current->rightNode->rightNode->rightNode->rightNode != NULL ) {
                string errorMessage = "ERROR (incorrect number of arguments) : if";
                SetError( INCORRECT_NUM_ARGUMENTS, errorMessage );
                return false;
              } // if: number error
            } // if: has three arguments
          } // if: has two arguments
          
          else {
            string errorMessage = "ERROR (incorrect number of arguments) : if";
            SetError( INCORRECT_NUM_ARGUMENTS, errorMessage );
            return false;
          } // else: number error
        } // if: has one argument
        
        else {
          string errorMessage = "ERROR (incorrect number of arguments) : if";
          SetError( INCORRECT_NUM_ARGUMENTS, errorMessage );
          return false;
        } // else: number error
      } // if: if
      
      else if ( current->leftToken->content == "cond" ) {
        if ( current->rightNode ) {
          argumentList.push_back( current->rightNode );
        } // if: has more than one argument
        
        else {
          string errorMessage = "ERROR (incorrect number of arguments) : cond";
          SetError( INCORRECT_NUM_ARGUMENTS, errorMessage );
          return false;
        } // else: number error
      } // else if: cond
      
      TreeStruct *walk = argumentList.front();
      
      while ( walk->rightNode ) {
        if ( walk->leftToken && !walk->leftToken->isQuoteResult ) {
          if ( walk->leftToken->tokenType == SYMBOL && NOT IsPrimitive( walk->leftToken ) ) {
            if ( NOT FoundDefineBindings( walk->leftToken->content ) ) {
              string errorMessage = "ERROR (unbound symbol) : " + walk->leftToken->content;
              SetError( DEFINE_UNBOUND, errorMessage );
              return false;
            } // if: cant find bound symbol
          } // if: tokenType is symbol
        } // if: has a leftToken
        
        walk = walk->rightNode;
        
        if ( walk->rightNode == NULL ) {
          if ( walk->leftToken && !walk->leftToken->isQuoteResult ) {
            if ( walk->leftToken->tokenType == SYMBOL && NOT IsPrimitive( walk->leftToken ) ) {
              if ( NOT FoundDefineBindings( walk->leftToken->content ) ) {
                string errorMessage = "ERROR (unbound symbol) : " + walk->leftToken->content;
                SetError( DEFINE_UNBOUND, errorMessage );
                return false;
              } // if: cant find bound symbol
            } // if: tokenType is symbol
          } // if: has a leftToken
        } // if: last node
      } // while: check the symbols in the arguments
      
      return true;
    } // else if: conditional
    
    else if ( current->leftToken->primitiveType == CLEAN_ENVIRONMENT ) {
      if ( current->rightNode ) {
        string errorMessage = "ERROR (incorrect number of arguments) : clean-environment";
        SetError( INCORRECT_NUM_ARGUMENTS, errorMessage );
        return false;
      } // if: number error
      
      else {
        return true;
      } // else: correct input
    } // else if: clean-environment
    
    else if ( current->leftToken->primitiveType == EXIT ) {
      if ( current->rightNode ) {
        string errorMessage = "ERROR (incorrect number of arguments) : exit";
        SetError( INCORRECT_NUM_ARGUMENTS, errorMessage );
        return false;
      } // if: number error
      
      return true;
    } // else if: exit
    
    return false;
  } // CheckArgument()
  
  bool CompareTwoTrees( TreeStruct *firstTree, TreeStruct *secondTree ) {
    if ( firstTree == NULL && secondTree == NULL ) {
      return 1;
    } // if: both empty
    
    if ( firstTree != NULL && secondTree != NULL ) {
      bool sameContent = false;
      
      if ( firstTree->leftToken && secondTree->leftToken ) {
        if ( firstTree->leftToken->content == secondTree->leftToken->content ) {
          sameContent = true;
        } // if: same content
      } // if: both have leftToken
      
      return ( sameContent &&
               CompareTwoTrees( firstTree->leftNode, secondTree->leftNode ) &&
               CompareTwoTrees( firstTree->rightNode, secondTree->rightNode ) );
    } // if: both not empty, compare two trees
    
    return false;
  } // CompareTwoTrees()
  
  /*
  ------------------ Function ----------------
  ------------------- Entry ------------------
  */
  
  bool CallCorrespondingFunction( TreeStruct *functionNode, vector<TreeStruct *> arguments ) {
    TokenStruct *function = functionNode->leftToken;
    ResultStruct *result = NewResult();
    bool noError = false;
    
    if ( function->primitiveType == CONSTRUCTOR ) {
      if ( function->content == "cons" ) {
        noError = Cons( arguments, result );
      } // if: cons
      
      else {
        noError = List( arguments, result );
      } // else if: list
    } // if: constructor
    
    else if ( function->primitiveType == QUOTE_BYPASSING ) {
      noError = Quote( arguments, result );
      result->tokenResult->isQuoteResult = true;
    } // else if: quote
    
    else if ( function->primitiveType == DEFINE_BINDING ) {
      noError = Define( arguments, result );
    } // else if: define
    
    else if ( function->primitiveType == PART_ACCESSOR ) {
      if ( function->content == "car" ) {
        noError = Car( arguments, result );
      } // if: car
      
      else {
        noError = Cdr( arguments, result );
      } // else: cdr
    } // else if: part accessor
    
    else if ( function->primitiveType == PRIMITIVE_PREDICATE ) {
      if ( function->content == "atom?" ) {
        noError = IsAtom( arguments, result );
      } // if: atom?
      
      else if ( function->content == "pair?" ) {
        noError = IsPair( arguments, result );
      } // else if: pair?
      
      else if ( function->content == "list?" ) {
        noError = IsList( arguments, result );
      } // else if: list?
      
      else if ( function->content == "null?" ) {
        noError = IsNull( arguments, result );
      } // else if: null?
      
      else if ( function->content == "integer?" ) {
        noError = IsInteger( arguments, result );
      } // else if: integer?
      
      else if ( function->content == "real?" || function->content == "number?" ) {
        noError = IsRealOrNumber( arguments, result );
      } // else if: real? or number?
      
      else if ( function->content == "string?" ) {
        noError = IsString( arguments, result );
      } // else if: string?
      
      else if ( function->content == "boolean?" ) {
        noError = IsBoolean( arguments, result );
      } // else if: boolean?
      
      else if ( function->content == "symbol?" ) {
        noError = IsSymbol( arguments, result );
      } // else if: symbol?
    } // else if: primitive predicate
    
    else if ( function->primitiveType == OPERATOR ) {
      if ( function->content == "+" ) {
        noError = Addition( arguments, result );
      } // if: +
      
      else if ( function->content == "-" ) {
        noError = Subtraction( arguments, result );
      } // else if: -
      
      else if ( function->content == "*" ) {
        noError = Mutiplication( arguments, result );
      } // else if: *
      
      else if ( function->content == "/" ) {
        noError = Division( arguments, result );
      } // else if: /
      
      else if ( function->content == "not" ) {
        noError = Not( arguments, result );
      } // else if: not
      
      else if ( function->content == "and" ) {
        noError = And( arguments, result );
      } // else if: and
      
      else if ( function->content == "or" ) {
        noError = Or( arguments, result );
      } // else if: or
      
      else if ( function->content == ">" ) {
        noError = GreaterThan( arguments, result );
      } // else if: >
      
      else if ( function->content == ">=" ) {
        noError = GreaterThanOrEqualTo( arguments, result );
      } // else if: >=
      
      else if ( function->content == "<" ) {
        noError = LessThan( arguments, result );
      } // else if: <
      
      else if ( function->content == "<=" ) {
        noError = LessThanOrEqualTo( arguments, result );
      } // else if: <=
      
      else if ( function->content == "=" ) {
        noError = EqualTo( arguments, result );
      } // else if: =
      
      else if ( function->content == "string-append" ) {
        noError = StringAppend( arguments, result );
      } // else if: string-append
      
      else if ( function->content == "string>?" ) {
        noError = StringGreaterThan( arguments, result );
      } // else if: string>?
      
      else if ( function->content == "string<?" ) {
        noError = StringLessThan( arguments, result );
      } // else if: string<?
      
      else if ( function->content == "string=?" ) {
        noError = StringEqualTo( arguments, result );
      } // else if: string=?
    } // else if; operator
    
    else if ( function->primitiveType == EQUIVALENCE ) {
      if ( function->content == "eqv?" ) {
        noError = Eqv( arguments, result );
      } // if: eqv?
      
      else if ( function->content == "equal?" ) {
        noError = Equal( arguments, result );
      } // else if: equal?
    } // else if: equivalence
    
    else if ( function->primitiveType == BEGIN ) {
      // Begin( arguments, result );
    } // else if: begin
    
    else if ( function->primitiveType == CONDITIONAL ) {
      if ( function->content == "if" ) {
        noError = If( arguments, result );
      } // if: if
      
      else if ( function->content == "cond" ) {
        noError = Cond( arguments, result );
      } // else if: cond
    } // else if: conditional
    
    else if ( function->primitiveType == CLEAN_ENVIRONMENT ) {
      m_DefineBindingList.clear();
      result->hasTokenResult = true;
      result->tokenResult->tokenType = STRING;
      result->tokenResult->content = "environment cleaned";
      noError = true;
    } // else if: clean-environment
    
    if ( noError ) {
      functionNode->leftToken = NULL;
      functionNode->rightNode = NULL;
      
      if ( result->hasTokenResult ) {
        if ( functionNode->previousNode ) {
          functionNode->previousNode->leftToken = result->tokenResult;
          functionNode->previousNode->leftNode = NULL;
          
          if ( function->primitiveType == QUOTE_BYPASSING ) {
            functionNode->previousNode->quoteResult = true;
          } // if: has previousNode
        } // if: quote
        
        else {
          m_Root->leftToken = result->tokenResult;
        } // else: root
      } // if: attach token result
      
      else if ( result->hasNodeResult ) {
        if ( functionNode->previousNode ) {
          functionNode->previousNode->leftNode = result->nodeResult;
          
          if ( function->primitiveType == QUOTE_BYPASSING ) {
            functionNode->previousNode->leftNode->quoteResult = true;
          } // if: has previousNode
        } // if: quote
        
        else {
          m_Root->leftNode = result->nodeResult;
        } // else: root
      } // else if: attach node result
    } // if: no error, update the tree
    
    return !noError;
  } // CallCorrespondingFunction()
  
  /*
  ------------------ Function ----------------
  ----------------- Definition ---------------
  */
  
  bool Cons( vector<TreeStruct *> arguments, ResultStruct *result ) {
    TreeStruct *resultRoot = new TreeStruct;
    // initialization
    resultRoot->leftNode = NULL;
    resultRoot->rightNode = NULL;
    resultRoot->previousNode = NULL;
    resultRoot->leftToken = NULL;
    resultRoot->rightToken = NULL;
    
    if ( arguments[0]->leftToken ) {
      if ( arguments[0]->leftToken->tokenType == SYMBOL ) {
        if ( IsPrimitive( arguments[0]->leftToken ) ) {
          resultRoot->leftToken = arguments[0]->leftToken;
        } // if: is a primitive
        
        else if ( !arguments[0]->leftToken->isQuoteResult ) {
          if ( GetDefineBindings( arguments[0]->leftToken->content )->leftNode ) {
            resultRoot->leftNode = GetDefineBindings( arguments[0]->leftToken->content )->leftNode;
          } // if: return a node
          
          else {
            resultRoot->leftToken = GetDefineBindings( arguments[0]->leftToken->content )->leftToken;
          } // else: return a token
        } // else if: found bindings
        
        else {
          resultRoot->leftToken = arguments[0]->leftToken;
        } // else: found no bindins
      } // if: defined bindings
      
      else {
        resultRoot->leftToken = arguments[0]->leftToken;
      } // else: not defined simple data
    } // if: token form, get leftToken
    
    else {
      resultRoot->leftNode = arguments[0]->leftNode;
    } // else: get from previous result
    
    if ( arguments[1]->leftToken ) {
      if ( arguments[1]->leftToken->tokenType == SYMBOL ) {
        if ( IsPrimitive( arguments[1]->leftToken ) ) {
          resultRoot->rightToken = arguments[0]->leftToken;
        } // if: is a primitive
        
        else if ( !arguments[1]->leftToken->isQuoteResult ) {
          if ( GetDefineBindings( arguments[1]->leftToken->content )->leftNode ) {
            resultRoot->rightNode = GetDefineBindings( arguments[1]->leftToken->content )->leftNode;
          } // if: return a node
          
          else {
            resultRoot->rightToken = GetDefineBindings( arguments[1]->leftToken->content )->leftToken;
          } // else: return a token
        } // else if: found binding
        
        else {
          resultRoot->rightToken = arguments[1]->leftToken;
        } // else: found no binding
      } // if: defined bindings
      
      else {
        resultRoot->rightToken = arguments[1]->leftToken;
      } // else: not defined binding
    } // if: token form
    
    else {
      resultRoot->rightNode = arguments[1]->leftNode;
    } // else: get from previous result
    
    result->hasNodeResult = true;
    result->nodeResult = resultRoot;
    return true;
  } // Cons()
  
  bool List( vector<TreeStruct *> arguments, ResultStruct *result ) {
    TreeStruct *resultRoot = new TreeStruct;
    // initialization
    resultRoot->leftNode = NULL;
    resultRoot->rightNode = NULL;
    resultRoot->previousNode = NULL;
    resultRoot->leftToken = NULL;
    resultRoot->rightToken = NULL;
    
    TreeStruct *resultWalk = resultRoot;
    TreeStruct *listWalk = arguments[0];
    
    if ( listWalk->rightNode == NULL ) {
      if ( listWalk->leftToken ) {
        if ( listWalk->leftToken->tokenType == SYMBOL ) {
          if ( IsPrimitive( listWalk->leftToken ) ) {
            resultWalk->leftToken = listWalk->leftToken;
          } // if: is primitive
          
          else if ( !listWalk->leftToken->isQuoteResult ) {
            if ( GetDefineBindings( listWalk->leftToken->content )->leftNode ) {
              resultWalk->leftNode = GetDefineBindings( listWalk->leftToken->content )->leftNode;
            } // if: return a node
            
            else {
              resultWalk->leftToken = GetDefineBindings( listWalk->leftToken->content )->leftToken;
            } // else: return a token
          } // if: defined bindings
          
          else {
            resultWalk->leftToken = listWalk->leftToken;
          } // else: not defined
        } // if: symbol
        
        else {
          resultWalk->leftToken = listWalk->leftToken;
        } // else: other type
      } // if: leftToken exist
      
      else {
        resultWalk->leftNode = listWalk->leftNode;
      } // else: get previous result
    } // if: only one data
    
    else {
      while ( listWalk->rightNode ) {
        if ( listWalk->leftToken ) {
          if ( listWalk->leftToken->tokenType == SYMBOL ) {
            if ( IsPrimitive( listWalk->leftToken ) ) {
              resultWalk->leftToken = listWalk->leftToken;
            } // if: is primitive
            
            else if ( !listWalk->leftToken->isQuoteResult ) {
              if ( GetDefineBindings( listWalk->leftToken->content )->leftNode ) {
                resultWalk->leftNode = GetDefineBindings( listWalk->leftToken->content )->leftNode;
              } // if: return a node
              
              else {
                resultWalk->leftToken = GetDefineBindings( listWalk->leftToken->content )->leftToken;
              } // else: return a token
            } // if: defined bindings
            
            else {
              resultWalk->leftToken = listWalk->leftToken;
            } // else: not defined
          } // if: symbol
          
          else {
            resultWalk->leftToken = listWalk->leftToken;
          } // else: other type
        } // if: leftToken exist
        
        else {
          resultWalk->leftNode = listWalk->leftNode;
        } // else: get previous result
        
        listWalk = listWalk->rightNode;
        resultWalk->rightNode = new TreeStruct;
        resultWalk->rightNode->previousNode = resultWalk;
        resultWalk = resultWalk->rightNode;
        // initialization
        resultWalk->leftNode = NULL;
        resultWalk->rightNode = NULL;
        resultWalk->leftToken = NULL;
        resultWalk->rightToken = NULL;
        
        if ( listWalk->rightNode == NULL ) {
          if ( listWalk->leftToken ) {
            if ( listWalk->leftToken->tokenType == SYMBOL ) {
              if ( IsPrimitive( listWalk->leftToken ) ) {
                resultWalk->leftToken = listWalk->leftToken;
              } // if: is primitive
              
              else if ( !listWalk->leftToken->isQuoteResult ) {
                if ( GetDefineBindings( listWalk->leftToken->content )->leftNode ) {
                  resultWalk->leftNode = GetDefineBindings( listWalk->leftToken->content )->leftNode;
                } // if: return a node
                
                else {
                  resultWalk->leftToken = GetDefineBindings( listWalk->leftToken->content )->leftToken;
                } // else: return a token
              } // if: defined bindings
              
              else {
                resultWalk->leftToken = listWalk->leftToken;
              } // else: not defined
            } // if: symbol
            
            else {
              resultWalk->leftToken = listWalk->leftToken;
            } // else: other type
          } // if: leftToken exist
          
          else {
            resultWalk->leftNode = listWalk->leftNode;
          } // else: get previous result
        } // if: last node
      } // while: go through all the nodes on the right side
    } // else: more than one data
    
    result->hasNodeResult = true;
    result->nodeResult = resultRoot;
    return true;
  } // List()
  
  bool Quote( vector<TreeStruct *> arguments, ResultStruct *result ) {
    if ( arguments.front()->leftNode ) {
      result->hasNodeResult = true;
      result->nodeResult = arguments.front()->leftNode;
    } // if: has leftNode
    
    else {
      result->hasTokenResult = true;
      result->tokenResult = arguments.front()->leftToken;
    } // else: single quote case
    
    return true;
  } // Quote()
  
  bool Define( vector<TreeStruct *> arguments, ResultStruct *result ) {
    result->tokenResult->content = arguments.front()->leftToken->content + " defined";
    result->tokenResult->tokenType = STRING;
    result->hasTokenResult = true;
    
    for ( int i = 0 ; i < m_DefineBindingList.size() ; i++ ) {
      if ( arguments.front()->leftToken->content == m_DefineBindingList[i]->leftToken->content ) {
        m_DefineBindingList.erase( m_DefineBindingList.begin() + i );
      } // if: find pre-defined , delete it
      
      if ( NOT m_DefineBindingList.empty() ) {
        if ( arguments.back()->leftToken ) {
          if ( arguments.back()->leftToken->content == m_DefineBindingList[i]->leftToken->content ) {
            arguments.front()->rightNode = m_DefineBindingList[i]->rightNode;
          } // if: double define
        } // if: the defined as is a token, not a node
      } // if: define list is not empty
    } // for: find any pre-defined
    
    m_DefineBindingList.push_back( arguments.front() );
    return true;
  } // Define()
  
  bool Car( vector<TreeStruct *> arguments, ResultStruct *result ) {
    if ( arguments.front()->leftToken ) {
      if ( arguments.front()->leftToken->tokenType == SYMBOL ) {
        if ( arguments.front()->leftToken->isQuoteResult ) {
          string errorMessage = "ERROR (car with incorrect argument type) : ";
          errorMessage += arguments.front()->leftToken->content;
          SetError( INCORRECT_ARGUMENT_TYPE, errorMessage );
          return false;
        } // if: is a quote symbol
        
        else {
          string w_String = arguments.front()->leftToken->content;
          
          if ( GetDefineBindings( w_String )->leftNode ) {
            if ( GetDefineBindings( w_String )->leftNode->leftNode ) {
              result->hasNodeResult = true;
              result->nodeResult = GetDefineBindings( w_String )->leftNode->leftNode;
            } // if: defined has leftNode
            
            else if ( GetDefineBindings( w_String )->leftNode->leftToken ) {
              result->hasTokenResult = true;
              result->tokenResult = GetDefineBindings( w_String )->leftNode->leftToken;
            } // else if: defined has leftToken
            
            else {
              result->hasTokenResult = true;
              result->tokenResult->tokenType = NIL;
            } // else: has nothing at the left
          } // if: defined is a node
          
          else {
            string errorString = "ERROR (car with incorrect argument type) : ";
            errorString += GetDefineBindings( arguments.front()->leftToken->content )->leftToken->content;
            SetError( INCORRECT_ARGUMENT_TYPE, errorString );
            return false;
          } // else if: defined is a value
        } // else: not quote result
      } // if: token type is a symbol
      
      else {
        string errorString = "ERROR (car with incorrect argument type) : ";
        errorString += arguments.front()->leftToken->content;
        SetError( INCORRECT_ARGUMENT_TYPE, errorString );
        return false;
      } // else: token type isn't a symbol
    } // if: the argument is a token
    
    else {
      if ( arguments.front()->leftNode->leftToken ) {
        result->hasTokenResult = true;
        result->tokenResult = arguments.front()->leftNode->leftToken;
      } // if: leftNode has leftToken
      else if ( arguments.front()->leftNode->leftNode ) {
        result->hasNodeResult = true;
        result->nodeResult = arguments.front()->leftNode->leftNode;
      } // else if: leftNode has leftNode
      else {
        result->hasTokenResult = true;
        result->tokenResult->content = "nil";
        result->tokenResult->tokenType = NIL;
      } // else: leftNode has nothing on the left
    } // else: the argument is a node
    
    return true;
  } // Car()
  
  bool Cdr( vector<TreeStruct *> arguments, ResultStruct *result ) {
    if ( arguments.front()->leftToken ) {
      if ( arguments.front()->leftToken->tokenType == SYMBOL ) {
        if ( arguments.front()->leftToken->isQuoteResult ) {
          string errorMessage = "ERROR (cdr with incorrect argument type) : ";
          errorMessage += arguments.front()->leftToken->content;
          SetError( INCORRECT_ARGUMENT_TYPE, errorMessage );
          return false;
        } // if: is a quote symbol
        
        else {
          string w_String = arguments.front()->leftToken->content;
          
          if ( GetDefineBindings( w_String )->leftNode ) {
            if ( GetDefineBindings( w_String )->leftNode->rightNode ) {
              result->hasNodeResult = true;
              result->nodeResult = GetDefineBindings( w_String )->leftNode->rightNode;
            } // if: defined has leftNode
            
            else if ( GetDefineBindings( w_String )->leftNode->rightToken ) {
              result->hasTokenResult = true;
              result->tokenResult = GetDefineBindings( w_String )->leftNode->rightToken;
            } // else if: defined has leftToken
            
            else {
              result->hasTokenResult = true;
              result->tokenResult->tokenType = NIL;
            } // else: has nothing at the left
          } // if: defined is a node
          
          else {
            string errorString = "ERROR (car with incorrect argument type) : ";
            errorString += GetDefineBindings( arguments.front()->leftToken->content )->leftToken->content;
            SetError( INCORRECT_ARGUMENT_TYPE, errorString );
            return false;
          } // else if: defined is a value
        } // else: not quote result
      } // if: token type is a symbol
      
      else {
        string errorString = "ERROR (car with incorrect argument type) : ";
        errorString += arguments.front()->leftToken->content;
        SetError( INCORRECT_ARGUMENT_TYPE, errorString );
        return false;
      } // else: token type isn't a symbol
    } // if: the argument is a token
    
    else {
      if ( arguments.front()->leftNode->rightToken ) {
        result->hasTokenResult = true;
        result->tokenResult = arguments.front()->leftNode->rightToken;
      } // if: leftNode has leftToken
      else if ( arguments.front()->leftNode->rightNode ) {
        result->hasNodeResult = true;
        result->nodeResult = arguments.front()->leftNode->rightNode;
      } // else if: leftNode has leftNode
      else {
        result->hasTokenResult = true;
        result->tokenResult->content = "nil";
        result->tokenResult->tokenType = NIL;
      } // else: leftNode has nothing on the left
    } // else: the argument is a node
    
    return true;
  } // Cdr()
  
  bool IsAtom( vector<TreeStruct *> arguments, ResultStruct *result ) {
    result->hasTokenResult = true;
    
    if ( arguments.front()->leftToken ) {
      if ( arguments.front()->leftToken->tokenType == SYMBOL ) {
        if ( arguments.front()->leftToken->isQuoteResult ) {
          result->tokenResult->tokenType = T;
        } // if: quote result
        
        else {
          if ( GetDefineBindings( arguments.front()->leftToken->content )->leftToken ) {
            result->tokenResult->tokenType = T;
          } // if: is a token form
          
          else {
            result->tokenResult->tokenType = NIL;
          } // else: is a node form, which won't be an atom
        } // else: not quote result
      } // if: is symbol, find if there's any defined bindings
      
      else {
        result->tokenResult->tokenType = T;
      } // else if: check if the argument's leftToken is a atom or not
    } // if: argument is in a token form
    
    else {
      result->tokenResult->tokenType = NIL;
    } // else: argument is in a node form, check previous result
    
    if ( result->tokenResult->tokenType == T ) {
      result->tokenResult->content = "#t";
    } // if: the result is true, set the content
    
    else {
      result->tokenResult->content = "nil";
    } // else: the result is nil, set the content
    
    return true;
  } // IsAtom()
  
  bool IsPair( vector<TreeStruct *> arguments, ResultStruct *result ) {
    result->hasTokenResult = true;
    
    if ( arguments.front()->leftToken ) {
      if ( arguments.front()->leftToken->tokenType == SYMBOL ) {
        if ( arguments.front()->leftToken->isQuoteResult ) {
          result->tokenResult->tokenType = NIL;
        } // if: quote result
        
        else {
          if ( GetDefineBindings( arguments.front()->leftToken->content )->leftToken ) {
            result->tokenResult->tokenType = NIL;
          } // if: the defined binding is only a token, then it won't be a pair
          
          else {
            result->tokenResult->tokenType = T;
          } // else: the pre-defined binding is a node, then it is a pair
        } // else: not quote result
      } // if: argument's leftToken is a symbol
      
      else {
        result->tokenResult->tokenType = NIL;
      } // else: other types of token won't be a pair
    } // if: argument is a token
    
    else {
      result->tokenResult->tokenType = T;
    } // else: argument is a node(function)
    
    if ( result->tokenResult->tokenType == T ) {
      result->tokenResult->content = "#t";
    } // if: the result is true, set the content
    
    else {
      result->tokenResult->content = "nil";
    } // else: the result is nil, set the content
    
    return true;
  } // IsPair()
  
  bool IsList( vector<TreeStruct *> arguments, ResultStruct *result ) {
    result->hasTokenResult = true;
    
    if ( arguments.front()->leftToken ) {
      if ( arguments.front()->leftToken->tokenType == SYMBOL ) {
        if ( arguments.front()->leftToken->isQuoteResult ) {
          result->tokenResult->tokenType = NIL;
        } // if: quote result
        
        else {
          if ( GetDefineBindings( arguments.front()->leftToken->content )->leftNode ) {
            TreeStruct *defineWalk = GetDefineBindings( arguments.front()->leftToken->content )->leftNode;
            
            while ( defineWalk->rightNode ) {
              defineWalk = defineWalk->rightNode;
            } // while: go through the previous node
            
            if ( defineWalk->rightToken ) {
              result->tokenResult->tokenType = NIL;
            } // if: the rightmost node has a rightToken
            
            else {
              result->tokenResult->tokenType = T;
            } // else: the rightmost node has no rightToken
          } // if: defined binding is a node, check if is a list
          
          else {
            result->tokenResult->tokenType = NIL;
          } // else: defined binding is a token, which won't be a list
        } // else: not quote result
      } // if: the argument is a symbol token
      
      else {
        result->tokenResult->tokenType = NIL;
      } // else: the argument token is not a symbo, which won't be a list
    } // if: the argument has a leftToken
    
    else {
      TreeStruct *nodeWalk = arguments.front()->leftNode;
      
      while ( nodeWalk->rightNode ) {
        nodeWalk = nodeWalk->rightNode;
      } // while: go through the node
      
      if ( nodeWalk->rightToken ) {
        result->tokenResult->tokenType = NIL;
      } // if: has rightToken, not list
      
      else {
        result->tokenResult->tokenType = T;
      } // else: no rightToken, is a list
    } // else: the argument has a leftNode(function)
    
    if ( result->tokenResult->tokenType == T ) {
      result->tokenResult->content = "#t";
    } // if: the result is true, set the content
    
    else {
      result->tokenResult->content = "nil";
    } // else: the result is nil, set the content
    
    return true;
  } // IsList()
  
  bool IsNull( vector<TreeStruct *> arguments, ResultStruct *result ) {
    result->hasTokenResult = true;
    
    if ( arguments.front()->leftToken ) {
      if ( arguments.front()->leftToken->tokenType == SYMBOL ) {
        if ( arguments.front()->leftToken->isQuoteResult ) {
          result->tokenResult->tokenType = NIL;
        } // if: quote result
        
        else {
          if ( GetDefineBindings( arguments.front()->leftToken->content )->leftToken ) {
            if ( GetDefineBindings( arguments.front()->leftToken->content )->leftToken->tokenType == NIL ) {
              result->tokenResult->tokenType = T;
            } // if: the defined tokenType is nil
            
            else {
              result->tokenResult->tokenType = NIL;
            } // else: the defined tokenType is not nil
          } // if: defined-binding is token
          
          else {
            result->tokenResult->tokenType = NIL;
          } // else: defined-binding is node
        } // else: not quote result
      } // if: argument is a symbol
      
      else if ( arguments.front()->leftToken->tokenType == NIL ) {
        result->tokenResult->tokenType = T;
      } // else if: argument is a nil
      
      else {
        result->tokenResult->tokenType = NIL;
      } // else: not nil or symbol, won't be nil
    } // if: argument is a token
    
    else {
      result->tokenResult->tokenType = NIL;
    } // else: a node
    
    if ( result->tokenResult->tokenType == T ) {
      result->tokenResult->content = "#t";
    } // if: the result is true, set the content
    
    else {
      result->tokenResult->content = "nil";
    } // else: the result is nil, set the content
    
    return true;
  } // IsNull()
  
  bool IsInteger( vector<TreeStruct *> arguments, ResultStruct *result ) {
    result->hasTokenResult = true;
    
    if ( arguments.front()->leftToken ) {
      if ( arguments.front()->leftToken->tokenType == SYMBOL ) {
        if ( arguments.front()->leftToken->isQuoteResult ) {
          result->tokenResult->tokenType = NIL;
        } // if: quote result
        
        else {
          if ( GetDefineBindings( arguments.front()->leftToken->content )->leftToken ) {
            if ( GetDefineBindings( arguments.front()->leftToken->content )->leftToken->tokenType == INT ) {
              result->tokenResult->tokenType = T;
            } // if: the defined tokenType is int
            
            else {
              result->tokenResult->tokenType = NIL;
            } // else: the defined tokenType is not int
          } // if: defined-binding is token
          
          else {
            result->tokenResult->tokenType = NIL;
          } // else: defined-binding is node
        } // else: not quote result
      } // if: argument is a symbol
      
      else if ( arguments.front()->leftToken->tokenType == INT ) {
        result->tokenResult->tokenType = T;
      } // else if: argument is a int
      
      else {
        result->tokenResult->tokenType = NIL;
      } // else: not int or symbol, won't be int
    } // if: argument is a token
    
    else {
      result->tokenResult->tokenType = NIL;
    } // else: a node
    
    if ( result->tokenResult->tokenType == T ) {
      result->tokenResult->content = "#t";
    } // if: the result is true, set the content
    
    else {
      result->tokenResult->content = "nil";
    } // else: the result is nil, set the content
    
    return true;
  } // IsInteger()
  
  bool IsRealOrNumber( vector<TreeStruct *> arguments, ResultStruct *result ) {
    result->hasTokenResult = true;
    
    if ( arguments.front()->leftToken ) {
      if ( arguments.front()->leftToken->tokenType == SYMBOL ) {
        if ( arguments.front()->leftToken->isQuoteResult ) {
          result->tokenResult->tokenType = NIL;
        } // if: quote result
        
        else {
          if ( GetDefineBindings( arguments.front()->leftToken->content )->leftToken ) {
            if ( GetDefineBindings( arguments.front()->leftToken->content )->leftToken->tokenType == INT ||
                 GetDefineBindings( arguments.front()->leftToken->content )->leftToken->tokenType ==
                 FLOAT ) {
              result->tokenResult->tokenType = T;
            } // if: the defined tokenType is int or float
            
            else {
              result->tokenResult->tokenType = NIL;
            } // else: the defined tokenType is not int or float
          } // if: defined-binding is token
          
          else {
            result->tokenResult->tokenType = NIL;
          } // else: defined-binding is node
        } // else: not quote result
      } // if: argument is a symbol
      
      else if ( arguments.front()->leftToken->tokenType == INT ||
                arguments.front()->leftToken->tokenType == FLOAT ) {
        result->tokenResult->tokenType = T;
      } // else if: argument is a int or float
      
      else {
        result->tokenResult->tokenType = NIL;
      } // else: not int, float or symbol, won't be real
    } // if: argument is a token
    
    else {
      result->tokenResult->tokenType = NIL;
    } // else: a node
    
    if ( result->tokenResult->tokenType == T ) {
      result->tokenResult->content = "#t";
    } // if: the result is true, set the content
    
    else {
      result->tokenResult->content = "nil";
    } // else: the result is nil, set the content
    
    return true;
  } // IsRealOrNumber()
  
  bool IsString( vector<TreeStruct *> arguments, ResultStruct *result ) {
    result->hasTokenResult = true;
    
    if ( arguments.front()->leftToken ) {
      if ( arguments.front()->leftToken->tokenType == SYMBOL ) {
        if ( arguments.front()->leftToken->isQuoteResult ) {
          result->tokenResult->tokenType = NIL;
        } // if: quote result
        
        else {
          if ( GetDefineBindings( arguments.front()->leftToken->content )->leftToken ) {
            if ( GetDefineBindings( arguments.front()->leftToken->content )->leftToken->tokenType ==
                 STRING ) {
              result->tokenResult->tokenType = T;
            } // if: the defined tokenType is string
            
            else {
              result->tokenResult->tokenType = NIL;
            } // else: the defined tokenType is not string
          } // if: defined-binding is token
          
          else {
            result->tokenResult->tokenType = NIL;
          } // else: defined-binding is node
        } // else: not quote result
      } // if: argument is a symbol
      
      else if ( arguments.front()->leftToken->tokenType == STRING ) {
        result->tokenResult->tokenType = T;
      } // else if: argument is a string
      
      else {
        result->tokenResult->tokenType = NIL;
      } // else: not string or symbol, won't be real
    } // if: argument is a token
    
    else {
      result->tokenResult->tokenType = NIL;
    } // else: a node, check previous result
    
    if ( result->tokenResult->tokenType == T ) {
      result->tokenResult->content = "#t";
    } // if: the result is true, set the content
    
    else {
      result->tokenResult->content = "nil";
    } // else: the result is nil, set the content
    
    return true;
  } // IsString()
  
  bool IsBoolean( vector<TreeStruct *> arguments, ResultStruct *result ) {
    result->hasTokenResult = true;
    
    if ( arguments.front()->leftToken ) {
      if ( arguments.front()->leftToken->tokenType == SYMBOL ) {
        if ( arguments.front()->leftToken->isQuoteResult ) {
          result->tokenResult->tokenType = NIL;
        } // if: quote result
        
        else {
          if ( GetDefineBindings( arguments.front()->leftToken->content )->leftToken ) {
            if ( GetDefineBindings( arguments.front()->leftToken->content )->leftToken->tokenType == T ||
                 GetDefineBindings( arguments.front()->leftToken->content )->leftToken->tokenType == NIL ) {
              result->tokenResult->tokenType = T;
            } // if: the defined tokenType is t or nil
            
            else {
              result->tokenResult->tokenType = NIL;
            } // else: the defined tokenType is not t or nil
          } // if: defined-binding is token
          
          else {
            result->tokenResult->tokenType = NIL;
          } // else: defined-binding is node
        } // else: not quote result
      } // if: argument is a symbol
      
      else if ( arguments.front()->leftToken->tokenType == T ||
                arguments.front()->leftToken->tokenType == NIL ) {
        result->tokenResult->tokenType = T;
      } // else if: argument is a t or nil
      
      else {
        result->tokenResult->tokenType = NIL;
      } // else: not t, nil or symbol, won't be real
    } // if: argument is a token
    
    else {
      result->tokenResult->tokenType = NIL;
    } // else: a node
    
    if ( result->tokenResult->tokenType == T ) {
      result->tokenResult->content = "#t";
    } // if: the result is true, set the content
    
    else {
      result->tokenResult->content = "nil";
    } // else: the result is nil, set the content
    
    return true;
  } // IsBoolean()
  
  bool IsSymbol( vector<TreeStruct *> arguments, ResultStruct *result ) {
    result->hasTokenResult = true;
    
    if ( arguments.front()->leftToken ) {
      if ( arguments.front()->leftToken->tokenType == SYMBOL ) {
        if ( arguments.front()->leftToken->isQuoteResult ) {
          result->tokenResult->tokenType = T;
        } // if: quote result
        
        else {
          if ( FoundDefineBindings( arguments.front()->leftToken->content ) ) {
            if ( GetDefineBindings( arguments.front()->leftToken->content )->leftToken ) {
              TokenStruct *defined = GetDefineBindings( arguments.front()->leftToken->content )->leftToken;
              if ( defined->tokenType == SYMBOL ) {
                result->tokenResult->tokenType = T;
              } // if: the defined tokenType is symbol
              
              else {
                result->tokenResult->tokenType = NIL;
              } // else: the defined tokenType is not symbol
            } // if: defined-binding is token
            
            else {
              result->tokenResult->tokenType = NIL;
            } // else: defined-binding is node
          } // if: is a pre-defined symbol
          
          else {
            result->tokenResult->tokenType = T;
          } // else: not defined, but is still a symbol
        } // else: not quote result
      } // if: argument is a symbol
      
      else {
        result->tokenResult->tokenType = NIL;
      } // else: not symbol
    } // if: argument is a token
    
    else {
      result->tokenResult->tokenType = NIL;
    } // else: a node
    
    if ( result->tokenResult->tokenType == T ) {
      result->tokenResult->content = "#t";
    } // if: the result is true, set the content
    
    else {
      result->tokenResult->content = "nil";
    } // else: the result is nil, set the content
    
    return true;
  } // IsSymbol()
  
  bool Addition( vector<TreeStruct *> arguments, ResultStruct *result ) {
    bool isFloat = false;
    float answer = 0.0;
    TreeStruct *walk = arguments.front();
    
    while ( walk->rightNode ) {
      if ( walk->leftToken ) {
        if ( walk->leftToken->tokenType == FLOAT || walk->leftToken->tokenType == INT ) {
          if ( walk->leftToken->tokenType == FLOAT ) {
            isFloat = true;
          } // if: argument is float, set isFLoat
          
          answer += atof( walk->leftToken->content.c_str() );
        } // if: check if add any float
        
        else if ( walk->leftToken->tokenType == SYMBOL ) {
          if ( walk->leftToken->isQuoteResult ) {
            string errorMessage = "ERROR (+ with incorrect argument type) : ";
            errorMessage += walk->leftToken->content;
            SetError( INCORRECT_ARGUMENT_TYPE, errorMessage );
            return false;
          } // if: is a quote symbol
          
          else {
            if ( GetDefineBindings( walk->leftToken->content )->leftToken ) {
              TokenStruct *definedToken = GetDefineBindings( walk->leftToken->content )->leftToken;
              
              if ( definedToken->tokenType == FLOAT || definedToken->tokenType == INT ) {
                if ( definedToken->tokenType == FLOAT ) {
                  isFloat = true;
                } // if: argument is float, set isFLoat
                
                answer += atof( definedToken->content.c_str() );
              } // if: check if add any float
              
              else {
                string errorMessage = "ERROR (+ with incorrect argument type) : ";
                errorMessage += definedToken->content;
                SetError( INCORRECT_ARGUMENT_TYPE, errorMessage );
                return false;
              } // else: wrong type
            } // if: get defined token
            
            else {
              TreeStruct *definedNode = GetDefineBindings( walk->leftToken->content )->leftNode;
              stringstream errorMessage;
              errorMessage << "ERROR (+ with incorrect argument type) : ";
              int layer = 0;
              AddTreeString( definedNode, layer, false, errorMessage );
              
              if ( layer > 0 ) {
                while ( layer > 0 ) {
                  layer--;
                  errorMessage << string( layer, ' ' ) << ')' << endl;
                } // while: loop print right-paren
              } // if: layer still greater than zero
              
              SetError( INCORRECT_ARGUMENT_TYPE, errorMessage.str() );
              return false;
            } // else: get defined node, can't evaluate
          } // else: not quote result
        } // else if: symbol, check defined or not
        
        else {
          string errorMessage = "ERROR (+ with incorrect argument type) : ";
          errorMessage += walk->leftToken->content;
          SetError( INCORRECT_ARGUMENT_TYPE, errorMessage );
          return false;
        } // else: wrong argument type
      } // if: argument is token
      
      else if ( walk->leftNode ) {
        TreeStruct *errorNode = GetDefineBindings( walk->leftToken->content )->leftNode;
        stringstream errorMessage;
        errorMessage << "ERROR (+ with incorrect argument type) : ";
        int layer = 0;
        AddTreeString( errorNode, layer, false, errorMessage );
        
        if ( layer > 0 ) {
          while ( layer > 0 ) {
            layer--;
            errorMessage << string( layer, ' ' ) << ')' << endl;
          } // while: loop print right-paren
        } // if: layer still greater than zero
        
        SetError( INCORRECT_ARGUMENT_TYPE, errorMessage.str() );
        return false;
      } // else if: argument is node
      
      walk = walk->rightNode;
      
      if ( walk->rightNode == NULL ) {
        if ( walk->leftToken ) {
          if ( walk->leftToken->tokenType == FLOAT || walk->leftToken->tokenType == INT ) {
            if ( walk->leftToken->tokenType == FLOAT ) {
              isFloat = true;
            } // if: argument is float, set isFLoat
            
            answer += atof( walk->leftToken->content.c_str() );
          } // if: check if add any float
          
          else if ( walk->leftToken->tokenType == SYMBOL ) {
            if ( walk->leftToken->isQuoteResult ) {
              string errorMessage = "ERROR (+ with incorrect argument type) : ";
              errorMessage += walk->leftToken->content;
              SetError( INCORRECT_ARGUMENT_TYPE, errorMessage );
              return false;
            } // if: is a quote symbol
            
            else {
              if ( GetDefineBindings( walk->leftToken->content )->leftToken ) {
                TokenStruct *definedToken = GetDefineBindings( walk->leftToken->content )->leftToken;
                
                if ( definedToken->tokenType == FLOAT || definedToken->tokenType == INT ) {
                  if ( definedToken->tokenType == FLOAT ) {
                    isFloat = true;
                  } // if: argument is float, set isFLoat
                  
                  answer += atof( definedToken->content.c_str() );
                } // if: check if add any float
                
                else {
                  string errorMessage = "ERROR (+ with incorrect argument type) : ";
                  errorMessage += definedToken->content;
                  SetError( INCORRECT_ARGUMENT_TYPE, errorMessage );
                  return false;
                } // else: wrong type
              } // if: get defined token
              
              else {
                TreeStruct *definedNode = GetDefineBindings( walk->leftToken->content )->leftNode;
                stringstream errorMessage;
                errorMessage << "ERROR (+ with incorrect argument type) : ";
                int layer = 0;
                AddTreeString( definedNode, layer, false, errorMessage );
                
                if ( layer > 0 ) {
                  while ( layer > 0 ) {
                    layer--;
                    errorMessage << string( layer, ' ' ) << ')' << endl;
                  } // while: loop print right-paren
                } // if: layer still greater than zero
                
                SetError( INCORRECT_ARGUMENT_TYPE, errorMessage.str() );
                return false;
              } // else: get defined node, can't evaluate
            } // else: not quote result
          } // else if: symbol, check defined or not
          
          else {
            string errorMessage = "ERROR (+ with incorrect argument type) : ";
            errorMessage += walk->leftToken->content;
            SetError( INCORRECT_ARGUMENT_TYPE, errorMessage );
            return false;
          } // else: wrong argument type
        } // if: argument is token
        
        else if ( walk->leftNode ) {
          TreeStruct *errorNode = GetDefineBindings( walk->leftToken->content )->leftNode;
          stringstream errorMessage;
          errorMessage << "ERROR (+ with incorrect argument type) : ";
          int layer = 0;
          AddTreeString( errorNode, layer, false, errorMessage );
          
          if ( layer > 0 ) {
            while ( layer > 0 ) {
              layer--;
              errorMessage << string( layer, ' ' ) << ')' << endl;
            } // while: loop print right-paren
          } // if: layer still greater than zero
          
          SetError( INCORRECT_ARGUMENT_TYPE, errorMessage.str() );
          return false;
        } // else if: argument is node
      } // if: add up the last argument
    } // while: add up the arguments
    
    result->hasTokenResult = true;
    
    if ( isFloat ) {
      result->tokenResult->tokenType = FLOAT;
      stringstream toString;
      toString << answer;
      result->tokenResult->content = toString.str();
    } // if: isFloat
    
    else {
      result->tokenResult->tokenType = INT;
      int intAnswer = ( int ) answer;
      stringstream toString;
      toString << intAnswer;
      result->tokenResult->content = toString.str();
    } // else: notFloat
    
    return true;
  } // Addition()
  
  bool Subtraction( vector<TreeStruct *> arguments, ResultStruct *result ) {
    bool isFloat = false;
    float answer = 0.0;
    TreeStruct *walk = arguments.front();
    bool isFirstNum = true;
    
    while ( walk->rightNode ) {
      if ( walk->leftToken ) {
        if ( walk->leftToken->tokenType == FLOAT || walk->leftToken->tokenType == INT ) {
          if ( walk->leftToken->tokenType == FLOAT ) {
            isFloat = true;
          } // if: argument is float, set isFLoat
          
          if ( isFirstNum ) {
            answer = atof( walk->leftToken->content.c_str() );
            isFirstNum = false;
          } // if: firstNum
          
          else {
            answer -= atof( walk->leftToken->content.c_str() );
          } // else: not firstNum
        } // if: check if add any float
        
        else if ( walk->leftToken->tokenType == SYMBOL ) {
          if ( walk->leftToken->isQuoteResult ) {
            string errorMessage = "ERROR (+ with incorrect argument type) : ";
            errorMessage += walk->leftToken->content;
            SetError( INCORRECT_ARGUMENT_TYPE, errorMessage );
            return false;
          } // if: is a quote symbol
          
          else {
            if ( GetDefineBindings( walk->leftToken->content )->leftToken ) {
              TokenStruct *definedToken = GetDefineBindings( walk->leftToken->content )->leftToken;
              
              if ( definedToken->tokenType == FLOAT || definedToken->tokenType == INT ) {
                if ( definedToken->tokenType == FLOAT ) {
                  isFloat = true;
                } // if: argument is float, set isFLoat
                
                if ( isFirstNum ) {
                  answer = atof( walk->leftToken->content.c_str() );
                  isFirstNum = false;
                } // if: firstNum
                
                else {
                  answer -= atof( walk->leftToken->content.c_str() );
                } // else: not firstNum
              } // if: check if add any float
              
              else {
                string errorMessage = "ERROR (- with incorrect argument type) : ";
                errorMessage += definedToken->content;
                SetError( INCORRECT_ARGUMENT_TYPE, errorMessage );
                return false;
              } // else: wrong type
            } // if: get defined token
            
            else {
              TreeStruct *definedNode = GetDefineBindings( walk->leftToken->content )->leftNode;
              stringstream errorMessage;
              errorMessage << "ERROR (- with incorrect argument type) : ";
              int layer = 0;
              AddTreeString( definedNode, layer, false, errorMessage );
              
              if ( layer > 0 ) {
                while ( layer > 0 ) {
                  layer--;
                  errorMessage << string( layer, ' ' ) << ')' << endl;
                } // while: loop print right-paren
              } // if: layer still greater than zero
              
              SetError( INCORRECT_ARGUMENT_TYPE, errorMessage.str() );
              return false;
            } // else: get defined node, can't evaluate
          } // else: not quote result
        } // else if: symbol, check defined or not
        
        else {
          string errorMessage = "ERROR (- with incorrect argument type) : ";
          errorMessage += walk->leftToken->content;
          SetError( INCORRECT_ARGUMENT_TYPE, errorMessage );
          return false;
        } // else: wrong argument type
      } // if: argument is token
      
      else if ( walk->leftNode ) {
        TreeStruct *errorNode = GetDefineBindings( walk->leftToken->content )->leftNode;
        stringstream errorMessage;
        errorMessage << "ERROR (- with incorrect argument type) : ";
        int layer = 0;
        AddTreeString( errorNode, layer, false, errorMessage );
        
        if ( layer > 0 ) {
          while ( layer > 0 ) {
            layer--;
            errorMessage << string( layer, ' ' ) << ')' << endl;
          } // while: loop print right-paren
        } // if: layer still greater than zero
        
        SetError( INCORRECT_ARGUMENT_TYPE, errorMessage.str() );
        return false;
      } // else if: argument is node
      
      walk = walk->rightNode;
      
      if ( walk->rightNode == NULL ) {
        if ( walk->leftToken ) {
          if ( walk->leftToken->tokenType == FLOAT || walk->leftToken->tokenType == INT ) {
            if ( walk->leftToken->tokenType == FLOAT ) {
              isFloat = true;
            } // if: argument is float, set isFLoat
            
            answer -= atof( walk->leftToken->content.c_str() );
          } // if: check if add any float
          
          else if ( walk->leftToken->tokenType == SYMBOL ) {
            if ( walk->leftToken->isQuoteResult ) {
              string errorMessage = "ERROR (+ with incorrect argument type) : ";
              errorMessage += walk->leftToken->content;
              SetError( INCORRECT_ARGUMENT_TYPE, errorMessage );
              return false;
            } // if: is a quote symbol
            
            else {
              if ( GetDefineBindings( walk->leftToken->content )->leftToken ) {
                TokenStruct *definedToken = GetDefineBindings( walk->leftToken->content )->leftToken;
                
                if ( definedToken->tokenType == FLOAT || definedToken->tokenType == INT ) {
                  if ( definedToken->tokenType == FLOAT ) {
                    isFloat = true;
                  } // if: argument is float, set isFLoat
                  
                  answer -= atof( definedToken->content.c_str() );
                } // if: check if add any float
                
                else {
                  string errorMessage = "ERROR (- with incorrect argument type) : ";
                  errorMessage += definedToken->content;
                  SetError( INCORRECT_ARGUMENT_TYPE, errorMessage );
                  return false;
                } // else: wrong type
              } // if: get defined token
              
              else {
                TreeStruct *definedNode = GetDefineBindings( walk->leftToken->content )->leftNode;
                stringstream errorMessage;
                errorMessage << "ERROR (- with incorrect argument type) : ";
                int layer = 0;
                AddTreeString( definedNode, layer, false, errorMessage );
                
                if ( layer > 0 ) {
                  while ( layer > 0 ) {
                    layer--;
                    errorMessage << string( layer, ' ' ) << ')' << endl;
                  } // while: loop print right-paren
                } // if: layer still greater than zero
                
                SetError( INCORRECT_ARGUMENT_TYPE, errorMessage.str() );
                return false;
              } // else: get defined node, can't evaluate
            } // else: not quote result
          } // else if: symbol, check defined or not
          
          else {
            string errorMessage = "ERROR (- with incorrect argument type) : ";
            errorMessage += walk->leftToken->content;
            SetError( INCORRECT_ARGUMENT_TYPE, errorMessage );
            return false;
          } // else: wrong argument type
        } // if: argument is token
        
        else if ( walk->leftNode ) {
          TreeStruct *errorNode = GetDefineBindings( walk->leftToken->content )->leftNode;
          stringstream errorMessage;
          errorMessage << "ERROR (- with incorrect argument type) : ";
          int layer = 0;
          AddTreeString( errorNode, layer, false, errorMessage );
          
          if ( layer > 0 ) {
            while ( layer > 0 ) {
              layer--;
              errorMessage << string( layer, ' ' ) << ')' << endl;
            } // while: loop print right-paren
          } // if: layer still greater than zero
          
          SetError( INCORRECT_ARGUMENT_TYPE, errorMessage.str() );
          return false;
        } // else if: argument is node
      } // if: add up the last argument
    } // while: add up the arguments
    
    result->hasTokenResult = true;
    
    if ( isFloat ) {
      result->tokenResult->tokenType = FLOAT;
      stringstream toString;
      toString << answer;
      result->tokenResult->content = toString.str();
    } // if: isFloat
    
    else {
      result->tokenResult->tokenType = INT;
      int intAnswer = ( int ) answer;
      stringstream toString;
      toString << intAnswer;
      result->tokenResult->content = toString.str();
    } // else: notFloat
    
    return true;
  } // Subtraction()
  
  bool Mutiplication( vector<TreeStruct *> arguments, ResultStruct *result ) {
    bool isFloat = false;
    float answer = 1.0;
    TreeStruct *walk = arguments.front();
    
    while ( walk->rightNode ) {
      if ( walk->leftToken ) {
        if ( walk->leftToken->tokenType == FLOAT || walk->leftToken->tokenType == INT ) {
          if ( walk->leftToken->tokenType == FLOAT ) {
            isFloat = true;
          } // if: argument is float, set isFLoat
          
          answer *= atof( walk->leftToken->content.c_str() );
        } // if: check if add any float
        
        else if ( walk->leftToken->tokenType == SYMBOL ) {
          if ( walk->leftToken->isQuoteResult ) {
            string errorMessage = "ERROR (+ with incorrect argument type) : ";
            errorMessage += walk->leftToken->content;
            SetError( INCORRECT_ARGUMENT_TYPE, errorMessage );
            return false;
          } // if: is a quote symbol
          
          else {
            if ( GetDefineBindings( walk->leftToken->content )->leftToken ) {
              TokenStruct *definedToken = GetDefineBindings( walk->leftToken->content )->leftToken;
              
              if ( definedToken->tokenType == FLOAT || definedToken->tokenType == INT ) {
                if ( definedToken->tokenType == FLOAT ) {
                  isFloat = true;
                } // if: argument is float, set isFLoat
                
                answer *= atof( definedToken->content.c_str() );
              } // if: check if add any float
              
              else {
                string errorMessage = "ERROR (* with incorrect argument type) : ";
                errorMessage += definedToken->content;
                SetError( INCORRECT_ARGUMENT_TYPE, errorMessage );
                return false;
              } // else: wrong type
            } // if: get defined token
            
            else {
              TreeStruct *definedNode = GetDefineBindings( walk->leftToken->content )->leftNode;
              stringstream errorMessage;
              errorMessage << "ERROR (* with incorrect argument type) : ";
              int layer = 0;
              AddTreeString( definedNode, layer, false, errorMessage );
              
              if ( layer > 0 ) {
                while ( layer > 0 ) {
                  layer--;
                  errorMessage << string( layer, ' ' ) << ')' << endl;
                } // while: loop print right-paren
              } // if: layer still greater than zero
              
              SetError( INCORRECT_ARGUMENT_TYPE, errorMessage.str() );
              return false;
            } // else: get defined node, can't evaluate
          } // else: not quote result
        } // else if: symbol, check defined or not
        
        else {
          string errorMessage = "ERROR (* with incorrect argument type) : ";
          errorMessage += walk->leftToken->content;
          SetError( INCORRECT_ARGUMENT_TYPE, errorMessage );
          return false;
        } // else: wrong argument type
      } // if: argument is token
      
      else if ( walk->leftNode ) {
        TreeStruct *errorNode = GetDefineBindings( walk->leftToken->content )->leftNode;
        stringstream errorMessage;
        errorMessage << "ERROR (* with incorrect argument type) : ";
        int layer = 0;
        AddTreeString( errorNode, layer, false, errorMessage );
        
        if ( layer > 0 ) {
          while ( layer > 0 ) {
            layer--;
            errorMessage << string( layer, ' ' ) << ')' << endl;
          } // while: loop print right-paren
        } // if: layer still greater than zero
        
        SetError( INCORRECT_ARGUMENT_TYPE, errorMessage.str() );
        return false;
      } // else if: argument is node
      
      walk = walk->rightNode;
      
      if ( walk->rightNode == NULL ) {
        if ( walk->leftToken ) {
          if ( walk->leftToken->tokenType == FLOAT || walk->leftToken->tokenType == INT ) {
            if ( walk->leftToken->tokenType == FLOAT ) {
              isFloat = true;
            } // if: argument is float, set isFLoat
            
            answer *= atof( walk->leftToken->content.c_str() );
          } // if: check if add any float
          
          else if ( walk->leftToken->tokenType == SYMBOL ) {
            if ( walk->leftToken->isQuoteResult ) {
              string errorMessage = "ERROR (+ with incorrect argument type) : ";
              errorMessage += walk->leftToken->content;
              SetError( INCORRECT_ARGUMENT_TYPE, errorMessage );
              return false;
            } // if: is a quote symbol
            
            else {
              if ( GetDefineBindings( walk->leftToken->content )->leftToken ) {
                TokenStruct *definedToken = GetDefineBindings( walk->leftToken->content )->leftToken;
                
                if ( definedToken->tokenType == FLOAT || definedToken->tokenType == INT ) {
                  if ( definedToken->tokenType == FLOAT ) {
                    isFloat = true;
                  } // if: argument is float, set isFLoat
                  
                  answer *= atof( definedToken->content.c_str() );
                } // if: check if add any float
                
                else {
                  string errorMessage = "ERROR (* with incorrect argument type) : ";
                  errorMessage += definedToken->content;
                  SetError( INCORRECT_ARGUMENT_TYPE, errorMessage );
                  return false;
                } // else: wrong type
              } // if: get defined token
              
              else {
                TreeStruct *definedNode = GetDefineBindings( walk->leftToken->content )->leftNode;
                stringstream errorMessage;
                errorMessage << "ERROR (* with incorrect argument type) : ";
                int layer = 0;
                AddTreeString( definedNode, layer, false, errorMessage );
                
                if ( layer > 0 ) {
                  while ( layer > 0 ) {
                    layer--;
                    errorMessage << string( layer, ' ' ) << ')' << endl;
                  } // while: loop print right-paren
                } // if: layer still greater than zero
                
                SetError( INCORRECT_ARGUMENT_TYPE, errorMessage.str() );
                return false;
              } // else: get defined node, can't evaluate
            } // else: not quote result
          } // else if: symbol, check defined or not
          
          else {
            string errorMessage = "ERROR (* with incorrect argument type) : ";
            errorMessage += walk->leftToken->content;
            SetError( INCORRECT_ARGUMENT_TYPE, errorMessage );
            return false;
          } // else: wrong argument type
        } // if: argument is token
        
        else if ( walk->leftNode ) {
          TreeStruct *errorNode = GetDefineBindings( walk->leftToken->content )->leftNode;
          stringstream errorMessage;
          errorMessage << "ERROR (* with incorrect argument type) : ";
          int layer = 0;
          AddTreeString( errorNode, layer, false, errorMessage );
          
          if ( layer > 0 ) {
            while ( layer > 0 ) {
              layer--;
              errorMessage << string( layer, ' ' ) << ')' << endl;
            } // while: loop print right-paren
          } // if: layer still greater than zero
          
          SetError( INCORRECT_ARGUMENT_TYPE, errorMessage.str() );
          return false;
        } // else if: argument is node
      } // if: add up the last argument
    } // while: add up the arguments
    
    result->hasTokenResult = true;
    
    if ( isFloat ) {
      result->tokenResult->tokenType = FLOAT;
      stringstream toString;
      toString << answer;
      result->tokenResult->content = toString.str();
    } // if: isFloat
    
    else {
      result->tokenResult->tokenType = INT;
      int intAnswer = ( int ) answer;
      stringstream toString;
      toString << intAnswer;
      result->tokenResult->content = toString.str();
    } // else: notFloat
    
    return true;
  } // Mutiplication()
  
  bool Division( vector<TreeStruct *> arguments, ResultStruct *result ) {
    bool isFloat = false;
    float answer = 0.0;
    TreeStruct *walk = arguments.front();
    bool isFirstNum = true;
    
    while ( walk->rightNode ) {
      if ( walk->leftToken ) {
        if ( walk->leftToken->tokenType == FLOAT || walk->leftToken->tokenType == INT ) {
          if ( walk->leftToken->tokenType == FLOAT ) {
            isFloat = true;
          } // if: argument is float, set isFLoat
          
          if ( isFirstNum ) {
            answer = atof( walk->leftToken->content.c_str() );
            isFirstNum = false;
          } // if: firstNum
          
          else {
            if ( atof( walk->leftToken->content.c_str() ) == 0 ) {
              string errorMessage = "ERROR (division by zero) : /";
              SetError( DIVISION_BY_ZERO, errorMessage );
              return false;
            } // if: divide by zero
            
            answer /= atof( walk->leftToken->content.c_str() );
          } // else: calculate
        } // if: check if add any float
        
        else if ( walk->leftToken->tokenType == SYMBOL ) {
          if ( walk->leftToken->isQuoteResult ) {
            string errorMessage = "ERROR (+ with incorrect argument type) : ";
            errorMessage += walk->leftToken->content;
            SetError( INCORRECT_ARGUMENT_TYPE, errorMessage );
            return false;
          } // if: is a quote symbol
          
          else {
            if ( GetDefineBindings( walk->leftToken->content )->leftToken ) {
              TokenStruct *definedToken = GetDefineBindings( walk->leftToken->content )->leftToken;
              
              if ( definedToken->tokenType == FLOAT || definedToken->tokenType == INT ) {
                if ( definedToken->tokenType == FLOAT ) {
                  isFloat = true;
                } // if: argument is float, set isFLoat
                
                if ( isFirstNum ) {
                  answer = atof( walk->leftToken->content.c_str() );
                  isFirstNum = false;
                } // if: firstNum
                
                else {
                  if ( atof( walk->leftToken->content.c_str() ) == 0 ) {
                    string errorMessage = "ERROR (division by zero) : /";
                    SetError( DIVISION_BY_ZERO, errorMessage );
                    return false;
                  } // if: divide by zero
                  
                  answer /= atof( walk->leftToken->content.c_str() );
                } // else: calculate
              } // if: check if add any float
              
              else {
                string errorMessage = "ERROR (/ with incorrect argument type) : ";
                errorMessage += definedToken->content;
                SetError( INCORRECT_ARGUMENT_TYPE, errorMessage );
                return false;
              } // else: wrong type
            } // if: get defined token
            
            else {
              TreeStruct *definedNode = GetDefineBindings( walk->leftToken->content )->leftNode;
              stringstream errorMessage;
              errorMessage << "ERROR (/ with incorrect argument type) : ";
              int layer = 0;
              AddTreeString( definedNode, layer, false, errorMessage );
              
              if ( layer > 0 ) {
                while ( layer > 0 ) {
                  layer--;
                  errorMessage << string( layer, ' ' ) << ')' << endl;
                } // while: loop print right-paren
              } // if: layer still greater than zero
              
              SetError( INCORRECT_ARGUMENT_TYPE, errorMessage.str() );
              return false;
            } // else: get defined node, can't evaluate
          } // else: not quote result
        } // else if: symbol, check defined or not
        
        else {
          string errorMessage = "ERROR (/ with incorrect argument type) : ";
          errorMessage += walk->leftToken->content;
          SetError( INCORRECT_ARGUMENT_TYPE, errorMessage );
          return false;
        } // else: wrong argument type
      } // if: argument is token
      
      else if ( walk->leftNode ) {
        TreeStruct *errorNode = GetDefineBindings( walk->leftToken->content )->leftNode;
        stringstream errorMessage;
        errorMessage << "ERROR (/ with incorrect argument type) : ";
        int layer = 0;
        AddTreeString( errorNode, layer, false, errorMessage );
        
        if ( layer > 0 ) {
          while ( layer > 0 ) {
            layer--;
            errorMessage << string( layer, ' ' ) << ')' << endl;
          } // while: loop print right-paren
        } // if: layer still greater than zero
        
        SetError( INCORRECT_ARGUMENT_TYPE, errorMessage.str() );
        return false;
      } // else if: argument is node
      
      walk = walk->rightNode;
      
      if ( walk->rightNode == NULL ) {
        if ( walk->leftToken ) {
          if ( walk->leftToken->tokenType == FLOAT || walk->leftToken->tokenType == INT ) {
            if ( walk->leftToken->tokenType == FLOAT ) {
              isFloat = true;
            } // if: argument is float, set isFLoat
            
            if ( atof( walk->leftToken->content.c_str() ) == 0 ) {
              string errorMessage = "ERROR (division by zero) : /";
              SetError( DIVISION_BY_ZERO, errorMessage );
              return false;
            } // if: divide by zero
            
            answer /= atof( walk->leftToken->content.c_str() );
          } // if: check if add any float
          
          else if ( walk->leftToken->tokenType == SYMBOL ) {
            if ( walk->leftToken->isQuoteResult ) {
              string errorMessage = "ERROR (+ with incorrect argument type) : ";
              errorMessage += walk->leftToken->content;
              SetError( INCORRECT_ARGUMENT_TYPE, errorMessage );
              return false;
            } // if: is a quote symbol
            
            else {
              if ( GetDefineBindings( walk->leftToken->content )->leftToken ) {
                TokenStruct *definedToken = GetDefineBindings( walk->leftToken->content )->leftToken;
                
                if ( definedToken->tokenType == FLOAT || definedToken->tokenType == INT ) {
                  if ( definedToken->tokenType == FLOAT ) {
                    isFloat = true;
                  } // if: argument is float, set isFLoat
                  
                  if ( atof( definedToken->content.c_str() ) == 0 ) {
                    string errorMessage = "ERROR (division by zero) : /";
                    SetError( DIVISION_BY_ZERO, errorMessage );
                    return false;
                  } // if: divide by zero
                  
                  answer /= atof( definedToken->content.c_str() );
                } // if: check if add any float
                
                else {
                  string errorMessage = "ERROR (/ with incorrect argument type) : ";
                  errorMessage += definedToken->content;
                  SetError( INCORRECT_ARGUMENT_TYPE, errorMessage );
                  return false;
                } // else: wrong type
              } // if: get defined token
              
              else {
                TreeStruct *definedNode = GetDefineBindings( walk->leftToken->content )->leftNode;
                stringstream errorMessage;
                errorMessage << "ERROR (/ with incorrect argument type) : ";
                int layer = 0;
                AddTreeString( definedNode, layer, false, errorMessage );
                
                if ( layer > 0 ) {
                  while ( layer > 0 ) {
                    layer--;
                    errorMessage << string( layer, ' ' ) << ')' << endl;
                  } // while: loop print right-paren
                } // if: layer still greater than zero
                
                SetError( INCORRECT_ARGUMENT_TYPE, errorMessage.str() );
                return false;
              } // else: get defined node, can't evaluate
            } // else: not quote result
          } // else if: symbol, check defined or not
          
          else {
            string errorMessage = "ERROR (/ with incorrect argument type) : ";
            errorMessage += walk->leftToken->content;
            SetError( INCORRECT_ARGUMENT_TYPE, errorMessage );
            return false;
          } // else: wrong argument type
        } // if: argument is token
        
        else if ( walk->leftNode ) {
          TreeStruct *errorNode = GetDefineBindings( walk->leftToken->content )->leftNode;
          stringstream errorMessage;
          errorMessage << "ERROR (/ with incorrect argument type) : ";
          int layer = 0;
          AddTreeString( errorNode, layer, false, errorMessage );
          
          if ( layer > 0 ) {
            while ( layer > 0 ) {
              layer--;
              errorMessage << string( layer, ' ' ) << ')' << endl;
            } // while: loop print right-paren
          } // if: layer still greater than zero
          
          SetError( INCORRECT_ARGUMENT_TYPE, errorMessage.str() );
          return false;
        } // else if: argument is node
      } // if: add up the last argument
    } // while: add up the arguments
    
    result->hasTokenResult = true;
    
    if ( isFloat ) {
      result->tokenResult->tokenType = FLOAT;
      stringstream toString;
      toString << answer;
      result->tokenResult->content = toString.str();
    } // if: isFloat
    
    else {
      result->tokenResult->tokenType = INT;
      int intAnswer = ( int ) answer;
      stringstream toString;
      toString << intAnswer;
      result->tokenResult->content = toString.str();
    } // else: notFloat
    
    return true;
  } // Division()
  
  bool Not( vector<TreeStruct *> arguments, ResultStruct *result ) {
    result->hasTokenResult = true;
    
    if ( arguments.front()->leftToken ) {
      if ( arguments.front()->leftToken->tokenType == SYMBOL ) {
        if ( arguments.front()->leftToken->isQuoteResult ) {
          result->tokenResult->tokenType = NIL;
        } // if: quote result
        
        else {
          if ( GetDefineBindings( arguments.front()->leftToken->content )->leftNode ) {
            result->tokenResult->tokenType = NIL;
          } // if: get defined node, cant evaluate
          
          else if ( GetDefineBindings( arguments.front()->leftToken->content )->leftToken ) {
            TokenStruct *defined = GetDefineBindings( arguments.front()->leftToken->content )->leftToken;
            
            if ( defined->tokenType == NIL ) {
              result->tokenResult->tokenType = T;
            } // else if: nil, return T
            
            else {
              result->tokenResult->tokenType = NIL;
            } // else: wrong argument type
          } // else if: get defined token, check tokenType
        } // else: not quote case
      } // if: symbol, find pre-defined symbol
      
      else if ( arguments.front()->leftToken->tokenType == NIL ) {
        result->tokenResult->tokenType = T;
      } // else if: nil, return T
      
      else {
        result->tokenResult->tokenType = NIL;
      } // else: wrong argument type
    } // if: argument is a token
    
    else {
      result->tokenResult->tokenType = NIL;
    } // else: argument is a node
    
    if ( result->tokenResult->tokenType == T ) {
      result->tokenResult->content = "#t";
    } // if: the result is true, set the content
    
    else {
      result->tokenResult->content = "nil";
    } // else: the result is nil, set the content
    
    return true;
  } // Not()
  
  bool And( vector<TreeStruct *> arguments, ResultStruct *result ) {
    TreeStruct *walk = arguments.front();
    
    while ( walk->rightNode ) {
      if ( walk->leftToken ) {
        if ( walk->leftToken->tokenType == NIL ) {
          if ( result->hasNodeResult ) {
            result->hasNodeResult = false;
            result->nodeResult = NULL;
          } // if: clear previous result
          
          result->hasTokenResult = true;
          result->tokenResult = walk->leftToken;
          return true;
        } // if: nil
        
        else if ( walk->leftToken->tokenType == SYMBOL ) {
          if ( walk->leftToken->isQuoteResult ) {
            if ( result->hasNodeResult ) {
              result->hasNodeResult = false;
              result->nodeResult = NULL;
            } // if: clear previous result
            
            result->hasTokenResult = true;
            result->tokenResult = walk->leftToken;
          } // if: quote result
          
          else {
            if ( GetDefineBindings( walk->leftToken->content )->leftToken ) {
              TokenStruct *definedToken = GetDefineBindings( walk->leftToken->content )->leftToken;
              
              if ( definedToken->tokenType == NIL ) {
                if ( result->hasNodeResult ) {
                  result->hasNodeResult = false;
                  result->nodeResult = NULL;
                } // if: clear previous result
                
                result->hasTokenResult = true;
                result->tokenResult = walk->leftToken;
                return true;
              } // if: nil
              
              else {
                if ( result->hasNodeResult ) {
                  result->hasNodeResult = false;
                  result->nodeResult = NULL;
                } // if: clear previous result
                
                result->hasTokenResult = true;
                result->tokenResult = definedToken;
              } // else: other types
            } // if: get defined token
            
            else {
              if ( result->hasTokenResult ) {
                result->hasTokenResult = false;
                result->tokenResult = NULL;
              } // if: clear previous result
              
              result->hasNodeResult = true;
              result->nodeResult = GetDefineBindings( walk->leftToken->content )->leftNode;
            } // else: get defined node
          } // else: not quote result
        } // else if: symbol, check defined or not
        
        else {
          if ( result->hasNodeResult ) {
            result->hasNodeResult = false;
            result->nodeResult = NULL;
          } // if: clear previous result
          
          result->hasTokenResult = true;
          result->tokenResult = walk->leftToken;
        } // else: other types
      } // if: the argument is a token
      
      else if ( walk->leftNode ) {
        if ( result->hasTokenResult ) {
          result->hasTokenResult = false;
          result->tokenResult = NULL;
        } // if: clear previous result
        
        result->hasNodeResult = true;
        result->nodeResult = walk->leftNode;
      } // else if: the argument is a node
      
      walk = walk->rightNode;
      
      if ( walk->rightNode == NULL ) {
        if ( walk->leftToken ) {
          if ( walk->leftToken->tokenType == NIL ) {
            if ( result->hasNodeResult ) {
              result->hasNodeResult = false;
              result->nodeResult = NULL;
            } // if: clear previous result
            
            result->hasTokenResult = true;
            result->tokenResult = walk->leftToken;
            return true;
          } // if: nil
          
          else if ( walk->leftToken->tokenType == SYMBOL ) {
            if ( walk->leftToken->isQuoteResult ) {
              if ( result->hasNodeResult ) {
                result->hasNodeResult = false;
                result->nodeResult = NULL;
              } // if: clear previous result
              
              result->hasTokenResult = true;
              result->tokenResult = walk->leftToken;
            } // if: quote result
            
            else {
              if ( GetDefineBindings( walk->leftToken->content )->leftToken ) {
                TokenStruct *definedToken = GetDefineBindings( walk->leftToken->content )->leftToken;
                
                if ( definedToken->tokenType == NIL ) {
                  if ( result->hasNodeResult ) {
                    result->hasNodeResult = false;
                    result->nodeResult = NULL;
                  } // if: clear previous result
                  
                  result->hasTokenResult = true;
                  result->tokenResult = walk->leftToken;
                  return true;
                } // if: nil
                
                else {
                  if ( result->hasNodeResult ) {
                    result->hasNodeResult = false;
                    result->nodeResult = NULL;
                  } // if: clear previous result
                  
                  result->hasTokenResult = true;
                  result->tokenResult = definedToken;
                } // else: other types
              } // if: get defined token
              
              else {
                if ( result->hasTokenResult ) {
                  result->hasTokenResult = false;
                  result->tokenResult = NULL;
                } // if: clear previous result
                
                result->hasNodeResult = true;
                result->nodeResult = GetDefineBindings( walk->leftToken->content )->leftNode;
              } // else: get defined node
            } // else: not quote result
          } // else if: symbol, check defined or not
          
          else {
            if ( result->hasNodeResult ) {
              result->hasNodeResult = false;
              result->nodeResult = NULL;
            } // if: clear previous result
            
            result->hasTokenResult = true;
            result->tokenResult = walk->leftToken;
          } // else: other types
        } // if: the argument is a token
        
        else if ( walk->leftNode ) {
          if ( result->hasTokenResult ) {
            result->hasTokenResult = false;
            result->tokenResult = NULL;
          } // if: clear previous result
          
          result->hasNodeResult = true;
          result->nodeResult = walk->leftNode;
        } // else if: the argument is a node
      } // if: last node
    } // while: go through the arguments
    
    return true;
  } // And()
  
  bool Or( vector<TreeStruct *> arguments, ResultStruct *result ) {
    TreeStruct *walk = arguments.front();
    
    while ( walk->rightNode ) {
      if ( walk->leftToken ) {
        if ( walk->leftToken->tokenType == SYMBOL ) {
          if ( walk->leftToken->isQuoteResult ) {
            result->hasTokenResult = true;
            result->tokenResult = walk->leftToken;
            return true;
          } // if: quote result
          
          else {
            if ( GetDefineBindings( walk->leftToken->content )->leftToken ) {
              TokenStruct *definedToken = GetDefineBindings( walk->leftToken->content )->leftToken;
              
              if ( definedToken->tokenType != NIL ) {
                result->hasTokenResult = true;
                result->tokenResult = definedToken;
                return true;
              } // if: nil
            } // if: get defined token
            
            else {
              result->hasNodeResult = true;
              result->nodeResult = GetDefineBindings( walk->leftToken->content )->leftNode;
              return true;
            } // else: get defined node
          } // else: not quote result
        } // if: symbol, check defined or not
        
        if ( walk->leftToken->tokenType != NIL ) {
          result->hasTokenResult = true;
          result->tokenResult = walk->leftToken;
          return true;
        } // if: nil
      } // if: the argument is a token
      
      else if ( walk->leftNode ) {
        result->hasNodeResult = true;
        result->nodeResult = walk->leftNode;
        return true;
      } // else if: the argument is a node
      
      walk = walk->rightNode;
      
      if ( walk->rightNode == NULL ) {
        if ( walk->leftToken ) {
          if ( walk->leftToken->tokenType == SYMBOL ) {
            if ( walk->leftToken->isQuoteResult ) {
              result->hasTokenResult = true;
              result->tokenResult = walk->leftToken;
              return true;
            } // if: quote result
            
            else {
              if ( GetDefineBindings( walk->leftToken->content )->leftToken ) {
                TokenStruct *definedToken = GetDefineBindings( walk->leftToken->content )->leftToken;
                
                if ( definedToken->tokenType != NIL ) {
                  result->hasTokenResult = true;
                  result->tokenResult = definedToken;
                  return true;
                } // if: nil
              } // if: get defined token
              
              else {
                result->hasNodeResult = true;
                result->nodeResult = GetDefineBindings( walk->leftToken->content )->leftNode;
                return true;
              } // else: get defined node
            } // else: not quote result
          } // if: symbol, check defined or not
          
          if ( walk->leftToken->tokenType != NIL ) {
            result->hasTokenResult = true;
            result->tokenResult = walk->leftToken;
            return true;
          } // if: nil
        } // if: the argument is a token
        
        else if ( walk->leftNode ) {
          result->hasNodeResult = true;
          result->nodeResult = walk->leftNode;
          return true;
        } // else if: the argument is a node
      } // if: last node
    } // while: go through the arguments
    
    return true;
  } // Or()
  
  bool GreaterThan( vector<TreeStruct *> arguments, ResultStruct *result ) {
    result->hasTokenResult = true;
    result->tokenResult->tokenType = T;
    TreeStruct *walk = arguments.front();
    float previousNumber = 0;
    bool isFirstNum = true;
    
    while ( walk->rightNode ) {
      if ( walk->leftToken ) {
        if ( walk->leftToken->tokenType == FLOAT || walk->leftToken->tokenType == INT ) {
          if ( isFirstNum ) {
            previousNumber = atof( walk->leftToken->content.c_str() );
            isFirstNum = false;
          } // if: firstNum
          
          else {
            if ( previousNumber > atof( walk->leftToken->content.c_str() ) ) {
              previousNumber = atof( walk->leftToken->content.c_str() );
            } // if: argument is float, set isFLoat
            
            else {
              result->tokenResult->tokenType = NIL;
              previousNumber = atof( walk->leftToken->content.c_str() );
            } // else: false
          } // else: not firstNum
        } // if: check if add any float
        
        else if ( walk->leftToken->tokenType == SYMBOL ) {
          if ( walk->leftToken->isQuoteResult ) {
            string errorMessage = "ERROR (> with incorrect argument type) : ";
            errorMessage += walk->leftToken->content;
            SetError( INCORRECT_ARGUMENT_TYPE, errorMessage );
            return false;
          } // if: quote result
          
          else {
            if ( GetDefineBindings( walk->leftToken->content )->leftToken ) {
              TokenStruct *definedToken = GetDefineBindings( walk->leftToken->content )->leftToken;
              
              if ( definedToken->tokenType == FLOAT || definedToken->tokenType == INT ) {
                if ( isFirstNum ) {
                  previousNumber = atof( definedToken->content.c_str() );
                  isFirstNum = false;
                } // if: firstNum
                
                else {
                  if ( previousNumber > atof( definedToken->content.c_str() ) ) {
                    previousNumber = atof( definedToken->content.c_str() );
                  } // if: argument is float, set isFLoat
                  
                  else {
                    result->tokenResult->tokenType = NIL;
                    previousNumber = atof( definedToken->content.c_str() );
                  } // else: false
                } // else: not firstNum
              } // if: check if add any float
              
              else {
                string errorMessage = "ERROR (> with incorrect argument type) : ";
                errorMessage += definedToken->content;
                SetError( INCORRECT_ARGUMENT_TYPE, errorMessage );
                return false;
              } // else: wrong type
            } // if: get defined token
            
            else {
              TreeStruct *definedNode = GetDefineBindings( walk->leftToken->content )->leftNode;
              stringstream errorMessage;
              errorMessage << "ERROR (> with incorrect argument type) : ";
              int layer = 0;
              AddTreeString( definedNode, layer, false, errorMessage );
              
              if ( layer > 0 ) {
                while ( layer > 0 ) {
                  layer--;
                  errorMessage << string( layer, ' ' ) << ')' << endl;
                } // while: loop print right-paren
              } // if: layer still greater than zero
              
              SetError( INCORRECT_ARGUMENT_TYPE, errorMessage.str() );
              return false;
            } // else: get defined node, can't evaluate
          } // else: not quote result
        } // else if: symbol, check defined or not
        
        else {
          string errorMessage = "ERROR (> with incorrect argument type) : ";
          errorMessage += walk->leftToken->content;
          SetError( INCORRECT_ARGUMENT_TYPE, errorMessage );
          return false;
        } // else: wrong argument type
      } // if: argument is token
      
      else if ( walk->leftNode ) {
        TreeStruct *errorNode = GetDefineBindings( walk->leftToken->content )->leftNode;
        stringstream errorMessage;
        errorMessage << "ERROR (> with incorrect argument type) : ";
        int layer = 0;
        AddTreeString( errorNode, layer, false, errorMessage );
        
        if ( layer > 0 ) {
          while ( layer > 0 ) {
            layer--;
            errorMessage << string( layer, ' ' ) << ')' << endl;
          } // while: loop print right-paren
        } // if: layer still greater than zero
        
        SetError( INCORRECT_ARGUMENT_TYPE, errorMessage.str() );
        return false;
      } // else if: argument is node
      
      walk = walk->rightNode;
      
      if ( walk->rightNode == NULL ) {
        if ( walk->leftToken ) {
          if ( walk->leftToken->tokenType == FLOAT || walk->leftToken->tokenType == INT ) {
            if ( previousNumber > atof( walk->leftToken->content.c_str() ) ) {
              previousNumber = atof( walk->leftToken->content.c_str() );
            } // if: argument is float, set isFLoat
            
            else {
              result->tokenResult->tokenType = NIL;
              previousNumber = atof( walk->leftToken->content.c_str() );
            } // else: false
          } // if: check if add any float
          
          else if ( walk->leftToken->tokenType == SYMBOL ) {
            if ( walk->leftToken->isQuoteResult ) {
              string errorMessage = "ERROR (> with incorrect argument type) : ";
              errorMessage += walk->leftToken->content;
              SetError( INCORRECT_ARGUMENT_TYPE, errorMessage );
              return false;
            } // if: quote result
            
            else {
              if ( GetDefineBindings( walk->leftToken->content )->leftToken ) {
                TokenStruct *definedToken = GetDefineBindings( walk->leftToken->content )->leftToken;
                
                if ( definedToken->tokenType == FLOAT || definedToken->tokenType == INT ) {
                  if ( previousNumber > atof( definedToken->content.c_str() ) ) {
                    previousNumber = atof( definedToken->content.c_str() );
                  } // if: argument is float, set isFLoat
                  
                  else {
                    result->tokenResult->tokenType = NIL;
                    previousNumber = atof( definedToken->content.c_str() );
                  } // else: false
                } // if: check if add any float
                
                else {
                  string errorMessage = "ERROR (> with incorrect argument type) : ";
                  errorMessage += definedToken->content;
                  SetError( INCORRECT_ARGUMENT_TYPE, errorMessage );
                  return false;
                } // else: wrong type
              } // if: get defined token
              
              else {
                TreeStruct *definedNode = GetDefineBindings( walk->leftToken->content )->leftNode;
                stringstream errorMessage;
                errorMessage << "ERROR (> with incorrect argument type) : ";
                int layer = 0;
                AddTreeString( definedNode, layer, false, errorMessage );
                
                if ( layer > 0 ) {
                  while ( layer > 0 ) {
                    layer--;
                    errorMessage << string( layer, ' ' ) << ')' << endl;
                  } // while: loop print right-paren
                } // if: layer still greater than zero
                
                SetError( INCORRECT_ARGUMENT_TYPE, errorMessage.str() );
                return false;
              } // else: get defined node, can't evaluate
            } // else: not quote result
          } // else if: symbol, check defined or not
          
          else {
            string errorMessage = "ERROR (> with incorrect argument type) : ";
            errorMessage += walk->leftToken->content;
            SetError( INCORRECT_ARGUMENT_TYPE, errorMessage );
            return false;
          } // else: wrong argument type
        } // if: argument is token
        
        else if ( walk->leftNode ) {
          TreeStruct *errorNode = GetDefineBindings( walk->leftToken->content )->leftNode;
          stringstream errorMessage;
          errorMessage << "ERROR (> with incorrect argument type) : ";
          int layer = 0;
          AddTreeString( errorNode, layer, false, errorMessage );
          
          if ( layer > 0 ) {
            while ( layer > 0 ) {
              layer--;
              errorMessage << string( layer, ' ' ) << ')' << endl;
            } // while: loop print right-paren
          } // if: layer still greater than zero
          
          SetError( INCORRECT_ARGUMENT_TYPE, errorMessage.str() );
          return false;
        } // else if: argument is node
      } // if: add up the last argument
    } // while: add up the arguments
    
    if ( result->tokenResult->tokenType == T ) {
      result->tokenResult->content = "#t";
    } // if: the result is true, set the content
    
    else {
      result->tokenResult->content = "nil";
    } // else: the result is nil, set the content
    
    return true;
  } // GreaterThan()
  
  bool GreaterThanOrEqualTo( vector<TreeStruct *> arguments, ResultStruct *result ) {
    result->hasTokenResult = true;
    result->tokenResult->tokenType = T;
    TreeStruct *walk = arguments.front();
    float previousNumber = 0;
    bool isFirstNum = true;
    
    while ( walk->rightNode ) {
      if ( walk->leftToken ) {
        if ( walk->leftToken->tokenType == FLOAT || walk->leftToken->tokenType == INT ) {
          if ( isFirstNum ) {
            previousNumber = atof( walk->leftToken->content.c_str() );
            isFirstNum = false;
          } // if: isFirstNum
          
          else {
            if ( previousNumber >= atof( walk->leftToken->content.c_str() ) ) {
              previousNumber = atof( walk->leftToken->content.c_str() );
            } // if: argument is float, set isFLoat
            
            else {
              result->tokenResult->tokenType = NIL;
              previousNumber = atof( walk->leftToken->content.c_str() );
            } // else: false
          } // else: not firstNum
        } // if: check if add any float
        
        else if ( walk->leftToken->tokenType == SYMBOL ) {
          if ( walk->leftToken->isQuoteResult ) {
            string errorMessage = "ERROR (>= with incorrect argument type) : ";
            errorMessage += walk->leftToken->content;
            SetError( INCORRECT_ARGUMENT_TYPE, errorMessage );
            return false;
          } // if: quote result
          
          else {
            if ( GetDefineBindings( walk->leftToken->content )->leftToken ) {
              TokenStruct *definedToken = GetDefineBindings( walk->leftToken->content )->leftToken;
              
              if ( definedToken->tokenType == FLOAT || definedToken->tokenType == INT ) {
                if ( isFirstNum ) {
                  previousNumber = atof( definedToken->content.c_str() );
                  isFirstNum = false;
                } // if: isFirstNum
                
                else {
                  if ( previousNumber >= atof( definedToken->content.c_str() ) ) {
                    previousNumber = atof( definedToken->content.c_str() );
                  } // if: argument is float, set isFLoat
                  
                  else {
                    result->tokenResult->tokenType = NIL;
                    previousNumber = atof( definedToken->content.c_str() );
                  } // else: false
                } // else: not firstNum
              } // if: check if add any float
              
              else {
                string errorMessage = "ERROR (>= with incorrect argument type) : ";
                errorMessage += definedToken->content;
                SetError( INCORRECT_ARGUMENT_TYPE, errorMessage );
                return false;
              } // else: wrong type
            } // if: get defined token
            
            else {
              TreeStruct *definedNode = GetDefineBindings( walk->leftToken->content )->leftNode;
              stringstream errorMessage;
              errorMessage << "ERROR (>= with incorrect argument type) : ";
              int layer = 0;
              AddTreeString( definedNode, layer, false, errorMessage );
              
              if ( layer > 0 ) {
                while ( layer > 0 ) {
                  layer--;
                  errorMessage << string( layer, ' ' ) << ')' << endl;
                } // while: loop print right-paren
              } // if: layer still greater than zero
              
              SetError( INCORRECT_ARGUMENT_TYPE, errorMessage.str() );
              return false;
            } // else: get defined node, can't evaluate
          } // else: not quote result
        } // else if: symbol, check defined or not
        
        else {
          string errorMessage = "ERROR (>= with incorrect argument type) : ";
          errorMessage += walk->leftToken->content;
          SetError( INCORRECT_ARGUMENT_TYPE, errorMessage );
          return false;
        } // else: wrong argument type
      } // if: argument is token
      
      else if ( walk->leftNode ) {
        TreeStruct *errorNode = GetDefineBindings( walk->leftToken->content )->leftNode;
        stringstream errorMessage;
        errorMessage << "ERROR (>= with incorrect argument type) : ";
        int layer = 0;
        AddTreeString( errorNode, layer, false, errorMessage );
        
        if ( layer > 0 ) {
          while ( layer > 0 ) {
            layer--;
            errorMessage << string( layer, ' ' ) << ')' << endl;
          } // while: loop print right-paren
        } // if: layer still greater than zero
        
        SetError( INCORRECT_ARGUMENT_TYPE, errorMessage.str() );
        return false;
      } // else if: argument is node
      
      walk = walk->rightNode;
      
      if ( walk->rightNode == NULL ) {
        if ( walk->leftToken ) {
          if ( walk->leftToken->tokenType == FLOAT || walk->leftToken->tokenType == INT ) {
            if ( previousNumber >= atof( walk->leftToken->content.c_str() ) ) {
              previousNumber = atof( walk->leftToken->content.c_str() );
            } // if: argument is float, set isFLoat
            
            else {
              result->tokenResult->tokenType = NIL;
              previousNumber = atof( walk->leftToken->content.c_str() );
            } // else: false
          } // if: check if add any float
          
          else if ( walk->leftToken->tokenType == SYMBOL ) {
            if ( walk->leftToken->isQuoteResult ) {
              string errorMessage = "ERROR (>= with incorrect argument type) : ";
              errorMessage += walk->leftToken->content;
              SetError( INCORRECT_ARGUMENT_TYPE, errorMessage );
              return false;
            } // if: quote result
            
            else {
              if ( GetDefineBindings( walk->leftToken->content )->leftToken ) {
                TokenStruct *definedToken = GetDefineBindings( walk->leftToken->content )->leftToken;
                
                if ( definedToken->tokenType == FLOAT || definedToken->tokenType == INT ) {
                  if ( previousNumber >= atof( definedToken->content.c_str() ) ) {
                    previousNumber = atof( definedToken->content.c_str() );
                  } // if: argument is float, set isFLoat
                  
                  else {
                    result->tokenResult->tokenType = NIL;
                    previousNumber = atof( definedToken->content.c_str() );
                  } // else: false
                } // if: check if add any float
                
                else {
                  string errorMessage = "ERROR (>= with incorrect argument type) : ";
                  errorMessage += definedToken->content;
                  SetError( INCORRECT_ARGUMENT_TYPE, errorMessage );
                  return false;
                } // else: wrong type
              } // if: get defined token
              
              else {
                TreeStruct *definedNode = GetDefineBindings( walk->leftToken->content )->leftNode;
                stringstream errorMessage;
                errorMessage << "ERROR (>= with incorrect argument type) : ";
                int layer = 0;
                AddTreeString( definedNode, layer, false, errorMessage );
                
                if ( layer > 0 ) {
                  while ( layer > 0 ) {
                    layer--;
                    errorMessage << string( layer, ' ' ) << ')' << endl;
                  } // while: loop print right-paren
                } // if: layer still greater than zero
                
                SetError( INCORRECT_ARGUMENT_TYPE, errorMessage.str() );
                return false;
              } // else: get defined node, can't evaluate
            } // else: not quote result
          } // else if: symbol, check defined or not
          
          else {
            string errorMessage = "ERROR (>= with incorrect argument type) : ";
            errorMessage += walk->leftToken->content;
            SetError( INCORRECT_ARGUMENT_TYPE, errorMessage );
            return false;
          } // else: wrong argument type
        } // if: argument is token
        
        else if ( walk->leftNode ) {
          TreeStruct *errorNode = GetDefineBindings( walk->leftToken->content )->leftNode;
          stringstream errorMessage;
          errorMessage << "ERROR (>= with incorrect argument type) : ";
          int layer = 0;
          AddTreeString( errorNode, layer, false, errorMessage );
          
          if ( layer > 0 ) {
            while ( layer > 0 ) {
              layer--;
              errorMessage << string( layer, ' ' ) << ')' << endl;
            } // while: loop print right-paren
          } // if: layer still greater than zero
          
          SetError( INCORRECT_ARGUMENT_TYPE, errorMessage.str() );
          return false;
        } // else if: argument is node
      } // if: add up the last argument
    } // while: add up the arguments
    
    if ( result->tokenResult->tokenType == T ) {
      result->tokenResult->content = "#t";
    } // if: the result is true, set the content
    
    else {
      result->tokenResult->content = "nil";
    } // else: the result is nil, set the content
    
    return true;
  } // GreaterThanOrEqualTo()
  
  bool LessThan( vector<TreeStruct *> arguments, ResultStruct *result ) {
    result->hasTokenResult = true;
    result->tokenResult->tokenType = T;
    TreeStruct *walk = arguments.front();
    float previousNumber = 0;
    bool isFirstNum = true;
    
    while ( walk->rightNode ) {
      if ( walk->leftToken ) {
        if ( walk->leftToken->tokenType == FLOAT || walk->leftToken->tokenType == INT ) {
          if ( isFirstNum ) {
            previousNumber = atof( walk->leftToken->content.c_str() );
            isFirstNum = false;
          } // if: isFirstNum
          
          else {
            if ( previousNumber < atof( walk->leftToken->content.c_str() ) ) {
              previousNumber = atof( walk->leftToken->content.c_str() );
            } // if: argument is float, set isFLoat
            
            else {
              result->tokenResult->tokenType = NIL;
              previousNumber = atof( walk->leftToken->content.c_str() );
            } // else: false
          } // else: not first num
        } // if: check if add any float
        
        else if ( walk->leftToken->tokenType == SYMBOL ) {
          if ( walk->leftToken->isQuoteResult ) {
            string errorMessage = "ERROR (< with incorrect argument type) : ";
            errorMessage += walk->leftToken->content;
            SetError( INCORRECT_ARGUMENT_TYPE, errorMessage );
            return false;
          } // if: quote result
          
          else {
            if ( GetDefineBindings( walk->leftToken->content )->leftToken ) {
              TokenStruct *definedToken = GetDefineBindings( walk->leftToken->content )->leftToken;
              
              if ( definedToken->tokenType == FLOAT || definedToken->tokenType == INT ) {
                if ( isFirstNum ) {
                  previousNumber = atof( definedToken->content.c_str() );
                  isFirstNum = false;
                } // if: isFirstNum
                
                else {
                  if ( previousNumber < atof( definedToken->content.c_str() ) ) {
                    previousNumber = atof( definedToken->content.c_str() );
                  } // if: argument is float, set isFLoat
                  
                  else {
                    result->tokenResult->tokenType = NIL;
                    previousNumber = atof( definedToken->content.c_str() );
                  } // else: false
                } // else: not first num
              } // if: check if add any float
              
              else {
                string errorMessage = "ERROR (< with incorrect argument type) : ";
                errorMessage += definedToken->content;
                SetError( INCORRECT_ARGUMENT_TYPE, errorMessage );
                return false;
              } // else: wrong type
            } // if: get defined token
            
            else {
              TreeStruct *definedNode = GetDefineBindings( walk->leftToken->content )->leftNode;
              stringstream errorMessage;
              errorMessage << "ERROR (< with incorrect argument type) : ";
              int layer = 0;
              AddTreeString( definedNode, layer, false, errorMessage );
              
              if ( layer > 0 ) {
                while ( layer > 0 ) {
                  layer--;
                  errorMessage << string( layer, ' ' ) << ')' << endl;
                } // while: loop print right-paren
              } // if: layer still greater than zero
              
              SetError( INCORRECT_ARGUMENT_TYPE, errorMessage.str() );
              return false;
            } // else: get defined node, can't evaluate
          } // else: not quote result
        } // else if: symbol, check defined or not
        
        else {
          string errorMessage = "ERROR (< with incorrect argument type) : ";
          errorMessage += walk->leftToken->content;
          SetError( INCORRECT_ARGUMENT_TYPE, errorMessage );
          return false;
        } // else: wrong argument type
      } // if: argument is token
      
      else if ( walk->leftNode ) {
        TreeStruct *errorNode = GetDefineBindings( walk->leftToken->content )->leftNode;
        stringstream errorMessage;
        errorMessage << "ERROR (< with incorrect argument type) : ";
        int layer = 0;
        AddTreeString( errorNode, layer, false, errorMessage );
        
        if ( layer > 0 ) {
          while ( layer > 0 ) {
            layer--;
            errorMessage << string( layer, ' ' ) << ')' << endl;
          } // while: loop print right-paren
        } // if: layer still greater than zero
        
        SetError( INCORRECT_ARGUMENT_TYPE, errorMessage.str() );
        return false;
      } // else if: argument is node
      
      walk = walk->rightNode;
      
      if ( walk->rightNode == NULL ) {
        if ( walk->leftToken ) {
          if ( walk->leftToken->tokenType == FLOAT || walk->leftToken->tokenType == INT ) {
            if ( previousNumber < atof( walk->leftToken->content.c_str() ) ) {
              previousNumber = atof( walk->leftToken->content.c_str() );
            } // if: argument is float, set isFLoat
            
            else {
              result->tokenResult->tokenType = NIL;
              previousNumber = atof( walk->leftToken->content.c_str() );
            } // else: false
          } // if: check if add any float
          
          else if ( walk->leftToken->tokenType == SYMBOL ) {
            if ( walk->leftToken->isQuoteResult ) {
              string errorMessage = "ERROR (< with incorrect argument type) : ";
              errorMessage += walk->leftToken->content;
              SetError( INCORRECT_ARGUMENT_TYPE, errorMessage );
              return false;
            } // if: quote result
            
            else {
              if ( GetDefineBindings( walk->leftToken->content )->leftToken ) {
                TokenStruct *definedToken = GetDefineBindings( walk->leftToken->content )->leftToken;
                
                if ( definedToken->tokenType == FLOAT || definedToken->tokenType == INT ) {
                  if ( previousNumber < atof( definedToken->content.c_str() ) ) {
                    previousNumber = atof( definedToken->content.c_str() );
                  } // if: argument is float, set isFLoat
                  
                  else {
                    result->tokenResult->tokenType = NIL;
                    previousNumber = atof( definedToken->content.c_str() );
                  } // else: false
                } // if: check if add any float
                
                else {
                  string errorMessage = "ERROR (< with incorrect argument type) : ";
                  errorMessage += definedToken->content;
                  SetError( INCORRECT_ARGUMENT_TYPE, errorMessage );
                  return false;
                } // else: wrong type
              } // if: get defined token
              
              else {
                TreeStruct *definedNode = GetDefineBindings( walk->leftToken->content )->leftNode;
                stringstream errorMessage;
                errorMessage << "ERROR (< with incorrect argument type) : ";
                int layer = 0;
                AddTreeString( definedNode, layer, false, errorMessage );
                
                if ( layer > 0 ) {
                  while ( layer > 0 ) {
                    layer--;
                    errorMessage << string( layer, ' ' ) << ')' << endl;
                  } // while: loop print right-paren
                } // if: layer still greater than zero
                
                SetError( INCORRECT_ARGUMENT_TYPE, errorMessage.str() );
                return false;
              } // else: get defined node, can't evaluate
            } // else: not quote result
          } // else if: symbol, check defined or not
          
          else {
            string errorMessage = "ERROR (< with incorrect argument type) : ";
            errorMessage += walk->leftToken->content;
            SetError( INCORRECT_ARGUMENT_TYPE, errorMessage );
            return false;
          } // else: wrong argument type
        } // if: argument is token
        
        else if ( walk->leftNode ) {
          TreeStruct *errorNode = GetDefineBindings( walk->leftToken->content )->leftNode;
          stringstream errorMessage;
          errorMessage << "ERROR (< with incorrect argument type) : ";
          int layer = 0;
          AddTreeString( errorNode, layer, false, errorMessage );
          
          if ( layer > 0 ) {
            while ( layer > 0 ) {
              layer--;
              errorMessage << string( layer, ' ' ) << ')' << endl;
            } // while: loop print right-paren
          } // if: layer still greater than zero
          
          SetError( INCORRECT_ARGUMENT_TYPE, errorMessage.str() );
          return false;
        } // else if: argument is node
      } // if: add up the last argument
    } // while: add up the arguments
    
    if ( result->tokenResult->tokenType == T ) {
      result->tokenResult->content = "#t";
    } // if: the result is true, set the content
    
    else {
      result->tokenResult->content = "nil";
    } // else: the result is nil, set the content
    
    return true;
  } // LessThan()
  
  bool LessThanOrEqualTo( vector<TreeStruct *> arguments, ResultStruct *result ) {
    result->hasTokenResult = true;
    result->tokenResult->tokenType = T;
    TreeStruct *walk = arguments.front();
    float previousNumber = 0;
    bool isFirstNum = true;
    
    while ( walk->rightNode ) {
      if ( walk->leftToken ) {
        if ( walk->leftToken->tokenType == FLOAT || walk->leftToken->tokenType == INT ) {
          if ( isFirstNum ) {
            previousNumber = atof( walk->leftToken->content.c_str() );
            isFirstNum = false;
          } // if: isFirstNum
          
          else {
            if ( previousNumber <= atof( walk->leftToken->content.c_str() ) ) {
              previousNumber = atof( walk->leftToken->content.c_str() );
            } // if: argument is float, set isFLoat
            
            else {
              result->tokenResult->tokenType = NIL;
              previousNumber = atof( walk->leftToken->content.c_str() );
            } // else: false
          } // else: not first num
        } // if: check if add any float
        
        else if ( walk->leftToken->tokenType == SYMBOL ) {
          if ( walk->leftToken->isQuoteResult ) {
            string errorMessage = "ERROR (<= with incorrect argument type) : ";
            errorMessage += walk->leftToken->content;
            SetError( INCORRECT_ARGUMENT_TYPE, errorMessage );
            return false;
          } // if: quote result
          
          else {
            if ( GetDefineBindings( walk->leftToken->content )->leftToken ) {
              TokenStruct *definedToken = GetDefineBindings( walk->leftToken->content )->leftToken;
              
              if ( definedToken->tokenType == FLOAT || definedToken->tokenType == INT ) {
                if ( isFirstNum ) {
                  previousNumber = atof( definedToken->content.c_str() );
                  isFirstNum = false;
                } // if: isFirstNum
                
                else {
                  if ( previousNumber <= atof( definedToken->content.c_str() ) ) {
                    previousNumber = atof( definedToken->content.c_str() );
                  } // if: argument is float, set isFLoat
                  
                  else {
                    result->tokenResult->tokenType = NIL;
                    previousNumber = atof( definedToken->content.c_str() );
                  } // else: false
                } // else: not first num
              } // if: check if add any float
              
              else {
                string errorMessage = "ERROR (<= with incorrect argument type) : ";
                errorMessage += definedToken->content;
                SetError( INCORRECT_ARGUMENT_TYPE, errorMessage );
                return false;
              } // else: wrong type
            } // if: get defined token
            
            else {
              TreeStruct *definedNode = GetDefineBindings( walk->leftToken->content )->leftNode;
              stringstream errorMessage;
              errorMessage << "ERROR (<= with incorrect argument type) : ";
              int layer = 0;
              AddTreeString( definedNode, layer, false, errorMessage );
              
              if ( layer > 0 ) {
                while ( layer > 0 ) {
                  layer--;
                  errorMessage << string( layer, ' ' ) << ')' << endl;
                } // while: loop print right-paren
              } // if: layer still greater than zero
              
              SetError( INCORRECT_ARGUMENT_TYPE, errorMessage.str() );
              return false;
            } // else: get defined node, can't evaluate
          } // else: not quote result
        } // else if: symbol, check defined or not
        
        else {
          string errorMessage = "ERROR (<= with incorrect argument type) : ";
          errorMessage += walk->leftToken->content;
          SetError( INCORRECT_ARGUMENT_TYPE, errorMessage );
          return false;
        } // else: wrong argument type
      } // if: argument is token
      
      else if ( walk->leftNode ) {
        TreeStruct *errorNode = GetDefineBindings( walk->leftToken->content )->leftNode;
        stringstream errorMessage;
        errorMessage << "ERROR (<= with incorrect argument type) : ";
        int layer = 0;
        AddTreeString( errorNode, layer, false, errorMessage );
        
        if ( layer > 0 ) {
          while ( layer > 0 ) {
            layer--;
            errorMessage << string( layer, ' ' ) << ')' << endl;
          } // while: loop print right-paren
        } // if: layer still greater than zero
        
        SetError( INCORRECT_ARGUMENT_TYPE, errorMessage.str() );
        return false;
      } // else if: argument is node
      
      walk = walk->rightNode;
      
      if ( walk->rightNode == NULL ) {
        if ( walk->leftToken ) {
          if ( walk->leftToken->tokenType == FLOAT || walk->leftToken->tokenType == INT ) {
            if ( previousNumber <= atof( walk->leftToken->content.c_str() ) ) {
              previousNumber = atof( walk->leftToken->content.c_str() );
            } // if: argument is float, set isFLoat
            
            else {
              result->tokenResult->tokenType = NIL;
              previousNumber = atof( walk->leftToken->content.c_str() );
            } // else: false
          } // if: check if add any float
          
          else if ( walk->leftToken->tokenType == SYMBOL ) {
            if ( walk->leftToken->isQuoteResult ) {
              string errorMessage = "ERROR (<= with incorrect argument type) : ";
              errorMessage += walk->leftToken->content;
              SetError( INCORRECT_ARGUMENT_TYPE, errorMessage );
              return false;
            } // if: quote result
            
            else {
              if ( GetDefineBindings( walk->leftToken->content )->leftToken ) {
                TokenStruct *definedToken = GetDefineBindings( walk->leftToken->content )->leftToken;
                
                if ( definedToken->tokenType == FLOAT || definedToken->tokenType == INT ) {
                  if ( previousNumber <= atof( definedToken->content.c_str() ) ) {
                    previousNumber = atof( definedToken->content.c_str() );
                  } // if: argument is float, set isFLoat
                  
                  else {
                    result->tokenResult->tokenType = NIL;
                    previousNumber = atof( definedToken->content.c_str() );
                  } // else: false
                } // if: check if add any float
                
                else {
                  string errorMessage = "ERROR (<= with incorrect argument type) : ";
                  errorMessage += definedToken->content;
                  SetError( INCORRECT_ARGUMENT_TYPE, errorMessage );
                  return false;
                } // else: wrong type
              } // if: get defined token
              
              else {
                TreeStruct *definedNode = GetDefineBindings( walk->leftToken->content )->leftNode;
                stringstream errorMessage;
                errorMessage << "ERROR (<= with incorrect argument type) : ";
                int layer = 0;
                AddTreeString( definedNode, layer, false, errorMessage );
                
                if ( layer > 0 ) {
                  while ( layer > 0 ) {
                    layer--;
                    errorMessage << string( layer, ' ' ) << ')' << endl;
                  } // while: loop print right-paren
                } // if: layer still greater than zero
                
                SetError( INCORRECT_ARGUMENT_TYPE, errorMessage.str() );
                return false;
              } // else: get defined node, can't evaluate
            } // else: not quote result
          } // else if: symbol, check defined or not
          
          else {
            string errorMessage = "ERROR (<= with incorrect argument type) : ";
            errorMessage += walk->leftToken->content;
            SetError( INCORRECT_ARGUMENT_TYPE, errorMessage );
            return false;
          } // else: wrong argument type
        } // if: argument is token
        
        else if ( walk->leftNode ) {
          TreeStruct *errorNode = GetDefineBindings( walk->leftToken->content )->leftNode;
          stringstream errorMessage;
          errorMessage << "ERROR (<= with incorrect argument type) : ";
          int layer = 0;
          AddTreeString( errorNode, layer, false, errorMessage );
          
          if ( layer > 0 ) {
            while ( layer > 0 ) {
              layer--;
              errorMessage << string( layer, ' ' ) << ')' << endl;
            } // while: loop print right-paren
          } // if: layer still greater than zero
          
          SetError( INCORRECT_ARGUMENT_TYPE, errorMessage.str() );
          return false;
        } // else if: argument is node
      } // if: add up the last argument
    } // while: add up the arguments
    
    if ( result->tokenResult->tokenType == T ) {
      result->tokenResult->content = "#t";
    } // if: the result is true, set the content
    
    else {
      result->tokenResult->content = "nil";
    } // else: the result is nil, set the content
    
    return true;
  } // LessThanOrEqualTo()
  
  bool EqualTo( vector<TreeStruct *> arguments, ResultStruct *result ) {
    result->hasTokenResult = true;
    result->tokenResult->tokenType = T;
    TreeStruct *walk = arguments.front();
    float previousNumber = 0;
    bool isFirstNum = true;
    
    while ( walk->rightNode ) {
      if ( walk->leftToken ) {
        if ( walk->leftToken->tokenType == FLOAT || walk->leftToken->tokenType == INT ) {
          if ( isFirstNum ) {
            previousNumber = atof( walk->leftToken->content.c_str() );
            isFirstNum = false;
          } // if: isFirstNum
          
          else {
            if ( previousNumber == atof( walk->leftToken->content.c_str() ) ) {
              previousNumber = atof( walk->leftToken->content.c_str() );
            } // if: argument is float, set isFLoat
            
            else {
              result->tokenResult->tokenType = NIL;
              previousNumber = atof( walk->leftToken->content.c_str() );
            } // else: false
          } // else: not firstNum
        } // if: check if add any float
        
        else if ( walk->leftToken->tokenType == SYMBOL ) {
          if ( walk->leftToken->isQuoteResult ) {
            string errorMessage = "ERROR (= with incorrect argument type) : ";
            errorMessage += walk->leftToken->content;
            SetError( INCORRECT_ARGUMENT_TYPE, errorMessage );
            return false;
          } // if: quote result
          
          else {
            if ( GetDefineBindings( walk->leftToken->content )->leftToken ) {
              TokenStruct *definedToken = GetDefineBindings( walk->leftToken->content )->leftToken;
              
              if ( definedToken->tokenType == FLOAT || definedToken->tokenType == INT ) {
                if ( isFirstNum ) {
                  previousNumber = atof( definedToken->content.c_str() );
                  isFirstNum = false;
                } // if: isFirstNum
                
                else {
                  if ( previousNumber == atof( definedToken->content.c_str() ) ) {
                    previousNumber = atof( definedToken->content.c_str() );
                  } // if: argument is float, set isFLoat
                  
                  else {
                    result->tokenResult->tokenType = NIL;
                    previousNumber = atof( definedToken->content.c_str() );
                  } // else: false
                } // else: not firstNum
              } // if: check if add any float
              
              else {
                string errorMessage = "ERROR (= with incorrect argument type) : ";
                errorMessage += definedToken->content;
                SetError( INCORRECT_ARGUMENT_TYPE, errorMessage );
                return false;
              } // else: wrong type
            } // if: get defined token
            
            else {
              TreeStruct *definedNode = GetDefineBindings( walk->leftToken->content )->leftNode;
              stringstream errorMessage;
              errorMessage << "ERROR (= with incorrect argument type) : ";
              int layer = 0;
              AddTreeString( definedNode, layer, false, errorMessage );
              
              if ( layer > 0 ) {
                while ( layer > 0 ) {
                  layer--;
                  errorMessage << string( layer, ' ' ) << ')' << endl;
                } // while: loop print right-paren
              } // if: layer still greater than zero
              
              SetError( INCORRECT_ARGUMENT_TYPE, errorMessage.str() );
              return false;
            } // else: get defined node, can't evaluate
          } // else: not quote result
        } // else if: symbol, check defined or not
        
        else {
          string errorMessage = "ERROR (= with incorrect argument type) : ";
          errorMessage += walk->leftToken->content;
          SetError( INCORRECT_ARGUMENT_TYPE, errorMessage );
          return false;
        } // else: wrong argument type
      } // if: argument is token
      
      else if ( walk->leftNode ) {
        TreeStruct *errorNode = GetDefineBindings( walk->leftToken->content )->leftNode;
        stringstream errorMessage;
        errorMessage << "ERROR (= with incorrect argument type) : ";
        int layer = 0;
        AddTreeString( errorNode, layer, false, errorMessage );
        
        if ( layer > 0 ) {
          while ( layer > 0 ) {
            layer--;
            errorMessage << string( layer, ' ' ) << ')' << endl;
          } // while: loop print right-paren
        } // if: layer still greater than zero
        
        SetError( INCORRECT_ARGUMENT_TYPE, errorMessage.str() );
        return false;
      } // else if: argument is node
      
      walk = walk->rightNode;
      
      if ( walk->rightNode == NULL ) {
        if ( walk->leftToken ) {
          if ( walk->leftToken->tokenType == FLOAT || walk->leftToken->tokenType == INT ) {
            if ( previousNumber == atof( walk->leftToken->content.c_str() ) ) {
              previousNumber = atof( walk->leftToken->content.c_str() );
            } // if: argument is float, set isFLoat
            
            else {
              result->tokenResult->tokenType = NIL;
              previousNumber = atof( walk->leftToken->content.c_str() );
            } // else: false
          } // if: check if add any float
          
          else if ( walk->leftToken->tokenType == SYMBOL ) {
            if ( walk->leftToken->isQuoteResult ) {
              string errorMessage = "ERROR (= with incorrect argument type) : ";
              errorMessage += walk->leftToken->content;
              SetError( INCORRECT_ARGUMENT_TYPE, errorMessage );
              return false;
            } // if: quote result
            
            else {
              if ( GetDefineBindings( walk->leftToken->content )->leftToken ) {
                TokenStruct *definedToken = GetDefineBindings( walk->leftToken->content )->leftToken;
                
                if ( definedToken->tokenType == FLOAT || definedToken->tokenType == INT ) {
                  if ( previousNumber == atof( definedToken->content.c_str() ) ) {
                    previousNumber = atof( definedToken->content.c_str() );
                  } // if: argument is float, set isFLoat
                  
                  else {
                    result->tokenResult->tokenType = NIL;
                    previousNumber = atof( definedToken->content.c_str() );
                  } // else: false
                } // if: check if add any float
                
                else {
                  string errorMessage = "ERROR (= with incorrect argument type) : ";
                  errorMessage += definedToken->content;
                  SetError( INCORRECT_ARGUMENT_TYPE, errorMessage );
                  return false;
                } // else: wrong type
              } // if: get defined token
              
              else {
                TreeStruct *definedNode = GetDefineBindings( walk->leftToken->content )->leftNode;
                stringstream errorMessage;
                errorMessage << "ERROR (= with incorrect argument type) : ";
                int layer = 0;
                AddTreeString( definedNode, layer, false, errorMessage );
                
                if ( layer > 0 ) {
                  while ( layer > 0 ) {
                    layer--;
                    errorMessage << string( layer, ' ' ) << ')' << endl;
                  } // while: loop print right-paren
                } // if: layer still greater than zero
                
                SetError( INCORRECT_ARGUMENT_TYPE, errorMessage.str() );
                return false;
              } // else: get defined node, can't evaluate
            } // else: not quote result
          } // else if: symbol, check defined or not
          
          else {
            string errorMessage = "ERROR (= with incorrect argument type) : ";
            errorMessage += walk->leftToken->content;
            SetError( INCORRECT_ARGUMENT_TYPE, errorMessage );
            return false;
          } // else: wrong argument type
        } // if: argument is token
        
        else if ( walk->leftNode ) {
          TreeStruct *errorNode = GetDefineBindings( walk->leftToken->content )->leftNode;
          stringstream errorMessage;
          errorMessage << "ERROR (= with incorrect argument type) : ";
          int layer = 0;
          AddTreeString( errorNode, layer, false, errorMessage );
          
          if ( layer > 0 ) {
            while ( layer > 0 ) {
              layer--;
              errorMessage << string( layer, ' ' ) << ')' << endl;
            } // while: loop print right-paren
          } // if: layer still greater than zero
          
          SetError( INCORRECT_ARGUMENT_TYPE, errorMessage.str() );
          return false;
        } // else if: argument is node
      } // if: add up the last argument
    } // while: add up the arguments
    
    if ( result->tokenResult->tokenType == T ) {
      result->tokenResult->content = "#t";
    } // if: the result is true, set the content
    
    else {
      result->tokenResult->content = "nil";
    } // else: the result is nil, set the content
    
    return true;
  } // EqualTo()
  
  bool StringAppend( vector<TreeStruct *> arguments, ResultStruct *result ) {
    result->hasTokenResult = true;
    result->tokenResult->tokenType = STRING;
    TreeStruct *walk = arguments.front();
    bool isFirstString = true;
    
    while ( walk->rightNode ) {
      if ( walk->leftToken ) {
        if ( walk->leftToken->tokenType == STRING ) {
          if ( isFirstString ) {
            walk->leftToken->content.erase( walk->leftToken->content.end() - 1 );
            result->tokenResult->content += walk->leftToken->content;
            isFirstString = false;
          } // if: isFirstString
          
          else {
            walk->leftToken->content.erase( walk->leftToken->content.begin() );
            walk->leftToken->content.erase( walk->leftToken->content.end() );
            result->tokenResult->content += walk->leftToken->content;
          } // else: not firstString
        } // if: string
        
        else if ( walk->leftToken->tokenType == SYMBOL ) {
          if ( walk->leftToken->isQuoteResult ) {
            string errorMessage = "ERROR (string-append with incorrect argument type) : ";
            errorMessage += walk->leftToken->content;
            SetError( INCORRECT_ARGUMENT_TYPE, errorMessage );
            return false;
          } // if: quote result
          
          else {
            if ( GetDefineBindings( walk->leftToken->content )->leftToken ) {
              TokenStruct *definedToken = GetDefineBindings( walk->leftToken->content )->leftToken;
              
              if ( definedToken->tokenType == STRING ) {
                if ( isFirstString ) {
                  walk->leftToken->content.erase( walk->leftToken->content.end() - 1 );
                  result->tokenResult->content += definedToken->content;
                  isFirstString = false;
                } // if: isFirstString
                
                else {
                  definedToken->content.erase( definedToken->content.begin() );
                  walk->leftToken->content.erase( walk->leftToken->content.end() );
                  result->tokenResult->content += definedToken->content;
                } // else: not firstString
              } // if: string
              
              else {
                string errorMessage = "ERROR (string-append with incorrect argument type) : ";
                errorMessage += definedToken->content;
                SetError( INCORRECT_ARGUMENT_TYPE, errorMessage );
                return false;
              } // else: other type, error
            } // if: defined as a token
            
            else {
              TreeStruct *definedNode = GetDefineBindings( walk->leftToken->content )->leftNode;
              stringstream errorMessage;
              errorMessage << "ERROR (string-append with incorrect argument type) : ";
              int layer = 0;
              AddTreeString( definedNode, layer, false, errorMessage );
              
              if ( layer > 0 ) {
                while ( layer > 0 ) {
                  layer--;
                  errorMessage << string( layer, ' ' ) << ')' << endl;
                } // while: loop print right-paren
              } // if: layer still greater than zero
              
              SetError( INCORRECT_ARGUMENT_TYPE, errorMessage.str() );
              return false;
            } // else: defined as a node
          } // else: not quote result
        } // else if: symbol
        
        else {
          string errorMessage = "ERROR (string-append with incorrect argument type) : ";
          errorMessage += walk->leftToken->content;
          SetError( INCORRECT_ARGUMENT_TYPE, errorMessage );
          return false;
        } // else: other type, error
      } // if: argument is token
      
      else {
        TreeStruct *errorNode = GetDefineBindings( walk->leftToken->content )->leftNode;
        stringstream errorMessage;
        errorMessage << "ERROR (string-append with incorrect argument type) : ";
        int layer = 0;
        AddTreeString( errorNode, layer, false, errorMessage );
        
        if ( layer > 0 ) {
          while ( layer > 0 ) {
            layer--;
            errorMessage << string( layer, ' ' ) << ')' << endl;
          } // while: loop print right-paren
        } // if: layer still greater than zero
        
        SetError( INCORRECT_ARGUMENT_TYPE, errorMessage.str() );
        return false;
      } // else: argument is node, error
      
      walk = walk->rightNode;
      
      if ( walk->rightNode == NULL ) {
        if ( walk->leftToken ) {
          if ( walk->leftToken->tokenType == STRING ) {
            walk->leftToken->content.erase( walk->leftToken->content.begin() );
            result->tokenResult->content += walk->leftToken->content;
          } // if: string
          
          else if ( walk->leftToken->tokenType == SYMBOL ) {
            if ( walk->leftToken->isQuoteResult ) {
              string errorMessage = "ERROR (string-append with incorrect argument type) : ";
              errorMessage += walk->leftToken->content;
              SetError( INCORRECT_ARGUMENT_TYPE, errorMessage );
              return false;
            } // if: quote result
            
            else {
              if ( GetDefineBindings( walk->leftToken->content )->leftToken ) {
                TokenStruct *definedToken = GetDefineBindings( walk->leftToken->content )->leftToken;
                
                if ( definedToken->tokenType == STRING ) {
                  definedToken->content.erase( definedToken->content.begin() );
                  result->tokenResult->content += definedToken->content;
                } // if: string
                
                else {
                  string errorMessage = "ERROR (string-append with incorrect argument type) : ";
                  errorMessage += definedToken->content;
                  SetError( INCORRECT_ARGUMENT_TYPE, errorMessage );
                  return false;
                } // else: other type, error
              } // if: defined as a token
              
              else {
                TreeStruct *definedNode = GetDefineBindings( walk->leftToken->content )->leftNode;
                stringstream errorMessage;
                errorMessage << "ERROR (string-append with incorrect argument type) : ";
                int layer = 0;
                AddTreeString( definedNode, layer, false, errorMessage );
                
                if ( layer > 0 ) {
                  while ( layer > 0 ) {
                    layer--;
                    errorMessage << string( layer, ' ' ) << ')' << endl;
                  } // while: loop print right-paren
                } // if: layer still greater than zero
                
                SetError( INCORRECT_ARGUMENT_TYPE, errorMessage.str() );
                return false;
              } // else: defined as a node
            } // else: not quote result
          } // else if: symbol
          
          else {
            string errorMessage = "ERROR (string-append with incorrect argument type) : ";
            errorMessage += walk->leftToken->content;
            SetError( INCORRECT_ARGUMENT_TYPE, errorMessage );
            return false;
          } // else: other type, error
        } // if: argument is token
        
        else {
          TreeStruct *errorNode = GetDefineBindings( walk->leftToken->content )->leftNode;
          stringstream errorMessage;
          errorMessage << "ERROR (string-append with incorrect argument type) : ";
          int layer = 0;
          AddTreeString( errorNode, layer, false, errorMessage );
          
          if ( layer > 0 ) {
            while ( layer > 0 ) {
              layer--;
              errorMessage << string( layer, ' ' ) << ')' << endl;
            } // while: loop print right-paren
          } // if: layer still greater than zero
          
          SetError( INCORRECT_ARGUMENT_TYPE, errorMessage.str() );
          return false;
        } // else: argument is node, error
      } // if: lastNode
    } // while: go through the arguments
    
    return true;
  } // StringAppend()
  
  bool StringGreaterThan( vector<TreeStruct *> arguments, ResultStruct *result ) {
    result->hasTokenResult = true;
    result->tokenResult->tokenType = T;
    TreeStruct *walk = arguments.front();
    string previousString = "\0";
    bool isFirstString = true;
    
    while ( walk->rightNode ) {
      if ( walk->leftToken ) {
        if ( walk->leftToken->tokenType == STRING ) {
          if ( isFirstString ) {
            previousString = walk->leftToken->content;
            isFirstString = false;
          } // if: isFirstString
          
          else {
            if ( previousString > walk->leftToken->content ) {
              previousString = walk->leftToken->content;
            } // if: greater than
            
            else {
              result->tokenResult->tokenType = NIL;
              previousString = walk->leftToken->content;
            } // else: less than
          } // else: not firstString
        } // if: string
        
        else if ( walk->leftToken->tokenType == SYMBOL ) {
          if ( walk->leftToken->isQuoteResult ) {
            string errorMessage = "ERROR (string>? with incorrect argument type) : ";
            errorMessage += walk->leftToken->content;
            SetError( INCORRECT_ARGUMENT_TYPE, errorMessage );
            return false;
          } // if: quote result
          
          else {
            if ( GetDefineBindings( walk->leftToken->content )->leftToken ) {
              TokenStruct *definedToken = GetDefineBindings( walk->leftToken->content )->leftToken;
              
              if ( definedToken->tokenType == STRING ) {
                if ( isFirstString ) {
                  previousString = walk->leftToken->content;
                  isFirstString = false;
                } // if: isFirstString
                
                else {
                  if ( previousString > walk->leftToken->content ) {
                    previousString = walk->leftToken->content;
                  } // if: greater than
                  
                  else {
                    result->tokenResult->tokenType = NIL;
                    previousString = walk->leftToken->content;
                  } // else: less than
                } // else: not firstString
              } // if: string
              
              else {
                string errorMessage = "ERROR (string>? with incorrect argument type) : ";
                errorMessage += definedToken->content;
                SetError( INCORRECT_ARGUMENT_TYPE, errorMessage );
                return false;
              } // else: other type, error
            } // if: defined as a token
            
            else {
              TreeStruct *definedNode = GetDefineBindings( walk->leftToken->content )->leftNode;
              stringstream errorMessage;
              errorMessage << "ERROR (string>? with incorrect argument type) : ";
              int layer = 0;
              AddTreeString( definedNode, layer, false, errorMessage );
              
              if ( layer > 0 ) {
                while ( layer > 0 ) {
                  layer--;
                  errorMessage << string( layer, ' ' ) << ')' << endl;
                } // while: loop print right-paren
              } // if: layer still greater than zero
              
              SetError( INCORRECT_ARGUMENT_TYPE, errorMessage.str() );
              return false;
            } // else: defined as a node
          } // else: not quote result
        } // else if: symbol
        
        else {
          string errorMessage = "ERROR (string>? with incorrect argument type) : ";
          errorMessage += walk->leftToken->content;
          SetError( INCORRECT_ARGUMENT_TYPE, errorMessage );
          return false;
        } // else: other type, error
      } // if: argument is token
      
      else {
        TreeStruct *errorNode = GetDefineBindings( walk->leftToken->content )->leftNode;
        stringstream errorMessage;
        errorMessage << "ERROR (string>? with incorrect argument type) : ";
        int layer = 0;
        AddTreeString( errorNode, layer, false, errorMessage );
        
        if ( layer > 0 ) {
          while ( layer > 0 ) {
            layer--;
            errorMessage << string( layer, ' ' ) << ')' << endl;
          } // while: loop print right-paren
        } // if: layer still greater than zero
        
        SetError( INCORRECT_ARGUMENT_TYPE, errorMessage.str() );
        return false;
      } // else: argument is node, error
      
      walk = walk->rightNode;
      
      if ( walk->rightNode == NULL ) {
        if ( walk->leftToken ) {
          if ( walk->leftToken->tokenType == STRING ) {
            if ( previousString > walk->leftToken->content ) {
              previousString = walk->leftToken->content;
            } // if: greater than
            
            else {
              result->tokenResult->tokenType = NIL;
              previousString = walk->leftToken->content;
            } // else: less than
          } // if: string
          
          else if ( walk->leftToken->tokenType == SYMBOL ) {
            if ( walk->leftToken->isQuoteResult ) {
              string errorMessage = "ERROR (string>? with incorrect argument type) : ";
              errorMessage += walk->leftToken->content;
              SetError( INCORRECT_ARGUMENT_TYPE, errorMessage );
              return false;
            } // if: quote result
            
            else {
              if ( GetDefineBindings( walk->leftToken->content )->leftToken ) {
                TokenStruct *definedToken = GetDefineBindings( walk->leftToken->content )->leftToken;
                
                if ( definedToken->tokenType == STRING ) {
                  
                  if ( previousString > walk->leftToken->content ) {
                    previousString = walk->leftToken->content;
                  } // if: greater than
                  
                  else {
                    result->tokenResult->tokenType = NIL;
                    previousString = walk->leftToken->content;
                  } // else: less than
                } // if: string
                
                else {
                  string errorMessage = "ERROR (string>? with incorrect argument type) : ";
                  errorMessage += definedToken->content;
                  SetError( INCORRECT_ARGUMENT_TYPE, errorMessage );
                  return false;
                } // else: other type, error
              } // if: defined as a token
              
              else {
                TreeStruct *definedNode = GetDefineBindings( walk->leftToken->content )->leftNode;
                stringstream errorMessage;
                errorMessage << "ERROR (string>? with incorrect argument type) : ";
                int layer = 0;
                AddTreeString( definedNode, layer, false, errorMessage );
                
                if ( layer > 0 ) {
                  while ( layer > 0 ) {
                    layer--;
                    errorMessage << string( layer, ' ' ) << ')' << endl;
                  } // while: loop print right-paren
                } // if: layer still greater than zero
                
                SetError( INCORRECT_ARGUMENT_TYPE, errorMessage.str() );
                return false;
              } // else: defined as a node
            } // else: not quote result
          } // else if: symbol
          
          else {
            string errorMessage = "ERROR (string>? with incorrect argument type) : ";
            errorMessage += walk->leftToken->content;
            SetError( INCORRECT_ARGUMENT_TYPE, errorMessage );
            return false;
          } // else: other type, error
        } // if: argument is token
        
        else {
          TreeStruct *errorNode = GetDefineBindings( walk->leftToken->content )->leftNode;
          stringstream errorMessage;
          errorMessage << "ERROR (string>? with incorrect argument type) : ";
          int layer = 0;
          AddTreeString( errorNode, layer, false, errorMessage );
          
          if ( layer > 0 ) {
            while ( layer > 0 ) {
              layer--;
              errorMessage << string( layer, ' ' ) << ')' << endl;
            } // while: loop print right-paren
          } // if: layer still greater than zero
          
          SetError( INCORRECT_ARGUMENT_TYPE, errorMessage.str() );
          return false;
        } // else: argument is node, error
      } // if: lastNode
    } // while: go through the arguments
    
    if ( result->tokenResult->tokenType == T ) {
      result->tokenResult->content = "#t";
    } // if: the result is true, set the content
    
    else {
      result->tokenResult->content = "nil";
    } // else: the result is nil, set the content
    
    return true;
  } // StringGreaterThan()
  
  bool StringLessThan( vector<TreeStruct *> arguments, ResultStruct *result ) {
    result->hasTokenResult = true;
    result->tokenResult->tokenType = T;
    TreeStruct *walk = arguments.front();
    string previousString = "\0";
    bool isFirstString = true;
    
    while ( walk->rightNode ) {
      if ( walk->leftToken ) {
        if ( walk->leftToken->tokenType == STRING ) {
          if ( isFirstString ) {
            previousString = walk->leftToken->content;
            isFirstString = false;
          } // if: isFirstString
          
          else {
            if ( previousString < walk->leftToken->content ) {
              previousString = walk->leftToken->content;
            } // if: greater than
            
            else {
              result->tokenResult->tokenType = NIL;
              previousString = walk->leftToken->content;
            } // else: less than
          } // else: not firstString
        } // if: string
        
        else if ( walk->leftToken->tokenType == SYMBOL ) {
          if ( walk->leftToken->isQuoteResult ) {
            string errorMessage = "ERROR (string<? with incorrect argument type) : ";
            errorMessage += walk->leftToken->content;
            SetError( INCORRECT_ARGUMENT_TYPE, errorMessage );
            return false;
          } // if: quote result
          
          else {
            if ( GetDefineBindings( walk->leftToken->content )->leftToken ) {
              TokenStruct *definedToken = GetDefineBindings( walk->leftToken->content )->leftToken;
              
              if ( definedToken->tokenType == STRING ) {
                if ( isFirstString ) {
                  previousString = walk->leftToken->content;
                  isFirstString = false;
                } // if: isFirstString
                
                else {
                  if ( previousString < walk->leftToken->content ) {
                    previousString = walk->leftToken->content;
                  } // if: greater than
                  
                  else {
                    result->tokenResult->tokenType = NIL;
                    previousString = walk->leftToken->content;
                  } // else: less than
                } // else: not firstString
              } // if: string
              
              else {
                string errorMessage = "ERROR (string<? with incorrect argument type) : ";
                errorMessage += definedToken->content;
                SetError( INCORRECT_ARGUMENT_TYPE, errorMessage );
                return false;
              } // else: other type, error
            } // if: defined as a token
            
            else {
              TreeStruct *definedNode = GetDefineBindings( walk->leftToken->content )->leftNode;
              stringstream errorMessage;
              errorMessage << "ERROR (string<? with incorrect argument type) : ";
              int layer = 0;
              AddTreeString( definedNode, layer, false, errorMessage );
              
              if ( layer > 0 ) {
                while ( layer > 0 ) {
                  layer--;
                  errorMessage << string( layer, ' ' ) << ')' << endl;
                } // while: loop print right-paren
              } // if: layer still greater than zero
              
              SetError( INCORRECT_ARGUMENT_TYPE, errorMessage.str() );
              return false;
            } // else: defined as a node
          } // else: not quote result
        } // else if: symbol
        
        else {
          string errorMessage = "ERROR (string<? with incorrect argument type) : ";
          errorMessage += walk->leftToken->content;
          SetError( INCORRECT_ARGUMENT_TYPE, errorMessage );
          return false;
        } // else: other type, error
      } // if: argument is token
      
      else {
        TreeStruct *errorNode = GetDefineBindings( walk->leftToken->content )->leftNode;
        stringstream errorMessage;
        errorMessage << "ERROR (string<? with incorrect argument type) : ";
        int layer = 0;
        AddTreeString( errorNode, layer, false, errorMessage );
        
        if ( layer > 0 ) {
          while ( layer > 0 ) {
            layer--;
            errorMessage << string( layer, ' ' ) << ')' << endl;
          } // while: loop print right-paren
        } // if: layer still greater than zero
        
        SetError( INCORRECT_ARGUMENT_TYPE, errorMessage.str() );
        return false;
      } // else: argument is node, error
      
      walk = walk->rightNode;
      
      if ( walk->rightNode == NULL ) {
        if ( walk->leftToken ) {
          if ( walk->leftToken->tokenType == STRING ) {
            if ( previousString < walk->leftToken->content ) {
              previousString = walk->leftToken->content;
            } // if: greater than
            
            else {
              result->tokenResult->tokenType = NIL;
              previousString = walk->leftToken->content;
            } // else: less than
          } // if: string
          
          else if ( walk->leftToken->tokenType == SYMBOL ) {
            if ( walk->leftToken->isQuoteResult ) {
              string errorMessage = "ERROR (string<? with incorrect argument type) : ";
              errorMessage += walk->leftToken->content;
              SetError( INCORRECT_ARGUMENT_TYPE, errorMessage );
              return false;
            } // if: quote result
            
            else {
              if ( GetDefineBindings( walk->leftToken->content )->leftToken ) {
                TokenStruct *definedToken = GetDefineBindings( walk->leftToken->content )->leftToken;
                
                if ( definedToken->tokenType == STRING ) {
                  
                  if ( previousString < walk->leftToken->content ) {
                    previousString = walk->leftToken->content;
                  } // if: greater than
                  
                  else {
                    result->tokenResult->tokenType = NIL;
                    previousString = walk->leftToken->content;
                  } // else: less than
                } // if: string
                
                else {
                  string errorMessage = "ERROR (string<? with incorrect argument type) : ";
                  errorMessage += definedToken->content;
                  SetError( INCORRECT_ARGUMENT_TYPE, errorMessage );
                  return false;
                } // else: other type, error
              } // if: defined as a token
              
              else {
                TreeStruct *definedNode = GetDefineBindings( walk->leftToken->content )->leftNode;
                stringstream errorMessage;
                errorMessage << "ERROR (string<? with incorrect argument type) : ";
                int layer = 0;
                AddTreeString( definedNode, layer, false, errorMessage );
                
                if ( layer > 0 ) {
                  while ( layer > 0 ) {
                    layer--;
                    errorMessage << string( layer, ' ' ) << ')' << endl;
                  } // while: loop print right-paren
                } // if: layer still greater than zero
                
                SetError( INCORRECT_ARGUMENT_TYPE, errorMessage.str() );
                return false;
              } // else: defined as a node
            } // else: not quote result
          } // else if: symbol
          
          else {
            string errorMessage = "ERROR (string<? with incorrect argument type) : ";
            errorMessage += walk->leftToken->content;
            SetError( INCORRECT_ARGUMENT_TYPE, errorMessage );
            return false;
          } // else: other type, error
        } // if: argument is token
        
        else {
          TreeStruct *errorNode = GetDefineBindings( walk->leftToken->content )->leftNode;
          stringstream errorMessage;
          errorMessage << "ERROR (string<? with incorrect argument type) : ";
          int layer = 0;
          AddTreeString( errorNode, layer, false, errorMessage );
          
          if ( layer > 0 ) {
            while ( layer > 0 ) {
              layer--;
              errorMessage << string( layer, ' ' ) << ')' << endl;
            } // while: loop print right-paren
          } // if: layer still greater than zero
          
          SetError( INCORRECT_ARGUMENT_TYPE, errorMessage.str() );
          return false;
        } // else: argument is node, error
      } // if: lastNode
    } // while: go through the arguments
    
    if ( result->tokenResult->tokenType == T ) {
      result->tokenResult->content = "#t";
    } // if: the result is true, set the content
    
    else {
      result->tokenResult->content = "nil";
    } // else: the result is nil, set the content
    
    return true;
  } // StringLessThan()
  
  bool StringEqualTo( vector<TreeStruct *> arguments, ResultStruct *result ) {
    result->hasTokenResult = true;
    result->tokenResult->tokenType = T;
    TreeStruct *walk = arguments.front();
    string previousString = "\0";
    bool isFirstString = true;
    
    while ( walk->rightNode ) {
      if ( walk->leftToken ) {
        if ( walk->leftToken->tokenType == STRING ) {
          if ( isFirstString ) {
            previousString = walk->leftToken->content;
            isFirstString = false;
          } // if: isFirstString
          
          else {
            if ( previousString == walk->leftToken->content ) {
              previousString = walk->leftToken->content;
            } // if: greater than
            
            else {
              result->tokenResult->tokenType = NIL;
              previousString = walk->leftToken->content;
            } // else: less than
          } // else: not firstString
        } // if: string
        
        else if ( walk->leftToken->tokenType == SYMBOL ) {
          if ( walk->leftToken->isQuoteResult ) {
            string errorMessage = "ERROR (string=? with incorrect argument type) : ";
            errorMessage += walk->leftToken->content;
            SetError( INCORRECT_ARGUMENT_TYPE, errorMessage );
            return false;
          } // if: quote result
          
          else {
            if ( GetDefineBindings( walk->leftToken->content )->leftToken ) {
              TokenStruct *definedToken = GetDefineBindings( walk->leftToken->content )->leftToken;
              
              if ( definedToken->tokenType == STRING ) {
                if ( isFirstString ) {
                  previousString = walk->leftToken->content;
                  isFirstString = false;
                } // if: isFirstString
                
                else {
                  if ( previousString == walk->leftToken->content ) {
                    previousString = walk->leftToken->content;
                  } // if: greater than
                  
                  else {
                    result->tokenResult->tokenType = NIL;
                    previousString = walk->leftToken->content;
                  } // else: less than
                } // else: not firstString
              } // if: string
              
              else {
                string errorMessage = "ERROR (string=? with incorrect argument type) : ";
                errorMessage += definedToken->content;
                SetError( INCORRECT_ARGUMENT_TYPE, errorMessage );
                return false;
              } // else: other type, error
            } // if: defined as a token
            
            else {
              TreeStruct *definedNode = GetDefineBindings( walk->leftToken->content )->leftNode;
              stringstream errorMessage;
              errorMessage << "ERROR (string=? with incorrect argument type) : ";
              int layer = 0;
              AddTreeString( definedNode, layer, false, errorMessage );
              
              if ( layer > 0 ) {
                while ( layer > 0 ) {
                  layer--;
                  errorMessage << string( layer, ' ' ) << ')' << endl;
                } // while: loop print right-paren
              } // if: layer still greater than zero
              
              SetError( INCORRECT_ARGUMENT_TYPE, errorMessage.str() );
              return false;
            } // else: defined as a node
          } // else: not quote result
        } // else if: symbol
        
        else {
          string errorMessage = "ERROR (string=? with incorrect argument type) : ";
          errorMessage += walk->leftToken->content;
          SetError( INCORRECT_ARGUMENT_TYPE, errorMessage );
          return false;
        } // else: other type, error
      } // if: argument is token
      
      else {
        TreeStruct *errorNode = GetDefineBindings( walk->leftToken->content )->leftNode;
        stringstream errorMessage;
        errorMessage << "ERROR (string=? with incorrect argument type) : ";
        int layer = 0;
        AddTreeString( errorNode, layer, false, errorMessage );
        
        if ( layer > 0 ) {
          while ( layer > 0 ) {
            layer--;
            errorMessage << string( layer, ' ' ) << ')' << endl;
          } // while: loop print right-paren
        } // if: layer still greater than zero
        
        SetError( INCORRECT_ARGUMENT_TYPE, errorMessage.str() );
        return false;
      } // else: argument is node, error
      
      walk = walk->rightNode;
      
      if ( walk->rightNode == NULL ) {
        if ( walk->leftToken ) {
          if ( walk->leftToken->tokenType == STRING ) {
            if ( previousString == walk->leftToken->content ) {
              previousString = walk->leftToken->content;
            } // if: greater than
            
            else {
              result->tokenResult->tokenType = NIL;
              previousString = walk->leftToken->content;
            } // else: less than
          } // if: string
          
          else if ( walk->leftToken->tokenType == SYMBOL ) {
            if ( walk->leftToken->isQuoteResult ) {
              string errorMessage = "ERROR (string=? with incorrect argument type) : ";
              errorMessage += walk->leftToken->content;
              SetError( INCORRECT_ARGUMENT_TYPE, errorMessage );
              return false;
            } // if: quote result
            
            else {
              if ( GetDefineBindings( walk->leftToken->content )->leftToken ) {
                TokenStruct *definedToken = GetDefineBindings( walk->leftToken->content )->leftToken;
                
                if ( definedToken->tokenType == STRING ) {
                  
                  if ( previousString == walk->leftToken->content ) {
                    previousString = walk->leftToken->content;
                  } // if: greater than
                  
                  else {
                    result->tokenResult->tokenType = NIL;
                    previousString = walk->leftToken->content;
                  } // else: less than
                } // if: string
                
                else {
                  string errorMessage = "ERROR (string=? with incorrect argument type) : ";
                  errorMessage += definedToken->content;
                  SetError( INCORRECT_ARGUMENT_TYPE, errorMessage );
                  return false;
                } // else: other type, error
              } // if: defined as a token
              
              else {
                TreeStruct *definedNode = GetDefineBindings( walk->leftToken->content )->leftNode;
                stringstream errorMessage;
                errorMessage << "ERROR (string=? with incorrect argument type) : ";
                int layer = 0;
                AddTreeString( definedNode, layer, false, errorMessage );
                
                if ( layer > 0 ) {
                  while ( layer > 0 ) {
                    layer--;
                    errorMessage << string( layer, ' ' ) << ')' << endl;
                  } // while: loop print right-paren
                } // if: layer still greater than zero
                
                SetError( INCORRECT_ARGUMENT_TYPE, errorMessage.str() );
                return false;
              } // else: defined as a node
            } // else: not quote result
          } // else if: symbol
          
          else {
            string errorMessage = "ERROR (string=? with incorrect argument type) : ";
            errorMessage += walk->leftToken->content;
            SetError( INCORRECT_ARGUMENT_TYPE, errorMessage );
            return false;
          } // else: other type, error
        } // if: argument is token
        
        else {
          TreeStruct *errorNode = GetDefineBindings( walk->leftToken->content )->leftNode;
          stringstream errorMessage;
          errorMessage << "ERROR (string=? with incorrect argument type) : ";
          int layer = 0;
          AddTreeString( errorNode, layer, false, errorMessage );
          
          if ( layer > 0 ) {
            while ( layer > 0 ) {
              layer--;
              errorMessage << string( layer, ' ' ) << ')' << endl;
            } // while: loop print right-paren
          } // if: layer still greater than zero
          
          SetError( INCORRECT_ARGUMENT_TYPE, errorMessage.str() );
          return false;
        } // else: argument is node, error
      } // if: lastNode
    } // while: go through the arguments
    
    if ( result->tokenResult->tokenType == T ) {
      result->tokenResult->content = "#t";
    } // if: the result is true, set the content
    
    else {
      result->tokenResult->content = "nil";
    } // else: the result is nil, set the content
    
    return true;
  } // StringEqualTo()
  
  bool Eqv( vector<TreeStruct *> arguments, ResultStruct *result ) {
    TreeStruct *firstTreeArgument = NULL;
    TreeStruct *secondTreeArgument = NULL;
    TokenStruct *firstTokenArgument = NULL;
    TokenStruct *secondTokenArgument = NULL;
    result->hasTokenResult = true;
    
    if ( arguments[0]->leftToken ) {
      if ( arguments[0]->leftToken->tokenType == SYMBOL ) {
        if ( arguments[0]->leftToken->isQuoteResult ) {
          firstTokenArgument = arguments[0]->leftToken;
        } // if: quote result
        
        else {
          if ( GetDefineBindings( arguments[0]->leftToken->content )->leftToken ) {
            firstTokenArgument = GetDefineBindings( arguments[0]->leftToken->content )->leftToken;
          } // if: defined as token
          
          else {
            firstTreeArgument = GetDefineBindings( arguments[0]->leftToken->content )->leftNode;
          } // else: defined as node
        } // else: not quote result
      } // if: symbol
      
      else {
        firstTokenArgument = arguments[0]->leftToken;
      } // else: other types
    } // if: first argument is token
    
    else {
      firstTreeArgument = arguments[0]->leftNode;
    } // else: first argument is node
    
    if ( arguments[1]->leftToken ) {
      if ( arguments[1]->leftToken->tokenType == SYMBOL ) {
        if ( arguments[1]->leftToken->isQuoteResult ) {
          secondTokenArgument = arguments[1]->leftToken;
        } // if: quote result
        
        else {
          if ( GetDefineBindings( arguments[1]->leftToken->content )->leftToken ) {
            secondTokenArgument = GetDefineBindings( arguments[1]->leftToken->content )->leftToken;
          } // if: defined as token
          
          else {
            secondTreeArgument = GetDefineBindings( arguments[1]->leftToken->content )->leftNode;
          } // else: defined as node
        } // else: not quote result
      } // if: symbol
      
      else {
        secondTokenArgument = arguments[1]->leftToken;
      } // else: other types
    } // if: second argument is token
    
    else {
      secondTreeArgument = arguments[1]->leftNode;
    } // else: second argument is node
    
    if ( firstTokenArgument ) {
      if ( secondTokenArgument ) {
        if ( firstTokenArgument->tokenType == STRING || secondTokenArgument->tokenType == STRING ) {
          result->tokenResult->tokenType = NIL;
        } // if: any of the token is a string, then nil
        
        else {
          result->tokenResult->tokenType = T;
        } // else: both atom, t
      } // if: secondTokenArgument
      
      else {
        result->tokenResult->tokenType = NIL;
      } // else: nil
    } // if: firstTokenArgument
    
    else {
      if ( secondTokenArgument ) {
        result->tokenResult->tokenType = NIL;
      } // if: secondTokenArgument
      
      else {
        if ( firstTreeArgument == secondTreeArgument ) {
          result->tokenResult->tokenType = T;
        } // if: node address the same
        
        else {
          result->tokenResult->tokenType = NIL;
        } // else: node address aren't the same
      } // else: secondTreeArgument
    } // else: firstTreeArgument
    
    if ( result->tokenResult->tokenType == T ) {
      result->tokenResult->content = "#t";
    } // if: the result is true, set the content
    
    else {
      result->tokenResult->content = "nil";
    } // else: the result is nil, set the content
    
    return true;
  } // Eqv()
  
  bool Equal( vector<TreeStruct *> arguments, ResultStruct *result ) {
    TreeStruct *firstTreeArgument = NULL;
    TreeStruct *secondTreeArgument = NULL;
    TokenStruct *firstTokenArgument = NULL;
    TokenStruct *secondTokenArgument = NULL;
    result->hasTokenResult = true;
    
    if ( arguments[0]->leftToken ) {
      if ( arguments[0]->leftToken->tokenType == SYMBOL ) {
        if ( arguments[0]->leftToken->isQuoteResult ) {
          firstTokenArgument = arguments[0]->leftToken;
        } // if: quote result
        
        else {
          if ( GetDefineBindings( arguments[0]->leftToken->content )->leftToken ) {
            firstTokenArgument = GetDefineBindings( arguments[0]->leftToken->content )->leftToken;
          } // if: defined as token
          
          else {
            firstTreeArgument = GetDefineBindings( arguments[0]->leftToken->content )->leftNode;
          } // else: defined as node
        } // else: not quote result
      } // if: symbol
      
      else {
        firstTokenArgument = arguments[0]->leftToken;
      } // else: other types
    } // if: first argument is token
    
    else {
      firstTreeArgument = arguments[0]->leftNode;
    } // else: first argument is node
    
    if ( arguments[1]->leftToken ) {
      if ( arguments[1]->leftToken->tokenType == SYMBOL ) {
        if ( arguments[1]->leftToken->isQuoteResult ) {
          secondTokenArgument = arguments[1]->leftToken;
        } // if: quote result
        
        else {
          if ( GetDefineBindings( arguments[1]->leftToken->content )->leftToken ) {
            secondTokenArgument = GetDefineBindings( arguments[1]->leftToken->content )->leftToken;
          } // if: defined as token
          
          else {
            secondTreeArgument = GetDefineBindings( arguments[1]->leftToken->content )->leftNode;
          } // else: defined as node
        } // else: not quote result
      } // if: symbol
      
      else {
        secondTokenArgument = arguments[1]->leftToken;
      } // else: other types
    } // if: second argument is token
    
    else {
      secondTreeArgument = arguments[1]->leftNode;
    } // else: second argument is node
    
    if ( firstTokenArgument ) {
      if ( secondTokenArgument ) {
        if ( firstTokenArgument->content == secondTokenArgument->content ) {
          result->tokenResult->tokenType = T;
        } // if: same content
        
        else {
          result->tokenResult->tokenType = NIL;
        } // else: both atom, t
      } // if: secondTokenArgument
      
      else {
        result->tokenResult->tokenType = NIL;
      } // else: nil
    } // if: firstTokenArgument
    
    else {
      if ( secondTokenArgument ) {
        result->tokenResult->tokenType = NIL;
      } // if: secondTokenArgument
      
      else {
        if ( CompareTwoTrees( firstTreeArgument, secondTreeArgument ) ) {
          result->tokenResult->tokenType = T;
        } // if: node address the same
        
        else {
          result->tokenResult->tokenType = NIL;
        } // else: node address aren't the same
      } // else: secondTreeArgument
    } // else: firstTreeArgument
    
    if ( result->tokenResult->tokenType == T ) {
      result->tokenResult->content = "#t";
    } // if: the result is true, set the content
    
    else {
      result->tokenResult->content = "nil";
    } // else: the result is nil, set the content
    
    return true;
  } // Equal()
  
  bool If( vector<TreeStruct *> arguments, ResultStruct *result ) {
    TreeStruct *firstTreeArgument = NULL;
    TreeStruct *secondTreeArgument = NULL;
    TokenStruct *firstTokenArgument = NULL;
    TokenStruct *secondTokenArgument = NULL;
    result->hasTokenResult = true;
    
    if ( arguments[0]->leftToken ) {
      if ( arguments[0]->leftToken->tokenType == SYMBOL ) {
        if ( GetDefineBindings( arguments[0]->leftToken->content )->leftToken ) {
          firstTokenArgument = GetDefineBindings( arguments[0]->leftToken->content )->leftToken;
        } // if: defined as token
        
        else {
          firstTreeArgument = GetDefineBindings( arguments[0]->leftToken->content )->leftNode;
        } // else: defined as node
      } // if: symbol
      
      else {
        firstTokenArgument = arguments[0]->leftToken;
      } // else: other types
    } // if: first argument is token
    
    else {
      firstTreeArgument = arguments[0]->leftNode;
    } // else: first argument is node
    
    if ( arguments[1]->leftToken ) {
      if ( arguments[1]->leftToken->tokenType == SYMBOL ) {
        if ( GetDefineBindings( arguments[1]->leftToken->content )->leftToken ) {
          secondTokenArgument = GetDefineBindings( arguments[1]->leftToken->content )->leftToken;
        } // if: defined as token
        
        else {
          secondTreeArgument = GetDefineBindings( arguments[1]->leftToken->content )->leftNode;
        } // else: defined as node
      } // if: symbol
      
      else {
        secondTokenArgument = arguments[1]->leftToken;
      } // else: other types
    } // if: second argument is token
    
    else {
      secondTreeArgument = arguments[1]->leftNode;
    } // else: second argument is node
    
    if ( firstTokenArgument ) {
      if ( secondTokenArgument ) {
        if ( firstTokenArgument->content == secondTokenArgument->content ) {
          result->tokenResult->tokenType = T;
        } // if: same content
        
        else {
          result->tokenResult->tokenType = NIL;
        } // else: both atom, t
      } // if: secondTokenArgument
      
      else {
        result->tokenResult->tokenType = NIL;
      } // else: nil
    } // if: firstTokenArgument
    
    else {
      if ( secondTokenArgument ) {
        result->tokenResult->tokenType = NIL;
      } // if: secondTokenArgument
      
      else {
        if ( CompareTwoTrees( firstTreeArgument, secondTreeArgument ) ) {
          result->tokenResult->tokenType = T;
        } // if: node address the same
        
        else {
          result->tokenResult->tokenType = NIL;
        } // else: node address aren't the same
      } // else: secondTreeArgument
    } // else: firstTreeArgument
    
    if ( result->tokenResult->tokenType == T ) {
      result->tokenResult->content = "#t";
    } // if: the result is true, set the content
    
    else {
      result->tokenResult->content = "nil";
    } // else: the result is nil, set the content
    
    return true;
  } // If()
  
  bool Cond( vector<TreeStruct *> arguments, ResultStruct *result ) {
    TreeStruct *firstTreeArgument = NULL;
    TreeStruct *secondTreeArgument = NULL;
    TokenStruct *firstTokenArgument = NULL;
    TokenStruct *secondTokenArgument = NULL;
    result->hasTokenResult = true;
    
    if ( arguments[0]->leftToken ) {
      if ( arguments[0]->leftToken->tokenType == SYMBOL ) {
        if ( GetDefineBindings( arguments[0]->leftToken->content )->leftToken ) {
          firstTokenArgument = GetDefineBindings( arguments[0]->leftToken->content )->leftToken;
        } // if: defined as token
        
        else {
          firstTreeArgument = GetDefineBindings( arguments[0]->leftToken->content )->leftNode;
        } // else: defined as node
      } // if: symbol
      
      else {
        firstTokenArgument = arguments[0]->leftToken;
      } // else: other types
    } // if: first argument is token
    
    else {
      firstTreeArgument = arguments[0]->leftNode;
    } // else: first argument is node
    
    if ( arguments[1]->leftToken ) {
      if ( arguments[1]->leftToken->tokenType == SYMBOL ) {
        if ( GetDefineBindings( arguments[1]->leftToken->content )->leftToken ) {
          secondTokenArgument = GetDefineBindings( arguments[1]->leftToken->content )->leftToken;
        } // if: defined as token
        
        else {
          secondTreeArgument = GetDefineBindings( arguments[1]->leftToken->content )->leftNode;
        } // else: defined as node
      } // if: symbol
      
      else {
        secondTokenArgument = arguments[1]->leftToken;
      } // else: other types
    } // if: second argument is token
    
    else {
      secondTreeArgument = arguments[1]->leftNode;
    } // else: second argument is node
    
    if ( firstTokenArgument ) {
      if ( secondTokenArgument ) {
        if ( firstTokenArgument->content == secondTokenArgument->content ) {
          result->tokenResult->tokenType = T;
        } // if: same content
        
        else {
          result->tokenResult->tokenType = NIL;
        } // else: both atom, t
      } // if: secondTokenArgument
      
      else {
        result->tokenResult->tokenType = NIL;
      } // else: nil
    } // if: firstTokenArgument
    
    else {
      if ( secondTokenArgument ) {
        result->tokenResult->tokenType = NIL;
      } // if: secondTokenArgument
      
      else {
        if ( CompareTwoTrees( firstTreeArgument, secondTreeArgument ) ) {
          result->tokenResult->tokenType = T;
        } // if: node address the same
        
        else {
          result->tokenResult->tokenType = NIL;
        } // else: node address aren't the same
      } // else: secondTreeArgument
    } // else: firstTreeArgument
    
    if ( result->tokenResult->tokenType == T ) {
      result->tokenResult->content = "#t";
    } // if: the result is true, set the content
    
    else {
      result->tokenResult->content = "nil";
    } // else: the result is nil, set the content
    
    return true;
  } // Cond()
  
  /*
  ------------------- Print ------------------
  ------------------ Result ------------------
  */
  
  void PrintResult() {
    if ( m_Root->leftToken == NULL && m_Root->leftNode == NULL ) {
      return;
    } // if: no results
    
    if ( m_Root->leftToken ) {
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
    
    else if ( m_Root->leftNode ) {
      int layer = 0;
      bool isRightNode = false;
      PrintTree( m_Root->leftNode, layer, isRightNode );
      
      if ( layer > 0 ) {
        while ( layer > 0 ) {
          layer--;
          cout << string( layer, ' ' ) << ')' << endl;
        } // while: loop print right-paren
      } // if: layer still greater than zero
    } // else if: tree structre form result
  } // PrintResult()
  
  void PrintTree( TreeStruct *currentRoot, int &layer, bool isRightNode ) {
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
      PrintTree( currentRoot->leftNode, layer, isRightNode );
    } // if: has leftNode
    
    if ( currentRoot->leftToken ) {
      if ( hasLineReturn ) {
        cout << string( layer*2, ' ' );
      } // if: has line return
      
      PrintData( currentRoot->leftToken );
      isRightNode = false;
      hasLineReturn = true;
      
      if ( currentRoot->rightToken && currentRoot->rightToken->tokenType != NIL ) {
        cout << string( layer*2, ' ' ) << '.' << endl << string( layer*2, ' ' );
        PrintData( currentRoot->rightToken );
      } // if: has data in rightToken
    } // if: has data in leftToken
    
    else if ( currentRoot->rightToken ) {
      cout << string( layer*2, ' ' ) << '.' << endl << string( layer*2, ' ' );
      PrintData( currentRoot->rightToken );
    } // else if: has rightToken
    
    if ( currentRoot->rightNode ) {
      isRightNode = true;
      PrintTree( currentRoot->rightNode, layer, isRightNode );
    } // if: has isRightNode
    
    if ( layer > 1 ) {
      layer--;
      cout << string( layer*2, ' ' ) << ')' << endl;
    } // if: still space print right paren
  } // PrintTree()
  
  void PrintData( TokenStruct *currentToken ) {
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
      if ( IsPrimitive( currentToken ) ) {
        cout << "#<procedure " << currentToken->content << ">" << endl;
      } // if: is primitive
      
      else {
        cout << currentToken->content << endl;
      } // else: not primitive
    } // else if: symbol
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
  
  string AddTreeString( TreeStruct *currentRoot, int &layer, bool isRightNode, stringstream &returnString ) {
    static bool hasLineReturn = false;
    
    if ( isRightNode == false ) {
      if ( ( currentRoot->leftNode ) ||
           ( currentRoot->leftToken && currentRoot->leftToken->content != "quote" ) ) {
        if ( hasLineReturn ) {
          returnString << string( layer*2, ' ' );
        } // if: has lineReturn
        
        returnString << "( ";
        hasLineReturn = false;
        layer++;
      } // if: not unclear quote
    } // if: currentNode is the leftNode of the previousNode
    
    if ( currentRoot->leftNode ) {
      isRightNode = false;
      AddTreeString( currentRoot->leftNode, layer, isRightNode, returnString );
    } // if: has leftNode
    
    if ( currentRoot->leftToken && currentRoot->leftToken->content != "quote" ) {
      if ( hasLineReturn ) {
        returnString << string( layer*2, ' ' );
      } // if: has line return
      
      AddDataString( currentRoot->leftToken, returnString );
      isRightNode = false;
      hasLineReturn = true;
      
      if ( currentRoot->rightToken && currentRoot->rightToken->tokenType != NIL ) {
        returnString << string( layer*2, ' ' ) << '.' << endl << string( layer*2, ' ' );
        AddDataString( currentRoot->rightToken, returnString );
      } // if: has data in rightToken
    } // if: has data in leftToken
    
    else if ( currentRoot->rightToken ) {
      returnString << string( layer*2, ' ' ) << '.' << endl << string( layer*2, ' ' );
      AddDataString( currentRoot->rightToken, returnString );
    } // else if: has rightToken
    
    if ( currentRoot->rightNode ) {
      isRightNode = true;
      AddTreeString( currentRoot->rightNode, layer, isRightNode, returnString );
    } // if: has isRightNode
    
    if ( layer > 1 ) {
      layer--;
      returnString << string( layer*2, ' ' ) << ')' << endl;
    } // if: still space print right paren
    
    return returnString.str();
  } // AddTreeString()
  
  void AddDataString( TokenStruct *currentToken, stringstream &returnString ) {
    if ( currentToken->tokenType == INT ) {
      returnString << atoi( currentToken->content.c_str() ) << endl;
    } // if: int case
    
    else if ( currentToken->tokenType == FLOAT ) {
      returnString << fixed << setprecision( 3 )
                   << round( atof( currentToken->content.c_str() )*1000 )/1000
                   << endl;
    } // else if: float case with precision and round
    
    else if ( currentToken->tokenType == NIL ) {
      returnString << "nil" << endl;
    } // else if: nil
    
    else if ( currentToken->tokenType == T ) {
      returnString << "#t" << endl;
    } // else if: #t
    
    else if ( currentToken->tokenType == STRING ) {
      AddStringString( currentToken->content, returnString );
    } // else if: string
    
    else if ( currentToken->tokenType == SYMBOL ) {
      if ( IsPrimitive( currentToken ) ) {
        returnString << "#<procedure " << currentToken->content << ">" << endl;
      } // if: is primitive
      
      else {
        returnString << currentToken->content << endl;
      } // else: not primitive
    } // else if: symbol
  } // AddDataString()
  
  void AddStringString( string stringContent, stringstream &returnString ) {
    for ( int index = 0 ; index < stringContent.length() ; index++ ) {
      if ( stringContent[index] == '\\' ) {
        if ( stringContent[index + 1] == 'n' ) {
          returnString << endl;
          index++;
        } // if: '\n'
        
        else if ( stringContent[index + 1] == 't' ) {
          returnString << '\t';
          index++;
        } // else if: '\t'
        
        else if ( stringContent[index + 1] == '"' ) {
          returnString << '"';
          index++;
        } // else if: '"'
        
        else if ( stringContent[index + 1] == '\\' ) {
          returnString << '\\';
          index++;
        } // else if: '\\'
        
        else {
          returnString << stringContent[index];
        } // else: normal '\\'
      } // if: escape
      
      else {
        returnString << stringContent[index];
      } // else: normal character
    } // for: go through the string
    
    returnString << endl;
  } // AddStringString()
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
          if ( project2.EvalSExp( project1 ) == true ) {
            project2.PrintResult();
          } // if: evaluate correct
          
          else {
            project2.ErrorHandling();
          } // else: evaluation error
          
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
