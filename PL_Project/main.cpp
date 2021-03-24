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
struct TokenStruct {
  string content;
  TokenType type;
}; // TokenStruct
struct TreeStruct {
  TokenStruct* leftToken;
  TokenStruct* rightToken;
  TreeStruct* leftNode;
  TreeStruct* rightNode;
}; // TreeStruct

class Project1Class {
private:
  vector<TokenStruct> m_LineOfTokens;
  
public:
  char GetChar() {
    char peekChar = '\0';
    
    while ( cin.peek() == ' ' || cin.peek() == '\n' ) {
      cin.get();
      peekChar = cin.peek();
    } // while: get the first non-whitespace
    
    return cin.get();
  } // GetChar()
  
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
      } // if: check the number of minus sign bit
      
      else if ( isdigit( newToken.content[i] ) ) {
        isNumber = true;
      } // if: there are digits in the token
    } // for: go through the token
    
    if ( ( plusSignBitCount > 0 && minusSignBitCount > 0 ) || plusSignBitCount > 1 || minusSignBitCount > 1 ) {
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
      if ( newToken.content == "#f" || newToken.content == "nil" ) {
        return NIL;
      } // if: nil

      else if ( newToken.content == "t" || newToken.content == "#t" ) {
        return T;
      } // if: #t
      
      else if ( newToken.content == "." ) {
        return DOT;
      } // if: dot

      else {
        return SYMBOL;
      } // else: symbols
    } // else: check t, nil or dot
  } // CheckTokenType()
  
//  TokenType IsAtom() {
//
//  }
  
