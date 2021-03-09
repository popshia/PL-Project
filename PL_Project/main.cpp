//  main.cpp
//  PL_Project

// project synatax
// <S-exp> ::= <ATOM> | LEFT-PAREN <S-exp> { <S-exp> } [ DOT <S-exp> ] RIGHT-PAREN | QUOTE <S-exp>
// <ATOM>  ::= SYMBOL | INT | FLOAT | STRING | NIL | T | LEFT-PAREN RIGHT-PAREN

// <BE> ::= <E> EQ <E>
// <E>  ::= <T> { ADD <T> | MINUS <T> }
// <T>  ::= <F> { * <F> | / <F> }
// <F>  ::= NUM | ID | ( <E> )

// Parser call Scanner with GetToken() and PeekToken()

// include libraries
#include <_ctype.h>
#include <iostream>
#include <stdio.h>
#include <string>

// definition
#define NOT !

// namespace
using namespace std;

// enum
enum Type {
    NONE, SYMBOL, INT, FLOAT, STRING, NIL, T, DOT
}; // enum type definition

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
        if ( peek == ' ' | peek == '\t' ) {
            cin.get();
            GetChar();
        } // if: check if it's a whitespace or tab
        return cin.get();
    } // GetChar()

    void GetToken() {
        char next = GetChar();
        switch ( int(next) ) {
            case 43: // + symbol
            case 45: // - symbol
            case 46: // . symbol
            case 48 ... 57: // numbers
            case 97 ... 122: // characters
                inputTerm += next;
            case 40: // '(' left bracket 
                CreateTree();
        } // switch: check the type of the current char
        char peek = cin.peek();
        if ( peek == '\n' | peek == EOF )
            return;
        else
            GetToken();
        return;
    } // get token using recursive

    Type IdentifyTokenType() {
        Type type;
        for ( char & c: inputTerm ) {
            if ( isdigit(c) ) type = INT;
            if ( type == INT && c == '.' ) {
                return FLOAT;
            } 
        }
        return type;
    }

    void PrintSExp() {
        cout << IdentifyTokenType() << endl;
    }

    void ReadSExp() {
        inputTerm = "\0";
        GetToken();
        PrintSExp();
        char * p;
        cout << strtol(inputTerm.c_str(), &p, 10) << endl;
        //call PeekToken
    } // read and process the expression

    string CheckInputTerm() {
        return inputTerm;
    } // CheckInputTerm: check(return) the input term
}; // Project1Class

// main function
int main(int argc, const char * argv[]) {
    //int uTestNum = 0;
    bool end = false;
    Project1Class project1;
    cout << "Welcome to OurScheme!" << endl;
    do {
        cout << "> ";
        project1.ReadSExp();
        //call GetToken and PeekToken
        if ( project1.CheckInputTerm() == "exit" ) {
            end = true;
        } // check exit
        //PrintSExp();
        cout << endl;
    } while ( NOT end );
    cout << endl << "Thanks for using OurScheme!" << endl << endl;
} // main screen
