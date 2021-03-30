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

// define global variable to track the cursor
int g_CursorColumn = 0;
int g_CursorLine = 1;

class Project1Class {
private:
  bool m_IsExit;
  vector<TokenStruct> m_LineOfTokens;
  TreeStruct *m_Root;
  TreeStruct *m_CurrentTreeLocation;
  stringstream m_ErrorStream;
  string m_ErrorMessage;
  
public:
  
  /*
    -------------------- Is --------------------
    ----------------- function -----------------
  */
  
  bool IsFloat( TokenStruct newToken ) {
    for ( int i = 0 ; i < newToken.content.length() ; i++ ) {
      if ( newToken.content[i] == '.' ) {
        return true;
      } // if: if there's a dot int the inputTerm
    } // for: go through the inputTerm
    
    return false;
  } // IsFloat()
 
  bool IsAtom( TokenStruct currentToken ) {
    if ( currentToken.type == SYMBOL ||
         currentToken.type == INT ||
         currentToken.type == FLOAT ||
         currentToken.type == STRING ||
         currentToken.type == NIL ||
         currentToken.type == T ||
         currentToken.content == "()" ) {
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
  
  void ReadSExp() {
    m_LineOfTokens.clear();
    m_Root = NULL;
    
    /*
    try {
      HasNextToken();
      CheckSExp();
      PrintSExp();
    } // try: run the code
    catch ( const char* errorMessage ) {
      cout << errorMessage;
    } // catch: error message
    */
    
    if ( HasNextToken() ) {
      if ( CheckSExp() == true ) {
        PrintSExp();
      } // if: no error
      else {
        cout << m_ErrorMessage << endl;
      } // else: error
    } // if: check if there's any command
    
    else {
      cout << m_ErrorMessage << endl;
    } // else: read nothing or EOF or encountered error
    
    g_CursorLine = 0;
    g_CursorColumn = 0;
    return;
  } // ReadSExp()
  
  char PeekCharAndGetRidOfComment() {
    char peekChar = cin.peek();
    
    while ( peekChar == ' ' || peekChar == '\n' || peekChar == '\r' ||
            peekChar == '\t' || peekChar == ';' ) {
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
      m_ErrorStream << "ERROR(no closing quote) : END-OF-LINE encounterd at "
                    << "Line " << g_CursorLine << " Column " << g_CursorColumn+1;
      m_ErrorMessage = m_ErrorStream.str();
      return m_ErrorMessage;
    } // check closure erroe
    
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
        m_ErrorStream << "ERROR(no closing quote) : END-OF-LINE encounterd at "
                      << "Line " << g_CursorLine << " Column " << g_CursorColumn+1;
        m_ErrorMessage = m_ErrorStream.str();
        return m_ErrorMessage;
      } // check closure error
      
      g_CursorColumn++;
    } // while: get the string

    currentString.push_back( currentChar );
    // push the last " into the string
    return currentString;
  } // GetString()
  
  bool HasNextToken() {
    char peekChar = PeekCharAndGetRidOfComment();
    
    if ( peekChar == '\0' ) {
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
      
      if ( m_ErrorMessage != "\0" ) {
        return false;
      } // if: closure error
      
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
              cin.peek() != '\r' ) {
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
      return false;
    } // else: can't get any token
  } // HasNextToken()
  
  TokenType CheckTokenType( TokenStruct newToken ) {
    bool isNumber = false;
    bool isSymbol = false;
    int plusSignBitCount = 0;
    int minusSignBitCount = 0;

    for ( int i = 0 ; i < newToken.content.length() ; i++ ) {
      if ( newToken.content[i] == '+' ) {
        plusSignBitCount++;
      } // if: check the number of plus sign bit
      
      else if ( newToken.content[i] == '-' ) {
        minusSignBitCount++;
      } // else if: check the number of minus sign bit
      
      else if ( isdigit( newToken.content[i] ) ) {
        isNumber = true;
      } // else if: there are digits in the token
    } // for: go through the token
    
    if ( ( plusSignBitCount > 0 && minusSignBitCount > 0 ) ||
         plusSignBitCount > 1 || minusSignBitCount > 1 ) {
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
          if ( m_LineOfTokens.at( m_LineOfTokens.size() - 2 ).type != DOT ) {
            CreateNode();
            LeftInsertToken();
          } // if: without dot case
          
          else {
            RightInsertToken();
          } // else: with case
        } // if: left is empty, insert left

        else {
          LeftInsertToken();
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
        CreateNode();
      } // else: create a node

      if ( HasNextToken() == false ) {
        return false;
      } // if: check if there is any token left
      
      // cout << "LEFT-PAREN ";
      
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
        
        // LEFT-PAREN <S-exp> { <S-exp> } [ DOT ]
        if ( m_LineOfTokens.back().type == DOT ) {
          if ( HasNextToken() == false ) {
            return false;
          } // if: check if there is any token left
          
          // cout << "DOT ";
          
          // LEFT-PAREN <S-exp> { <S-exp> } [ DOT <S-exp> ]
          if ( CheckSExp() == true ) {
            if ( HasNextToken() == false ) {
              return false;
            } // if: check if there is any token left
          } // if: <S-exp>
          
          else {
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
          stringstream m_ErrorStream;
          m_ErrorStream << "ERROR(unexpected token) : ')' expected when token at "
                        << "Line " << g_CursorLine << " Column " << g_CursorColumn
                        << " is >>" << m_LineOfTokens.back().content << "<<";
          m_ErrorMessage = m_ErrorStream.str();
          return false;
        } // else: syntax error
      } // if: <S-exp>
      
      else {
        return false;
      } // else: syntax error
    } // else if: LEFT-PAREN
    