//  void CreateTree() {
//    TreeStruct *root = new TreeStruct;
//    root->leftNode = NULL;
//    root->rightNode = NULL;
//    root->leftToken = NULL;
//    root->rightToken = NULL;
//    // initialize the first node
//    TreeStruct *currentNode = root;
//    // declare the walking pointer
//    int leftParenthesisCount = 1;
//    int rightParenthesisCount = 0;
//    // count of the parenthesis
//    string currentValue = "\0";
//    cin >> currentValue;
//
//    if ( currentValue == "exit)") {
//      m_IsExitCase = true;
//      return;
//    } // if: (exit)
//
//
//  } // CreateTree()
  
  void PrintString() {
    char currentChar = cin.get();
    
    while ( currentChar != '\"' ) {
      if ( currentChar == '\\' ) {
        char peekChar = cin.peek();
        
        if ( peekChar == 'n' ) {
          cout << endl;
        } // if: "\n"
        
        else if ( peekChar == 't' ) {
          cout << "\t";
        } // if: "\t"
        
        else if ( peekChar == '\"' ) {
          cout << "\"";
        } // if: "\""
        
        else if ( peekChar == '\\' ) {
          cout << "\\";
        } // if: "\\"
        
        cin.get();
      } // if: escape case
      
      else {
        cout << currentChar;
      } // else: not escape case
      
      currentChar = cin.get();
    } // while: get the whole string
    
    cout << currentChar;
  } // PrintString()

  string GetString() {
    char currentChar = cin.get();
    string currentString = "\0";

    while ( currentChar != '\"' ) {
      char peekChar = cin.peek();
      
      if ( currentChar == '\\' && peekChar == '\"' ) {
        currentString.push_back( currentChar );
        currentString.push_back( cin.get() );
      } // if: "\"" case
      
      else {
        currentString.push_back( currentChar );
      } // else: normal string

      currentChar = cin.get();
    } // while: get the string

    currentString.push_back( currentChar );
    // push the last " into the string
    return currentString;
  } // GetString()
  
  bool HasNextToken() {
    char currentChar = GetChar();
    
    if ( currentChar == '\0' ) {
      return false;
    } // if gets nothing
    
    TokenStruct newToken;
    newToken.content.push_back( currentChar );
    // initialize new token
    
    if ( currentChar == '(' ) {
      char peekChar = cin.peek();
      if ( peekChar == ')' ) {
        newToken.content.push_back( cin.get() );
        newToken.type = NIL;
      } // if: () case
      else {
        newToken.type = LEFT_PAREN;
      } // else: left parenthesis
    } // if: left parenthesis

    else if ( currentChar == ')' ) {
      newToken.type = RIGHT_PAREN;
    } // if: right parenthesis
    
    else if ( currentChar == '"' ) {
      newToken.content.append( GetString() );
      newToken.type = STRING;
    } // if: string

    else if ( currentChar == '\'' ) {
      newToken.content = currentChar;
      newToken.type = QUOTE;
    } // if: quote
    
    else if ( currentChar == ';' ) {
      while ( cin.get() != '\n' ) {
        cin.get();
      } // while: get rid off the remain line
      
      newToken.content = "\0";
    } // if: comment
    
    else {
      char peekChar = cin.peek();
      
      while ( peekChar != '\n' && peekChar != ' ' && peekChar != '\t' && peekChar != '\'' &&
             peekChar != '"' && peekChar != '(' && peekChar != ')' && peekChar != ';' ) {
        newToken.content.push_back( cin.get() );
        peekChar = cin.peek();
      } // while: get the rest of the token
      
      newToken.type = CheckTokenType( newToken );
    } // else: other types
    
    cout << newToken.content << " " << newToken.type << endl;
    
    m_LineOfTokens.push_back( newToken ); // push the newToken to the vector
    
    if ( newToken.content != "\0" ) {
      return true;
    } // if: get a single token successfully
    
    else {
      return false;
    } // else: can't get any token
  } // GetToken()

  bool CheckSExp() {
    if ( IsAtom( m_LineOfTokens.back() ) ) {
      cout << "<ATOM> ";
      return true;
    } // if: <ATOM>
    
    else if ( m_LineOfTokens.back().type == LEFT_PAREN ) {
      if ( NOT HasNextToken() ) {
        return false;
      } // if: check if there is any token left
      cout << "LEFT-PAREN ";
      
      if ( CheckSExp() == true ) {
        if ( NOT HasNextToken() ) {
          return false;
        } // if: check if there is any token left

        while ( CheckSExp() == true ) {
          if ( NOT HasNextToken() ) {
            return false;
          } // if: check if there is any token left
        } // while: { <S-exp> }
        
        if ( m_LineOfTokens.back().type == DOT ) {
          if ( NOT HasNextToken() ) {
            return false;
          } // if: check if there is any token left
          cout << "DOT ";
          
          if ( CheckSExp() == true ) {
            if ( NOT HasNextToken() ) {
              return false;
            } // if: check if there is any token left
          } // if: <S-exp>
          
          else {
            return false;
          } // else: syntax error
        } // if: [ DOT <S-exp> ]
        
        if ( m_LineOfTokens.back().type == RIGHT_PAREN ) {
          cout << "RIGHT_PAREN";
          return true;
        } // if: RIGHT-PAREN
        
        else {
          return false;
        } // else: syntax error
      } // if: <S-exp>
      
      else {
        return false;
      } // else: syntax error
    } // if: LEFT-PAREN
    
    else if ( m_LineOfTokens.back().type == QUOTE ) {
      if ( NOT HasNextToken() ) {
        return false;
      } // if: check if there is any token left
      cout << "QUOTE ";
      
      if ( CheckSExp() == true ) {
        cout << "<S-exp" << endl;
        return true;
      } // if: <S-exp>
      
      else {
        return false;
      } // else: syntax error
    } // if: QUOTE
    
    return false;
  } // SyntaxAnalyze()
  
//  void PrintSExp() {
//    if ( m_IsExitCase ) {
//      return;
//    } // if: exit case
//
//    else if ( CheckTokenType() == INT ) {
//      cout << atoi( m_CurrentToken.c_str() ) << endl;
//    } // else if: int case
//
//    else if ( CheckTokenType() == FLOAT ) {
//      cout << fixed << setprecision( 3 ) << round( atof( m_CurrentToken.c_str() )*1000 ) / 1000 << endl;
//    } // else if: float case with precision and round
//
//    else if ( CheckTokenType() == NIL ) {
//      cout << "nil" << endl;
//    } // else if: nil
//
//    else if ( CheckTokenType() == T ) {
//      cout << "#t" << endl;
//    } // else if: #t case
//
//    else {
//      cout << m_CurrentToken << endl;
//    } // else: symbol
//  } // PrintSExp()
  
  void ReadSExp() {
    m_LineOfTokens.clear();
    if ( HasNextToken() ) {
      if ( CheckSExp() == true ) {
        //      PrintSExp();
      } // if: no error
      //    else {
      //      PrintErrorMessage();
      //    } // else: error
    } // if: check if there's any command
    
    else return;
  } // ReadSExp()
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
    
//    if ( project1.CheckExit() ) {
//      end = true;
//    } // if: check exit
    
    cout << endl;
  } while ( NOT end );
  
  cout << endl << "Thanks for using OurScheme!" << endl << endl;
} // main(): main function
