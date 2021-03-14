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
    NONE, SYMBOLS, INT, FLOAT, STRING, NIL, T, DOT
}; // enum token type definition
static const char *TokenTypeStr[] = {
    "NONE", "SYMBOLS", "INT", "FLOAT", "STRING", "NIL", "T", "DOT"
}; // enum token string
enum CharType {
    NUM, CHAR, SYMBOL, POUND, L_PARA, R_PARA 
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
    string outputTerm = "\0";
    
    void CreateTree() {

    } // create dot list tree

    char GetChar() {
        char peek = cin.peek();
        while ( peek == ' ' | peek == '\n' ) {
            cin.get();
            peek = cin.peek();
        } // while: get the first non-whitespace
        return cin.get();
    } // GetChar()

    CharType CheckCharType( char next ) {
        if ( 48 <= int(next) && int(next) <= 57 ) // number 
            return NUM;
        else if ( int(next) == 40 ) // left paranthesis 
            return L_PARA;
        else if ( int(next) == 41 ) // right paranthesis 
            return R_PARA;
        else if ( ( 65 <= int(next) && int(next) <= 90 ) || 
                  ( 97 <= int(next) && int(next) <= 122 ) ) // charcters 
            return CHAR; 
        else // others 
            return SYMBOL;
    } // CheckCharType()

    bool IsFloat() {
        for ( char& c: inputTerm ) {
            if ( c == '.' )
                return true;
        } // for: check if there's a dot in the number
        return false;
    } // IsFloat()

    TokenType CheckTokenType() {
        bool isNumber = false;
        bool isSymbol = false;
        //cout << inputTerm << endl;
        for ( char & c: inputTerm ) {
            if ( isdigit(c) || c == '.' ) isNumber = true;
        } // for: check is the input is a int or float
        for ( char & c: inputTerm ) {
            if ( isupper(c) | islower(c) ) isSymbol = true;
        } // for: check if there's any characters in the inputTerm
        if ( NOT isSymbol ) {
            if ( IsFloat() )
                return FLOAT;
            else
                return INT;
        } // if: check if the number is float
        else
            return SYMBOLS; 
    } // CheckTokenType()

    void PrintSExp() {
        cout << TokenTypeStr[CheckTokenType()] << endl;
        if ( CheckTokenType() == INT ) 
            cout << stoi(inputTerm) << endl;
        else if ( CheckTokenType() == FLOAT )
            cout << fixed << setprecision(3) << roundf(stof(inputTerm)*1000)/1000 << endl;
        else
            cout << inputTerm << endl;
    } // PrintSExp()

    void GetToken() {
        char next = GetChar();
        if ( CheckCharType( next ) == L_PARA ) CreateTree();
        else inputTerm += next;
        char peek = cin.peek();
        if ( peek == ' ' | peek == '\n' | peek == EOF ) // next token or end
            return;
        else
            GetToken();
        return;
    } // GetToken(): get token using recursive

    void ReadSExp() {
        inputTerm = "\0";
        GetToken();
        PrintSExp();
    } // ReadSExp(): read and process the expression

    string CheckInputTerm() {
        return inputTerm;
    } // CheckInputTerm: check(return) the input term
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
        } // check exit
        //PrintSExp();
        cout << endl;
    } while ( NOT end );
    cout << endl << "Thanks for using OurScheme!" << endl << endl;
} // main(): main function
