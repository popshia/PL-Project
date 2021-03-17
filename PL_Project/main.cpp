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
#include <_ctype.h>
#include <cctype>
#include <cmath>
#include <iostream>
#include <stdio.h>
#include <string>
#include <iomanip>
#include <math.h>

// definition
#define NOT !

// namespace
using namespace std;

// enum and enum strings
enum TokenType {
    NONE, SYMBOLS, INT, FLOAT, STRING, NIL, T, DOT, QUOTE
}; // enum token type definition
static const char *TokenTypeStr[] = {
    "NONE", "SYMBOLS", "INT", "FLOAT", "STRING", "NIL", "T", "DOT"
}; // enum token string
enum CharType {
    NUM, CHAR, SYMBOL, POUND, L_PARA, R_PARA, DOUBLE_Q
}; // enum char type definition
static const char *CharTypeStr[] = {
    "NUM", "CHAR", "SYMBOL", "POUND", "L_PARA", "R_PARA"
}; // enum token string

// define structures
typedef struct TokenStruct{
    string value = "\0";
} TokenStruct; // single char structure
typedef struct TreeStruct{
    TokenStruct* leftToken = NULL;
    TokenStruct* rightToken = NULL;
    TreeStruct* leftNode = NULL;
    TreeStruct* rightNode = NULL;
} TreeStruct; // single expression structure

class Project1Class {
public:
    string inputTerm = "\0";
    
    string CheckInputTerm() {
        return inputTerm;
    } // CheckInputTerm: check(return) the input term
    
    char GetChar() {
        char peek = cin.peek();
        while ( peek == ' ' | peek == '\n' ) {
            cin.get();
            peek = cin.peek();
        } // while: get the first non-whitespace
        return cin.get();
    } // GetChar()
    
    bool IsFloat() {
        for ( char& c: inputTerm ) {
            if ( c == '.' ) {
                return true;
            } // if: if there's a dot int the inputTerm
        } // for: go through the inputTerm
        return false;
    } // IsFloat()
    
    CharType CheckCharType( char next ) {
        if ( 48 <= int( next ) && int( next ) <= 57 ) {
            return NUM;
        } // if: number 
        else if ( int( next ) == 40 ) {
            return L_PARA;
        } // if: left paranthesis 
        else if ( int( next ) == 41 ) {
            return R_PARA;
        } // if: right paranthesis 
        else if ( ( 65 <= int( next ) && int( next ) <= 90 ) || ( 97 <= int( next ) && int( next ) <= 122 ) ) {
            return CHAR; 
        } // if: charcters 
        else if ( int( next ) == 34 ) {
            return DOUBLE_Q;
        } // if: double quote
        else {
            return SYMBOL;
        } // else: others 
    } // CheckCharType()

    TokenType CheckTokenType() {
        bool isNumber = false;
        bool isSymbol = false;
        //cout << inputTerm << endl;
        for ( char & c: inputTerm ) {
            if ( isdigit(c) || c == '.' ) isNumber = true;
        } // for: check is the input is a int or float
        for ( char & c: inputTerm ) {
            if ( isupper( c ) | islower( c ) ) isSymbol = true;
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
            if ( inputTerm == "#f" || inputTerm == "nil" || inputTerm == "()" ) {
                return NIL;
            } // if: nil
            else if ( inputTerm == "t" || inputTerm == "#t" ) {
                return T;
            } // if: #t
            else {
                return SYMBOLS;
            } // else: symbols
        } // else: check t or nil
    } // CheckTokenType()
    
    void CreateTree() {

    } // CreateTree()

    void ProcessString() {
        char next = cin.get();
        while ( next != '\"' ) {
            if ( next == '\\' ) {
                char peek = cin.peek();
                if ( peek == 'n' ) {
                    cout << endl;
                } // if: "\n"
                else if ( peek == 't' ) {
                    cout << "\t";
                } // if: "\t"
                else if ( peek == '\"' ) {
                    cout << "\"";
                } // if: "\""
                else if ( peek == '\\' ) {
                    cout << "\\";
                } // if: "\\"
                cin.get();
            } // if: escape case
            else {
                cout << next;
            } // else: not escape case
            next = cin.get();
        } // while: get the whole string
        cout << next;
    } // ProcessString()

    void GetToken() {
        char next = GetChar();
        char peek = cin.peek();
        if ( CheckCharType( next ) == L_PARA && peek != ')' ) {
            CreateTree();
        } // if: left-paranthesis and CreateTree()
        else if ( CheckCharType( next ) == DOUBLE_Q ) {
            cout << next;
            ProcessString();
        } // if: process the afterward string if the char is "
        else {
            inputTerm += next;
        } // else: attach to the inputTerm
        peek = cin.peek();
        if ( peek == ' ' | peek == '\n' | peek == EOF ) {
            return;
        } // if: next token or end
        else {
            GetToken();
        } // else: not end keep GetToken()
        return;
    } // GetToken()
    
    void PrintSExp() {
        //cout << TokenTypeStr[CheckTokenType()] << endl;
        if ( CheckTokenType() == INT ) {
            cout << stoi( inputTerm ) << endl;
        } // if: int case
        else if ( CheckTokenType() == FLOAT ) {
            cout << fixed << setprecision( 3 ) << roundf( stof( inputTerm )*1000 ) / 1000 << endl;
        } // else if: float case with precision and round
        else if ( CheckTokenType() == NIL ) {
            cout << "nil" << endl;
        } // else if: nil
        else if ( CheckTokenType() == T ) {
            cout << "#t" << endl;
        } // else if: #t case
        else {
            cout << inputTerm << endl;
        } // else: symbol
    } // PrintSExp()
    
    void ReadSExp() {
        inputTerm = "\0";
        GetToken();
        PrintSExp();
    } // ReadSExp()
}; // Project1Class

int main(int argc, const char * argv[]) {
    //int uTestNum = 0;
    bool end = false;
    Project1Class project1;
    cout << "Welcome to OurScheme!" << endl;
    do {
        cout << "> ";
        project1.ReadSExp();
        if ( project1.CheckInputTerm() == "exit" ) {
            end = true;
        } // if: check exit
        cout << endl;
    } while ( NOT end );
    cout << endl << "Thanks for using OurScheme!" << endl << endl;
} // main(): main function
