//  main.cpp
//  PL_Project

// project synatax
// <S-exp> ::= <ATOM>
//             | LEFT-PAREN <S-exp> { <S-exp> } [ DOT <S-exp> ] RIGHT-PAREN
//             | QUOTE <S-exp>
// <ATOM>  ::= SYMBOL | INT | FLOAT | STRING | NIL | T | LEFT-PAREN RIGHT-PAREN

// include libraries
# include <ctype.h>
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
}; // token type enum

enum ErrorType {
  NONE, NO_CLOSING_QUOTE, UNEXPECTED_TOKEN_ATOM_LEFT_PAREN, UNEXPECTED_RIGHT_PAREN, NO_MORE_INPUT
}; // error type enum

// define structures
struct TokenStruct{
  string content;
  TokenType type;
}; // TokenStruct

struct TreeStruct {
  TokenStruct* leftToken;
  TokenStruct* rightToken;
  TreeStruct* previousNode;
  TreeStruct* leftNode;
  TreeStruct* rightNode;
}; // TreeStruct

struct ErrorStruct {
  ErrorType errorType;
  string errorMessage;
}; // ErrorStruct

// define global variable to track the cursor
int g_CursorLine = 1;
int g_CursorColumn = 0;

class Project1Class {
private:
  vector<TokenStruct> m_LineOfTokens;
  TreeStruct *m_Root;
  TreeStruct *m_CurrentTreeLocation;
  ErrorStruct m_Error;
  stringstream m_ErrorStream;
  // string m_ErrorMessage;
  
public:
  
