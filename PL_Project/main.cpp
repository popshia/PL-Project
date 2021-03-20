//  main.cpp
//  PL_Project

// project synatax
// <S-exp> ::= <ATOM> | LEFT-PAREN <S-exp> { <S-exp> } [ DOT <S-exp> ] RIGHT-PAREN | QUOTE <S-exp>
// <ATOM>  ::= SYMBOL | INT | FLOAT | STRING | NIL | T | LEFT-PAREN RIGHT-PAREN
// <BE> ::= <E> EQ <E>
// <E>  ::= <T> { ADD <T> | MINUS <T> }
// <T>  ::= <F> { * <F> | / <F> }
// <F>  ::= NUM | ID | ( <E> )

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

// definition
# define NOT !

// namespace
using namespace std;

// enum and enum strings
enum TokenType {
  NONE, SYMBOLS, INT, FLOAT, STRING, NIL, T, DOT, QUOTE, DOT_PAIR
}; // enum token type definition
enum CharType {
  NUM, CHAR, SYMBOL, POUND, L_PARA, R_PARA, DOUBLE_Q
}; // enum char type definition

// define structures
struct TokenStruct {
  string value;
}; // TokenStruct
struct TreeStruct {
  TokenStruct* leftToken;
  TokenStruct* rightToken;
  TreeStruct* leftNode;
  TreeStruct* rightNode;
}; // TreeStruct

class Project1Class {
private:
  string m_InputTerm;
public:
  string GetInputTerm() {
    return m_InputTerm;
  } // GetInputTerm()
  
  char GetChar() {
    char peekChar = cin.peek();
    
    while ( peekChar == ' ' || peekChar == '\n' ) {
      cin.get();
      peekChar = cin.peek();
    } // while: get the first non-whitespace
    
    return cin.get();
  } // GetChar()
  
  bool IsFloat() {
    for ( int i = 0 ; i < m_InputTerm.length() ; i++ ) {
      if ( m_InputTerm[i] == '.' ) {
        return true;
      } // if: if there's a dot int the inputTerm
    } // for: go through the inputTerm
    
    return false;
  } // IsFloat()
  
  CharType CheckCharType( char currentChar ) {
    if ( 48 <= ( int ) currentChar && ( int ) currentChar <= 57 ) {
      return NUM;
    } // if: number
    
    else if ( ( int ) currentChar == 40 ) {
      return L_PARA;
    } // if: left parenthesis
    
    else if ( ( int ) currentChar == 41 ) {
      return R_PARA;
    } // if: right parenthesis
    
    else if ( ( 65 <= ( int ) currentChar && ( int ) currentChar <= 90 ) ||
             ( 97 <= ( int ) currentChar && ( int ) currentChar <= 122 ) ) {
      return CHAR;
    } // if: charcters
    
    else if ( ( int ) currentChar == 34 ) {
      return DOUBLE_Q;
    } // if: double quote
    
    else {
      return SYMBOL;
    } // else: others
  } // CheckCharType()
  
  TokenType CheckTokenType() {
    bool isNumber = false;
    bool isSymbol = false;
    
    if ( m_InputTerm == "DOT_PAIR" ) {
      return DOT_PAIR;
    } // if: dot pair
    
    for ( int i = 0 ; i < m_InputTerm.length() ; i++ ) {
      if ( isdigit( m_InputTerm[i] ) ) isNumber = true;
    } // for: check is the input is a int or float
    
    for ( int i = 0 ; i < m_InputTerm.length() ; i++ ) {
      if ( isupper( m_InputTerm[i] ) || islower( m_InputTerm[i] ) ) isSymbol = true;
    } // for: check if there's any characters in the inputTerm
    
    if ( isNumber && NOT isSymbol ) {
      if ( IsFloat() ) {
        return FLOAT;
      } // if: float
      
      else {
        return INT;
      } // else: int
    } // if: check if the number is float
    
    else {
      if ( m_InputTerm == "#f" || m_InputTerm == "nil" || m_InputTerm == "()" ) {
        return NIL;
      } // if: nil
      
      else if ( m_InputTerm == "t" || m_InputTerm == "#t" ) {
        return T;
      } // if: #t
      
      else {
        return SYMBOLS;
      } // else: symbols
    } // else: check t or nil
  } // CheckTokenType()
  
  void CreateTree() {
    TreeStruct *root = new TreeStruct;
    root->leftNode = NULL;
    root->rightNode = NULL;
    root->leftToken = NULL;
    root->rightToken = NULL;
    // initialize the first node
    TreeStruct *currentNode = root;
    // declare the walking pointer
    int leftParenthesisCount = 1;
    int rightParenthesisCount = 0;
    // count of the parenthesis
    string currentValue = "\0";
    cin >> currentValue;

    if ( currentValue == "exit)") {
      m_InputTerm = "(exit)";
      return;
    } // if: (exit)
    
    
  } // CreateTree()
  
  void ProcessString() {
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
  } // ProcessString()
  
  void GetToken() {
    char currentChar = GetChar();
    char peekChar = cin.peek();
    
    if ( CheckCharType( currentChar ) == L_PARA && peekChar != ')' ) {
      CreateTree();
      if ( m_InputTerm != "(exit)" ) {
        m_InputTerm = "DOT_PAIR";
      } // if: not exit term

      else {
        return;
      } // else: exit case
    } // if: left-parenthesis and CreateTree()
    
    else if ( CheckCharType( currentChar ) == DOUBLE_Q ) {
      cout << currentChar;
      ProcessString();
    } // if: process the afterward string if the char is "
    
    else {
      m_InputTerm += currentChar;
    } // else: attach to the inputTerm
    
    peekChar = cin.peek();
    
    if ( peekChar == ' ' || peekChar == '\n' || peekChar == EOF ) {
      return;
    } // if: next token or end
    
    else {
      GetToken();
    } // else: not end keep GetToken()
    
    return;
  } // GetToken()
  
  void PrintSExp() {
    if ( m_InputTerm == "(exit)" ) {
      return;
    } // if: exit case
    
    else if ( CheckTokenType() == INT ) {
      cout << atoi( m_InputTerm.c_str() ) << endl;
    } // else if: int case
    
    else if ( CheckTokenType() == FLOAT ) {
      cout << fixed << setprecision( 3 ) << round( atof( m_InputTerm.c_str() )*1000 ) / 1000 << endl;
    } // else if: float case with precision and round
    
    else if ( CheckTokenType() == NIL ) {
      cout << "nil" << endl;
    } // else if: nil
    
    else if ( CheckTokenType() == T ) {
      cout << "#t" << endl;
    } // else if: #t case
    
    else {
      cout << m_InputTerm << endl;
    } // else: symbol
  } // PrintSExp()
  
  void ReadSExp() {
    m_InputTerm = "\0";
    GetToken();
    PrintSExp();
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
    
    if ( project1.GetInputTerm() == "(exit)" ) {
      end = true;
    } // if: check exit
    
    cout << endl;
  } while ( NOT end );
  
  cout << endl << "Thanks for using OurScheme!" << endl << endl;
} // main(): main function