    // QUOTE
    else if ( m_LineOfTokens.back().type == QUOTE ) {
      if ( HasNextToken() == false ) {
        return false;
      } // if: check if there is any token left
      
      // cout << "QUOTE ";
      
      // QUOTE <S-exp>
      if ( CheckSExp() == true ) {
        // cout << "<S-exp>" << endl;
        return true;
      } // if: <S-exp>
      
      else {
        return false;
      } // else: syntax error
    } // else if: QUOTE
    
    if ( m_ErrorStream.str() != "\0" ) {
      m_ErrorStream.clear();
    } // if: clear previous error message
    
    stringstream m_ErrorStream;
    m_ErrorStream << "ERROR(unexpected token) : atom or '(' expected when token at "
         << "Line " << g_CursorLine << " Column " << g_CursorColumn
         << " is >>" << m_LineOfTokens.back().content << "<<";
    m_ErrorMessage = m_ErrorStream.str();
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
    while ( m_CurrentTreeLocation->rightNode != NULL ) {
      m_CurrentTreeLocation = m_CurrentTreeLocation->previousNode;
    } // while: move currentTreeLocation to an available position
    
    if ( m_CurrentTreeLocation->leftToken == NULL &&
        m_CurrentTreeLocation->leftNode == NULL) {
      m_CurrentTreeLocation->leftNode = new TreeStruct;
      m_CurrentTreeLocation->leftNode->previousNode = m_CurrentTreeLocation;
      m_CurrentTreeLocation = m_CurrentTreeLocation->leftNode;
    } // if: check left or right
    
    else {
      m_CurrentTreeLocation->rightNode = new TreeStruct;
      m_CurrentTreeLocation->rightNode->previousNode = m_CurrentTreeLocation;
      m_CurrentTreeLocation = m_CurrentTreeLocation->rightNode;
    } // else: create at right
    
    m_CurrentTreeLocation->leftNode = NULL;
    m_CurrentTreeLocation->rightNode = NULL;
    m_CurrentTreeLocation->leftToken = NULL;
    m_CurrentTreeLocation->rightToken = NULL;
  } // CreateNode()

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
  
  void PrintSExp() {
    if ( m_LineOfTokens.front().type == INT ) {
      cout << atoi( m_LineOfTokens.back().content.c_str() ) << endl;
    } // if: int case

    else if ( m_LineOfTokens.front().type == FLOAT ) {
      cout << fixed << setprecision( 3 )
           << round( atof( m_LineOfTokens.back().content.c_str() )*1000 ) / 1000
           << endl;
    } // else if: float case with precision and round

    else if ( m_LineOfTokens.front().type == NIL ) {
      cout << "nil" << endl;
    } // else if: nil

    else if ( m_LineOfTokens.front().type == T ) {
      cout << "#t" << endl;
    } // else if: #t

    else if ( m_LineOfTokens.front().type == STRING ) {
      PrintString( m_LineOfTokens.front().content );
    } // else if: string

    else if ( m_LineOfTokens.front().type == QUOTE ) {
      PrintQuote();
    } // else if: quote

    else if ( m_LineOfTokens.front().type == LEFT_PAREN ) {
      // PrintTree();
    } // else if: dot pair, print tree

    else {
      cout << m_LineOfTokens.front().content << endl;
    } // else: symbol
  } // PrintSExp()

  void PrintString( string stringContent ) {
    for ( int index = 0 ; index < stringContent.length() ; index++ ) {
      if ( stringContent[ index ] == '\\' ) {
        if ( stringContent[ index + 1 ] == 'n' ) {
          cout << endl;
        } // if: '\n'

        else if ( stringContent[ index + 1 ] == 't' ) {
          cout << '\t';
        } // else if: '\t'

        else if ( stringContent[ index + 1 ] == '"' ) {
          cout << '"';
        } // else if: '"'

        else if ( stringContent[ index + 1 ] == '\\' ) {
          cout << '\\';
        } // else if: '\\'
        
        index++;
      } // if: escape

      else {
        cout << stringContent[ index ];
      } // else: normal character
    } // for: go through the string
  } // PrintString()
  
  void PrintQuote() {
    ;
  } // PrintQuote()
}; // Project1Class

int main() {
  //  int uTestNum = 0;
  //  char lineReturn = '\0';
  //  char * testLabel = new char[100];
  //  cin >> uTestNum;
  //  cin >> lineReturn;
  //  cin.getline( testLabel, 90 );
  bool end = false;
  Project1Class project1;
  cout << "Welcome to OurScheme!" << endl;
  
  do {
    cout << "> ";
    project1.ReadSExp();
    
    //  if ( project1.CheckExit() ) {
    //  end = true;
    //  } // if: check exit
    
    cout << endl;
  } while ( NOT end );
  
  cout << endl << "Thanks for using OurScheme!" << endl << endl;
} // main(): main function