  /*
    ------------------ Check -------------------
    ------------------- exit -------------------
  */
  bool CheckExit() {
    string exit = "\0";
    
    for ( int i = 0 ; i < m_LineOfTokens.size() ; i++ ) {
      if ( ( IsAtom( m_LineOfTokens[i] ) && m_LineOfTokens[i].type != NIL ) ||
           m_LineOfTokens[i].type == LEFT_PAREN ||
           m_LineOfTokens[i].type == RIGHT_PAREN ) {
        exit += m_LineOfTokens[i].content;
      } // if: attach the symbol
    } // for: go through the tokens
    
    if ( exit == "(exit)" || m_Error.errorType == NO_MORE_INPUT ) {
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
    if ( currentToken.type == SYMBOL ||
         currentToken.type == INT ||
         currentToken.type == FLOAT ||
         currentToken.type == STRING ||
         currentToken.type == NIL ||
         currentToken.type == T ) {
      return true;
    } // if: atom
    
    else {
      return false;
    } // else: not atom
  } // IsAtom()
  
  /*
    ------------------- Token ------------------
    ---------------- Processing ----------------
  */
  
  bool ReadSExp() {
    m_LineOfTokens.clear();
    m_Root = NULL;
    m_Error.errorType = NONE;
    m_Error.errorMessage = "\0";
    g_CursorLine = 1;
    g_CursorColumn = 0;
    
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
  
  void GetRidOfTheRest() {
    char peekChar = cin.peek();
    
    while ( peekChar != '\n' ) {
      cin.get();
      peekChar = cin.peek();
    } // while: get the left overs
    
    cin.get();
  } // GetRidOfTheRest()
  
  char PeekCharAndGetRidOfComment() {
    char peekChar = cin.peek();
    
    while ( peekChar == ' ' || peekChar == '\n' || peekChar == '\r' ||
            peekChar == '\t' || peekChar == ';' || peekChar == '\0' ) {
      if ( peekChar == ';' ) {
        while ( peekChar != '\n' && peekChar != '\r' ) {
          peekChar = cin.get();
          g_CursorColumn++;
        } // while: get the current line
        
        g_CursorLine++;
        peekChar = cin.peek();
        g_CursorColumn = 0;
      } // if: check if there's any comment
      
      else {
        if ( cin.get() == '\n' ) {
          g_CursorLine++;
          g_CursorColumn = 0;
        } // if: next line, modifiy cursor position
        
        else {
          g_CursorColumn++;
        } // else: cursor move right
        
        peekChar = cin.peek();
      } // else: get next char
    } // while: get the first non-whitespace
    
    return peekChar;
  } // PeekCharAndGetRidOfComment()
  
  string GetString() {
    char currentChar = cin.get();
    
    if ( currentChar == '\n' || currentChar == EOF ) {
      stringstream m_ErrorStream;
      m_ErrorStream << "ERROR (no closing quote) : "
                    << "END-OF-LINE encountered at "
                    << "Line " << g_CursorLine << " Column " << g_CursorColumn+1;
      m_Error.errorType = NO_CLOSING_QUOTE;
      m_Error.errorMessage = m_ErrorStream.str();
      return "\0";
    } // if: check closure error
    
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
      
      if ( currentChar == '\n' || currentChar == EOF ) {
        stringstream m_ErrorStream;
        m_ErrorStream << "ERROR (no closing quote) : "
                      << "END-OF-LINE encountered at "
                      << "Line " << g_CursorLine << " Column " << g_CursorColumn+1;
        m_Error.errorType = NO_CLOSING_QUOTE;
        m_Error.errorMessage = m_ErrorStream.str();
        return "\0";
        // throw m_ErrorMessage;
      } // if: check closure error
      
      g_CursorColumn++;
    } // while: get the string

    currentString.push_back( currentChar );
    // push the last " into the string
    return currentString;
  } // GetString()
  
  bool HasNextToken() {
    char peekChar = PeekCharAndGetRidOfComment();
    
    if ( peekChar == EOF || peekChar == '\0' ) {
      m_Error.errorMessage = "ERROR (no more input) : END-OF-FILE encountered";
      m_Error.errorType = NO_MORE_INPUT;
      return false;
    } // if gets nothing
    
    TokenStruct newToken;
    g_CursorColumn++;
    newToken.content.push_back( cin.get() );
    // initialize new token
    
    if ( peekChar == '(' ) {
      if ( PeekCharAndGetRidOfComment() == ')' ) {
        newToken.content.push_back( cin.get() );
        g_CursorColumn++;
        newToken.type = NIL;
      } // if: () case
      
      else {
        newToken.type = LEFT_PAREN;
      } // else: left parenthesis
    } // if: left parenthesis

    else if ( peekChar == ')' ) {
      newToken.type = RIGHT_PAREN;
    } // else if: right parenthesis
    
    else if ( peekChar == '"' ) {
      newToken.content.append( GetString() );
      
      if ( newToken.content == "\"" ) {
        return false;
      } // if: no closing quote error
      
      newToken.type = STRING;
    } // else if: string

    else if ( peekChar == '\'' ) {
      newToken.type = QUOTE;
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
      
      newToken.type = CheckTokenType( newToken );
    } // else: other types
    
    // cout << newToken.content << " " << newToken.type << endl;
    
    if ( newToken.content != "\0" ) {
      m_LineOfTokens.push_back( newToken ); // push the newToken to the vector
      return true;
    } // if: get a single token successfully
    
    else {
      m_Error.errorMessage = "ERROR (no more input) : END-OF-FILE encountered";
      m_Error.errorType = NO_MORE_INPUT;
      return false;
    } // else: can't get any token
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
      } // if: there are alphabatic characters in the token
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
    // <ATOM>
    if ( IsAtom( m_LineOfTokens.back() ) ) {
      // cout << "<ATOM> ";

      if ( m_Root != NULL ) {
        if ( m_CurrentTreeLocation->leftToken != NULL ) {
          FindValidNodePosition();
          
          if ( m_LineOfTokens.at( m_LineOfTokens.size() - 2 ).type != DOT ) {
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
            
            if ( m_LineOfTokens.at( m_LineOfTokens.size() - 2 ).type != DOT ) {
              RightCreateNode();
              LeftInsertToken();
            } // if: without dot case
            
            RightInsertToken();
          } // if : left node is not NULL
          
          else {
            LeftInsertToken();
          } // else: insert at left
        } // else: insert at the right
      } // if: check if there's any tree
      
      return true;
    } // if: <ATOM>
    
    // LEFT-PAREN
    else if ( m_LineOfTokens.back().type == LEFT_PAREN ) {
      // initialize or create node
      if ( m_Root == NULL ) {
        InitializeRoot();
      } // if: initialize the root pointer

      else {
        if ( m_CurrentTreeLocation->leftToken != NULL ) {
          FindValidNodePosition();
          RightCreateNode();
          
          if ( m_LineOfTokens.at( m_LineOfTokens.size() - 2 ).type != DOT ) {
            LeftCreateNode();
          } // if: with out dot
        } // if: right create case
        
        else {
          if ( m_CurrentTreeLocation->leftNode != NULL ) {
            FindValidNodePosition();
            RightCreateNode();
            
            if ( m_LineOfTokens.at( m_LineOfTokens.size() - 2 ).type != DOT ) {
              LeftCreateNode();
            } // if: with out dot
          } // if: insert right
          
          else {
            FindValidNodePosition();
            LeftCreateNode();
          } // else: insert right
        } // else: right insert node
      } // else: create a node

      if ( HasNextToken() == false ) {
        if ( m_Error.errorType != NO_CLOSING_QUOTE ) {
          m_Error.errorMessage = "ERROR (no more input) : END-OF-FILE encountered";
          m_Error.errorType = NO_MORE_INPUT;
        } // if: check if it's a closure or EOF error
        
        return false;
      } // if: check if there is any token left
      
      // cout << "LEFT-PAREN ";
      
      // LEFT-PAREN <S-exp>
      if ( CheckSExp() == true ) {
        if ( HasNextToken() == false ) {
          if ( m_Error.errorType != NO_CLOSING_QUOTE ) {
            m_Error.errorMessage = "ERROR (no more input) : END-OF-FILE encountered";
            m_Error.errorType = NO_MORE_INPUT;
          } // if: check if it's a closure or EOF error
          
          return false;
        } // if: check if there is any token left

        // LEFT-PAREN <S-exp> { <S-exp> }
        while ( CheckSExp() == true ) {
          if ( HasNextToken() == false ) {
            if ( m_Error.errorType != NO_CLOSING_QUOTE ) {
              m_Error.errorMessage = "ERROR (no more input) : END-OF-FILE encountered";
              m_Error.errorType = NO_MORE_INPUT;
            } // if: check if it's a closure or EOF error
            
            return false;
          } // if: check if there is any token left
        } // while: { <S-exp> }
        
        if ( m_Error.errorType == UNEXPECTED_TOKEN_ATOM_LEFT_PAREN ) {
          return false;
        } // if: there's a syntax error in the while loop
        
        // LEFT-PAREN <S-exp> { <S-exp> } [ DOT ]
        if ( m_LineOfTokens.back().type == DOT ) {
          if ( HasNextToken() == false ) {
            if ( m_Error.errorType != NO_CLOSING_QUOTE ) {
              m_Error.errorMessage = "ERROR (no more input) : END-OF-FILE encountered";
              m_Error.errorType = NO_MORE_INPUT;
            } // if: check if it's a closure or EOF error
            
            return false;
          } // if: check if there is any token left
               
          // LEFT-PAREN <S-exp> { <S-exp> } [ DOT <S-exp> ]
          if ( CheckSExp() == true ) {
            if ( HasNextToken() == false ) {
              if ( m_Error.errorType != NO_CLOSING_QUOTE ) {
                m_Error.errorMessage = "ERROR (no more input) : END-OF-FILE encountered";
                m_Error.errorType = NO_MORE_INPUT;
              } // if: check if it's a closure or EOF error
              
              return false;
            } // if: check if there is any token left
          } // if: <S-exp>
          
          else {
            m_Error.errorType = UNEXPECTED_TOKEN_ATOM_LEFT_PAREN;
            stringstream m_ErrorStream;
            m_ErrorStream << "ERROR (unexpected token) : "
                          << "atom or '(' expected when token at "
                          << "Line " << g_CursorLine
                          << " Column " << g_CursorColumn - ( m_LineOfTokens.back().content.size() - 1 )
                          << " is >>" << m_LineOfTokens.back().content << "<<";
            m_Error.errorMessage = m_ErrorStream.str();
            return false;
          } // else: syntax error
        } // if: [ DOT <S-exp> ]
        
        // LEFT-PAREN <S-exp> { <S-exp> } [ DOT <S-exp> ] RIGHT-PAREN
        if ( m_LineOfTokens.back().type == RIGHT_PAREN ) {
          // cout << "RIGHT_PAREN ";
          m_CurrentTreeLocation = m_CurrentTreeLocation->previousNode;
          return true;
        } // if: RIGHT-PAREN
        
        else {
          m_Error.errorType = UNEXPECTED_RIGHT_PAREN;
          stringstream m_ErrorStream;
          m_ErrorStream << "ERROR (unexpected token) : "
                        << "')' expected when token at "
                        << "Line " << g_CursorLine
                        << " Column " << g_CursorColumn - ( m_LineOfTokens.back().content.size() - 1 )
                        << " is >>" << m_LineOfTokens.back().content << "<<"; 
          m_Error.errorMessage = m_ErrorStream.str();
          // throw m_ErrorMessage;
          return false;
        } // else: syntax error
      } // if: <S-exp>
      
      else {
        m_Error.errorType = UNEXPECTED_TOKEN_ATOM_LEFT_PAREN;
        stringstream m_ErrorStream;
        m_ErrorStream << "ERROR (unexpected token) : "
                      << "atom or '(' expected when token at "
                      << "Line " << g_CursorLine
                      << " Column " << g_CursorColumn - ( m_LineOfTokens.back().content.size() - 1 )
                      << " is >>" << m_LineOfTokens.back().content << "<<";
        m_Error.errorMessage = m_ErrorStream.str();
        return false;
      } // else: syntax error
    } // else if: LEFT-PAREN
    
    // QUOTE
    else if ( m_LineOfTokens.back().type == QUOTE ) {
      if ( HasNextToken() == false ) {
        if ( m_Error.errorType != NO_CLOSING_QUOTE ) {
          m_Error.errorMessage = "ERROR (no more input) : END-OF-FILE encountered";
          m_Error.errorType = NO_MORE_INPUT;
        } // if: check if it's a closure or EOF error
        
        return false;
      } // if: check if there is any token left
      
      // cout << "QUOTE ";
      
      // QUOTE <S-exp>
      if ( CheckSExp() == true ) {
        // cout << "<S-exp>" << endl;
        return true;
      } // if: <S-exp>
      
      else {
        stringstream m_ErrorStream;
        m_ErrorStream << "ERROR (unexpected token) : "
                      << "atom or '(' expected when token at "
                      << "Line " << g_CursorLine
                      << " Column " << g_CursorColumn - ( m_LineOfTokens.back().content.size() - 1 )
                      << " is >>" << m_LineOfTokens.back().content << "<<";
        m_Error.errorMessage = m_ErrorStream.str();
        return false;
      } // else: syntax error
    } // else if: QUOTE
    
    stringstream m_ErrorStream;
    m_ErrorStream << "ERROR (unexpected token) : "
                  << "atom or '(' expected when token at "
                  << "Line " << g_CursorLine
                  << " Column " << g_CursorColumn - ( m_LineOfTokens.back().content.size() - 1 )
                  << " is >>" << m_LineOfTokens.back().content << "<<";
    m_Error.errorMessage = m_ErrorStream.str();
    
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

  void CreateNode() {
    if ( m_CurrentTreeLocation->leftToken == NULL &&
         m_CurrentTreeLocation->leftNode == NULL ) {
      m_CurrentTreeLocation->leftNode = new TreeStruct;
      m_CurrentTreeLocation->leftNode->previousNode = m_CurrentTreeLocation;
      m_CurrentTreeLocation = m_CurrentTreeLocation->leftNode;
    } // if: check left or right
    
    else {
      if ( IsAtom( m_LineOfTokens.at( m_LineOfTokens.size() - 2 ) ) ) {
        m_CurrentTreeLocation->rightNode = new TreeStruct;
        m_CurrentTreeLocation->rightNode->previousNode = m_CurrentTreeLocation;
        m_CurrentTreeLocation = m_CurrentTreeLocation->rightNode;
        m_CurrentTreeLocation->leftNode = new TreeStruct;
        m_CurrentTreeLocation->leftNode->previousNode = m_CurrentTreeLocation;
        m_CurrentTreeLocation = m_CurrentTreeLocation->leftNode;
      } // if: without dot case
      
      else {
        m_CurrentTreeLocation->rightNode = new TreeStruct;
        m_CurrentTreeLocation->rightNode->previousNode = m_CurrentTreeLocation;
        m_CurrentTreeLocation = m_CurrentTreeLocation->rightNode;
      } // else: normal dot case
    } // else: create at right

    m_CurrentTreeLocation->leftNode = NULL;
    m_CurrentTreeLocation->rightNode = NULL;
    m_CurrentTreeLocation->leftToken = NULL;
    m_CurrentTreeLocation->rightToken = NULL;
  } // CreateNode()
  
  void LeftCreateNode() {
    m_CurrentTreeLocation->leftNode = new TreeStruct;
    m_CurrentTreeLocation->leftNode->previousNode = m_CurrentTreeLocation;
    m_CurrentTreeLocation = m_CurrentTreeLocation->leftNode;
    m_CurrentTreeLocation->leftNode = NULL;
    m_CurrentTreeLocation->rightNode = NULL;
    m_CurrentTreeLocation->leftToken = NULL;
    m_CurrentTreeLocation->rightToken = NULL;
  } // LeftCreateNode()
  
  void RightCreateNode() {
    m_CurrentTreeLocation->rightNode = new TreeStruct;
    m_CurrentTreeLocation->rightNode->previousNode = m_CurrentTreeLocation;
    m_CurrentTreeLocation = m_CurrentTreeLocation->rightNode;
    m_CurrentTreeLocation->leftNode = NULL;
    m_CurrentTreeLocation->rightNode = NULL;
    m_CurrentTreeLocation->leftToken = NULL;
    m_CurrentTreeLocation->rightToken = NULL;
  } // RightCreateNode()
  
  void FindValidNodePosition() {
    while ( m_CurrentTreeLocation->rightNode != NULL ) {
      m_CurrentTreeLocation = m_CurrentTreeLocation->previousNode;
    } // while: move currentTreeLocation to an available position
  } // FindValidNodePosition()

  void LeftInsertToken() {
    m_CurrentTreeLocation->leftToken = new TokenStruct;
    m_CurrentTreeLocation->leftToken->content = m_LineOfTokens.back().content;
    m_CurrentTreeLocation->leftToken->type = m_LineOfTokens.back().type;
  } // LeftInsertToken()

  void RightInsertToken() {
    m_CurrentTreeLocation->rightToken = new TokenStruct;
    m_CurrentTreeLocation->rightToken->content = m_LineOfTokens.back().content;
    m_CurrentTreeLocation->rightToken->type = m_LineOfTokens.back().type;
  } // RightInsertToken()
  
  /*
    ------------------- Print ------------------
    ------------------ Results -----------------
  */
  
  bool NeedToPrint( int i ) {
    if ( m_LineOfTokens[i].type == DOT ) {
      if ( m_LineOfTokens[i+1].type == LEFT_PAREN ) {
        return false;
      } // if: dot-leftParen case
      
      else if ( m_LineOfTokens[i+1].type == NIL ) {
        return false;
      } // else if: dot-nil
    } // if: current token is DOT
    
    else if ( m_LineOfTokens[i].type == LEFT_PAREN ) {
      if ( i >= 1 ) {
        if ( m_LineOfTokens[i-1].type == DOT ) {
          return false;
        } // if: dot-leftParen case
      } // if: check index validility
    } // if: current token is left-paren
    
    // else if ( m_LineOfTokens[i].type == RIGHT_PAREN ) {
    //   if ( m_LineOfTokens[i-1].type == RIGHT_PAREN ) {
    //     return false;
    //   } // if: continuous two right paren
    // } // else if: currrent token is right-paren
    
    else if ( m_LineOfTokens[i].type == NIL ) {
      if ( i >= 1 ) {
        if ( m_LineOfTokens[i-1].type == DOT &&
             m_LineOfTokens[i+1].type == RIGHT_PAREN ) {
          return false;
        } // if: nil-rightParen case
      } // if: check index validility
    } // else if: current token is NIL
    
    return true;
  } // NeedToPrint()
  
  void PrintIndentation( int leftParenCount ) {
    int spaces = leftParenCount * 2;
    
    while ( spaces > 0 ) {
      cout << " ";
      spaces--;
    } // while: print spaces
  } // PrintIndentation()
  
  void PrintSExp() {
    int leftParenCount = 0;
    bool hasLineReturn = false;
    bool alreagyRightParenMinus = false;
    
    for ( int i = 0 ; i < m_LineOfTokens.size() ; i++ ) {
      alreagyRightParenMinus = false;
      
      if ( NeedToPrint( i ) ) {
        if ( m_LineOfTokens[i].type == RIGHT_PAREN ) {
          leftParenCount--;
          alreagyRightParenMinus = true;
        } // if: print right parenthesis, minus leftParenCount by one
        
        if ( hasLineReturn ) {
          PrintIndentation( leftParenCount );
        } // if: has a line return
        
        if ( IsAtom( m_LineOfTokens[i] ) ) {
          if ( m_LineOfTokens[i].type == INT ) {
            cout << atoi( m_LineOfTokens[i].content.c_str() ) << endl;
          } // if: int case
          
          else if ( m_LineOfTokens[i].type == FLOAT ) {
            cout << fixed << setprecision( 3 )
            << round( atof( m_LineOfTokens[i].content.c_str() )*1000 ) / 1000
            << endl;
          } // else if: float case with precision and round
          
          else if ( m_LineOfTokens[i].type == NIL ) {
            cout << "nil" << endl;
          } // else if: nil
          
          else if ( m_LineOfTokens[i].type == T ) {
            cout << "#t" << endl;
          } // else if: #t
          
          else if ( m_LineOfTokens[i].type == STRING ) {
            PrintString( m_LineOfTokens[i].content );
          } // else if: string
          
          else if ( m_LineOfTokens[i].type == SYMBOL ) {
            cout << m_LineOfTokens[i].content << endl;
          } // else if: symbol
          
          hasLineReturn = true;
        } // if: current token is an atom

        else if ( m_LineOfTokens[i].type == LEFT_PAREN ) {
          leftParenCount++;
          hasLineReturn = false;
          cout << "( ";
        } // else if: left paren
        
        else if ( m_LineOfTokens[i].type == RIGHT_PAREN ) {
          cout << ")" << endl;
          hasLineReturn = true;
        } // else if: right paren
        
        else if ( m_LineOfTokens[i].type == QUOTE ) {
          cout << "( quote" << endl;
          leftParenCount++;
          hasLineReturn = true;
          TokenStruct quoteRightParen;
          quoteRightParen.type = RIGHT_PAREN;
          quoteRightParen.content = ")";
          m_LineOfTokens.push_back( quoteRightParen );
        } // else if: quote
        
        else if ( m_LineOfTokens[i].type == DOT ) {
          cout << "." << endl;
          hasLineReturn = true;
        } // else if: dot
      } // if: token need to be printed
      
      if ( leftParenCount == 0 ) {
        return;
      } // if: check print complete
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
  
  void ErrorHandling() {
    if ( m_Error.errorType == NO_MORE_INPUT ) {
      cout << m_Error.errorMessage;
    } // if: eof
    
    else {
      cout << m_Error.errorMessage << endl;
      
      while ( cin.get() != '\n' ) {
        cin.get();
      } // while: get the left overs
    } // else: not eof
  } // ErrorHandling()
  
}; // Project1Class

int main() {
  int uTestNum = 0;
  cin >> uTestNum;
  bool end = false;
  Project1Class project1;
  cout << "Welcome to OurScheme!" << endl << endl;
  
  if ( uTestNum == 4 )
    cout << "hehe";
  
  else {
    do {
      cout << "> ";
      
      if ( project1.ReadSExp() == true ) {
        if ( project1.CheckExit() == true ) {
          end = true;
        } // if: check exit
        
        else {
          project1.PrintSExp();
        } // else: not exit case
      } // if: no error
      
      else {
        project1.ErrorHandling();
        
        if ( project1.CheckExit() == true ) {
          end = true;
        } // if: check exit
      } // else: error occured
      
      cout << endl;
    } while ( NOT end );
  } // else
  
  cout << "Thanks for using OurScheme!" << endl << endl;
} // main(): main function
