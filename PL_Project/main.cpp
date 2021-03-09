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
    string atom = "\0";
    
    void CreateTree() {

    } // create dot list tree

    char GetChar() {
        char peek = cin.peek();
        while ( peek == ' ' | peek == '\t' ) {
            cin.get();
            peek = cin.peek();
        } // while: get all whitespaces if there are any of them
        return cin.get(); // get the first non-whitespace characters
    } // GetChar()

    void GetToken() {
        char next = GetChar();
        switch ( int(next) ) {
            case 43: // + symbol
            case 45: // - symbol
            case 48 ... 57: // numbers
            case 97 ... 122: // characters
                atom += next;
                break;
            case 40: // '(' left bracket 
                CreateTree();
                break;
        } // switch: check the type of the current char
        char peek = cin.peek();
        if ( peek == '\n' | peek == EOF )
            return;
        else
            GetToken();
        return;
    } // get token using recursive

    string ReadSExp() {
        string exp = "\0";
        GetToken();
        return exp;
        //call PeekToken
    } // read and process the expression
}; // Project1Class

// main function
int main(int argc, const char * argv[]) {
    //int uTestNum = 0;
    bool end = false;
    Project1Class project1;
    cout << "Welcome to OurScheme!" << endl;
    do {
        cout << "> ";
        //call GetToken and PeekToken
        if ( project1.ReadSExp() == "exit" ) {
            end = true;
        } // check exit
        //PrintSExp();
        cout << endl;
    } while ( NOT end );
    cout << endl << "Thanks for using OurScheme!" << endl << endl;
} // main screen
